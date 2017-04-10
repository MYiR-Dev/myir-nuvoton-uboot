/*
 * (C) Copyright 2016
 * Vikas Manocha, <vikas.manocha@st.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <ram.h>
#include <asm/io.h>
#include <asm/armv7m.h>
#include <asm/arch/stm32.h>
#include <asm/arch/gpio.h>
#include <dm/platdata.h>
#include <dm/platform_data/serial_stm32x7.h>
#include <asm/arch/stm32_periph.h>
#include <asm/arch/stm32_defs.h>
#include <asm/arch/syscfg.h>

DECLARE_GLOBAL_DATA_PTR;

const struct stm32_gpio_ctl gpio_ctl_gpout = {
	.mode = STM32_GPIO_MODE_OUT,
	.otype = STM32_GPIO_OTYPE_PP,
	.speed = STM32_GPIO_SPEED_50M,
	.pupd = STM32_GPIO_PUPD_NO,
	.af = STM32_GPIO_AF0
};

static int fmc_setup_gpio(void)
{
	clock_setup(GPIO_B_CLOCK_CFG);
	clock_setup(GPIO_C_CLOCK_CFG);
	clock_setup(GPIO_D_CLOCK_CFG);
	clock_setup(GPIO_E_CLOCK_CFG);
	clock_setup(GPIO_F_CLOCK_CFG);
	clock_setup(GPIO_G_CLOCK_CFG);
	clock_setup(GPIO_H_CLOCK_CFG);

	return 0;
}

int dram_init(void)
{
	struct udevice *dev;
	struct ram_info ram;
	int rv;

	rv = fmc_setup_gpio();
	if (rv)
		return rv;

	clock_setup(FMC_CLOCK_CFG);

	rv = uclass_get_device(UCLASS_RAM, 0, &dev);
	if (rv) {
		debug("DRAM init failed: %d\n", rv);
		return rv;
	}
	rv = ram_get_info(dev, &ram);
	if (rv) {
		debug("Cannot get DRAM size: %d\n", rv);
		return rv;
	}
	debug("SDRAM base=%lx, size=%x\n", ram.base, ram.size);
	gd->ram_size = ram.size;

	/*
	 * Fill in global info with description of SRAM configuration
	 */
	gd->bd->bi_dram[0].start = CONFIG_SYS_RAM_BASE;
	gd->bd->bi_dram[0].size  = ram.size;

	return rv;
}

int uart_setup_gpio(void)
{
	clock_setup(GPIO_A_CLOCK_CFG);
	clock_setup(GPIO_B_CLOCK_CFG);
	return 0;
}

#ifdef CONFIG_ETH_DESIGNWARE

static int stmmac_setup(void)
{
	clock_setup(SYSCFG_CLOCK_CFG);
	/* Set >RMII mode */
	STM32_SYSCFG->pmc |= SYSCFG_PMC_MII_RMII_SEL;

	clock_setup(GPIO_A_CLOCK_CFG);
	clock_setup(GPIO_C_CLOCK_CFG);
	clock_setup(GPIO_G_CLOCK_CFG);
	clock_setup(STMMAC_CLOCK_CFG);

	return 0;
}
#endif

#ifdef CONFIG_STM32_QSPI

static int qspi_setup(void)
{
	clock_setup(GPIO_B_CLOCK_CFG);
	clock_setup(GPIO_D_CLOCK_CFG);
	clock_setup(GPIO_E_CLOCK_CFG);
	return 0;
}
#endif

u32 get_board_rev(void)
{
	return 0;
}

int board_early_init_f(void)
{
	int res;

	res = uart_setup_gpio();
	if (res)
		return res;

#ifdef CONFIG_ETH_DESIGNWARE
	res = stmmac_setup();
	if (res)
		return res;
#endif

#ifdef CONFIG_STM32_QSPI
	res = qspi_setup();
	if (res)
		return res;
#endif

	return 0;
}

int board_init(void)
{
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;

	return 0;
}
