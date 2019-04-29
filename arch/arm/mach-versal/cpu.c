// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2016 - 2018 Xilinx, Inc.
 * Michal Simek <michal.simek@xilinx.com>
 */

#include <common.h>
#include <asm/armv8/mmu.h>
#include <asm/io.h>
#include <asm/arch/hardware.h>
#include <asm/arch/sys_proto.h>

DECLARE_GLOBAL_DATA_PTR;

#define VERSAL_MEM_MAP_USED	6

#define DRAM_BANKS CONFIG_NR_DRAM_BANKS

/* +1 is end of list which needs to be empty */
#define VERSAL_MEM_MAP_MAX (VERSAL_MEM_MAP_USED + DRAM_BANKS + 1)

static struct mm_region versal_mem_map[VERSAL_MEM_MAP_MAX] = {
	{
		.virt = 0x80000000UL,
		.phys = 0x80000000UL,
		.size = 0x70000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		.virt = 0xf0000000UL,
		.phys = 0xf0000000UL,
		.size = 0x0fe00000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		.virt = 0xffe00000UL,
		.phys = 0xffe00000UL,
		.size = 0x00200000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		.virt = 0x400000000UL,
		.phys = 0x400000000UL,
		.size = 0x200000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		.virt = 0x600000000UL,
		.phys = 0x600000000UL,
		.size = 0x800000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		.virt = 0xe00000000UL,
		.phys = 0xe00000000UL,
		.size = 0xf200000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}
};

void mem_map_fill(void)
{
	int banks = VERSAL_MEM_MAP_USED;

	for (int i = 0; i < CONFIG_NR_DRAM_BANKS; i++) {
		/* Zero size means no more DDR that's this is end */
		if (!gd->bd->bi_dram[i].size)
			break;

		versal_mem_map[banks].virt = gd->bd->bi_dram[i].start;
		versal_mem_map[banks].phys = gd->bd->bi_dram[i].start;
		versal_mem_map[banks].size = gd->bd->bi_dram[i].size;
		versal_mem_map[banks].attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
					      PTE_BLOCK_INNER_SHARE;
		banks = banks + 1;
	}
}

struct mm_region *mem_map = versal_mem_map;

u64 get_page_table_size(void)
{
	return 0x14000;
}

#if defined(CONFIG_SYS_MEM_RSVD_FOR_MMU)
int reserve_mmu(void)
{
	tcm_init(TCM_LOCK);
	gd->arch.tlb_size = PGTABLE_SIZE;
	gd->arch.tlb_addr = VERSAL_TCM_BASE_ADDR;

	return 0;
}
#endif

#if defined(CONFIG_OF_BOARD)
void *board_fdt_blob_setup(void)
{
	static void *fw_dtb = (void *)CONFIG_VERSAL_OF_BOARD_DTB_ADDR;

	if (fdt_magic(fw_dtb) != FDT_MAGIC) {
		printf("DTB is not passed via %llx\n", (u64)fw_dtb);
		return NULL;
	}

	return fw_dtb;
}
#endif
