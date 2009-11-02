/*
 * Copyright 2007-2009 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>

DECLARE_GLOBAL_DATA_PTR;

/*
 * PCI/PCIE Controller initialization for mpc85xx/mpc86xx soc's
 *
 * Initialize controller and call the common driver/pci pci_hose_scan to
 * scan for bridges and devices.
 *
 * Hose fields which need to be pre-initialized by board specific code:
 *   regions[]
 *   first_busno
 *
 * Fields updated:
 *   last_busno
 */

#include <pci.h>
#include <asm/io.h>
#include <asm/fsl_pci.h>

/* Freescale-specific PCI config registers */
#define FSL_PCI_PBFR		0x44
#define FSL_PCIE_CAP_ID		0x4c
#define FSL_PCIE_CFG_RDY	0x4b0
#define FSL_PROG_IF_AGENT	0x1

void pciauto_prescan_setup_bridge(struct pci_controller *hose,
				pci_dev_t dev, int sub_bus);
void pciauto_postscan_setup_bridge(struct pci_controller *hose,
				pci_dev_t dev, int sub_bus);
void pciauto_config_init(struct pci_controller *hose);

#ifndef CONFIG_SYS_PCI_MEMORY_BUS
#define CONFIG_SYS_PCI_MEMORY_BUS 0
#endif

#ifndef CONFIG_SYS_PCI_MEMORY_PHYS
#define CONFIG_SYS_PCI_MEMORY_PHYS 0
#endif

#if defined(CONFIG_SYS_PCI_64BIT) && !defined(CONFIG_SYS_PCI64_MEMORY_BUS)
#define CONFIG_SYS_PCI64_MEMORY_BUS (64ull*1024*1024*1024)
#endif

/* Setup one inbound ATMU window.
 *
 * We let the caller decide what the window size should be
 */
static void set_inbound_window(volatile pit_t *pi,
				struct pci_region *r,
				u64 size)
{
	u32 sz = (__ilog2_u64(size) - 1);
	u32 flag = PIWAR_EN | PIWAR_LOCAL |
			PIWAR_READ_SNOOP | PIWAR_WRITE_SNOOP;

	out_be32(&pi->pitar, r->phys_start >> 12);
	out_be32(&pi->piwbar, r->bus_start >> 12);
#ifdef CONFIG_SYS_PCI_64BIT
	out_be32(&pi->piwbear, r->bus_start >> 44);
#else
	out_be32(&pi->piwbear, 0);
#endif
	if (r->flags & PCI_REGION_PREFETCH)
		flag |= PIWAR_PF;
	out_be32(&pi->piwar, flag | sz);
}

static int fsl_pci_setup_inbound_windows(struct pci_controller *hose,
					 u64 out_lo, u8 pcie_cap,
					 volatile pit_t *pi)
{
	struct pci_region *r = hose->regions + hose->region_count;
	u64 sz = min((u64)gd->ram_size, (1ull << 32));

	phys_addr_t phys_start = CONFIG_SYS_PCI_MEMORY_PHYS;
	pci_addr_t bus_start = CONFIG_SYS_PCI_MEMORY_BUS;
	pci_size_t pci_sz;

	/* we have no space available for inbound memory mapping */
	if (bus_start > out_lo) {
		printf ("no space for inbound mapping of memory\n");
		return 0;
	}

	/* limit size */
	if ((bus_start + sz) > out_lo) {
		sz = out_lo - bus_start;
		debug ("limiting size to %llx\n", sz);
	}

	pci_sz = 1ull << __ilog2_u64(sz);
	/*
	 * we can overlap inbound/outbound windows on PCI-E since RX & TX
	 * links a separate
	 */
	if ((pcie_cap == PCI_CAP_ID_EXP) && (pci_sz < sz)) {
		debug ("R0 bus_start: %llx phys_start: %llx size: %llx\n",
			(u64)bus_start, (u64)phys_start, (u64)sz);
		pci_set_region(r, bus_start, phys_start, sz,
				PCI_REGION_MEM | PCI_REGION_SYS_MEMORY |
				PCI_REGION_PREFETCH);

		/* if we aren't an exact power of two match, pci_sz is smaller
		 * round it up to the next power of two.  We report the actual
		 * size to pci region tracking.
		 */
		if (pci_sz != sz)
			sz = 2ull << __ilog2_u64(sz);

		set_inbound_window(pi--, r++, sz);
		sz = 0; /* make sure we dont set the R2 window */
	} else {
		debug ("R0 bus_start: %llx phys_start: %llx size: %llx\n",
			(u64)bus_start, (u64)phys_start, (u64)pci_sz);
		pci_set_region(r, bus_start, phys_start, pci_sz,
				PCI_REGION_MEM | PCI_REGION_SYS_MEMORY |
				PCI_REGION_PREFETCH);
		set_inbound_window(pi--, r++, pci_sz);

		sz -= pci_sz;
		bus_start += pci_sz;
		phys_start += pci_sz;

		pci_sz = 1ull << __ilog2_u64(sz);
		if (sz) {
			debug ("R1 bus_start: %llx phys_start: %llx size: %llx\n",
				(u64)bus_start, (u64)phys_start, (u64)pci_sz);
			pci_set_region(r, bus_start, phys_start, pci_sz,
					PCI_REGION_MEM | PCI_REGION_SYS_MEMORY |
					PCI_REGION_PREFETCH);
			set_inbound_window(pi--, r++, pci_sz);
			sz -= pci_sz;
			bus_start += pci_sz;
			phys_start += pci_sz;
		}
	}

#if defined(CONFIG_PHYS_64BIT) && defined(CONFIG_SYS_PCI_64BIT)
	/*
	 * On 64-bit capable systems, set up a mapping for all of DRAM
	 * in high pci address space.
	 */
	pci_sz = 1ull << __ilog2_u64(gd->ram_size);
	/* round up to the next largest power of two */
	if (gd->ram_size > pci_sz)
		pci_sz = 1ull << (__ilog2_u64(gd->ram_size) + 1);
	debug ("R64 bus_start: %llx phys_start: %llx size: %llx\n",
		(u64)CONFIG_SYS_PCI64_MEMORY_BUS,
		(u64)CONFIG_SYS_PCI_MEMORY_PHYS,
		(u64)pci_sz);
	pci_set_region(r,
			CONFIG_SYS_PCI64_MEMORY_BUS,
			CONFIG_SYS_PCI_MEMORY_PHYS,
			pci_sz,
			PCI_REGION_MEM | PCI_REGION_SYS_MEMORY |
			PCI_REGION_PREFETCH);
	set_inbound_window(pi--, r++, pci_sz);
#else
	pci_sz = 1ull << __ilog2_u64(sz);
	if (sz) {
		debug ("R2 bus_start: %llx phys_start: %llx size: %llx\n",
			(u64)bus_start, (u64)phys_start, (u64)pci_sz);
		pci_set_region(r, bus_start, phys_start, pci_sz,
				PCI_REGION_MEM | PCI_REGION_SYS_MEMORY |
				PCI_REGION_PREFETCH);
		sz -= pci_sz;
		bus_start += pci_sz;
		phys_start += pci_sz;
		set_inbound_window(pi--, r++, pci_sz);
	}
#endif

#ifdef CONFIG_PHYS_64BIT
	if (sz && (((u64)gd->ram_size) < (1ull << 32)))
		printf("Was not able to map all of memory via "
			"inbound windows -- %lld remaining\n", sz);
#endif

	hose->region_count = r - hose->regions;

	return 1;
}

void fsl_pci_init(struct pci_controller *hose, u32 cfg_addr, u32 cfg_data)
{
	u16 temp16;
	u32 temp32;
	int enabled, r, inbound = 0;
	u16 ltssm;
	u8 temp8, pcie_cap;
	volatile ccsr_fsl_pci_t *pci = (ccsr_fsl_pci_t *)cfg_addr;
	struct pci_region *reg = hose->regions + hose->region_count;
	pci_dev_t dev = PCI_BDF(hose->first_busno, 0, 0);

	/* Initialize ATMU registers based on hose regions and flags */
	volatile pot_t *po = &pci->pot[1];	/* skip 0 */
	volatile pit_t *pi = &pci->pit[2];	/* ranges from: 3 to 1 */

	u64 out_hi = 0, out_lo = -1ULL;
	u32 pcicsrbar, pcicsrbar_sz;

#ifdef DEBUG
	int neg_link_w;
#endif

	pci_setup_indirect(hose, cfg_addr, cfg_data);

	/* Handle setup of outbound windows first */
	for (r = 0; r < hose->region_count; r++) {
		unsigned long flags = hose->regions[r].flags;
		u32 sz = (__ilog2_u64((u64)hose->regions[r].size) - 1);

		flags &= PCI_REGION_SYS_MEMORY|PCI_REGION_TYPE;
		if (flags != PCI_REGION_SYS_MEMORY) {
			u64 start = hose->regions[r].bus_start;
			u64 end = start + hose->regions[r].size;

			out_be32(&po->powbar, hose->regions[r].phys_start >> 12);
			out_be32(&po->potar, start >> 12);
#ifdef CONFIG_SYS_PCI_64BIT
			out_be32(&po->potear, start >> 44);
#else
			out_be32(&po->potear, 0);
#endif
			if (hose->regions[r].flags & PCI_REGION_IO) {
				out_be32(&po->powar, POWAR_EN | sz |
					POWAR_IO_READ | POWAR_IO_WRITE);
			} else {
				out_be32(&po->powar, POWAR_EN | sz |
					POWAR_MEM_READ | POWAR_MEM_WRITE);
				out_lo = min(start, out_lo);
				out_hi = max(end, out_hi);
			}
			po++;
		}
	}
	debug("Outbound memory range: %llx:%llx\n", out_lo, out_hi);

	/* setup PCSRBAR/PEXCSRBAR */
	pci_hose_write_config_dword(hose, dev, PCI_BASE_ADDRESS_0, 0xffffffff);
	pci_hose_read_config_dword (hose, dev, PCI_BASE_ADDRESS_0, &pcicsrbar_sz);
	pcicsrbar_sz = ~pcicsrbar_sz + 1;

	if (out_hi < (0x100000000ull - pcicsrbar_sz) ||
		(out_lo > 0x100000000ull))
		pcicsrbar = 0x100000000ull - pcicsrbar_sz;
	else
		pcicsrbar = (out_lo - pcicsrbar_sz) & -pcicsrbar_sz;
	pci_hose_write_config_dword(hose, dev, PCI_BASE_ADDRESS_0, pcicsrbar);

	out_lo = min(out_lo, (u64)pcicsrbar);

	debug("PCICSRBAR @ 0x%x\n", pcicsrbar);

	pci_set_region(reg++, pcicsrbar, CONFIG_SYS_CCSRBAR_PHYS,
			pcicsrbar_sz, PCI_REGION_SYS_MEMORY);
	hose->region_count++;

	/* see if we are a PCIe or PCI controller */
	pci_hose_read_config_byte(hose, dev, FSL_PCIE_CAP_ID, &pcie_cap);

	/* inbound */
	inbound = fsl_pci_setup_inbound_windows(hose, out_lo, pcie_cap, pi);

	for (r = 0; r < hose->region_count; r++)
		debug("PCI reg:%d %016llx:%016llx %016llx %08x\n", r,
			(u64)hose->regions[r].phys_start,
			hose->regions[r].bus_start,
			hose->regions[r].size,
			hose->regions[r].flags);

	pci_register_hose(hose);
	pciauto_config_init(hose);	/* grab pci_{mem,prefetch,io} */
	hose->current_busno = hose->first_busno;

	out_be32(&pci->pedr, 0xffffffff);	/* Clear any errors */
	out_be32(&pci->peer, ~0x20140);	/* Enable All Error Interupts except
					 * - Master abort (pci)
					 * - Master PERR (pci)
					 * - ICCA (PCIe)
					 */
	pci_hose_read_config_dword(hose, dev, PCI_DCR, &temp32);
	temp32 |= 0xf000e;		/* set URR, FER, NFER (but not CER) */
	pci_hose_write_config_dword(hose, dev, PCI_DCR, temp32);

	if (pcie_cap == PCI_CAP_ID_EXP) {
		pci_hose_read_config_word(hose, dev, PCI_LTSSM, &ltssm);
		enabled = ltssm >= PCI_LTSSM_L0;

#ifdef CONFIG_FSL_PCIE_RESET
		if (ltssm == 1) {
			int i;
			debug("....PCIe link error. " "LTSSM=0x%02x.", ltssm);
			/* assert PCIe reset */
			setbits_be32(&pci->pdb_stat, 0x08000000);
			(void) in_be32(&pci->pdb_stat);
			udelay(100);
			debug("  Asserting PCIe reset @%x = %x\n",
			      &pci->pdb_stat, in_be32(&pci->pdb_stat));
			/* clear PCIe reset */
			clrbits_be32(&pci->pdb_stat, 0x08000000);
			asm("sync;isync");
			for (i=0; i<100 && ltssm < PCI_LTSSM_L0; i++) {
				pci_hose_read_config_word(hose, dev, PCI_LTSSM,
							&ltssm);
				udelay(1000);
				debug("....PCIe link error. "
				      "LTSSM=0x%02x.\n", ltssm);
			}
			enabled = ltssm >= PCI_LTSSM_L0;

			/* we need to re-write the bar0 since a reset will
			 * clear it
			 */
			pci_hose_write_config_dword(hose, dev,
					PCI_BASE_ADDRESS_0, pcicsrbar);
		}
#endif

		if (!enabled) {
			debug("....PCIE link error.  Skipping scan."
			      "LTSSM=0x%02x\n", ltssm);
			hose->last_busno = hose->first_busno;
			return;
		}

		out_be32(&pci->pme_msg_det, 0xffffffff);
		out_be32(&pci->pme_msg_int_en, 0xffffffff);
#ifdef DEBUG
		pci_hose_read_config_word(hose, dev, PCI_LSR, &temp16);
		neg_link_w = (temp16 & 0x3f0 ) >> 4;
		printf("...PCIE LTSSM=0x%x, Negotiated link width=%d\n",
		      ltssm, neg_link_w);
#endif
		hose->current_busno++; /* Start scan with secondary */
		pciauto_prescan_setup_bridge(hose, dev, hose->current_busno);
	}

	/* Use generic setup_device to initialize standard pci regs,
	 * but do not allocate any windows since any BAR found (such
	 * as PCSRBAR) is not in this cpu's memory space.
	 */
	pciauto_setup_device(hose, dev, 0, hose->pci_mem,
			     hose->pci_prefetch, hose->pci_io);

	if (inbound) {
		pci_hose_read_config_word(hose, dev, PCI_COMMAND, &temp16);
		pci_hose_write_config_word(hose, dev, PCI_COMMAND,
					   temp16 | PCI_COMMAND_MEMORY);
	}

#ifndef CONFIG_PCI_NOSCAN
	pci_hose_read_config_byte(hose, dev, PCI_CLASS_PROG, &temp8);

	/* Programming Interface (PCI_CLASS_PROG)
	 * 0 == pci host or pcie root-complex,
	 * 1 == pci agent or pcie end-point
	 */
	if (!temp8) {
		printf("               Scanning PCI bus %02x\n",
			hose->current_busno);
		hose->last_busno = pci_hose_scan_bus(hose, hose->current_busno);
	} else {
		debug("               Not scanning PCI bus %02x. PI=%x\n",
			hose->current_busno, temp8);
		hose->last_busno = hose->current_busno;
	}

	/* if we are PCIe - update limit regs and subordinate busno
	 * for the virtual P2P bridge
	 */
	if (pcie_cap == PCI_CAP_ID_EXP) {
		pciauto_postscan_setup_bridge(hose, dev, hose->last_busno);
	}
#else
	hose->last_busno = hose->current_busno;
#endif

	/* Clear all error indications */
	if (pcie_cap == PCI_CAP_ID_EXP)
		out_be32(&pci->pme_msg_det, 0xffffffff);
	out_be32(&pci->pedr, 0xffffffff);

	pci_hose_read_config_word (hose, dev, PCI_DSR, &temp16);
	if (temp16) {
		pci_hose_write_config_word(hose, dev, PCI_DSR, 0xffff);
	}

	pci_hose_read_config_word (hose, dev, PCI_SEC_STATUS, &temp16);
	if (temp16) {
		pci_hose_write_config_word(hose, dev, PCI_SEC_STATUS, 0xffff);
	}
}

int fsl_is_pci_agent(struct pci_controller *hose)
{
	u8 prog_if;
	pci_dev_t dev = PCI_BDF(hose->first_busno, 0, 0);

	pci_hose_read_config_byte(hose, dev, PCI_CLASS_PROG, &prog_if);

	return (prog_if == FSL_PROG_IF_AGENT);
}

int fsl_pci_init_port(struct fsl_pci_info *pci_info,
			struct pci_controller *hose, int busno)
{
	volatile ccsr_fsl_pci_t *pci;
	struct pci_region *r;

	pci = (ccsr_fsl_pci_t *) pci_info->regs;

	/* on non-PCIe controllers we don't have pme_msg_det so this code
	 * should do nothing since the read will return 0
	 */
	if (in_be32(&pci->pme_msg_det)) {
		out_be32(&pci->pme_msg_det, 0xffffffff);
		debug (" with errors.  Clearing.  Now 0x%08x",
			pci->pme_msg_det);
	}

	r = hose->regions + hose->region_count;

	/* outbound memory */
	pci_set_region(r++,
			pci_info->mem_bus,
			pci_info->mem_phys,
			pci_info->mem_size,
			PCI_REGION_MEM);

	/* outbound io */
	pci_set_region(r++,
			pci_info->io_bus,
			pci_info->io_phys,
			pci_info->io_size,
			PCI_REGION_IO);

	hose->region_count = r - hose->regions;
	hose->first_busno = busno;

	fsl_pci_init(hose, (u32)&pci->cfg_addr, (u32)&pci->cfg_data);

	if (fsl_is_pci_agent(hose)) {
		fsl_pci_config_unlock(hose);
		hose->last_busno = hose->first_busno;
	}

	printf("    PCIE%x on bus %02x - %02x\n", pci_info->pci_num,
			hose->first_busno, hose->last_busno);

	return(hose->last_busno + 1);
}

/* Enable inbound PCI config cycles for agent/endpoint interface */
void fsl_pci_config_unlock(struct pci_controller *hose)
{
	pci_dev_t dev = PCI_BDF(hose->first_busno,0,0);
	u8 agent;
	u8 pcie_cap;
	u16 pbfr;

	pci_hose_read_config_byte(hose, dev, PCI_CLASS_PROG, &agent);
	if (!agent)
		return;

	pci_hose_read_config_byte(hose, dev, FSL_PCIE_CAP_ID, &pcie_cap);
	if (pcie_cap != 0x0) {
		/* PCIe - set CFG_READY bit of Configuration Ready Register */
		pci_hose_write_config_byte(hose, dev, FSL_PCIE_CFG_RDY, 0x1);
	} else {
		/* PCI - clear ACL bit of PBFR */
		pci_hose_read_config_word(hose, dev, FSL_PCI_PBFR, &pbfr);
		pbfr &= ~0x20;
		pci_hose_write_config_word(hose, dev, FSL_PCI_PBFR, pbfr);
	}
}

#ifdef CONFIG_OF_BOARD_SETUP
#include <libfdt.h>
#include <fdt_support.h>

void ft_fsl_pci_setup(void *blob, const char *pci_alias,
			struct pci_controller *hose)
{
	int off = fdt_path_offset(blob, pci_alias);

	if (off >= 0) {
		u32 bus_range[2];

		bus_range[0] = 0;
		bus_range[1] = hose->last_busno - hose->first_busno;
		fdt_setprop(blob, off, "bus-range", &bus_range[0], 2*4);
		fdt_pci_dma_ranges(blob, off, hose);
	}
}
#endif
