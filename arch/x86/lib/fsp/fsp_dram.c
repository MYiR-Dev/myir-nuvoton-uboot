// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2014, Bin Meng <bmeng.cn@gmail.com>
 */

#include <common.h>
#include <handoff.h>
#include <asm/fsp/fsp_support.h>
#include <asm/e820.h>
#include <asm/mrccache.h>
#include <asm/post.h>

DECLARE_GLOBAL_DATA_PTR;

int fsp_scan_for_ram_size(void)
{
	phys_size_t ram_size = 0;
	const struct hob_header *hdr;
	struct hob_res_desc *res_desc;

	hdr = gd->arch.hob_list;
	while (!end_of_hob(hdr)) {
		if (hdr->type == HOB_TYPE_RES_DESC) {
			res_desc = (struct hob_res_desc *)hdr;
			if (res_desc->type == RES_SYS_MEM ||
			    res_desc->type == RES_MEM_RESERVED)
				ram_size += res_desc->len;
		}
		hdr = get_next_hob(hdr);
	}

	gd->ram_size = ram_size;
	post_code(POST_DRAM);

	return 0;
};

int dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = 0;
	gd->bd->bi_dram[0].size = gd->ram_size;

	return 0;
}

unsigned int install_e820_map(unsigned int max_entries,
			      struct e820_entry *entries)
{
	unsigned int num_entries = 0;
	const struct hob_header *hdr;
	struct hob_res_desc *res_desc;

	hdr = gd->arch.hob_list;

	while (!end_of_hob(hdr)) {
		if (hdr->type == HOB_TYPE_RES_DESC) {
			res_desc = (struct hob_res_desc *)hdr;
			entries[num_entries].addr = res_desc->phys_start;
			entries[num_entries].size = res_desc->len;

			if (res_desc->type == RES_SYS_MEM)
				entries[num_entries].type = E820_RAM;
			else if (res_desc->type == RES_MEM_RESERVED)
				entries[num_entries].type = E820_RESERVED;

			num_entries++;
		}
		hdr = get_next_hob(hdr);
	}

	/* Mark PCIe ECAM address range as reserved */
	entries[num_entries].addr = CONFIG_PCIE_ECAM_BASE;
	entries[num_entries].size = CONFIG_PCIE_ECAM_SIZE;
	entries[num_entries].type = E820_RESERVED;
	num_entries++;

#ifdef CONFIG_HAVE_ACPI_RESUME
	/*
	 * Everything between U-Boot's stack and ram top needs to be
	 * reserved in order for ACPI S3 resume to work.
	 */
	entries[num_entries].addr = gd->start_addr_sp - CONFIG_STACK_SIZE;
	entries[num_entries].size = gd->ram_top - gd->start_addr_sp +
		CONFIG_STACK_SIZE;
	entries[num_entries].type = E820_RESERVED;
	num_entries++;
#endif

	return num_entries;
}
