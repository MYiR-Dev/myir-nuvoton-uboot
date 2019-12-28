// SPDX-License-Identifier: GPL-2.0
/*
 * board/renesas/silk/silk.c
 *
 * Copyright (C) 2015 Renesas Electronics Corporation
 * Copyright (C) 2015 Cogent Embedded, Inc.
 */

#include <common.h>
#include <cpu_func.h>
#include <env.h>
#include <malloc.h>
#include <dm.h>
#include <dm/platform_data/serial_sh.h>
#include <env_internal.h>
#include <asm/processor.h>
#include <asm/mach-types.h>
#include <asm/io.h>
#include <linux/errno.h>
#include <asm/arch/sys_proto.h>
#include <asm/gpio.h>
#include <asm/arch/rmobile.h>
#include <asm/arch/rcar-mstp.h>
#include <asm/arch/mmc.h>
#include <asm/arch/sh_sdhi.h>
#include <netdev.h>
#include <miiphy.h>
#include <i2c.h>
#include <div64.h>
#include "qos.h"

DECLARE_GLOBAL_DATA_PTR;

void s_init(void)
{
	struct rcar_rwdt *rwdt = (struct rcar_rwdt *)RWDT_BASE;
	struct rcar_swdt *swdt = (struct rcar_swdt *)SWDT_BASE;

	/* Watchdog init */
	writel(0xA5A5A500, &rwdt->rwtcsra);
	writel(0xA5A5A500, &swdt->swtcsra);

	/* QoS */
	qos_init();
}

#define TMU0_MSTP125	BIT(25)
#define MMC0_MSTP315	BIT(15)

#define SD1CKCR		0xE6150078
#define SD_97500KHZ	0x7

int board_early_init_f(void)
{
	/* TMU */
	mstp_clrbits_le32(MSTPSR1, SMSTPCR1, TMU0_MSTP125);

	/* Set SD1 to the 97.5MHz */
	writel(SD_97500KHZ, SD1CKCR);

	return 0;
}

#define ETHERNET_PHY_RESET	56	/* GPIO 1 24 */

int board_init(void)
{
	/* adress of boot parameters */
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;

	/* Force ethernet PHY out of reset */
	gpio_request(ETHERNET_PHY_RESET, "phy_reset");
	gpio_direction_output(ETHERNET_PHY_RESET, 0);
	mdelay(20);
	gpio_direction_output(ETHERNET_PHY_RESET, 1);
	udelay(1);

	return 0;
}

int dram_init(void)
{
	if (fdtdec_setup_mem_size_base() != 0)
		return -EINVAL;

	return 0;
}

int dram_init_banksize(void)
{
	fdtdec_setup_memory_banksize();

	return 0;
}

/* porter has KSZ8041RNLI */
#define PHY_CONTROL1		0x1E
#define PHY_LED_MODE		0xC000
#define PHY_LED_MODE_ACK	0x4000
int board_phy_config(struct phy_device *phydev)
{
	int ret = phy_read(phydev, MDIO_DEVAD_NONE, PHY_CONTROL1);
	ret &= ~PHY_LED_MODE;
	ret |= PHY_LED_MODE_ACK;
	ret = phy_write(phydev, MDIO_DEVAD_NONE, PHY_CONTROL1, (u16)ret);

	return 0;
}

void reset_cpu(ulong addr)
{
	struct udevice *dev;
	const u8 pmic_bus = 1;
	const u8 pmic_addr = 0x5a;
	u8 data;
	int ret;

	ret = i2c_get_chip_for_busnum(pmic_bus, pmic_addr, 1, &dev);
	if (ret)
		hang();

	ret = dm_i2c_read(dev, 0x13, &data, 1);
	if (ret)
		hang();

	data |= BIT(1);

	ret = dm_i2c_write(dev, 0x13, &data, 1);
	if (ret)
		hang();
}

enum env_location env_get_location(enum env_operation op, int prio)
{
	const u32 load_magic = 0xb33fc0de;

	/* Block environment access if loaded using JTAG */
	if ((readl(CONFIG_SPL_TEXT_BASE + 0x24) == load_magic) &&
	    (op != ENVOP_INIT))
		return ENVL_UNKNOWN;

	if (prio)
		return ENVL_UNKNOWN;

	return ENVL_SPI_FLASH;
}
