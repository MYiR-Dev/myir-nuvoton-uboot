/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2017-2018 NXP
 * Copyright 2014-2015, Freescale Semiconductor
 */

#ifndef _FSL_LAYERSCAPE_CPU_H
#define _FSL_LAYERSCAPE_CPU_H

#ifdef CONFIG_FSL_LSCH3
#define CONFIG_SYS_FSL_CCSR_BASE	0x00000000
#define CONFIG_SYS_FSL_CCSR_SIZE	0x10000000
#define CONFIG_SYS_FSL_QSPI_BASE1	0x20000000
#define CONFIG_SYS_FSL_QSPI_SIZE1	0x10000000
#ifndef CONFIG_NXP_LSCH3_2
#define CONFIG_SYS_FSL_IFC_BASE1	0x30000000
#define CONFIG_SYS_FSL_IFC_SIZE1	0x10000000
#define CONFIG_SYS_FSL_IFC_SIZE1_1	0x400000
#endif
#define CONFIG_SYS_FSL_DRAM_BASE1	0x80000000
#define CONFIG_SYS_FSL_DRAM_SIZE1	0x80000000
#define CONFIG_SYS_FSL_QSPI_BASE2	0x400000000
#define CONFIG_SYS_FSL_QSPI_SIZE2	0x100000000
#ifndef CONFIG_NXP_LSCH3_2
#define CONFIG_SYS_FSL_IFC_BASE2	0x500000000
#define CONFIG_SYS_FSL_IFC_SIZE2	0x100000000
#endif
#define CONFIG_SYS_FSL_DCSR_BASE	0x700000000
#define CONFIG_SYS_FSL_DCSR_SIZE	0x40000000
#define CONFIG_SYS_FSL_MC_BASE		0x80c000000
#define CONFIG_SYS_FSL_MC_SIZE		0x4000000
#define CONFIG_SYS_FSL_NI_BASE		0x810000000
#define CONFIG_SYS_FSL_NI_SIZE		0x8000000
#define CONFIG_SYS_FSL_QBMAN_BASE	0x818000000
#define CONFIG_SYS_FSL_QBMAN_SIZE	0x8000000
#define CONFIG_SYS_FSL_QBMAN_SIZE_1	0x4000000
#define CONFIG_SYS_PCIE1_PHYS_SIZE	0x200000000
#define CONFIG_SYS_PCIE2_PHYS_SIZE	0x200000000
#define CONFIG_SYS_PCIE3_PHYS_SIZE	0x200000000
#define CONFIG_SYS_PCIE4_PHYS_SIZE	0x200000000
#define CONFIG_SYS_FSL_WRIOP1_BASE	0x4300000000
#define CONFIG_SYS_FSL_WRIOP1_SIZE	0x100000000
#define CONFIG_SYS_FSL_AIOP1_BASE	0x4b00000000
#define CONFIG_SYS_FSL_AIOP1_SIZE	0x100000000
#ifndef CONFIG_ARCH_LX2160A
#define CONFIG_SYS_FSL_PEBUF_BASE	0x4c00000000
#else
#define CONFIG_SYS_FSL_PEBUF_BASE	0x1c00000000
#endif
#define CONFIG_SYS_FSL_PEBUF_SIZE	0x400000000
#ifdef CONFIG_NXP_LSCH3_2
#define CONFIG_SYS_FSL_DRAM_BASE2	0x2080000000
#define CONFIG_SYS_FSL_DRAM_SIZE2	0x1F80000000
#define CONFIG_SYS_FSL_DRAM_BASE3	0x6000000000
#define CONFIG_SYS_FSL_DRAM_SIZE3	0x2000000000
#else
#define CONFIG_SYS_FSL_DRAM_BASE2	0x8080000000
#define CONFIG_SYS_FSL_DRAM_SIZE2	0x7F80000000
#endif
#elif defined(CONFIG_FSL_LSCH2)
#define CONFIG_SYS_FSL_BOOTROM_BASE	0x0
#define CONFIG_SYS_FSL_BOOTROM_SIZE	0x1000000
#define CONFIG_SYS_FSL_CCSR_BASE	0x1000000
#define CONFIG_SYS_FSL_CCSR_SIZE	0xf000000
#define CONFIG_SYS_FSL_DCSR_BASE	0x20000000
#define CONFIG_SYS_FSL_DCSR_SIZE	0x4000000
#define CONFIG_SYS_FSL_QSPI_BASE	0x40000000
#define CONFIG_SYS_FSL_QSPI_SIZE	0x20000000
#define CONFIG_SYS_FSL_IFC_BASE		0x60000000
#define CONFIG_SYS_FSL_IFC_SIZE		0x20000000
#define CONFIG_SYS_FSL_DRAM_BASE1	0x80000000
#define CONFIG_SYS_FSL_DRAM_SIZE1	0x80000000
#define CONFIG_SYS_FSL_QBMAN_BASE	0x500000000
#define CONFIG_SYS_FSL_QBMAN_SIZE	0x10000000
#define CONFIG_SYS_FSL_DRAM_BASE2	0x880000000
#define CONFIG_SYS_FSL_DRAM_SIZE2	0x780000000	/* 30GB */
#define CONFIG_SYS_PCIE1_PHYS_SIZE	0x800000000
#define CONFIG_SYS_PCIE2_PHYS_SIZE	0x800000000
#define CONFIG_SYS_PCIE3_PHYS_SIZE	0x800000000
#define CONFIG_SYS_FSL_DRAM_BASE3	0x8800000000
#define CONFIG_SYS_FSL_DRAM_SIZE3	0x7800000000	/* 480GB */
#endif

int fsl_qoriq_core_to_cluster(unsigned int core);
u32 cpu_mask(void);

#endif /* _FSL_LAYERSCAPE_CPU_H */
