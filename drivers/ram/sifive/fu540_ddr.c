// SPDX-License-Identifier: GPL-2.0+ OR BSD-3-Clause
/*
 * (C) Copyright 2020 SiFive, Inc.
 *
 * Authors:
 *   Pragnesh Patel <pragnesh.patel@sifive.com>
 */

#include <common.h>
#include <dm.h>
#include <init.h>
#include <ram.h>
#include <regmap.h>
#include <syscon.h>
#include <asm/io.h>
#include <clk.h>
#include <wait_bit.h>
#include <linux/bitops.h>

#define DENALI_CTL_0	0
#define DENALI_CTL_21	21
#define DENALI_CTL_120	120
#define DENALI_CTL_132	132
#define DENALI_CTL_136	136
#define DENALI_CTL_170	170
#define DENALI_CTL_181	181
#define DENALI_CTL_182	182
#define DENALI_CTL_184	184
#define DENALI_CTL_208	208
#define DENALI_CTL_209	209
#define DENALI_CTL_210	210
#define DENALI_CTL_212	212
#define DENALI_CTL_214	214
#define DENALI_CTL_216	216
#define DENALI_CTL_224	224
#define DENALI_CTL_225	225
#define DENALI_CTL_260	260

#define DENALI_PHY_1152	1152
#define DENALI_PHY_1214	1214

#define PAYLOAD_DEST	0x80000000
#define DDR_MEM_SIZE	(8UL * 1024UL * 1024UL * 1024UL)

#define DRAM_CLASS_OFFSET			8
#define DRAM_CLASS_DDR4				0xA
#define OPTIMAL_RMODW_EN_OFFSET			0
#define DISABLE_RD_INTERLEAVE_OFFSET		16
#define OUT_OF_RANGE_OFFSET			1
#define MULTIPLE_OUT_OF_RANGE_OFFSET		2
#define PORT_COMMAND_CHANNEL_ERROR_OFFSET	7
#define MC_INIT_COMPLETE_OFFSET			8
#define LEVELING_OPERATION_COMPLETED_OFFSET	22
#define DFI_PHY_WRLELV_MODE_OFFSET		24
#define DFI_PHY_RDLVL_MODE_OFFSET		24
#define DFI_PHY_RDLVL_GATE_MODE_OFFSET		0
#define VREF_EN_OFFSET				24
#define PORT_ADDR_PROTECTION_EN_OFFSET		0
#define AXI0_ADDRESS_RANGE_ENABLE		8
#define AXI0_RANGE_PROT_BITS_0_OFFSET		24
#define RDLVL_EN_OFFSET				16
#define RDLVL_GATE_EN_OFFSET			24
#define WRLVL_EN_OFFSET				0

#define PHY_RX_CAL_DQ0_0_OFFSET			0
#define PHY_RX_CAL_DQ1_0_OFFSET			16

struct fu540_ddrctl {
	volatile u32 denali_ctl[265];
};

struct fu540_ddrphy {
	volatile u32 denali_phy[1215];
};

/**
 * struct fu540_ddr_info
 *
 * @dev                         : pointer for the device
 * @info                        : UCLASS RAM information
 * @ctl                         : DDR controller base address
 * @phy                         : DDR PHY base address
 * @ctrl                        : DDR control base address
 * @physical_filter_ctrl        : DDR physical filter control base address
 */
struct fu540_ddr_info {
	struct udevice *dev;
	struct ram_info info;
	struct fu540_ddrctl *ctl;
	struct fu540_ddrphy *phy;
	struct clk ddr_clk;
	u32 *physical_filter_ctrl;
};

#if defined(CONFIG_SPL_BUILD)
struct fu540_ddr_params {
	struct fu540_ddrctl pctl_regs;
	struct fu540_ddrphy phy_regs;
};

struct sifive_dmc_plat {
	struct fu540_ddr_params ddr_params;
};

/*
 * TODO : It can be possible to use common sdram_copy_to_reg() API
 * n: Unit bytes
 */
static void sdram_copy_to_reg(volatile u32 *dest,
			      volatile u32 *src, u32 n)
{
	int i;

	for (i = 0; i < n / sizeof(u32); i++) {
		writel(*src, dest);
		src++;
		dest++;
	}
}

static void fu540_ddr_setup_range_protection(volatile u32 *ctl, u64 end_addr)
{
	u32 end_addr_16kblocks = ((end_addr >> 14) & 0x7FFFFF) - 1;

	writel(0x0, DENALI_CTL_209 + ctl);
	writel(end_addr_16kblocks, DENALI_CTL_210 + ctl);
	writel(0x0, DENALI_CTL_212 + ctl);
	writel(0x0, DENALI_CTL_214 + ctl);
	writel(0x0, DENALI_CTL_216 + ctl);
	setbits_le32(DENALI_CTL_224 + ctl,
		     0x3 << AXI0_RANGE_PROT_BITS_0_OFFSET);
	writel(0xFFFFFFFF, DENALI_CTL_225 + ctl);
	setbits_le32(DENALI_CTL_208 + ctl, 0x1 << AXI0_ADDRESS_RANGE_ENABLE);
	setbits_le32(DENALI_CTL_208 + ctl,
		     0x1 << PORT_ADDR_PROTECTION_EN_OFFSET);
}

static void fu540_ddr_start(volatile u32 *ctl, u32 *physical_filter_ctrl,
			    u64 ddr_end)
{
	volatile u64 *filterreg = (volatile u64 *)physical_filter_ctrl;

	setbits_le32(DENALI_CTL_0 + ctl, 0x1);

	wait_for_bit_le32((void *)ctl + DENALI_CTL_132,
			  BIT(MC_INIT_COMPLETE_OFFSET), false, 100, false);

	/* Disable the BusBlocker in front of the controller AXI slave ports */
	filterreg[0] = 0x0f00000000000000UL | (ddr_end >> 2);
}

static void fu540_ddr_check_errata(u32 regbase, u32 updownreg)
{
	u64 fails     = 0;
	u32 dq        = 0;
	u32 down, up;
	u8 failc0, failc1;
	u32 phy_rx_cal_dqn_0_offset;

	for (u32 bit = 0; bit < 2; bit++) {
		if (bit == 0) {
			phy_rx_cal_dqn_0_offset =
				PHY_RX_CAL_DQ0_0_OFFSET;
		} else {
			phy_rx_cal_dqn_0_offset =
				PHY_RX_CAL_DQ1_0_OFFSET;
		}

		down = (updownreg >>
			phy_rx_cal_dqn_0_offset) & 0x3F;
		up = (updownreg >>
		      (phy_rx_cal_dqn_0_offset + 6)) &
		      0x3F;

		failc0 = ((down == 0) && (up == 0x3F));
		failc1 = ((up == 0) && (down == 0x3F));

		/* print error message on failure */
		if (failc0 || failc1) {
			if (fails == 0)
				printf("DDR error in fixing up\n");

			fails |= (1 << dq);

			char slicelsc = '0';
			char slicemsc = '0';

			slicelsc += (dq % 10);
			slicemsc += (dq / 10);
			printf("S ");
			printf("%c", slicemsc);
			printf("%c", slicelsc);

			if (failc0)
				printf("U");
			else
				printf("D");

			printf("\n");
		}
		dq++;
	}
}

static u64 fu540_ddr_phy_fixup(volatile u32 *ddrphyreg)
{
	u32 slicebase = 0;

	/* check errata condition */
	for (u32 slice = 0; slice < 8; slice++) {
		u32 regbase = slicebase + 34;

		for (u32 reg = 0; reg < 4; reg++) {
			u32 updownreg = readl(regbase + reg + ddrphyreg);

			fu540_ddr_check_errata(regbase, updownreg);
		}
		slicebase += 128;
	}

	return(0);
}

static u32 fu540_ddr_get_dram_class(volatile u32 *ctl)
{
	u32 reg = readl(DENALI_CTL_0 + ctl);

	return ((reg >> DRAM_CLASS_OFFSET) & 0xF);
}

static int fu540_ddr_setup(struct udevice *dev)
{
	struct fu540_ddr_info *priv = dev_get_priv(dev);
	struct sifive_dmc_plat *plat = dev_get_platdata(dev);
	struct fu540_ddr_params *params = &plat->ddr_params;
	volatile u32 *denali_ctl =  priv->ctl->denali_ctl;
	volatile u32 *denali_phy =  priv->phy->denali_phy;
	const u64 ddr_size = DDR_MEM_SIZE;
	const u64 ddr_end = PAYLOAD_DEST + ddr_size;
	int ret, i;
	u32 physet;

	ret = dev_read_u32_array(dev, "sifive,ddr-params",
				 (u32 *)&plat->ddr_params,
				 sizeof(plat->ddr_params) / sizeof(u32));
	if (ret) {
		printf("%s: Cannot read sifive,ddr-params %d\n",
		       __func__, ret);
		return ret;
	}

	sdram_copy_to_reg(priv->ctl->denali_ctl,
			  params->pctl_regs.denali_ctl,
			  sizeof(struct fu540_ddrctl));

	/* phy reset */
	for (i = DENALI_PHY_1152; i <= DENALI_PHY_1214; i++) {
		physet = params->phy_regs.denali_phy[i];
		priv->phy->denali_phy[i] = physet;
	}

	for (i = 0; i < DENALI_PHY_1152; i++) {
		physet = params->phy_regs.denali_phy[i];
		priv->phy->denali_phy[i] = physet;
	}

	/* Disable read interleave DENALI_CTL_120 */
	setbits_le32(DENALI_CTL_120 + denali_ctl,
		     1 << DISABLE_RD_INTERLEAVE_OFFSET);

	/* Disable optimal read/modify/write logic DENALI_CTL_21 */
	clrbits_le32(DENALI_CTL_21 + denali_ctl, 1 << OPTIMAL_RMODW_EN_OFFSET);

	/* Enable write Leveling DENALI_CTL_170 */
	setbits_le32(DENALI_CTL_170 + denali_ctl, (1 << WRLVL_EN_OFFSET)
		     | (1 << DFI_PHY_WRLELV_MODE_OFFSET));

	/* Enable read leveling DENALI_CTL_181 and DENALI_CTL_260 */
	setbits_le32(DENALI_CTL_181 + denali_ctl,
		     1 << DFI_PHY_RDLVL_MODE_OFFSET);
	setbits_le32(DENALI_CTL_260 + denali_ctl, 1 << RDLVL_EN_OFFSET);

	/* Enable read leveling gate DENALI_CTL_260 and DENALI_CTL_182 */
	setbits_le32(DENALI_CTL_260 + denali_ctl, 1 << RDLVL_GATE_EN_OFFSET);
	setbits_le32(DENALI_CTL_182 + denali_ctl,
		     1 << DFI_PHY_RDLVL_GATE_MODE_OFFSET);

	if (fu540_ddr_get_dram_class(denali_ctl) == DRAM_CLASS_DDR4) {
		/* Enable vref training DENALI_CTL_184 */
		setbits_le32(DENALI_CTL_184 + denali_ctl, 1 << VREF_EN_OFFSET);
	}

	/* Mask off leveling completion interrupt DENALI_CTL_136 */
	setbits_le32(DENALI_CTL_136 + denali_ctl,
		     1 << LEVELING_OPERATION_COMPLETED_OFFSET);

	/* Mask off MC init complete interrupt DENALI_CTL_136 */
	setbits_le32(DENALI_CTL_136 + denali_ctl, 1 << MC_INIT_COMPLETE_OFFSET);

	/* Mask off out of range interrupts DENALI_CTL_136 */
	setbits_le32(DENALI_CTL_136 + denali_ctl, (1 << OUT_OF_RANGE_OFFSET)
		     | (1 << MULTIPLE_OUT_OF_RANGE_OFFSET));

	/* set up range protection */
	fu540_ddr_setup_range_protection(denali_ctl, DDR_MEM_SIZE);

	/* Mask off port command error interrupt DENALI_CTL_136 */
	setbits_le32(DENALI_CTL_136 + denali_ctl,
		     1 << PORT_COMMAND_CHANNEL_ERROR_OFFSET);

	fu540_ddr_start(denali_ctl, priv->physical_filter_ctrl, ddr_end);

	fu540_ddr_phy_fixup(denali_phy);

	/* check size */
	priv->info.size = get_ram_size((long *)priv->info.base,
				       DDR_MEM_SIZE);

	debug("%s : %lx\n", __func__, priv->info.size);

	/* check memory access for all memory */
	if (priv->info.size != DDR_MEM_SIZE) {
		printf("DDR invalid size : 0x%lx, expected 0x%lx\n",
		       priv->info.size, DDR_MEM_SIZE);
		return -EINVAL;
	}

	return 0;
}
#endif

static int fu540_ddr_probe(struct udevice *dev)
{
	struct fu540_ddr_info *priv = dev_get_priv(dev);

#if defined(CONFIG_SPL_BUILD)
	struct regmap *map;
	int ret;
	u32 clock = 0;

	debug("FU540 DDR probe\n");
	priv->dev = dev;

	ret = regmap_init_mem(dev_ofnode(dev), &map);
	if (ret)
		return ret;

	ret = clk_get_by_index(dev, 0, &priv->ddr_clk);
	if (ret) {
		debug("clk get failed %d\n", ret);
		return ret;
	}

	ret = dev_read_u32(dev, "clock-frequency", &clock);
	if (ret) {
		debug("clock-frequency not found in dt %d\n", ret);
		return ret;
	} else {
		ret = clk_set_rate(&priv->ddr_clk, clock);
		if (ret < 0) {
			debug("Could not set DDR clock\n");
			return ret;
		}
	}

	ret = clk_enable(&priv->ddr_clk);
	priv->ctl = regmap_get_range(map, 0);
	priv->phy = regmap_get_range(map, 1);
	priv->physical_filter_ctrl = regmap_get_range(map, 2);

	priv->info.base = CONFIG_SYS_SDRAM_BASE;

	priv->info.size = 0;
	return fu540_ddr_setup(dev);
#else
	priv->info.base = CONFIG_SYS_SDRAM_BASE;
	priv->info.size = DDR_MEM_SIZE;
#endif
	return 0;
}

static int fu540_ddr_get_info(struct udevice *dev, struct ram_info *info)
{
	struct fu540_ddr_info *priv = dev_get_priv(dev);

	*info = priv->info;

	return 0;
}

static struct ram_ops fu540_ddr_ops = {
	.get_info = fu540_ddr_get_info,
};

static const struct udevice_id fu540_ddr_ids[] = {
	{ .compatible = "sifive,fu540-c000-ddr" },
	{ }
};

U_BOOT_DRIVER(fu540_ddr) = {
	.name = "fu540_ddr",
	.id = UCLASS_RAM,
	.of_match = fu540_ddr_ids,
	.ops = &fu540_ddr_ops,
	.probe = fu540_ddr_probe,
	.priv_auto_alloc_size = sizeof(struct fu540_ddr_info),
#if defined(CONFIG_SPL_BUILD)
	.platdata_auto_alloc_size = sizeof(struct sifive_dmc_plat),
#endif
};
