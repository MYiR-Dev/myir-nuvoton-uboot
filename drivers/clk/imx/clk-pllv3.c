// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2019 DENX Software Engineering
 * Lukasz Majewski, DENX Software Engineering, lukma@denx.de
 */

#include <common.h>
#include <asm/io.h>
#include <malloc.h>
#include <clk-uclass.h>
#include <dm/device.h>
#include <dm/uclass.h>
#include <clk.h>
#include "clk.h"

#define UBOOT_DM_CLK_IMX_PLLV3_GENERIC	"imx_clk_pllv3_generic"
#define UBOOT_DM_CLK_IMX_PLLV3_SYS	"imx_clk_pllv3_sys"
#define UBOOT_DM_CLK_IMX_PLLV3_USB	"imx_clk_pllv3_usb"

#define BM_PLL_POWER		(0x1 << 12)
#define BM_PLL_LOCK		(0x1 << 31)

struct clk_pllv3 {
	struct clk	clk;
	void __iomem	*base;
	u32		power_bit;
	bool		powerup_set;
	u32		div_mask;
	u32		div_shift;
};

#define to_clk_pllv3(_clk) container_of(_clk, struct clk_pllv3, clk)

static ulong clk_pllv3_generic_get_rate(struct clk *clk)
{
	struct clk_pllv3 *pll = to_clk_pllv3(dev_get_clk_ptr(clk->dev));
	unsigned long parent_rate = clk_get_parent_rate(clk);

	u32 div = (readl(pll->base) >> pll->div_shift) & pll->div_mask;

	return (div == 1) ? parent_rate * 22 : parent_rate * 20;
}

static ulong clk_pllv3_generic_set_rate(struct clk *clk, ulong rate)
{
	struct clk_pllv3 *pll = to_clk_pllv3(clk);
	unsigned long parent_rate = clk_get_parent_rate(clk);
	u32 val, div;

	if (rate == parent_rate * 22)
		div = 1;
	else if (rate == parent_rate * 20)
		div = 0;
	else
		return -EINVAL;

	val = readl(pll->base);
	val &= ~(pll->div_mask << pll->div_shift);
	val |= (div << pll->div_shift);
	writel(val, pll->base);

	/* Wait for PLL to lock */
	while (!(readl(pll->base) & BM_PLL_LOCK))
		;

	return 0;
}

static int clk_pllv3_generic_enable(struct clk *clk)
{
	struct clk_pllv3 *pll = to_clk_pllv3(clk);
	u32 val;

	val = readl(pll->base);
	if (pll->powerup_set)
		val |= pll->power_bit;
	else
		val &= ~pll->power_bit;
	writel(val, pll->base);

	return 0;
}

static int clk_pllv3_generic_disable(struct clk *clk)
{
	struct clk_pllv3 *pll = to_clk_pllv3(clk);
	u32 val;

	val = readl(pll->base);
	if (pll->powerup_set)
		val &= ~pll->power_bit;
	else
		val |= pll->power_bit;
	writel(val, pll->base);

	return 0;
}

static const struct clk_ops clk_pllv3_generic_ops = {
	.get_rate	= clk_pllv3_generic_get_rate,
	.enable		= clk_pllv3_generic_enable,
	.disable	= clk_pllv3_generic_disable,
	.set_rate	= clk_pllv3_generic_set_rate,
};

static ulong clk_pllv3_sys_get_rate(struct clk *clk)
{
	struct clk_pllv3 *pll = to_clk_pllv3(clk);
	unsigned long parent_rate = clk_get_parent_rate(clk);
	u32 div = readl(pll->base) & pll->div_mask;

	return parent_rate * div / 2;
}

static ulong clk_pllv3_sys_set_rate(struct clk *clk, ulong rate)
{
	struct clk_pllv3 *pll = to_clk_pllv3(clk);
	unsigned long parent_rate = clk_get_parent_rate(clk);
	unsigned long min_rate = parent_rate * 54 / 2;
	unsigned long max_rate = parent_rate * 108 / 2;
	u32 val, div;

	if (rate < min_rate || rate > max_rate)
		return -EINVAL;

	div = rate * 2 / parent_rate;
	val = readl(pll->base);
	val &= ~pll->div_mask;
	val |= div;
	writel(val, pll->base);

	/* Wait for PLL to lock */
	while (!(readl(pll->base) & BM_PLL_LOCK))
		;

	return 0;
}

static const struct clk_ops clk_pllv3_sys_ops = {
	.enable 	= clk_pllv3_generic_enable,
	.disable	= clk_pllv3_generic_disable,
	.get_rate	= clk_pllv3_sys_get_rate,
	.set_rate	= clk_pllv3_sys_set_rate,
};

struct clk *imx_clk_pllv3(enum imx_pllv3_type type, const char *name,
			  const char *parent_name, void __iomem *base,
			  u32 div_mask)
{
	struct clk_pllv3 *pll;
	struct clk *clk;
	char *drv_name;
	int ret;

	pll = kzalloc(sizeof(*pll), GFP_KERNEL);
	if (!pll)
		return ERR_PTR(-ENOMEM);

	pll->power_bit = BM_PLL_POWER;

	switch (type) {
	case IMX_PLLV3_GENERIC:
		drv_name = UBOOT_DM_CLK_IMX_PLLV3_GENERIC;
		pll->div_shift = 0;
		pll->powerup_set = false;
		break;
	case IMX_PLLV3_SYS:
		drv_name = UBOOT_DM_CLK_IMX_PLLV3_SYS;
		pll->div_shift = 0;
		pll->powerup_set = false;
		break;
	case IMX_PLLV3_USB:
		drv_name = UBOOT_DM_CLK_IMX_PLLV3_USB;
		pll->div_shift = 1;
		pll->powerup_set = true;
		break;
	default:
		kfree(pll);
		return ERR_PTR(-ENOTSUPP);
	}

	pll->base = base;
	pll->div_mask = div_mask;
	clk = &pll->clk;

	ret = clk_register(clk, drv_name, name, parent_name);
	if (ret) {
		kfree(pll);
		return ERR_PTR(ret);
	}

	return clk;
}

U_BOOT_DRIVER(clk_pllv3_generic) = {
	.name	= UBOOT_DM_CLK_IMX_PLLV3_GENERIC,
	.id	= UCLASS_CLK,
	.ops	= &clk_pllv3_generic_ops,
	.flags = DM_FLAG_PRE_RELOC,
};

U_BOOT_DRIVER(clk_pllv3_sys) = {
	.name	= UBOOT_DM_CLK_IMX_PLLV3_SYS,
	.id	= UCLASS_CLK,
	.ops	= &clk_pllv3_sys_ops,
	.flags = DM_FLAG_PRE_RELOC,
};

U_BOOT_DRIVER(clk_pllv3_usb) = {
	.name	= UBOOT_DM_CLK_IMX_PLLV3_USB,
	.id	= UCLASS_CLK,
	.ops	= &clk_pllv3_generic_ops,
	.flags = DM_FLAG_PRE_RELOC,
};
