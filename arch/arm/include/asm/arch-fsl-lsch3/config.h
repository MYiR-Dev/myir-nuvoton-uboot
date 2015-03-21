/*
 * Copyright 2014, Freescale Semiconductor
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _ASM_ARMV8_FSL_LSCH3_CONFIG_
#define _ASM_ARMV8_FSL_LSCH3_CONFIG_

#include <fsl_ddrc_version.h>

#define CONFIG_SYS_PAGE_SIZE		0x10000

#ifndef L1_CACHE_BYTES
#define L1_CACHE_SHIFT		6
#define L1_CACHE_BYTES		(1 << L1_CACHE_SHIFT)
#endif

#define CONFIG_MP
#define CONFIG_SYS_FSL_OCRAM_BASE	0x18000000	/* initial RAM */
/* Link Definitions */
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_FSL_OCRAM_BASE + 0xfff0)

#define CONFIG_SYS_IMMR				0x01000000
#define CONFIG_SYS_FSL_DDR_ADDR			(CONFIG_SYS_IMMR + 0x00080000)
#define CONFIG_SYS_FSL_DDR2_ADDR		(CONFIG_SYS_IMMR + 0x00090000)
#define CONFIG_SYS_FSL_DDR3_ADDR		0x08210000
#define CONFIG_SYS_FSL_GUTS_ADDR		(CONFIG_SYS_IMMR + 0x00E00000)
#define CONFIG_SYS_FSL_PMU_ADDR			(CONFIG_SYS_IMMR + 0x00E30000)
#define CONFIG_SYS_FSL_RST_ADDR			(CONFIG_SYS_IMMR + 0x00E60000)
#define CONFIG_SYS_FSL_CH3_CLK_GRPA_ADDR	(CONFIG_SYS_IMMR + 0x00300000)
#define CONFIG_SYS_FSL_CH3_CLK_GRPB_ADDR	(CONFIG_SYS_IMMR + 0x00310000)
#define CONFIG_SYS_FSL_CH3_CLK_CTRL_ADDR	(CONFIG_SYS_IMMR + 0x00370000)
#define CONFIG_SYS_IFC_ADDR			(CONFIG_SYS_IMMR + 0x01240000)
#define CONFIG_SYS_NS16550_COM1			(CONFIG_SYS_IMMR + 0x011C0500)
#define CONFIG_SYS_NS16550_COM2			(CONFIG_SYS_IMMR + 0x011C0600)
#define CONFIG_SYS_FSL_TIMER_ADDR		0x023d0000
#define CONFIG_SYS_FSL_PMU_CLTBENR		(CONFIG_SYS_FSL_PMU_ADDR + \
						 0x18A0)

/* SP (Cortex-A5) related */
#define CONFIG_SYS_FSL_SP_ADDR			(CONFIG_SYS_IMMR + 0x00F00000)
#define CONFIG_SYS_FSL_SP_VSG_GIC_ADDR		(CONFIG_SYS_FSL_SP_ADDR)
#define CONFIG_SYS_FSL_SP_VSG_GIC_VIGR1		(CONFIG_SYS_FSL_SP_ADDR)
#define CONFIG_SYS_FSL_SP_VSG_GIC_VIGR2		\
					(CONFIG_SYS_FSL_SP_ADDR + 0x0008)
#define CONFIG_SYS_FSL_SP_LOOPBACK_DUART	\
					(CONFIG_SYS_FSL_SP_ADDR + 0x1000)

#define CONFIG_SYS_FSL_DCSR_DDR_ADDR		0x70012c000ULL
#define CONFIG_SYS_FSL_DCSR_DDR2_ADDR		0x70012d000ULL
#define CONFIG_SYS_FSL_DCSR_DDR3_ADDR		0x700132000ULL
#define CONFIG_SYS_FSL_DCSR_DDR4_ADDR		0x700133000ULL

#define I2C1_BASE_ADDR				(CONFIG_SYS_IMMR + 0x01000000)
#define I2C2_BASE_ADDR				(CONFIG_SYS_IMMR + 0x01010000)
#define I2C3_BASE_ADDR				(CONFIG_SYS_IMMR + 0x01020000)
#define I2C4_BASE_ADDR				(CONFIG_SYS_IMMR + 0x01030000)

/* TZ Protection Controller Definitions */
#define TZPC_BASE				0x02200000
#define TZPCR0SIZE_BASE				(TZPC_BASE)
#define TZPCDECPROT_0_STAT_BASE			(TZPC_BASE + 0x800)
#define TZPCDECPROT_0_SET_BASE			(TZPC_BASE + 0x804)
#define TZPCDECPROT_0_CLR_BASE			(TZPC_BASE + 0x808)
#define TZPCDECPROT_1_STAT_BASE			(TZPC_BASE + 0x80C)
#define TZPCDECPROT_1_SET_BASE			(TZPC_BASE + 0x810)
#define TZPCDECPROT_1_CLR_BASE			(TZPC_BASE + 0x814)
#define TZPCDECPROT_2_STAT_BASE			(TZPC_BASE + 0x818)
#define TZPCDECPROT_2_SET_BASE			(TZPC_BASE + 0x81C)
#define TZPCDECPROT_2_CLR_BASE			(TZPC_BASE + 0x820)

/* TZ Address Space Controller Definitions */
#define TZASC1_BASE			0x01100000	/* as per CCSR map. */
#define TZASC2_BASE			0x01110000	/* as per CCSR map. */
#define TZASC3_BASE			0x01120000	/* as per CCSR map. */
#define TZASC4_BASE			0x01130000	/* as per CCSR map. */
#define TZASC_BUILD_CONFIG_REG(x)	((TZASC1_BASE + (x * 0x10000)))
#define TZASC_ACTION_REG(x)		((TZASC1_BASE + (x * 0x10000)) + 0x004)
#define TZASC_GATE_KEEPER(x)		((TZASC1_BASE + (x * 0x10000)) + 0x008)
#define TZASC_REGION_BASE_LOW_0(x)	((TZASC1_BASE + (x * 0x10000)) + 0x100)
#define TZASC_REGION_BASE_HIGH_0(x)	((TZASC1_BASE + (x * 0x10000)) + 0x104)
#define TZASC_REGION_TOP_LOW_0(x)	((TZASC1_BASE + (x * 0x10000)) + 0x108)
#define TZASC_REGION_TOP_HIGH_0(x)	((TZASC1_BASE + (x * 0x10000)) + 0x10C)
#define TZASC_REGION_ATTRIBUTES_0(x)	((TZASC1_BASE + (x * 0x10000)) + 0x110)
#define TZASC_REGION_ID_ACCESS_0(x)	((TZASC1_BASE + (x * 0x10000)) + 0x114)

/* Generic Interrupt Controller Definitions */
#define GICD_BASE		0x06000000
#define GICR_BASE		0x06100000

/* SMMU Defintions */
#define SMMU_BASE		0x05000000 /* GR0 Base */

/* DDR */
#define CONFIG_SYS_FSL_DDR_LE
#define CONFIG_VERY_BIG_RAM
#ifdef CONFIG_SYS_FSL_DDR4
#define CONFIG_SYS_FSL_DDRC_GEN4
#else
#define CONFIG_SYS_FSL_DDRC_ARM_GEN3	/* Enable Freescale ARM DDR3 driver */
#endif
#define CONFIG_SYS_FSL_DDR		/* Freescale DDR driver */
#define CONFIG_SYS_LS2_DDR_BLOCK1_SIZE	((phys_size_t)2 << 30)
#define CONFIG_MAX_MEM_MAPPED		CONFIG_SYS_LS2_DDR_BLOCK1_SIZE
#define CONFIG_SYS_FSL_DDR_VER		FSL_DDR_VER_5_0

/* IFC */
#define CONFIG_SYS_FSL_IFC_LE

#ifdef CONFIG_LS2085A
#define CONFIG_MAX_CPUS				16
#define CONFIG_SYS_FSL_IFC_BANK_COUNT		8
#define CONFIG_NUM_DDR_CONTROLLERS		3
#define CONFIG_SYS_FSL_CLUSTER_CLOCKS		{ 1, 1, 4, 4 }
#else
#error SoC not defined
#endif

#ifdef CONFIG_LS2085A
#define CONFIG_SYS_FSL_ERRATUM_A008336
#define CONFIG_SYS_FSL_ERRATUM_A008514
#define CONFIG_SYS_FSL_ERRATUM_A008585
#endif

#endif /* _ASM_ARMV8_FSL_LSCH3_CONFIG_ */
