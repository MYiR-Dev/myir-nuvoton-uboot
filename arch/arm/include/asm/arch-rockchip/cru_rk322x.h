/*
 * (C) Copyright 2017 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#ifndef _ASM_ARCH_CRU_RK322X_H
#define _ASM_ARCH_CRU_RK322X_H

#include <common.h>

#define MHz		1000000
#define OSC_HZ		(24 * MHz)

#define APLL_HZ		(600 * MHz)
#define GPLL_HZ		(594 * MHz)

#define CORE_PERI_HZ	150000000
#define CORE_ACLK_HZ	300000000

#define BUS_ACLK_HZ	148500000
#define BUS_HCLK_HZ	148500000
#define BUS_PCLK_HZ	74250000

#define PERI_ACLK_HZ	148500000
#define PERI_HCLK_HZ	148500000
#define PERI_PCLK_HZ	74250000

/* Private data for the clock driver - used by rockchip_get_cru() */
struct rk322x_clk_priv {
	struct rk322x_cru *cru;
	ulong rate;
};

struct rk322x_cru {
	struct rk322x_pll {
		unsigned int con0;
		unsigned int con1;
		unsigned int con2;
	} pll[4];
	unsigned int reserved0[4];
	unsigned int cru_mode_con;
	unsigned int cru_clksel_con[35];
	unsigned int cru_clkgate_con[16];
	unsigned int cru_softrst_con[9];
	unsigned int cru_misc_con;
	unsigned int reserved1[2];
	unsigned int cru_glb_cnt_th;
	unsigned int reserved2[3];
	unsigned int cru_glb_rst_st;
	unsigned int reserved3[(0x1c0 - 0x150) / 4 - 1];
	unsigned int cru_sdmmc_con[2];
	unsigned int cru_sdio_con[2];
	unsigned int reserved4[2];
	unsigned int cru_emmc_con[2];
	unsigned int reserved5[4];
	unsigned int cru_glb_srst_fst_value;
	unsigned int cru_glb_srst_snd_value;
	unsigned int cru_pll_mask_con;
};
check_member(rk322x_cru, cru_pll_mask_con, 0x01f8);

struct pll_div {
	u32 refdiv;
	u32 fbdiv;
	u32 postdiv1;
	u32 postdiv2;
	u32 frac;
};

enum {
	/* PLLCON0*/
	PLL_BP_SHIFT		= 15,
	PLL_POSTDIV1_SHIFT	= 12,
	PLL_POSTDIV1_MASK	= 7 << PLL_POSTDIV1_SHIFT,
	PLL_FBDIV_SHIFT		= 0,
	PLL_FBDIV_MASK		= 0xfff,

	/* PLLCON1 */
	PLL_RST_SHIFT		= 14,
	PLL_PD_SHIFT		= 13,
	PLL_PD_MASK		= 1 << PLL_PD_SHIFT,
	PLL_DSMPD_SHIFT		= 12,
	PLL_DSMPD_MASK		= 1 << PLL_DSMPD_SHIFT,
	PLL_LOCK_STATUS_SHIFT	= 10,
	PLL_LOCK_STATUS_MASK	= 1 << PLL_LOCK_STATUS_SHIFT,
	PLL_POSTDIV2_SHIFT	= 6,
	PLL_POSTDIV2_MASK	= 7 << PLL_POSTDIV2_SHIFT,
	PLL_REFDIV_SHIFT	= 0,
	PLL_REFDIV_MASK		= 0x3f,

	/* CRU_MODE */
	GPLL_MODE_SHIFT		= 12,
	GPLL_MODE_MASK		= 1 << GPLL_MODE_SHIFT,
	GPLL_MODE_SLOW		= 0,
	GPLL_MODE_NORM,
	CPLL_MODE_SHIFT		= 8,
	CPLL_MODE_MASK		= 1 << CPLL_MODE_SHIFT,
	CPLL_MODE_SLOW		= 0,
	CPLL_MODE_NORM,
	DPLL_MODE_SHIFT		= 4,
	DPLL_MODE_MASK		= 1 << DPLL_MODE_SHIFT,
	DPLL_MODE_SLOW		= 0,
	DPLL_MODE_NORM,
	APLL_MODE_SHIFT		= 0,
	APLL_MODE_MASK		= 1 << APLL_MODE_SHIFT,
	APLL_MODE_SLOW		= 0,
	APLL_MODE_NORM,

	/* CRU_CLK_SEL0_CON */
	BUS_ACLK_PLL_SEL_SHIFT	= 13,
	BUS_ACLK_PLL_SEL_MASK	= 3 << BUS_ACLK_PLL_SEL_SHIFT,
	BUS_ACLK_PLL_SEL_APLL	= 0,
	BUS_ACLK_PLL_SEL_GPLL,
	BUS_ACLK_PLL_SEL_HDMIPLL,
	BUS_ACLK_DIV_SHIFT	= 8,
	BUS_ACLK_DIV_MASK	= 0x1f << BUS_ACLK_DIV_SHIFT,
	CORE_CLK_PLL_SEL_SHIFT	= 6,
	CORE_CLK_PLL_SEL_MASK	= 3 << CORE_CLK_PLL_SEL_SHIFT,
	CORE_CLK_PLL_SEL_APLL	= 0,
	CORE_CLK_PLL_SEL_GPLL,
	CORE_CLK_PLL_SEL_DPLL,
	CORE_DIV_CON_SHIFT	= 0,
	CORE_DIV_CON_MASK	= 0x1f << CORE_DIV_CON_SHIFT,

	/* CRU_CLK_SEL1_CON */
	BUS_PCLK_DIV_SHIFT	= 12,
	BUS_PCLK_DIV_MASK	= 7 << BUS_PCLK_DIV_SHIFT,
	BUS_HCLK_DIV_SHIFT	= 8,
	BUS_HCLK_DIV_MASK	= 3 << BUS_HCLK_DIV_SHIFT,
	CORE_ACLK_DIV_SHIFT	= 4,
	CORE_ACLK_DIV_MASK	= 7 << CORE_ACLK_DIV_SHIFT,
	CORE_PERI_DIV_SHIFT	= 0,
	CORE_PERI_DIV_MASK	= 0xf << CORE_PERI_DIV_SHIFT,

	/* CRU_CLKSEL5_CON */
	GMAC_OUT_PLL_SHIFT	= 15,
	GMAC_OUT_PLL_MASK	= 1 << GMAC_OUT_PLL_SHIFT,
	GMAC_OUT_DIV_SHIFT	= 8,
	GMAC_OUT_DIV_MASK	= 0x1f << GMAC_OUT_DIV_SHIFT,
	MAC_PLL_SEL_SHIFT	= 7,
	MAC_PLL_SEL_MASK	= 1 << MAC_PLL_SEL_SHIFT,
	RMII_EXTCLK_SLE_SHIFT	= 5,
	RMII_EXTCLK_SEL_MASK	= 1 << RMII_EXTCLK_SLE_SHIFT,
	RMII_EXTCLK_SEL_INT		= 0,
	RMII_EXTCLK_SEL_EXT,
	CLK_MAC_DIV_SHIFT	= 0,
	CLK_MAC_DIV_MASK	= 0x1f << CLK_MAC_DIV_SHIFT,

	/* CRU_CLKSEL10_CON */
	PERI_PCLK_DIV_SHIFT	= 12,
	PERI_PCLK_DIV_MASK	= 7 << PERI_PCLK_DIV_SHIFT,
	PERI_PLL_SEL_SHIFT	= 10,
	PERI_PLL_SEL_MASK	= 3 << PERI_PLL_SEL_SHIFT,
	PERI_PLL_CPLL		= 0,
	PERI_PLL_GPLL,
	PERI_PLL_HDMIPLL,
	PERI_HCLK_DIV_SHIFT	= 8,
	PERI_HCLK_DIV_MASK	= 3 << PERI_HCLK_DIV_SHIFT,
	PERI_ACLK_DIV_SHIFT	= 0,
	PERI_ACLK_DIV_MASK	= 0x1f << PERI_ACLK_DIV_SHIFT,

	/* CRU_CLKSEL11_CON */
	EMMC_PLL_SHIFT		= 12,
	EMMC_PLL_MASK		= 3 << EMMC_PLL_SHIFT,
	EMMC_SEL_APLL		= 0,
	EMMC_SEL_DPLL,
	EMMC_SEL_GPLL,
	EMMC_SEL_24M,
	SDIO_PLL_SHIFT		= 10,
	SDIO_PLL_MASK		= 3 << SDIO_PLL_SHIFT,
	SDIO_SEL_APLL		= 0,
	SDIO_SEL_DPLL,
	SDIO_SEL_GPLL,
	SDIO_SEL_24M,
	MMC0_PLL_SHIFT		= 8,
	MMC0_PLL_MASK		= 3 << MMC0_PLL_SHIFT,
	MMC0_SEL_APLL		= 0,
	MMC0_SEL_DPLL,
	MMC0_SEL_GPLL,
	MMC0_SEL_24M,
	MMC0_DIV_SHIFT		= 0,
	MMC0_DIV_MASK		= 0xff << MMC0_DIV_SHIFT,

	/* CRU_CLKSEL12_CON */
	EMMC_DIV_SHIFT		= 8,
	EMMC_DIV_MASK		= 0xff << EMMC_DIV_SHIFT,
	SDIO_DIV_SHIFT		= 0,
	SDIO_DIV_MASK		= 0xff << SDIO_DIV_SHIFT,

	/* CRU_CLKSEL26_CON */
	DDR_CLK_PLL_SEL_SHIFT	= 8,
	DDR_CLK_PLL_SEL_MASK	= 3 << DDR_CLK_PLL_SEL_SHIFT,
	DDR_CLK_SEL_DPLL	= 0,
	DDR_CLK_SEL_GPLL,
	DDR_CLK_SEL_APLL,
	DDR_DIV_SEL_SHIFT	= 0,
	DDR_DIV_SEL_MASK	= 3 << DDR_DIV_SEL_SHIFT,

	/* CRU_CLKSEL27_CON */
	VOP_DCLK_DIV_SHIFT	= 8,
	VOP_DCLK_DIV_MASK	= 0xff << VOP_DCLK_DIV_SHIFT,
	VOP_PLL_SEL_SHIFT	= 1,
	VOP_PLL_SEL_MASK	= 1 << VOP_PLL_SEL_SHIFT,

	/* CRU_CLKSEL29_CON */
	GMAC_CLK_SRC_SHIFT	= 12,
	GMAC_CLK_SRC_MASK	= 1 << GMAC_CLK_SRC_SHIFT,

	/* CRU_SOFTRST5_CON */
	DDRCTRL_PSRST_SHIFT	= 11,
	DDRCTRL_SRST_SHIFT	= 10,
	DDRPHY_PSRST_SHIFT	= 9,
	DDRPHY_SRST_SHIFT	= 8,
};
#endif
