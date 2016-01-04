/*
 * (C) Copyright 2010
 * ISEE 2007 SL, <www.iseebcn.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <status_led.h>
#include <dm.h>
#include <ns16550.h>
#include <twl4030.h>
#include <netdev.h>
#include <asm/gpio.h>
#include <asm/io.h>
#include <asm/arch/mem.h>
#include <asm/arch/mmc_host_def.h>
#include <asm/arch/mux.h>
#include <asm/arch/sys_proto.h>
#include <asm/mach-types.h>
#include "igep00x0.h"

DECLARE_GLOBAL_DATA_PTR;

#if defined(CONFIG_CMD_NET)
/* GPMC definitions for LAN9221 chips */
static const u32 gpmc_lan_config[] = {
	NET_LAN9221_GPMC_CONFIG1,
	NET_LAN9221_GPMC_CONFIG2,
	NET_LAN9221_GPMC_CONFIG3,
	NET_LAN9221_GPMC_CONFIG4,
	NET_LAN9221_GPMC_CONFIG5,
	NET_LAN9221_GPMC_CONFIG6,
};
#endif

static const struct ns16550_platdata igep_serial = {
	OMAP34XX_UART3,
	2,
	V_NS16550_CLK
};

U_BOOT_DEVICE(igep_uart) = {
	"ns16550_serial",
	&igep_serial
};

/*
 * Routine: board_init
 * Description: Early hardware init.
 */
int board_init(void)
{
	gpmc_init(); /* in SRAM or SDRAM, finish GPMC */
	/* boot param addr */
	gd->bd->bi_boot_params = (OMAP34XX_SDRC_CS0 + 0x100);

#if defined(CONFIG_STATUS_LED) && defined(STATUS_LED_BOOT)
	status_led_set(STATUS_LED_BOOT, STATUS_LED_ON);
#endif

	return 0;
}

#ifdef CONFIG_SPL_BUILD
/*
 * Routine: omap_rev_string
 * Description: For SPL builds output board rev
 */
void omap_rev_string(void)
{
}

/*
 * Routine: get_board_mem_timings
 * Description: If we use SPL then there is no x-loader nor config header
 * so we have to setup the DDR timings ourself on both banks.
 */
void get_board_mem_timings(struct board_sdrc_timings *timings)
{
	timings->mr = MICRON_V_MR_165;
#ifdef CONFIG_BOOT_NAND
	timings->mcfg = MICRON_V_MCFG_200(256 << 20);
	timings->ctrla = MICRON_V_ACTIMA_200;
	timings->ctrlb = MICRON_V_ACTIMB_200;
	timings->rfr_ctrl = SDP_3430_SDRC_RFR_CTRL_200MHz;
#else
	if (get_cpu_family() == CPU_OMAP34XX) {
		timings->mcfg = NUMONYX_V_MCFG_165(256 << 20);
		timings->ctrla = NUMONYX_V_ACTIMA_165;
		timings->ctrlb = NUMONYX_V_ACTIMB_165;
		timings->rfr_ctrl = SDP_3430_SDRC_RFR_CTRL_165MHz;

	} else {
		timings->mcfg = NUMONYX_V_MCFG_200(256 << 20);
		timings->ctrla = NUMONYX_V_ACTIMA_200;
		timings->ctrlb = NUMONYX_V_ACTIMB_200;
		timings->rfr_ctrl = SDP_3430_SDRC_RFR_CTRL_200MHz;
	}
#endif
}
#endif

#if defined(CONFIG_CMD_NET)

static void reset_net_chip(int gpio)
{
	if (!gpio_request(gpio, "eth nrst")) {
		gpio_direction_output(gpio, 1);
		udelay(1);
		gpio_set_value(gpio, 0);
		udelay(40);
		gpio_set_value(gpio, 1);
		mdelay(10);
	}
}

/*
 * Routine: setup_net_chip
 * Description: Setting up the configuration GPMC registers specific to the
 *		Ethernet hardware.
 */
static void setup_net_chip(void)
{
	struct ctrl *ctrl_base = (struct ctrl *)OMAP34XX_CTRL_BASE;

	enable_gpmc_cs_config(gpmc_lan_config, &gpmc_cfg->cs[5],
			CONFIG_SMC911X_BASE, GPMC_SIZE_16M);

	/* Enable off mode for NWE in PADCONF_GPMC_NWE register */
	writew(readw(&ctrl_base->gpmc_nwe) | 0x0E00, &ctrl_base->gpmc_nwe);
	/* Enable off mode for NOE in PADCONF_GPMC_NADV_ALE register */
	writew(readw(&ctrl_base->gpmc_noe) | 0x0E00, &ctrl_base->gpmc_noe);
	/* Enable off mode for ALE in PADCONF_GPMC_NADV_ALE register */
	writew(readw(&ctrl_base->gpmc_nadv_ale) | 0x0E00,
		&ctrl_base->gpmc_nadv_ale);

	reset_net_chip(64);
}
#else
static inline void setup_net_chip(void) {}
#endif

#if defined(CONFIG_GENERIC_MMC) && !defined(CONFIG_SPL_BUILD)
int board_mmc_init(bd_t *bis)
{
	return omap_mmc_init(0, 0, 0, -1, -1);
}
#endif

#if defined(CONFIG_GENERIC_MMC)
void board_mmc_power_init(void)
{
	twl4030_power_mmc_init(0);
}
#endif

void set_fdt(void)
{
	switch (gd->bd->bi_arch_number) {
	case MACH_TYPE_IGEP0020:
		setenv("fdtfile", "omap3-igep0020.dtb");
		break;
	case MACH_TYPE_IGEP0030:
		setenv("fdtfile", "omap3-igep0030.dtb");
		break;
	}
}

/*
 * Routine: misc_init_r
 * Description: Configure board specific parts
 */
int misc_init_r(void)
{
	twl4030_power_init();

	setup_net_chip();

	omap_die_id_display();

	set_fdt();

	return 0;
}

/*
 * Routine: set_muxconf_regs
 * Description: Setting up the configuration Mux registers specific to the
 *		hardware. Many pins need to be moved from protect to primary
 *		mode.
 */
void set_muxconf_regs(void)
{
	MUX_DEFAULT();

#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0020)
	MUX_IGEP0020();
#endif

#if (CONFIG_MACH_TYPE == MACH_TYPE_IGEP0030)
	MUX_IGEP0030();
#endif
}

#if defined(CONFIG_CMD_NET)
int board_eth_init(bd_t *bis)
{
#ifdef CONFIG_SMC911X
	return smc911x_initialize(0, CONFIG_SMC911X_BASE);
#else
	return 0;
#endif
}
#endif
