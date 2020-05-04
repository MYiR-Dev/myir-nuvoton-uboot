/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Configuation settings for the bonito board
 *
 * Copyright (C) 2012 Renesas Solutions Corp.
 */

#ifndef __ARMADILLO_800EVA_H
#define __ARMADILLO_800EVA_H

#define CONFIG_SH_GPIO_PFC

#include <asm/arch/rmobile.h>

#define BOARD_LATE_INIT

#define CONFIG_TMU_TIMER
#define CONFIG_SYS_TIMER_COUNTS_DOWN
#define CONFIG_SYS_TIMER_COUNTER	(TMU_BASE + 0xc)	/* TCNT0 */
#define CONFIG_SYS_TIMER_RATE		(CONFIG_SYS_CLK_FREQ / 4)

/* STACK */
#define CONFIG_SYS_INIT_SP_ADDR		0xE8083000
#define STACK_AREA_SIZE				0xC000
#define LOW_LEVEL_MERAM_STACK	\
		(CONFIG_SYS_INIT_SP_ADDR + STACK_AREA_SIZE - 4)

/* MEMORY */
#define ARMADILLO_800EVA_SDRAM_BASE	0x40000000
#define ARMADILLO_800EVA_SDRAM_SIZE	(512 * 1024 * 1024)

#define CONFIG_SYS_PBSIZE		256
#define CONFIG_SYS_BAUDRATE_TABLE	{ 115200 }

/* SCIF */
#define CONFIG_CONS_SCIF1
#define SCIF0_BASE		0xe6c40000
#define SCIF1_BASE		0xe6c50000
#define SCIF2_BASE		0xe6c60000
#define SCIF4_BASE		0xe6c80000
#define	CONFIG_SCIF_A

#define CONFIG_SYS_MEMTEST_START	(ARMADILLO_800EVA_SDRAM_BASE)
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START + \
					 504 * 1024 * 1024)
#undef	CONFIG_SYS_LOADS_BAUD_CHANGE

#define CONFIG_SYS_SDRAM_BASE		(ARMADILLO_800EVA_SDRAM_BASE)
#define CONFIG_SYS_SDRAM_SIZE		(ARMADILLO_800EVA_SDRAM_SIZE)
#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + \
					 64 * 1024 * 1024)

#define CONFIG_SYS_MONITOR_BASE		0x00000000
#define CONFIG_SYS_MONITOR_LEN		(256 * 1024)
#define CONFIG_SYS_MALLOC_LEN		(1 * 1024 * 1024)
#define CONFIG_SYS_BOOTMAPSZ		(8 * 1024 * 1024)

/* FLASH */
#define CONFIG_SYS_FLASH_CFI_WIDTH	FLASH_CFI_16BIT
#define CONFIG_SYS_FLASH_BASE		0x00000000
#define CONFIG_SYS_MAX_FLASH_SECT	512
#define CONFIG_SYS_MAX_FLASH_BANKS	1
#define CONFIG_SYS_FLASH_BANKS_LIST	{ (CONFIG_SYS_FLASH_BASE) }

#define CONFIG_SYS_FLASH_ERASE_TOUT	3000
#define CONFIG_SYS_FLASH_WRITE_TOUT	3000
#define CONFIG_SYS_FLASH_LOCK_TOUT	3000
#define CONFIG_SYS_FLASH_UNLOCK_TOUT	3000

/* ENV setting */
#define CONFIG_ENV_OVERWRITE	1

/* SH Ether */
#define CONFIG_SH_ETHER_USE_PORT	0
#define CONFIG_SH_ETHER_PHY_ADDR	0x0
#define CONFIG_SH_ETHER_BASE_ADDR	0xe9a00000
#define CONFIG_SH_ETHER_SH7734_MII	(0x01)
#define CONFIG_SH_ETHER_PHY_MODE PHY_INTERFACE_MODE_MII
#define CONFIG_BITBANGMII_MULTI

/* Board Clock */
#define CONFIG_SYS_CLK_FREQ	50000000
#define CONFIG_SH_SCIF_CLK_FREQ CONFIG_SYS_CLK_FREQ

#endif	/* __ARMADILLO_800EVA_H */
