// SPDX-License-Identifier: GPL-2.0
/*
 * r8a77965 Clock Pulse Generator / Module Standby and Software Reset
 *
 * Copyright (C) 2018 Jacopo Mondi <jacopo+renesas@jmondi.org>
 *
 * Based on r8a7795-cpg-mssr.c
 *
 * Copyright (C) 2015 Glider bvba
 * Copyright (C) 2015 Renesas Electronics Corp.
 */

#include <common.h>
#include <clk-uclass.h>
#include <dm.h>

#include <dt-bindings/clock/r8a77965-cpg-mssr.h>

#include "renesas-cpg-mssr.h"
#include "rcar-gen3-cpg.h"

enum clk_ids {
	/* Core Clock Outputs exported to DT */
	LAST_DT_CORE_CLK = R8A77965_CLK_OSC,

	/* External Input Clocks */
	CLK_EXTAL,
	CLK_EXTALR,

	/* Internal Core Clocks */
	CLK_MAIN,
	CLK_PLL0,
	CLK_PLL1,
	CLK_PLL3,
	CLK_PLL4,
	CLK_PLL1_DIV2,
	CLK_PLL1_DIV4,
	CLK_S0,
	CLK_S1,
	CLK_S2,
	CLK_S3,
	CLK_SDSRC,
	CLK_RPCSRC,
	CLK_SSPSRC,
	CLK_RINT,

	/* Module Clocks */
	MOD_CLK_BASE
};

static const struct cpg_core_clk r8a77965_core_clks[] = {
	/* External Clock Inputs */
	DEF_INPUT("extal",      CLK_EXTAL),
	DEF_INPUT("extalr",     CLK_EXTALR),

	/* Internal Core Clocks */
	DEF_BASE(".main",	CLK_MAIN, CLK_TYPE_GEN3_MAIN, CLK_EXTAL),
	DEF_BASE(".pll0",	CLK_PLL0, CLK_TYPE_GEN3_PLL0, CLK_MAIN),
	DEF_BASE(".pll1",	CLK_PLL1, CLK_TYPE_GEN3_PLL1, CLK_MAIN),
	DEF_BASE(".pll3",	CLK_PLL3, CLK_TYPE_GEN3_PLL3, CLK_MAIN),
	DEF_BASE(".pll4",	CLK_PLL4, CLK_TYPE_GEN3_PLL4, CLK_MAIN),

	DEF_FIXED(".pll1_div2", CLK_PLL1_DIV2,		CLK_PLL1,	2, 1),
	DEF_FIXED(".pll1_div4", CLK_PLL1_DIV4,		CLK_PLL1_DIV2,	2, 1),
	DEF_FIXED(".s0",	CLK_S0,			CLK_PLL1_DIV2,	2, 1),
	DEF_FIXED(".s1",	CLK_S1,			CLK_PLL1_DIV2,	3, 1),
	DEF_FIXED(".s2",	CLK_S2,			CLK_PLL1_DIV2,	4, 1),
	DEF_FIXED(".s3",	CLK_S3,			CLK_PLL1_DIV2,	6, 1),
	DEF_FIXED(".sdsrc",	CLK_SDSRC,		CLK_PLL1_DIV2,	2, 1),
	DEF_FIXED(".rpcsrc",	CLK_RPCSRC,		CLK_PLL1,	2, 1),

	DEF_GEN3_OSC(".r",	CLK_RINT,		CLK_EXTAL,	32),

	/* Core Clock Outputs */
	DEF_BASE("z",		R8A77965_CLK_Z,		CLK_TYPE_GEN3_Z, CLK_PLL0),
	DEF_FIXED("ztr",	R8A77965_CLK_ZTR,	CLK_PLL1_DIV2,	6, 1),
	DEF_FIXED("ztrd2",	R8A77965_CLK_ZTRD2,	CLK_PLL1_DIV2,	12, 1),
	DEF_FIXED("zt",		R8A77965_CLK_ZT,	CLK_PLL1_DIV2,	4, 1),
	DEF_FIXED("zx",		R8A77965_CLK_ZX,	CLK_PLL1_DIV2,	2, 1),
	DEF_FIXED("s0d1",	R8A77965_CLK_S0D1,	CLK_S0,		1, 1),
	DEF_FIXED("s0d2",	R8A77965_CLK_S0D2,	CLK_S0,		2, 1),
	DEF_FIXED("s0d3",	R8A77965_CLK_S0D3,	CLK_S0,		3, 1),
	DEF_FIXED("s0d4",	R8A77965_CLK_S0D4,	CLK_S0,		4, 1),
	DEF_FIXED("s0d6",	R8A77965_CLK_S0D6,	CLK_S0,		6, 1),
	DEF_FIXED("s0d8",	R8A77965_CLK_S0D8,	CLK_S0,		8, 1),
	DEF_FIXED("s0d12",	R8A77965_CLK_S0D12,	CLK_S0,		12, 1),
	DEF_FIXED("s1d1",	R8A77965_CLK_S1D1,	CLK_S1,		1, 1),
	DEF_FIXED("s1d2",	R8A77965_CLK_S1D2,	CLK_S1,		2, 1),
	DEF_FIXED("s1d4",	R8A77965_CLK_S1D4,	CLK_S1,		4, 1),
	DEF_FIXED("s2d1",	R8A77965_CLK_S2D1,	CLK_S2,		1, 1),
	DEF_FIXED("s2d2",	R8A77965_CLK_S2D2,	CLK_S2,		2, 1),
	DEF_FIXED("s2d4",	R8A77965_CLK_S2D4,	CLK_S2,		4, 1),
	DEF_FIXED("s3d1",	R8A77965_CLK_S3D1,	CLK_S3,		1, 1),
	DEF_FIXED("s3d2",	R8A77965_CLK_S3D2,	CLK_S3,		2, 1),
	DEF_FIXED("s3d4",	R8A77965_CLK_S3D4,	CLK_S3,		4, 1),

	DEF_GEN3_SD("sd0",	R8A77965_CLK_SD0,	CLK_SDSRC,	0x074),
	DEF_GEN3_SD("sd1",	R8A77965_CLK_SD1,	CLK_SDSRC,	0x078),
	DEF_GEN3_SD("sd2",	R8A77965_CLK_SD2,	CLK_SDSRC,	0x268),
	DEF_GEN3_SD("sd3",	R8A77965_CLK_SD3,	CLK_SDSRC,	0x26c),

	DEF_GEN3_RPC("rpc",	R8A77965_CLK_RPC,	CLK_RPCSRC,	0x238),

	DEF_FIXED("cl",		R8A77965_CLK_CL,	CLK_PLL1_DIV2,	48, 1),
	DEF_FIXED("cp",		R8A77965_CLK_CP,	CLK_EXTAL,	2, 1),
	DEF_FIXED("cpex",	R8A77965_CLK_CPEX,	CLK_EXTAL,	2, 1),

	DEF_DIV6P1("canfd",	R8A77965_CLK_CANFD,	CLK_PLL1_DIV4,	0x244),
	DEF_DIV6P1("csi0",	R8A77965_CLK_CSI0,	CLK_PLL1_DIV4,	0x00c),
	DEF_DIV6P1("mso",	R8A77965_CLK_MSO,	CLK_PLL1_DIV4,	0x014),
	DEF_DIV6P1("hdmi",	R8A77965_CLK_HDMI,	CLK_PLL1_DIV4,	0x250),

	DEF_GEN3_OSC("osc",	R8A77965_CLK_OSC,	CLK_EXTAL,	8),

	DEF_BASE("r",		R8A77965_CLK_R,	CLK_TYPE_GEN3_R, CLK_RINT),
};

static const struct mssr_mod_clk r8a77965_mod_clks[] = {
	DEF_MOD("fdp1-0",		119,	R8A77965_CLK_S0D1),
	DEF_MOD("scif5",		202,	R8A77965_CLK_S3D4),
	DEF_MOD("scif4",		203,	R8A77965_CLK_S3D4),
	DEF_MOD("scif3",		204,	R8A77965_CLK_S3D4),
	DEF_MOD("scif1",		206,	R8A77965_CLK_S3D4),
	DEF_MOD("scif0",		207,	R8A77965_CLK_S3D4),
	DEF_MOD("msiof3",		208,	R8A77965_CLK_MSO),
	DEF_MOD("msiof2",		209,	R8A77965_CLK_MSO),
	DEF_MOD("msiof1",		210,	R8A77965_CLK_MSO),
	DEF_MOD("msiof0",		211,	R8A77965_CLK_MSO),
	DEF_MOD("sys-dmac2",		217,	R8A77965_CLK_S0D3),
	DEF_MOD("sys-dmac1",		218,	R8A77965_CLK_S0D3),
	DEF_MOD("sys-dmac0",		219,	R8A77965_CLK_S0D3),

	DEF_MOD("cmt3",			300,	R8A77965_CLK_R),
	DEF_MOD("cmt2",			301,	R8A77965_CLK_R),
	DEF_MOD("cmt1",			302,	R8A77965_CLK_R),
	DEF_MOD("cmt0",			303,	R8A77965_CLK_R),
	DEF_MOD("scif2",		310,	R8A77965_CLK_S3D4),
	DEF_MOD("sdif3",		311,	R8A77965_CLK_SD3),
	DEF_MOD("sdif2",		312,	R8A77965_CLK_SD2),
	DEF_MOD("sdif1",		313,	R8A77965_CLK_SD1),
	DEF_MOD("sdif0",		314,	R8A77965_CLK_SD0),
	DEF_MOD("pcie1",		318,	R8A77965_CLK_S3D1),
	DEF_MOD("pcie0",		319,	R8A77965_CLK_S3D1),
	DEF_MOD("usb3-if0",		328,	R8A77965_CLK_S3D1),
	DEF_MOD("usb-dmac0",		330,	R8A77965_CLK_S3D1),
	DEF_MOD("usb-dmac1",		331,	R8A77965_CLK_S3D1),

	DEF_MOD("rwdt",			402,	R8A77965_CLK_R),
	DEF_MOD("intc-ex",		407,	R8A77965_CLK_CP),
	DEF_MOD("intc-ap",		408,	R8A77965_CLK_S0D3),

	DEF_MOD("audmac1",		501,	R8A77965_CLK_S0D3),
	DEF_MOD("audmac0",		502,	R8A77965_CLK_S0D3),
	DEF_MOD("drif7",		508,	R8A77965_CLK_S3D2),
	DEF_MOD("drif6",		509,	R8A77965_CLK_S3D2),
	DEF_MOD("drif5",		510,	R8A77965_CLK_S3D2),
	DEF_MOD("drif4",		511,	R8A77965_CLK_S3D2),
	DEF_MOD("drif3",		512,	R8A77965_CLK_S3D2),
	DEF_MOD("drif2",		513,	R8A77965_CLK_S3D2),
	DEF_MOD("drif1",		514,	R8A77965_CLK_S3D2),
	DEF_MOD("drif0",		515,	R8A77965_CLK_S3D2),
	DEF_MOD("hscif4",		516,	R8A77965_CLK_S3D1),
	DEF_MOD("hscif3",		517,	R8A77965_CLK_S3D1),
	DEF_MOD("hscif2",		518,	R8A77965_CLK_S3D1),
	DEF_MOD("hscif1",		519,	R8A77965_CLK_S3D1),
	DEF_MOD("hscif0",		520,	R8A77965_CLK_S3D1),
	DEF_MOD("thermal",		522,	R8A77965_CLK_CP),
	DEF_MOD("pwm",			523,	R8A77965_CLK_S0D12),

	DEF_MOD("fcpvd1",		602,	R8A77965_CLK_S0D2),
	DEF_MOD("fcpvd0",		603,	R8A77965_CLK_S0D2),
	DEF_MOD("fcpvb0",		607,	R8A77965_CLK_S0D1),
	DEF_MOD("fcpvi0",		611,	R8A77965_CLK_S0D1),
	DEF_MOD("fcpf0",		615,	R8A77965_CLK_S0D1),
	DEF_MOD("fcpcs",		619,	R8A77965_CLK_S0D2),
	DEF_MOD("vspd1",		622,	R8A77965_CLK_S0D2),
	DEF_MOD("vspd0",		623,	R8A77965_CLK_S0D2),
	DEF_MOD("vspb",			626,	R8A77965_CLK_S0D1),
	DEF_MOD("vspi0",		631,	R8A77965_CLK_S0D1),

	DEF_MOD("ehci1",		702,	R8A77965_CLK_S3D4),
	DEF_MOD("ehci0",		703,	R8A77965_CLK_S3D4),
	DEF_MOD("hsusb",		704,	R8A77965_CLK_S3D4),
	DEF_MOD("csi20",		714,	R8A77965_CLK_CSI0),
	DEF_MOD("csi40",		716,	R8A77965_CLK_CSI0),
	DEF_MOD("du3",			721,	R8A77965_CLK_S2D1),
	DEF_MOD("du1",			723,	R8A77965_CLK_S2D1),
	DEF_MOD("du0",			724,	R8A77965_CLK_S2D1),
	DEF_MOD("lvds",			727,	R8A77965_CLK_S2D1),
	DEF_MOD("hdmi0",		729,	R8A77965_CLK_HDMI),

	DEF_MOD("vin7",			804,	R8A77965_CLK_S0D2),
	DEF_MOD("vin6",			805,	R8A77965_CLK_S0D2),
	DEF_MOD("vin5",			806,	R8A77965_CLK_S0D2),
	DEF_MOD("vin4",			807,	R8A77965_CLK_S0D2),
	DEF_MOD("vin3",			808,	R8A77965_CLK_S0D2),
	DEF_MOD("vin2",			809,	R8A77965_CLK_S0D2),
	DEF_MOD("vin1",			810,	R8A77965_CLK_S0D2),
	DEF_MOD("vin0",			811,	R8A77965_CLK_S0D2),
	DEF_MOD("etheravb",		812,	R8A77965_CLK_S0D6),
	DEF_MOD("sata0",		815,	R8A77965_CLK_S3D2),
	DEF_MOD("imr1",			822,	R8A77965_CLK_S0D2),
	DEF_MOD("imr0",			823,	R8A77965_CLK_S0D2),

	DEF_MOD("gpio7",		905,	R8A77965_CLK_S3D4),
	DEF_MOD("gpio6",		906,	R8A77965_CLK_S3D4),
	DEF_MOD("gpio5",		907,	R8A77965_CLK_S3D4),
	DEF_MOD("gpio4",		908,	R8A77965_CLK_S3D4),
	DEF_MOD("gpio3",		909,	R8A77965_CLK_S3D4),
	DEF_MOD("gpio2",		910,	R8A77965_CLK_S3D4),
	DEF_MOD("gpio1",		911,	R8A77965_CLK_S3D4),
	DEF_MOD("gpio0",		912,	R8A77965_CLK_S3D4),
	DEF_MOD("can-fd",		914,	R8A77965_CLK_S3D2),
	DEF_MOD("can-if1",		915,	R8A77965_CLK_S3D4),
	DEF_MOD("can-if0",		916,	R8A77965_CLK_S3D4),
	DEF_MOD("rpc",			917,	R8A77965_CLK_RPC),
	DEF_MOD("i2c6",			918,	R8A77965_CLK_S0D6),
	DEF_MOD("i2c5",			919,	R8A77965_CLK_S0D6),
	DEF_MOD("i2c-dvfs",		926,	R8A77965_CLK_CP),
	DEF_MOD("i2c4",			927,	R8A77965_CLK_S0D6),
	DEF_MOD("i2c3",			928,	R8A77965_CLK_S0D6),
	DEF_MOD("i2c2",			929,	R8A77965_CLK_S3D2),
	DEF_MOD("i2c1",			930,	R8A77965_CLK_S3D2),
	DEF_MOD("i2c0",			931,	R8A77965_CLK_S3D2),

	DEF_MOD("ssi-all",		1005,	R8A77965_CLK_S3D4),
	DEF_MOD("ssi9",			1006,	MOD_CLK_ID(1005)),
	DEF_MOD("ssi8",			1007,	MOD_CLK_ID(1005)),
	DEF_MOD("ssi7",			1008,	MOD_CLK_ID(1005)),
	DEF_MOD("ssi6",			1009,	MOD_CLK_ID(1005)),
	DEF_MOD("ssi5",			1010,	MOD_CLK_ID(1005)),
	DEF_MOD("ssi4",			1011,	MOD_CLK_ID(1005)),
	DEF_MOD("ssi3",			1012,	MOD_CLK_ID(1005)),
	DEF_MOD("ssi2",			1013,	MOD_CLK_ID(1005)),
	DEF_MOD("ssi1",			1014,	MOD_CLK_ID(1005)),
	DEF_MOD("ssi0",			1015,	MOD_CLK_ID(1005)),
	DEF_MOD("scu-all",		1017,	R8A77965_CLK_S3D4),
	DEF_MOD("scu-dvc1",		1018,	MOD_CLK_ID(1017)),
	DEF_MOD("scu-dvc0",		1019,	MOD_CLK_ID(1017)),
	DEF_MOD("scu-ctu1-mix1",	1020,	MOD_CLK_ID(1017)),
	DEF_MOD("scu-ctu0-mix0",	1021,	MOD_CLK_ID(1017)),
	DEF_MOD("scu-src9",		1022,	MOD_CLK_ID(1017)),
	DEF_MOD("scu-src8",		1023,	MOD_CLK_ID(1017)),
	DEF_MOD("scu-src7",		1024,	MOD_CLK_ID(1017)),
	DEF_MOD("scu-src6",		1025,	MOD_CLK_ID(1017)),
	DEF_MOD("scu-src5",		1026,	MOD_CLK_ID(1017)),
	DEF_MOD("scu-src4",		1027,	MOD_CLK_ID(1017)),
	DEF_MOD("scu-src3",		1028,	MOD_CLK_ID(1017)),
	DEF_MOD("scu-src2",		1029,	MOD_CLK_ID(1017)),
	DEF_MOD("scu-src1",		1030,	MOD_CLK_ID(1017)),
	DEF_MOD("scu-src0",		1031,	MOD_CLK_ID(1017)),
};

/*
 * CPG Clock Data
 */

/*
 *   MD		EXTAL		PLL0	PLL1	PLL3	PLL4	OSC
 * 14 13 19 17	(MHz)
 *-----------------------------------------------------------------
 * 0  0  0  0	16.66 x 1	x180	x192	x192	x144	/16
 * 0  0  0  1	16.66 x 1	x180	x192	x128	x144	/16
 * 0  0  1  0	Prohibited setting
 * 0  0  1  1	16.66 x 1	x180	x192	x192	x144	/16
 * 0  1  0  0	20    x 1	x150	x160	x160	x120	/19
 * 0  1  0  1	20    x 1	x150	x160	x106	x120	/19
 * 0  1  1  0	Prohibited setting
 * 0  1  1  1	20    x 1	x150	x160	x160	x120	/19
 * 1  0  0  0	25    x 1	x120	x128	x128	x96	/24
 * 1  0  0  1	25    x 1	x120	x128	x84	x96	/24
 * 1  0  1  0	Prohibited setting
 * 1  0  1  1	25    x 1	x120	x128	x128	x96	/24
 * 1  1  0  0	33.33 / 2	x180	x192	x192	x144	/32
 * 1  1  0  1	33.33 / 2	x180	x192	x128	x144	/32
 * 1  1  1  0	Prohibited setting
 * 1  1  1  1	33.33 / 2	x180	x192	x192	x144	/32
 */
#define CPG_PLL_CONFIG_INDEX(md)	((((md) & BIT(14)) >> 11) | \
					 (((md) & BIT(13)) >> 11) | \
					 (((md) & BIT(19)) >> 18) | \
					 (((md) & BIT(17)) >> 17))

static const struct rcar_gen3_cpg_pll_config cpg_pll_configs[16] = {
	/* EXTAL div	PLL1 mult/div	PLL3 mult/div	OSC prediv */
	{ 1,		192,	1,	192,	1,	16,	},
	{ 1,		192,	1,	128,	1,	16,	},
	{ 0, /* Prohibited setting */				},
	{ 1,		192,	1,	192,	1,	16,	},
	{ 1,		160,	1,	160,	1,	19,	},
	{ 1,		160,	1,	106,	1,	19,	},
	{ 0, /* Prohibited setting */				},
	{ 1,		160,	1,	160,	1,	19,	},
	{ 1,		128,	1,	128,	1,	24,	},
	{ 1,		128,	1,	84,	1,	24,	},
	{ 0, /* Prohibited setting */				},
	{ 1,		128,	1,	128,	1,	24,	},
	{ 2,		192,	1,	192,	1,	32,	},
	{ 2,		192,	1,	128,	1,	32,	},
	{ 0, /* Prohibited setting */				},
	{ 2,		192,	1,	192,	1,	32,	},
};

static const struct mstp_stop_table r8a77965_mstp_table[] = {
	{ 0x00200000, 0x0, 0x00200000, 0 },
	{ 0xFFFFFFFF, 0x0, 0xFFFFFFFF, 0 },
	{ 0x340E2FDC, 0x2040, 0x340E2FDC, 0 },
	{ 0xFFFFFFDF, 0x400, 0xFFFFFFDF, 0 },
	{ 0x80000184, 0x180, 0x80000184, 0 },
	{ 0xC3FFFFFF, 0x0, 0xC3FFFFFF, 0 },
	{ 0xFFFFFFFF, 0x0, 0xFFFFFFFF, 0 },
	{ 0xFFFFFFFF, 0x0, 0xFFFFFFFF, 0 },
	{ 0x01F1FFF7, 0x0, 0x01F1FFF7, 0 },
	{ 0xFFFFFFFE, 0x0, 0xFFFFFFFE, 0 },
	{ 0xFFFEFFE0, 0x0, 0xFFFEFFE0, 0 },
	{ 0x000000B7, 0x0, 0x000000B7, 0 },
};

static const void *r8a77965_get_pll_config(const u32 cpg_mode)
{
	return &cpg_pll_configs[CPG_PLL_CONFIG_INDEX(cpg_mode)];
}

static const struct cpg_mssr_info r8a77965_cpg_mssr_info = {
	.core_clk		= r8a77965_core_clks,
	.core_clk_size		= ARRAY_SIZE(r8a77965_core_clks),
	.mod_clk		= r8a77965_mod_clks,
	.mod_clk_size		= ARRAY_SIZE(r8a77965_mod_clks),
	.mstp_table		= r8a77965_mstp_table,
	.mstp_table_size	= ARRAY_SIZE(r8a77965_mstp_table),
	.reset_node		= "renesas,r8a77965-rst",
	.extalr_node		= "extalr",
	.mod_clk_base		= MOD_CLK_BASE,
	.clk_extal_id		= CLK_EXTAL,
	.clk_extalr_id		= CLK_EXTALR,
	.get_pll_config		= r8a77965_get_pll_config,
};

static const struct udevice_id r8a77965_clk_ids[] = {
	{
		.compatible	= "renesas,r8a77965-cpg-mssr",
		.data		= (ulong)&r8a77965_cpg_mssr_info,
	},
	{ }
};

U_BOOT_DRIVER(clk_r8a77965) = {
	.name		= "clk_r8a77965",
	.id		= UCLASS_CLK,
	.of_match	= r8a77965_clk_ids,
	.priv_auto_alloc_size = sizeof(struct gen3_clk_priv),
	.ops		= &gen3_clk_ops,
	.probe		= gen3_clk_probe,
	.remove		= gen3_clk_remove,
};
