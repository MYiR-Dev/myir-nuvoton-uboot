/*
 * include/configs/lager.h
 *     This file is lager board configuration.
 *
 * Copyright (C) 2013 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: GPL-2.0
 */

#ifndef __LAGER_H
#define __LAGER_H

#undef DEBUG
#define CONFIG_ARMV7
#define CONFIG_R8A7790
#define CONFIG_RMOBILE
#define CONFIG_RMOBILE_BOARD_STRING "Lager"
#define CONFIG_SH_GPIO_PFC

#include <asm/arch/rmobile.h>

#define	CONFIG_CMD_EDITENV
#define	CONFIG_CMD_SAVEENV
#define CONFIG_CMD_MEMORY
#define CONFIG_CMD_DFL
#define CONFIG_CMD_SDRAM
#define CONFIG_CMD_RUN
#define CONFIG_CMD_LOADS
#define CONFIG_CMD_NET
#define CONFIG_CMD_MII
#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_NFS
#define CONFIG_CMD_BOOTZ

#define CONFIG_CMD_SF
#define CONFIG_CMD_SPI
#define CONFIG_SYS_TEXT_BASE	0xE8080000
#define CONFIG_SYS_THUMB_BUILD

#define	CONFIG_CMDLINE_TAG
#define	CONFIG_SETUP_MEMORY_TAGS
#define	CONFIG_INITRD_TAG
#define	CONFIG_CMDLINE_EDITING
#define	CONFIG_OF_LIBFDT

/* #define CONFIG_OF_LIBFDT */
#define BOARD_LATE_INIT

#define CONFIG_BAUDRATE		38400
#define CONFIG_BOOTDELAY	3
#define CONFIG_BOOTARGS		""

#define CONFIG_VERSION_VARIABLE
#undef	CONFIG_SHOW_BOOT_PROGRESS

#define CONFIG_ARCH_CPU_INIT
#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO
#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_TMU_TIMER

/* STACK */
#define CONFIG_SYS_INIT_SP_ADDR		0xE827fffc
#define STACK_AREA_SIZE				0xC000
#define LOW_LEVEL_MERAM_STACK	\
		(CONFIG_SYS_INIT_SP_ADDR + STACK_AREA_SIZE - 4)

/* MEMORY */
#define LAGER_SDRAM_BASE	0x40000000
#define LAGER_SDRAM_SIZE	(2048u * 1024 * 1024)
#define LAGER_UBOOT_SDRAM_SIZE	(512 * 1024 * 1024)

#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_CBSIZE		256
#define CONFIG_SYS_PBSIZE		256
#define CONFIG_SYS_MAXARGS		16
#define CONFIG_SYS_BARGSIZE		512
#define CONFIG_SYS_BAUDRATE_TABLE	{ 38400, 115200 }

/* SCIF */
#define CONFIG_SCIF_CONSOLE
#define CONFIG_CONS_SCIF0
#undef	CONFIG_SYS_CONSOLE_INFO_QUIET
#undef	CONFIG_SYS_CONSOLE_OVERWRITE_ROUTINE
#undef	CONFIG_SYS_CONSOLE_ENV_OVERWRITE

#define CONFIG_SYS_MEMTEST_START	(LAGER_SDRAM_BASE)
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START + \
					 504 * 1024 * 1024)
#undef	CONFIG_SYS_ALT_MEMTEST
#undef	CONFIG_SYS_MEMTEST_SCRATCH
#undef	CONFIG_SYS_LOADS_BAUD_CHANGE

#define CONFIG_SYS_SDRAM_BASE		(LAGER_SDRAM_BASE)
#define CONFIG_SYS_SDRAM_SIZE		(LAGER_UBOOT_SDRAM_SIZE)
#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + 0x7fc0)
#define CONFIG_NR_DRAM_BANKS		1

#define CONFIG_SYS_MONITOR_BASE		0x00000000
#define CONFIG_SYS_MONITOR_LEN		(256 * 1024)
#define CONFIG_SYS_MALLOC_LEN		(1 * 1024 * 1024)
#define CONFIG_SYS_BOOTMAPSZ		(8 * 1024 * 1024)

/* USE SPI */
#define CONFIG_SPI
#define CONFIG_SPI_FLASH_BAR
#define CONFIG_SH_QSPI
#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_SPANSION
#define CONFIG_SYS_NO_FLASH

/* ENV setting */
#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_ADDR	0xC0000

/* Common ENV setting */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_ENV_SECT_SIZE	(256 * 1024)
#define CONFIG_ENV_OFFSET	(CONFIG_ENV_ADDR)
#define CONFIG_ENV_SIZE		(CONFIG_ENV_SECT_SIZE)
#define CONFIG_ENV_SIZE_REDUND	(CONFIG_SYS_MONITOR_LEN)

/* SH Ether */
#define	CONFIG_NET_MULTI
#define CONFIG_SH_ETHER
#define CONFIG_SH_ETHER_USE_PORT	0
#define CONFIG_SH_ETHER_PHY_ADDR	0x1
#define CONFIG_SH_ETHER_PHY_MODE PHY_INTERFACE_MODE_RMII
#define CONFIG_SH_ETHER_ALIGNE_SIZE	64
#define CONFIG_SH_ETHER_CACHE_WRITEBACK
#define CONFIG_SH_ETHER_CACHE_INVALIDATE
#define CONFIG_PHYLIB
#define CONFIG_PHY_MICREL
#define CONFIG_BITBANGMII
#define CONFIG_BITBANGMII_MULTI

/* I2C */
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_RCAR
#define CONFIG_SYS_RCAR_I2C0_BASE	0xE6508000
#define CONFIG_SYS_RCAR_I2C0_SPEED	400000
#define CONFIG_SYS_RCAR_I2C1_BASE	0xE6518000
#define CONFIG_SYS_RCAR_I2C1_SPEED	400000
#define CONFIG_SYS_RCAR_I2C2_BASE	0xE6530000
#define CONFIG_SYS_RCAR_I2C2_SPEED	400000
#define CONFIG_SYS_RCAR_I2C3_BASE	0xE6540000
#define CONFIG_SYS_RCAR_I2C3_SPEED	400000
#define CONFIF_SYS_RCAR_I2C_NUM_CONTROLLERS	4

#define CONFIG_SYS_I2C_POWERIC_ADDR 0x58 /* da9063 */

/* Board Clock */
#define RMOBILE_XTAL_CLK	20000000u
#define CONFIG_SYS_CLK_FREQ	RMOBILE_XTAL_CLK
#define CONFIG_SH_TMU_CLK_FREQ	(CONFIG_SYS_CLK_FREQ / 2) /* EXT / 2 */
#define CONFIG_PLL1_CLK_FREQ	(CONFIG_SYS_CLK_FREQ * 156 / 2)
#define CONFIG_PLL1_DIV2_CLK_FREQ	(CONFIG_PLL1_CLK_FREQ / 2)
#define CONFIG_MP_CLK_FREQ	(CONFIG_PLL1_DIV2_CLK_FREQ / 15)
#define CONFIG_HP_CLK_FREQ	(CONFIG_PLL1_CLK_FREQ / 12)
#define CONFIG_SH_SCIF_CLK_FREQ	CONFIG_MP_CLK_FREQ

#define CONFIG_SYS_TMU_CLK_DIV	4

#endif	/* __LAGER_H */
