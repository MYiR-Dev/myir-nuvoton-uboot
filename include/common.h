/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Common header file for U-Boot
 *
 * This file still includes quite a bit of stuff that should be in separate
 * headers like command.h. Please think before adding more things.
 * Patches to remove things are welcome.
 *
 * (C) Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 */

#ifndef __COMMON_H_
#define __COMMON_H_	1

#ifndef __ASSEMBLY__		/* put C only stuff in this section */

typedef unsigned char		uchar;
typedef volatile unsigned long	vu_long;
typedef volatile unsigned short vu_short;
typedef volatile unsigned char	vu_char;

#include <config.h>
#include <errno.h>
#include <time.h>
#include <asm-offsets.h>
#include <linux/bitops.h>
#include <linux/bug.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <linux/printk.h>
#include <linux/string.h>
#include <linux/stringify.h>
#include <asm/ptrace.h>
#include <stdarg.h>
#include <stdio.h>
#include <linux/kernel.h>

#include <part.h>
#include <flash.h>
#include <image.h>

#ifdef __LP64__
#define CONFIG_SYS_SUPPORT_64BIT_DATA
#endif

#include <log.h>

#include <asm/u-boot.h> /* boot information for Linux kernel */
#include <asm/global_data.h>	/* global data used for startup functions */

/* startup functions, used in:
 * common/board_f.c
 * common/init/board_init.c
 * common/board_r.c
 * common/board_info.c
 */
#include <init.h>

/*
 * Function Prototypes
 */
void	hang		(void) __attribute__ ((noreturn));

#include <display_options.h>

/* common/main.c */
void	main_loop	(void);
int run_command(const char *cmd, int flag);
int run_command_repeatable(const char *cmd, int flag);

/**
 * Run a list of commands separated by ; or even \0
 *
 * Note that if 'len' is not -1, then the command does not need to be nul
 * terminated, Memory will be allocated for the command in that case.
 *
 * @param cmd	List of commands to run, each separated bu semicolon
 * @param len	Length of commands excluding terminator if known (-1 if not)
 * @param flag	Execution flags (CMD_FLAG_...)
 * @return 0 on success, or != 0 on error.
 */
int run_command_list(const char *cmd, int len, int flag);

int checkflash(void);
int checkdram(void);
extern u8 __dtb_dt_begin[];	/* embedded device tree blob */
extern u8 __dtb_dt_spl_begin[];	/* embedded device tree blob for SPL/TPL */
int mdm_init(void);

/**
 * Show the DRAM size in a board-specific way
 *
 * This is used by boards to display DRAM information in their own way.
 *
 * @param size	Size of DRAM (which should be displayed along with other info)
 */
void board_show_dram(phys_size_t size);

/**
 * Get the uppermost pointer that is valid to access
 *
 * Some systems may not map all of their address space. This function allows
 * boards to indicate what their highest support pointer value is for DRAM
 * access.
 *
 * @param total_size	Size of U-Boot (unused?)
 */
ulong board_get_usable_ram_top(ulong total_size);

/**
 * arch_fixup_fdt() - Write arch-specific information to fdt
 *
 * Defined in arch/$(ARCH)/lib/bootm-fdt.c
 *
 * @blob:	FDT blob to write to
 * @return 0 if ok, or -ve FDT_ERR_... on failure
 */
int arch_fixup_fdt(void *blob);

/* common/flash.c */
void flash_perror (int);

/* common/cmd_source.c */
int	source (ulong addr, const char *fit_uname);

extern ulong load_addr;		/* Default Load Address */
extern ulong save_addr;		/* Default Save Address */
extern ulong save_size;		/* Default Save Size */

/* common/cmd_net.c */
int do_tftpb(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

/* common/cmd_fat.c */
int do_fat_fsload(cmd_tbl_t *, int, int, char * const []);

/* common/cmd_ext2.c */
int do_ext2load(cmd_tbl_t *, int, int, char * const []);

void	pci_init_board(void);

/* common/exports.c */
void	jumptable_init(void);

/* common/kallsysm.c */
const char *symbol_lookup(unsigned long addr, unsigned long *caddr);

/* common/memsize.c */
long	get_ram_size  (long *, long);
phys_size_t get_effective_memsize(void);

/* $(BOARD)/$(BOARD).c */
void	reset_phy     (void);
void	fdc_hw_init   (void);

/* $(BOARD)/eeprom.c */
#ifdef CONFIG_CMD_EEPROM
void eeprom_init  (int bus);
int  eeprom_read  (unsigned dev_addr, unsigned offset, uchar *buffer, unsigned cnt);
int  eeprom_write (unsigned dev_addr, unsigned offset, uchar *buffer, unsigned cnt);
#else
/*
 * Some EEPROM code is depecated because it used the legacy I2C interface. Add
 * some macros here so we don't have to touch every one of those uses
 */
#define eeprom_init(bus)
#define eeprom_read(dev_addr, offset, buffer, cnt) ((void)-ENOSYS)
#define eeprom_write(dev_addr, offset, buffer, cnt) ((void)-ENOSYS)
#endif

#if !defined(CONFIG_ENV_EEPROM_IS_ON_I2C) && defined(CONFIG_SYS_I2C_EEPROM_ADDR)
# define CONFIG_SYS_DEF_EEPROM_ADDR CONFIG_SYS_I2C_EEPROM_ADDR
#endif

/* $(BOARD)/$(BOARD).c */
int board_early_init_f (void);
int board_fix_fdt (void *rw_fdt_blob); /* manipulate the U-Boot fdt before its relocation */
int board_late_init (void);
int board_postclk_init (void); /* after clocks/timebase, before env/serial */
int board_early_init_r (void);

#if defined(CONFIG_SYS_DRAM_TEST)
int testdram(void);
#endif /* CONFIG_SYS_DRAM_TEST */

#if defined(CONFIG_ARM)
void	relocate_code(ulong);
#else
void	relocate_code(ulong, gd_t *, ulong) __attribute__ ((noreturn));
#endif
ulong	get_endaddr   (void);
void	trap_init     (ulong);

void s_init(void);

void	upmconfig     (unsigned int, unsigned int *, unsigned int);
ulong	get_tbclk     (void);
void	reset_misc    (void);
void	reset_cpu     (ulong addr);
void ft_cpu_setup(void *blob, bd_t *bd);
void ft_pci_setup(void *blob, bd_t *bd);

/* $(CPU)/speed.c */
int	get_clocks (void);
ulong	get_bus_freq  (ulong);
int get_serial_clock(void);

/* lib/uuid.c */
#include <uuid.h>

/* lib/vsprintf.c */
#include <vsprintf.h>

/* lib/net_utils.c */
#include <net.h>

#include <bootstage.h>

#else	/* __ASSEMBLY__ */

#endif	/* __ASSEMBLY__ */

/* Put only stuff here that the assembler can digest */

#ifdef CONFIG_POST
#define CONFIG_HAS_POST
#ifndef CONFIG_POST_ALT_LIST
#define CONFIG_POST_STD_LIST
#endif
#endif

#define ROUND(a,b)		(((a) + (b) - 1) & ~((b) - 1))

/*
 * check_member() - Check the offset of a structure member
 *
 * @structure:	Name of structure (e.g. global_data)
 * @member:	Name of member (e.g. baudrate)
 * @offset:	Expected offset in bytes
 */
#define check_member(structure, member, offset) _Static_assert( \
	offsetof(struct structure, member) == offset, \
	"`struct " #structure "` offset for `" #member "` is not " #offset)

/* Avoid using CONFIG_EFI_STUB directly as we may boot from other loaders */
#ifdef CONFIG_EFI_STUB
#define ll_boot_init()	false
#else
#define ll_boot_init()	true
#endif

/* Pull in stuff for the build system */
#ifdef DO_DEPS_ONLY
# include <env_internal.h>
#endif

#endif	/* __COMMON_H_ */
