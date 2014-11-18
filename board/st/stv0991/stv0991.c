/*
 * (C) Copyright 2014
 * Vikas Manocha, ST Micoelectronics, vikas.manocha@st.com.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <miiphy.h>
#include <asm/arch/stv0991_periph.h>
#include <asm/arch/stv0991_defs.h>
#include <asm/arch/hardware.h>
#include <asm/arch/gpio.h>
#include <netdev.h>
#include <asm/io.h>

DECLARE_GLOBAL_DATA_PTR;

struct gpio_regs *const gpioa_regs =
		(struct gpio_regs *) GPIOA_BASE_ADDR;

#ifdef CONFIG_SHOW_BOOT_PROGRESS
void show_boot_progress(int progress)
{
	printf("%i\n", progress);
}
#endif

void enable_eth_phy(void)
{
	/* Set GPIOA_06 pad HIGH (Appli board)*/
	writel(readl(&gpioa_regs->dir) | 0x40, &gpioa_regs->dir);
	writel(readl(&gpioa_regs->data) | 0x40, &gpioa_regs->data);
}
int board_eth_enable(void)
{
	stv0991_pinmux_config(ETH_GPIOB_10_31_C_0_4);
	clock_setup(ETH_CLOCK_CFG);
	enable_eth_phy();
	return 0;
}

/*
 * Miscellaneous platform dependent initialisations
 */
int board_init(void)
{
	board_eth_enable();
	return 0;
}

int board_uart_init(void)
{
	stv0991_pinmux_config(UART_GPIOC_30_31);
	clock_setup(UART_CLOCK_CFG);
	return 0;
}

#ifdef CONFIG_BOARD_EARLY_INIT_F
int board_early_init_f(void)
{
	board_uart_init();
	return 0;
}
#endif

int dram_init(void)
{
	gd->ram_size = PHYS_SDRAM_1_SIZE;
	return 0;
}

void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
}

#ifdef CONFIG_CMD_NET
int board_eth_init(bd_t *bis)
{
	int ret = 0;

#if defined(CONFIG_DESIGNWARE_ETH)
	u32 interface = PHY_INTERFACE_MODE_MII;
	if (designware_initialize(GMAC_BASE_ADDR, interface) >= 0)
		ret++;
#endif
	return ret;
}
#endif
