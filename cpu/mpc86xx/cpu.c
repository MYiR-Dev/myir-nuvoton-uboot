/*
 * Copyright 2004 Freescale Semiconductor
 * Jeff Brown (jeffrey@freescale.com)
 * Srikanth Srinivasan (srikanth.srinivasan@freescale.com)
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <watchdog.h>
#include <command.h>
#include <asm/cache.h>
#include <mpc86xx.h>

#if defined(CONFIG_OF_FLAT_TREE)
#include <ft_build.h>
#endif

#include "../board/mpc8641hpcn/pixis.h"


int checkcpu (void)
{
	sys_info_t sysinfo;
	uint pvr, svr;
	uint ver;
	uint major, minor;
	uint lcrr;		/* local bus clock ratio register */
	uint clkdiv;		/* clock divider portion of lcrr */

	puts("Freescale PowerPC\n");

	pvr = get_pvr();
	ver = PVR_VER(pvr);
	major = PVR_MAJ(pvr);
	minor = PVR_MIN(pvr);

	puts("CPU:\n");

	printf("    Core: ");

	switch (ver) {
	case PVR_VER(PVR_86xx):
	    puts("E600");
	    break;
	default:
	    puts("Unknown");
	    break;
	}
	printf(", Version: %d.%d, (0x%08x)\n", major, minor, pvr);

	svr = get_svr();
	ver = SVR_VER(svr);
	major = SVR_MAJ(svr);
	minor = SVR_MIN(svr);

	puts("    System: ");
	switch (ver) {
	case SVR_8641:
	        puts("8641");
	        break;
	case SVR_8641D:
		puts("8641D");
		break;
	default:
		puts("Unknown");
		break;
	}
	printf(", Version: %d.%d, (0x%08x)\n", major, minor, svr);

	get_sys_info(&sysinfo);

	puts("    Clocks: ");
	printf("CPU:%4lu MHz, ", sysinfo.freqProcessor / 1000000);
	printf("MPX:%4lu MHz, ", sysinfo.freqSystemBus / 1000000);
	printf("DDR:%4lu MHz, ", sysinfo.freqSystemBus / 2000000);

#if defined(CFG_LBC_LCRR)
	lcrr = CFG_LBC_LCRR;
#else
	{
	    volatile immap_t *immap = (immap_t *)CFG_IMMR;
	    volatile ccsr_lbc_t *lbc= &immap->im_lbc;

	    lcrr = lbc->lcrr;
	}
#endif
	clkdiv = lcrr & 0x0f;
	if (clkdiv == 2 || clkdiv == 4 || clkdiv == 8) {
		printf("LBC:%4lu MHz\n",
		       sysinfo.freqSystemBus / 1000000 / clkdiv);
	} else {
		printf("    LBC: unknown (lcrr: 0x%08x)\n", lcrr);
	}

	printf("    L2: ");
	if (get_l2cr() & 0x80000000)
		printf("Enabled\n");
	else
		printf("Disabled\n");

	return 0;
}


static inline void
soft_restart(unsigned long addr)
{

#ifndef CONFIG_MPC8641HPCN

	/* SRR0 has system reset vector, SRR1 has default MSR value */
	/* rfi restores MSR from SRR1 and sets the PC to the SRR0 value */

	__asm__ __volatile__ ("mtspr	26, %0"		:: "r" (addr));
	__asm__ __volatile__ ("li	4, (1 << 6)"	::: "r4");
	__asm__ __volatile__ ("mtspr	27, 4");
	__asm__ __volatile__ ("rfi");

#else /* CONFIG_MPC8641HPCN */
	out8(PIXIS_BASE+PIXIS_RST,0);
#endif /* !CONFIG_MPC8641HPCN */
	while(1);	/* not reached */
}


/*
 * No generic way to do board reset. Simply call soft_reset.
 */
void
do_reset(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char cmd;
	ulong addr, val;
	ulong corepll;

#ifdef CFG_RESET_ADDRESS
	addr = CFG_RESET_ADDRESS;
#else
	/*
	 * note: when CFG_MONITOR_BASE points to a RAM address,
	 * CFG_MONITOR_BASE - sizeof (ulong) is usually a valid
	 * address. Better pick an address known to be invalid on your
	 * system and assign it to CFG_RESET_ADDRESS.
	 */
	addr = CFG_MONITOR_BASE - sizeof (ulong);
#endif

#ifndef CONFIG_MPC8641HPCN

	/* flush and disable I/D cache */
	__asm__ __volatile__ ("mfspr	3, 1008"	::: "r3");
	__asm__ __volatile__ ("ori	5, 5, 0xcc00"	::: "r5");
	__asm__ __volatile__ ("ori	4, 3, 0xc00"	::: "r4");
	__asm__ __volatile__ ("andc	5, 3, 5"	::: "r5");
	__asm__ __volatile__ ("sync");
	__asm__ __volatile__ ("mtspr	1008, 4");
	__asm__ __volatile__ ("isync");
	__asm__ __volatile__ ("sync");
	__asm__ __volatile__ ("mtspr	1008, 5");
	__asm__ __volatile__ ("isync");
	__asm__ __volatile__ ("sync");

	soft_restart(addr);

#else /* CONFIG_MPC8641HPCN */

	if (argc > 1) {
		cmd = argv[1][1];
		switch(cmd) {
		case 'f':    /* reset with frequency changed */
			if (argc < 5)
				goto my_usage;
			read_from_px_regs(0);

			val = set_px_sysclk(simple_strtoul(argv[2],NULL,10));

			corepll = strfractoint(argv[3]);
			val = val + set_px_corepll(corepll);
			val = val + set_px_mpxpll(simple_strtoul(argv[4],
								 NULL, 10));
			if (val == 3) {
				printf("Setting registers VCFGEN0 and VCTL\n");
				read_from_px_regs(1);
				printf("Resetting board with values from VSPEED0, VSPEED1, VCLKH, and VCLKL ....\n");
				set_px_go();
			} else
				goto my_usage;

			while (1); /* Not reached */

		case 'l':
			if (argv[2][1] == 'f') {
				read_from_px_regs(0);
				read_from_px_regs_altbank(0);
				/* reset with frequency changed */
				val = set_px_sysclk(simple_strtoul(argv[3],NULL,10));

				corepll = strfractoint(argv[4]);
				val = val + set_px_corepll(corepll);
				val = val + set_px_mpxpll(simple_strtoul(argv[5],NULL,10));
				if (val == 3) {
					printf("Setting registers VCFGEN0, VCFGEN1, VBOOT, and VCTL\n");
					set_altbank();
					read_from_px_regs(1);
					read_from_px_regs_altbank(1);
					printf("Enabling watchdog timer on the FPGA and resetting board with values from VSPEED0, VSPEED1, VCLKH, and VCLKL to boot from the other bank ....\n");
					set_px_go_with_watchdog();
				} else
					goto my_usage;

				while(1); /* Not reached */
			} else if(argv[2][1] == 'd'){
				/* Reset from next bank without changing frequencies but with watchdog timer enabled */
				read_from_px_regs(0);
				read_from_px_regs_altbank(0);
				printf("Setting registers VCFGEN1, VBOOT, and VCTL\n");
				set_altbank();
				read_from_px_regs_altbank(1);
				printf("Enabling watchdog timer on the FPGA and resetting board to boot from the other bank....\n");
				set_px_go_with_watchdog();
				while(1); /* Not reached */
			} else {
				/* Reset from next bank without changing frequency and without watchdog timer enabled */
				read_from_px_regs(0);
				read_from_px_regs_altbank(0);
				if(argc > 2)
					goto my_usage;
				printf("Setting registers VCFGNE1, VBOOT, and VCTL\n");
				set_altbank();
				read_from_px_regs_altbank(1);
				printf("Resetting board to boot from the other bank....\n");
				set_px_go();
			}

		default:
			goto my_usage;
		}

my_usage:
		printf("\nUsage: reset cf <SYSCLK freq> <COREPLL ratio> <MPXPLL ratio>\n");
		printf("       reset altbank [cf <SYSCLK freq> <COREPLL ratio> <MPXPLL ratio>]\n");
		printf("For example:   reset cf 40 2.5 10\n");
		printf("See MPC8641HPCN Design Workbook for valid values of command line parameters.\n");
		return;
	} else
		out8(PIXIS_BASE+PIXIS_RST,0);

#endif /* !CONFIG_MPC8641HPCN */

	while(1);	/* not reached */
}


/*
 * Get timebase clock frequency
 */
unsigned long get_tbclk(void)
{
	sys_info_t  sys_info;

	get_sys_info(&sys_info);
	return (sys_info.freqSystemBus + 3L) / 4L;
}


#if defined(CONFIG_WATCHDOG)
void
watchdog_reset(void)
{
}
#endif	/* CONFIG_WATCHDOG */


#if defined(CONFIG_DDR_ECC)
void dma_init(void)
{
	volatile immap_t *immap = (immap_t *)CFG_IMMR;
	volatile ccsr_dma_t *dma = &immap->im_dma;

	dma->satr0 = 0x00040000;
	dma->datr0 = 0x00040000;
	asm("sync; isync");
}

uint dma_check(void)
{
	volatile immap_t *immap = (immap_t *)CFG_IMMR;
	volatile ccsr_dma_t *dma = &immap->im_dma;
	volatile uint status = dma->sr0;

	/* While the channel is busy, spin */
	while((status & 4) == 4) {
		status = dma->sr0;
	}

	if (status != 0) {
		printf ("DMA Error: status = %x\n", status);
	}
	return status;
}

int dma_xfer(void *dest, uint count, void *src)
{
	volatile immap_t *immap = (immap_t *)CFG_IMMR;
	volatile ccsr_dma_t *dma = &immap->im_dma;

	dma->dar0 = (uint) dest;
	dma->sar0 = (uint) src;
	dma->bcr0 = count;
	dma->mr0 = 0xf000004;
	asm("sync;isync");
	dma->mr0 = 0xf000005;
	asm("sync;isync");
	return dma_check();
}

#endif	/* CONFIG_DDR_ECC */


#ifdef CONFIG_OF_FLAT_TREE
void ft_cpu_setup(void *blob, bd_t *bd)
{
	u32 *p;
	ulong clock;
	int len;

	clock = bd->bi_busfreq;
	p = ft_get_prop(blob, "/cpus/" OF_CPU "/bus-frequency", &len);
	if (p != NULL)
		*p = cpu_to_be32(clock);

	p = ft_get_prop(blob, "/" OF_SOC "/serial@4500/clock-frequency", &len);
	if (p != NULL)
		*p = cpu_to_be32(clock);

	p = ft_get_prop(blob, "/" OF_SOC "/serial@4600/clock-frequency", &len);
	if (p != NULL)
		*p = cpu_to_be32(clock);

#if defined(CONFIG_MPC86XX_TSEC1)
	p = ft_get_prop(blob, "/" OF_SOC "/ethernet@24000/address", &len);
	memcpy(p, bd->bi_enetaddr, 6);
#endif

#if defined(CONFIG_MPC86XX_TSEC2)
	p = ft_get_prop(blob, "/" OF_SOC "/ethernet@25000/address", &len);
	memcpy(p, bd->bi_enet1addr, 6);
#endif

#if defined(CONFIG_MPC86XX_TSEC3)
	p = ft_get_prop(blob, "/" OF_SOC "/ethernet@26000/address", &len);
	memcpy(p, bd->bi_enet2addr, 6);
#endif

#if defined(CONFIG_MPC86XX_TSEC4)
	p = ft_get_prop(blob, "/" OF_SOC "/ethernet@27000/address", &len);
	 memcpy(p, bd->bi_enet3addr, 6);
#endif

}
#endif
