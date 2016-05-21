/*
 * Copyright (C) 2015-2016 Wills Wang <wills.wang@live.com>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/addrspace.h>
#include <asm/types.h>
#include <mach/ar71xx_regs.h>
#include <mach/ddr.h>
#include <debug_uart.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_DEBUG_UART_BOARD_INIT
void board_debug_uart_init(void)
{
	void __iomem *regs;
	u32 val;

	regs = map_physmem(AR71XX_GPIO_BASE, AR71XX_GPIO_SIZE,
			   MAP_NOCACHE);

	/*
	 * GPIO9 as input, GPIO10 as output
	 */
	val = readl(regs + AR71XX_GPIO_REG_OE);
	val &= ~AR933X_GPIO(9);
	val |= AR933X_GPIO(10);
	writel(val, regs + AR71XX_GPIO_REG_OE);

	/*
	 * Enable UART, GPIO9 as UART_SI, GPIO10 as UART_SO
	 */
	val = readl(regs + AR71XX_GPIO_REG_FUNC);
	val |= AR933X_GPIO_FUNC_UART_EN | AR933X_GPIO_FUNC_RES_TRUE;
	writel(val, regs + AR71XX_GPIO_REG_FUNC);
}
#endif

int board_early_init_f(void)
{
#ifdef CONFIG_DEBUG_UART
	debug_uart_init();
#endif
	ddr_init();
	return 0;
}
