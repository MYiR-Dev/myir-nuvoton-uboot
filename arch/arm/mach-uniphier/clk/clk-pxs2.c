/*
 * Copyright (C) 2015 Masahiro Yamada <yamada.masahiro@socionext.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <linux/io.h>

#include "../init.h"
#include "../sc-regs.h"

void uniphier_pxs2_clk_init(void)
{
	u32 tmp;

	/* deassert reset */
	tmp = readl(SC_RSTCTRL);
#ifdef CONFIG_USB_XHCI_UNIPHIER
	tmp |= SC_RSTCTRL_NRST_USB3B0 | SC_RSTCTRL_NRST_GIO;
#endif
#ifdef CONFIG_UNIPHIER_ETH
	tmp |= SC_RSTCTRL_NRST_ETHER;
#endif
#ifdef CONFIG_NAND_DENALI
	tmp |= SC_RSTCTRL_NRST_NAND;
#endif
	writel(tmp, SC_RSTCTRL);
	readl(SC_RSTCTRL); /* dummy read */

#ifdef CONFIG_USB_XHCI_UNIPHIER
	tmp = readl(SC_RSTCTRL2);
	tmp |= SC_RSTCTRL2_NRST_USB3B1;
	writel(tmp, SC_RSTCTRL2);
	readl(SC_RSTCTRL2); /* dummy read */
#endif

	/* privide clocks */
	tmp = readl(SC_CLKCTRL);
#ifdef CONFIG_USB_XHCI_UNIPHIER
	tmp |= SC_CLKCTRL_CEN_USB31 | SC_CLKCTRL_CEN_USB30 |
		SC_CLKCTRL_CEN_GIO;
#endif
#ifdef CONFIG_UNIPHIER_ETH
	tmp |= SC_CLKCTRL_CEN_ETHER;
#endif
#ifdef CONFIG_NAND_DENALI
	tmp |= SC_CLKCTRL_CEN_NAND;
#endif
	writel(tmp, SC_CLKCTRL);
	readl(SC_CLKCTRL); /* dummy read */
}
