/*
 * Olimex MX23 Olinuxino Boot setup
 *
 * Copyright (C) 2013 Marek Vasut <marex@denx.de>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <config.h>
#include <asm/io.h>
#include <asm/arch/iomux-mx23.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/sys_proto.h>

#define	MUX_CONFIG_EMI	(MXS_PAD_3V3 | MXS_PAD_12MA | MXS_PAD_PULLUP)
#define	MUX_CONFIG_SSP	(MXS_PAD_8MA | MXS_PAD_PULLUP)

const iomux_cfg_t iomux_setup[] = {
	/* DUART */
	MX23_PAD_PWM0__DUART_RX,
	MX23_PAD_PWM1__DUART_TX,

	/* EMI */
	MX23_PAD_EMI_D00__EMI_D00 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_D01__EMI_D01 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_D02__EMI_D02 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_D03__EMI_D03 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_D04__EMI_D04 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_D05__EMI_D05 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_D06__EMI_D06 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_D07__EMI_D07 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_D08__EMI_D08 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_D09__EMI_D09 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_D10__EMI_D10 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_D11__EMI_D11 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_D12__EMI_D12 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_D13__EMI_D13 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_D14__EMI_D14 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_D15__EMI_D15 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_DQM0__EMI_DQM0 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_DQM1__EMI_DQM1 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_DQS0__EMI_DQS0 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_DQS1__EMI_DQS1 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_CLK__EMI_CLK | MUX_CONFIG_EMI,
	MX23_PAD_EMI_CLKN__EMI_CLKN | MUX_CONFIG_EMI,

	MX23_PAD_EMI_A00__EMI_A00 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_A01__EMI_A01 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_A02__EMI_A02 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_A03__EMI_A03 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_A04__EMI_A04 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_A05__EMI_A05 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_A06__EMI_A06 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_A07__EMI_A07 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_A08__EMI_A08 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_A09__EMI_A09 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_A10__EMI_A10 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_A11__EMI_A11 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_A12__EMI_A12 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_BA0__EMI_BA0 | MUX_CONFIG_EMI,
	MX23_PAD_EMI_BA1__EMI_BA1 | MUX_CONFIG_EMI,

	MX23_PAD_EMI_CASN__EMI_CASN | MUX_CONFIG_EMI,
	MX23_PAD_EMI_CE0N__EMI_CE0N | MUX_CONFIG_EMI,
	MX23_PAD_EMI_CE1N__EMI_CE1N | MUX_CONFIG_EMI,
	MX23_PAD_EMI_CKE__EMI_CKE | MUX_CONFIG_EMI,
	MX23_PAD_EMI_RASN__EMI_RASN | MUX_CONFIG_EMI,
	MX23_PAD_EMI_WEN__EMI_WEN | MUX_CONFIG_EMI,

	/* Green LED */
	MX23_PAD_SSP1_DETECT__GPIO_2_1 |
		(MXS_PAD_3V3 | MXS_PAD_4MA | MXS_PAD_NOPULL),

	/* MMC 0 */
	MX23_PAD_SSP1_CMD__SSP1_CMD | MUX_CONFIG_SSP,
	MX23_PAD_SSP1_DATA0__SSP1_DATA0 | MUX_CONFIG_SSP,
	MX23_PAD_SSP1_DATA1__SSP1_DATA1 | MUX_CONFIG_SSP,
	MX23_PAD_SSP1_DATA2__SSP1_DATA2 | MUX_CONFIG_SSP,
	MX23_PAD_SSP1_DATA3__SSP1_DATA3 | MUX_CONFIG_SSP,
	MX23_PAD_SSP1_SCK__SSP1_SCK | MUX_CONFIG_SSP,

	/* Ethernet */
	MX23_PAD_GPMI_ALE__GPIO_0_17 |
		(MXS_PAD_3V3 | MXS_PAD_12MA | MXS_PAD_NOPULL),
};

void board_init_ll(const uint32_t arg, const uint32_t *resptr)
{
	mxs_common_spl_init(arg, resptr, iomux_setup, ARRAY_SIZE(iomux_setup));
}

/* Fine-tune the DRAM configuration. */
void mxs_adjust_memory_params(uint32_t *dram_vals)
{
	/* Enable Auto Precharge. */
	dram_vals[3] |= 1 << 8;
	/* Enable Fast Writes. */
	dram_vals[5] |= 1 << 8;
	/* tEMRS = 3*tCK */
	dram_vals[10] &= ~(0x3 << 8);
	dram_vals[10] |= (0x3 << 8);
	/* CASLAT = 3*tCK */
	dram_vals[11] &= ~(0x3 << 0);
	dram_vals[11] |= (0x3 << 0);
	/* tCKE = 1*tCK */
	dram_vals[12] &= ~(0x7 << 0);
	dram_vals[12] |= (0x1 << 0);
	/* CASLAT_LIN_GATE = 3*tCK , CASLAT_LIN = 3*tCK, tWTR=2*tCK */
	dram_vals[13] &= ~((0xf << 16) | (0xf << 24) | (0xf << 0));
	dram_vals[13] |= (0x6 << 16) | (0x6 << 24) | (0x2 << 0);
	/* tDAL = 6*tCK */
	dram_vals[15] &= ~(0xf << 16);
	dram_vals[15] |= (0x6 << 16);
	/* tREF = 1040*tCK */
	dram_vals[26] &= ~0xffff;
	dram_vals[26] |= 0x0410;
	/* tRAS_MAX = 9334*tCK */
	dram_vals[32] &= ~0xffff;
	dram_vals[32] |= 0x2475;
}
