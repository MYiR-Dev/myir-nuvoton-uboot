/*
 * (C) Copyright 2008 Semihalf
 *
 * (C) Copyright 2000-2006
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */


#include <common.h>
#include <watchdog.h>
#include <command.h>
#include <image.h>
#include <malloc.h>
#include <u-boot/zlib.h>
#include <bzlib.h>
#include <environment.h>
#include <asm/byteorder.h>
#include <asm/mp.h>

#if defined(CONFIG_OF_LIBFDT)
#include <libfdt.h>
#include <fdt_support.h>
#endif

#ifdef CONFIG_SYS_INIT_RAM_LOCK
#include <asm/cache.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

extern ulong get_effective_memsize(void);
static ulong get_sp (void);
static void set_clocks_in_mhz (bd_t *kbd);

#ifndef CONFIG_SYS_LINUX_LOWMEM_MAX_SIZE
#define CONFIG_SYS_LINUX_LOWMEM_MAX_SIZE	(768*1024*1024)
#endif

static void boot_jump_linux(bootm_headers_t *images)
{
	void	(*kernel)(bd_t *, ulong r4, ulong r5, ulong r6,
			  ulong r7, ulong r8, ulong r9);
#ifdef CONFIG_OF_LIBFDT
	char *of_flat_tree = images->ft_addr;
#endif

	kernel = (void (*)(bd_t *, ulong, ulong, ulong,
			   ulong, ulong, ulong))images->ep;
	debug ("## Transferring control to Linux (at address %08lx) ...\n",
		(ulong)kernel);

	bootstage_mark(BOOTSTAGE_ID_RUN_OS);

#if defined(CONFIG_SYS_INIT_RAM_LOCK) && !defined(CONFIG_E500)
	unlock_ram_in_cache();
#endif

#if defined(CONFIG_OF_LIBFDT)
	if (of_flat_tree) {	/* device tree; boot new style */
		/*
		 * Linux Kernel Parameters (passing device tree):
		 *   r3: pointer to the fdt
		 *   r4: 0
		 *   r5: 0
		 *   r6: epapr magic
		 *   r7: size of IMA in bytes
		 *   r8: 0
		 *   r9: 0
		 */
		debug ("   Booting using OF flat tree...\n");
		WATCHDOG_RESET ();
		(*kernel) ((bd_t *)of_flat_tree, 0, 0, EPAPR_MAGIC,
			   getenv_bootm_mapsize(), 0, 0);
		/* does not return */
	} else
#endif
	{
		/*
		 * Linux Kernel Parameters (passing board info data):
		 *   r3: ptr to board info data
		 *   r4: initrd_start or 0 if no initrd
		 *   r5: initrd_end - unused if r4 is 0
		 *   r6: Start of command line string
		 *   r7: End   of command line string
		 *   r8: 0
		 *   r9: 0
		 */
		ulong cmd_start = images->cmdline_start;
		ulong cmd_end = images->cmdline_end;
		ulong initrd_start = images->initrd_start;
		ulong initrd_end = images->initrd_end;
		bd_t *kbd = images->kbd;

		debug ("   Booting using board info...\n");
		WATCHDOG_RESET ();
		(*kernel) (kbd, initrd_start, initrd_end,
			   cmd_start, cmd_end, 0, 0);
		/* does not return */
	}
	return ;
}

void arch_lmb_reserve(struct lmb *lmb)
{
	phys_size_t bootm_size;
	ulong size, sp, bootmap_base;

	bootmap_base = getenv_bootm_low();
	bootm_size = getenv_bootm_size();

#ifdef DEBUG
	if (((u64)bootmap_base + bootm_size) >
	    (CONFIG_SYS_SDRAM_BASE + (u64)gd->ram_size))
		puts("WARNING: bootm_low + bootm_size exceed total memory\n");
	if ((bootmap_base + bootm_size) > get_effective_memsize())
		puts("WARNING: bootm_low + bootm_size exceed eff. memory\n");
#endif

	size = min(bootm_size, get_effective_memsize());
	size = min(size, CONFIG_SYS_LINUX_LOWMEM_MAX_SIZE);

	if (size < bootm_size) {
		ulong base = bootmap_base + size;
		printf("WARNING: adjusting available memory to %lx\n", size);
		lmb_reserve(lmb, base, bootm_size - size);
	}

	/*
	 * Booting a (Linux) kernel image
	 *
	 * Allocate space for command line and board info - the
	 * address should be as high as possible within the reach of
	 * the kernel (see CONFIG_SYS_BOOTMAPSZ settings), but in unused
	 * memory, which means far enough below the current stack
	 * pointer.
	 */
	sp = get_sp();
	debug ("## Current stack ends at 0x%08lx\n", sp);

	/* adjust sp by 4K to be safe */
	sp -= 4096;
	lmb_reserve(lmb, sp, (CONFIG_SYS_SDRAM_BASE + get_effective_memsize() - sp));

#ifdef CONFIG_MP
	cpu_mp_lmb_reserve(lmb);
#endif

	return ;
}

static void boot_prep_linux(bootm_headers_t *images)
{
#ifdef CONFIG_MP
	/*
	 * if we are MP make sure to flush the device tree so any changes are
	 * made visibile to all other cores.  In AMP boot scenarios the cores
	 * might not be HW cache coherent with each other.
	 */
	flush_cache((unsigned long)images->ft_addr, images->ft_len);
#endif
}

static int boot_cmdline_linux(bootm_headers_t *images)
{
	ulong of_size = images->ft_len;
	struct lmb *lmb = &images->lmb;
	ulong *cmd_start = &images->cmdline_start;
	ulong *cmd_end = &images->cmdline_end;

	int ret = 0;

	if (!of_size) {
		/* allocate space and init command line */
		ret = boot_get_cmdline (lmb, cmd_start, cmd_end);
		if (ret) {
			puts("ERROR with allocation of cmdline\n");
			return ret;
		}
	}

	return ret;
}

static int boot_bd_t_linux(bootm_headers_t *images)
{
	ulong of_size = images->ft_len;
	struct lmb *lmb = &images->lmb;
	bd_t **kbd = &images->kbd;

	int ret = 0;

	if (!of_size) {
		/* allocate space for kernel copy of board info */
		ret = boot_get_kbd (lmb, kbd);
		if (ret) {
			puts("ERROR with allocation of kernel bd\n");
			return ret;
		}
		set_clocks_in_mhz(*kbd);
	}

	return ret;
}

static int boot_body_linux(bootm_headers_t *images)
{
	int ret;

	/* allocate space for kernel copy of board info */
	ret = boot_bd_t_linux(images);
	if (ret)
		return ret;

	ret = image_setup_linux(images);
	if (ret)
		return ret;

	return 0;
}

noinline
int do_bootm_linux(int flag, int argc, char * const argv[], bootm_headers_t *images)
{
	int	ret;

	if (flag & BOOTM_STATE_OS_CMDLINE) {
		boot_cmdline_linux(images);
		return 0;
	}

	if (flag & BOOTM_STATE_OS_BD_T) {
		boot_bd_t_linux(images);
		return 0;
	}

	if (flag & BOOTM_STATE_OS_PREP) {
		boot_prep_linux(images);
		return 0;
	}

	boot_prep_linux(images);
	ret = boot_body_linux(images);
	if (ret)
		return ret;
	boot_jump_linux(images);

	return 0;
}

static ulong get_sp (void)
{
	ulong sp;

	asm( "mr %0,1": "=r"(sp) : );
	return sp;
}

static void set_clocks_in_mhz (bd_t *kbd)
{
	char	*s;

	if ((s = getenv ("clocks_in_mhz")) != NULL) {
		/* convert all clock information to MHz */
		kbd->bi_intfreq /= 1000000L;
		kbd->bi_busfreq /= 1000000L;
#if defined(CONFIG_CPM2)
		kbd->bi_cpmfreq /= 1000000L;
		kbd->bi_brgfreq /= 1000000L;
		kbd->bi_sccfreq /= 1000000L;
		kbd->bi_vco	/= 1000000L;
#endif
#if defined(CONFIG_MPC5xxx)
		kbd->bi_ipbfreq /= 1000000L;
		kbd->bi_pcifreq /= 1000000L;
#endif /* CONFIG_MPC5xxx */
	}
}
