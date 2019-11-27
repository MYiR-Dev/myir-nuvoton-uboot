// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2016-2019 Intel Corporation <www.intel.com>
 *
 */

#include <common.h>
#include <cpu_func.h>
#include <dm.h>
#include <errno.h>
#include <div64.h>
#include <fdtdec.h>
#include <ram.h>
#include <reset.h>
#include "sdram_soc64.h"
#include <wait_bit.h>
#include <asm/arch/firewall.h>
#include <asm/arch/system_manager.h>
#include <asm/arch/reset_manager.h>
#include <asm/io.h>
#include <linux/sizes.h>

#define PGTABLE_OFF	0x4000

u32 hmc_readl(struct altera_sdram_platdata *plat, u32 reg)
{
	return readl(plat->iomhc + reg);
}

u32 hmc_ecc_readl(struct altera_sdram_platdata *plat, u32 reg)
{
	return readl(plat->hmc + reg);
}

u32 hmc_ecc_writel(struct altera_sdram_platdata *plat,
		   u32 data, u32 reg)
{
	return writel(data, plat->hmc + reg);
}

u32 ddr_sch_writel(struct altera_sdram_platdata *plat, u32 data,
		   u32 reg)
{
	return writel(data, plat->ddr_sch + reg);
}

int emif_clear(struct altera_sdram_platdata *plat)
{
	hmc_ecc_writel(plat, 0, RSTHANDSHAKECTRL);

	return wait_for_bit_le32((const void *)(plat->hmc +
				 RSTHANDSHAKESTAT),
				 DDR_HMC_RSTHANDSHAKE_MASK,
				 false, 1000, false);
}

int emif_reset(struct altera_sdram_platdata *plat)
{
	u32 c2s, s2c, ret;

	c2s = hmc_ecc_readl(plat, RSTHANDSHAKECTRL) & DDR_HMC_RSTHANDSHAKE_MASK;
	s2c = hmc_ecc_readl(plat, RSTHANDSHAKESTAT) & DDR_HMC_RSTHANDSHAKE_MASK;

	debug("DDR: c2s=%08x s2c=%08x nr0=%08x nr1=%08x nr2=%08x dst=%08x\n",
	      c2s, s2c, hmc_readl(plat, NIOSRESERVED0),
	      hmc_readl(plat, NIOSRESERVED1), hmc_readl(plat, NIOSRESERVED2),
	      hmc_readl(plat, DRAMSTS));

	if (s2c && emif_clear(plat)) {
		printf("DDR: emif_clear() failed\n");
		return -1;
	}

	debug("DDR: Triggerring emif reset\n");
	hmc_ecc_writel(plat, DDR_HMC_CORE2SEQ_INT_REQ, RSTHANDSHAKECTRL);

	/* if seq2core[3] = 0, we are good */
	ret = wait_for_bit_le32((const void *)(plat->hmc +
				 RSTHANDSHAKESTAT),
				 DDR_HMC_SEQ2CORE_INT_RESP_MASK,
				 false, 1000, false);
	if (ret) {
		printf("DDR: failed to get ack from EMIF\n");
		return ret;
	}

	ret = emif_clear(plat);
	if (ret) {
		printf("DDR: emif_clear() failed\n");
		return ret;
	}

	debug("DDR: %s triggered successly\n", __func__);
	return 0;
}

int poll_hmc_clock_status(void)
{
	return wait_for_bit_le32((const void *)(socfpga_get_sysmgr_addr() +
				 SYSMGR_SOC64_HMC_CLK),
				 SYSMGR_HMC_CLK_STATUS_MSK, true, 1000, false);
}

void sdram_clear_mem(phys_addr_t addr, phys_size_t size)
{
	phys_size_t i;

	if (addr % CONFIG_SYS_CACHELINE_SIZE) {
		printf("DDR: address 0x%llx is not cacheline size aligned.\n",
		       addr);
		hang();
	}

	if (size % CONFIG_SYS_CACHELINE_SIZE) {
		printf("DDR: size 0x%llx is not multiple of cacheline size\n",
		       size);
		hang();
	}

	/* Use DC ZVA instruction to clear memory to zeros by a cache line */
	for (i = 0; i < size; i = i + CONFIG_SYS_CACHELINE_SIZE) {
		asm volatile("dc zva, %0"
		     :
		     : "r"(addr)
		     : "memory");
		addr += CONFIG_SYS_CACHELINE_SIZE;
	}
}

void sdram_init_ecc_bits(bd_t *bd)
{
	phys_size_t size, size_init;
	phys_addr_t start_addr;
	int bank = 0;
	unsigned int start = get_timer(0);

	icache_enable();

	start_addr = bd->bi_dram[0].start;
	size = bd->bi_dram[0].size;

	/* Initialize small block for page table */
	memset((void *)start_addr, 0, PGTABLE_SIZE + PGTABLE_OFF);
	gd->arch.tlb_addr = start_addr + PGTABLE_OFF;
	gd->arch.tlb_size = PGTABLE_SIZE;
	start_addr += PGTABLE_SIZE + PGTABLE_OFF;
	size -= (PGTABLE_OFF + PGTABLE_SIZE);
	dcache_enable();

	while (1) {
		while (size) {
			size_init = min((phys_addr_t)SZ_1G, (phys_addr_t)size);
			sdram_clear_mem(start_addr, size_init);
			size -= size_init;
			start_addr += size_init;
			WATCHDOG_RESET();
		}

		bank++;
		if (bank >= CONFIG_NR_DRAM_BANKS)
			break;

		start_addr = bd->bi_dram[bank].start;
		size = bd->bi_dram[bank].size;
	}

	dcache_disable();
	icache_disable();

	printf("SDRAM-ECC: Initialized success with %d ms\n",
	       (unsigned int)get_timer(start));
}

void sdram_size_check(bd_t *bd)
{
	phys_size_t total_ram_check = 0;
	phys_size_t ram_check = 0;
	phys_addr_t start = 0;
	int bank;

	/* Sanity check ensure correct SDRAM size specified */
	debug("DDR: Running SDRAM size sanity check\n");

	for (bank = 0; bank < CONFIG_NR_DRAM_BANKS; bank++) {
		start = bd->bi_dram[bank].start;
		while (ram_check < bd->bi_dram[bank].size) {
			ram_check += get_ram_size((void *)(start + ram_check),
						 (phys_size_t)SZ_1G);
		}
		total_ram_check += ram_check;
		ram_check = 0;
	}

	/* If the ram_size is 2GB smaller, we can assume the IO space is
	 * not mapped in.  gd->ram_size is the actual size of the dram
	 * not the accessible size.
	 */
	if (total_ram_check != gd->ram_size) {
		puts("DDR: SDRAM size check failed!\n");
		hang();
	}

	debug("DDR: SDRAM size check passed!\n");
}

/**
 * sdram_calculate_size() - Calculate SDRAM size
 *
 * Calculate SDRAM device size based on SDRAM controller parameters.
 * Size is specified in bytes.
 */
phys_size_t sdram_calculate_size(struct altera_sdram_platdata *plat)
{
	u32 dramaddrw = hmc_readl(plat, DRAMADDRW);

	phys_size_t size = 1 << (DRAMADDRW_CFG_CS_ADDR_WIDTH(dramaddrw) +
			 DRAMADDRW_CFG_BANK_GRP_ADDR_WIDTH(dramaddrw) +
			 DRAMADDRW_CFG_BANK_ADDR_WIDTH(dramaddrw) +
			 DRAMADDRW_CFG_ROW_ADDR_WIDTH(dramaddrw) +
			 DRAMADDRW_CFG_COL_ADDR_WIDTH(dramaddrw));

	size *= (2 << (hmc_ecc_readl(plat, DDRIOCTRL) &
			DDR_HMC_DDRIOCTRL_IOSIZE_MSK));

	return size;
}

static int altera_sdram_ofdata_to_platdata(struct udevice *dev)
{
	struct altera_sdram_platdata *plat = dev->platdata;
	fdt_addr_t addr;

	addr = dev_read_addr_index(dev, 0);
	if (addr == FDT_ADDR_T_NONE)
		return -EINVAL;
	plat->ddr_sch = (void __iomem *)addr;

	addr = dev_read_addr_index(dev, 1);
	if (addr == FDT_ADDR_T_NONE)
		return -EINVAL;
	plat->iomhc = (void __iomem *)addr;

	addr = dev_read_addr_index(dev, 2);
	if (addr == FDT_ADDR_T_NONE)
		return -EINVAL;
	plat->hmc = (void __iomem *)addr;

	return 0;
}

static int altera_sdram_probe(struct udevice *dev)
{
	int ret;
	struct altera_sdram_priv *priv = dev_get_priv(dev);

	ret = reset_get_bulk(dev, &priv->resets);
	if (ret) {
		dev_err(dev, "Can't get reset: %d\n", ret);
		return -ENODEV;
	}
	reset_deassert_bulk(&priv->resets);

	if (sdram_mmr_init_full(dev) != 0) {
		puts("SDRAM init failed.\n");
		goto failed;
	}

	return 0;

failed:
	reset_release_bulk(&priv->resets);
	return -ENODEV;
}

static int altera_sdram_get_info(struct udevice *dev,
				 struct ram_info *info)
{
	struct altera_sdram_priv *priv = dev_get_priv(dev);

	info->base = priv->info.base;
	info->size = priv->info.size;

	return 0;
}

static struct ram_ops altera_sdram_ops = {
	.get_info = altera_sdram_get_info,
};

static const struct udevice_id altera_sdram_ids[] = {
	{ .compatible = "altr,sdr-ctl-s10" },
	{ /* sentinel */ }
};

U_BOOT_DRIVER(altera_sdram) = {
	.name = "altr_sdr_ctl",
	.id = UCLASS_RAM,
	.of_match = altera_sdram_ids,
	.ops = &altera_sdram_ops,
	.ofdata_to_platdata = altera_sdram_ofdata_to_platdata,
	.platdata_auto_alloc_size = sizeof(struct altera_sdram_platdata),
	.probe = altera_sdram_probe,
	.priv_auto_alloc_size = sizeof(struct altera_sdram_priv),
};
