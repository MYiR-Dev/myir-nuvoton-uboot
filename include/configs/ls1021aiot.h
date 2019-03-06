/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2016 Freescale Semiconductor, Inc.
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_ARMV7_SECURE_BASE OCRAM_BASE_S_ADDR

#define CONFIG_SYS_FSL_CLK

/*
 * Size of malloc() pool
 */
#define CONFIG_SYS_MALLOC_LEN	(CONFIG_ENV_SIZE + 16 * 1024 * 1024)

#define CONFIG_SYS_INIT_RAM_ADDR	OCRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	OCRAM_SIZE

#define CONFIG_SYS_CLK_FREQ		100000000
#define CONFIG_DDR_CLK_FREQ		100000000

/*
 * DDR: 800 MHz ( 1600 MT/s data rate )
 */

#define DDR_SDRAM_CFG			0x470c0008
#define DDR_CS0_BNDS			0x008000bf
#define DDR_CS0_CONFIG			0x80014302
#define DDR_TIMING_CFG_0		0x50550004
#define DDR_TIMING_CFG_1		0xbcb38c56
#define DDR_TIMING_CFG_2		0x0040d120
#define DDR_TIMING_CFG_3		0x010e1000
#define DDR_TIMING_CFG_4		0x00000001
#define DDR_TIMING_CFG_5		0x03401400
#define DDR_SDRAM_CFG_2			0x00401010
#define DDR_SDRAM_MODE			0x00061c60
#define DDR_SDRAM_MODE_2		0x00180000
#define DDR_SDRAM_INTERVAL		0x18600618
#define DDR_DDR_WRLVL_CNTL		0x8655f605
#define DDR_DDR_WRLVL_CNTL_2	0x05060607
#define DDR_DDR_WRLVL_CNTL_3	0x05050505
#define DDR_DDR_CDR1			0x80040000
#define DDR_DDR_CDR2			0x00000001
#define DDR_SDRAM_CLK_CNTL		0x02000000
#define DDR_DDR_ZQ_CNTL			0x89080600
#define DDR_CS0_CONFIG_2		0
#define DDR_SDRAM_CFG_MEM_EN	0x80000000
#define SDRAM_CFG2_D_INIT		0x00000010
#define DDR_CDR2_VREF_TRAIN_EN	0x00000080
#define SDRAM_CFG2_FRC_SR		0x80000000
#define SDRAM_CFG_BI			0x00000001

#ifdef CONFIG_RAMBOOT_PBL
#define CONFIG_SYS_FSL_PBL_PBI	\
	board/freescale/ls1021aiot/ls102xa_pbi.cfg
#endif

#ifdef CONFIG_SD_BOOT
#define CONFIG_SYS_FSL_PBL_RCW	\
	board/freescale/ls1021aiot/ls102xa_rcw_sd.cfg
#define CONFIG_SPL_LIBCOMMON_SUPPORT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_SPL_ENV_SUPPORT
#define CONFIG_SPL_MPC8XXX_INIT_DDR_SUPPORT
#define CONFIG_SPL_I2C_SUPPORT
#define CONFIG_SPL_WATCHDOG_SUPPORT
#define CONFIG_SPL_MMC_SUPPORT
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR	0xe8

#define CONFIG_SPL_TEXT_BASE	0x10000000
#define CONFIG_SPL_MAX_SIZE		0x1a000
#define CONFIG_SPL_STACK		0x1001d000
#define CONFIG_SPL_PAD_TO		0x1c000

#define CONFIG_SYS_SPL_MALLOC_START	(CONFIG_SYS_TEXT_BASE + \
		CONFIG_SYS_MONITOR_LEN)
#define CONFIG_SYS_SPL_MALLOC_SIZE	0x100000
#define CONFIG_SPL_BSS_START_ADDR	0x80100000
#define CONFIG_SPL_BSS_MAX_SIZE		0x80000
#define CONFIG_SYS_MONITOR_LEN		0x80000
#endif

#define CONFIG_SYS_DDR_SDRAM_BASE	0x80000000UL
#define CONFIG_SYS_SDRAM_BASE		CONFIG_SYS_DDR_SDRAM_BASE

#define CONFIG_CHIP_SELECTS_PER_CTRL	4

/*
 * Serial Port
 */
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE	1
#define CONFIG_SYS_NS16550_CLK		get_serial_clock()

/*
 * I2C
 */
#define CONFIG_CMD_I2C
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_MXC_I2C1		/* enable I2C bus 1 */
#define CONFIG_SYS_I2C_MXC_I2C2		/* enable I2C bus 2 */
#define CONFIG_SYS_I2C_MXC_I2C3		/* enable I2C bus 3 */

/* EEPROM */
#define CONFIG_ID_EEPROM
#define CONFIG_SYS_I2C_EEPROM_NXID
#define CONFIG_SYS_EEPROM_BUS_NUM		0
#define CONFIG_SYS_I2C_EEPROM_ADDR		0x51
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN	2

/*
 * MMC
 */
#define CONFIG_CMD_MMC

/* SATA */
#define CONFIG_SCSI_AHCI_PLAT
#ifndef PCI_DEVICE_ID_FREESCALE_AHCI
#define PCI_DEVICE_ID_FREESCALE_AHCI	0x0440
#endif
#define CONFIG_SCSI_DEV_LIST		{PCI_VENDOR_ID_FREESCALE, \
	PCI_DEVICE_ID_FREESCALE_AHCI}

#define CONFIG_SYS_SCSI_MAX_SCSI_ID	1
#define CONFIG_SYS_SCSI_MAX_LUN		1
#define CONFIG_SYS_SCSI_MAX_DEVICE	(CONFIG_SYS_SCSI_MAX_SCSI_ID * \
		CONFIG_SYS_SCSI_MAX_LUN)

/* SPI */
#if defined(CONFIG_QSPI_BOOT) || defined(CONFIG_SD_BOOT_QSPI)
#define CONFIG_SPI_FLASH_SPANSION

/* QSPI */
#define QSPI0_AMBA_BASE			0x40000000
#define FSL_QSPI_FLASH_SIZE		(1 << 24)
#define FSL_QSPI_FLASH_NUM		2
#define CONFIG_SPI_FLASH_SPANSION
#endif

/* DM SPI */
#if defined(CONFIG_FSL_DSPI) || defined(CONFIG_FSL_QSPI)
#define CONFIG_CMD_SF
#define CONFIG_DM_SPI_FLASH
#endif

/*
 * eTSEC
 */

#ifdef CONFIG_TSEC_ENET
#define CONFIG_MII_DEFAULT_TSEC		1
#define CONFIG_TSEC1			1
#define CONFIG_TSEC1_NAME		"eTSEC1"
#define CONFIG_TSEC2			1
#define CONFIG_TSEC2_NAME		"eTSEC2"

#define TSEC1_PHY_ADDR			1
#define TSEC2_PHY_ADDR			3

#define TSEC1_FLAGS			(TSEC_GIGABIT | TSEC_REDUCED)
#define TSEC2_FLAGS			(TSEC_GIGABIT | TSEC_REDUCED)

#define TSEC1_PHYIDX			0
#define TSEC2_PHYIDX			0

#define CONFIG_ETHPRIME			"eTSEC2"

#define CONFIG_PHY_ATHEROS

#define CONFIG_HAS_ETH0
#define CONFIG_HAS_ETH1
#define CONFIG_HAS_ETH2
#endif

/* PCIe */
#define CONFIG_PCIE1		/* PCIE controler 1 */
#define CONFIG_PCIE2		/* PCIE controler 2 */

#define FSL_PCIE_COMPAT		"fsl,ls1021a-pcie"

#ifdef CONFIG_PCI
#define CONFIG_PCI_SCAN_SHOW
#endif

#define CONFIG_CMD_MII

#define CONFIG_CMDLINE_TAG

#define CONFIG_PEN_ADDR_BIG_ENDIAN
#define CONFIG_LAYERSCAPE_NS_ACCESS
#define CONFIG_SMP_PEN_ADDR		0x01ee0200
#define COUNTER_FREQUENCY		12500000

#define CONFIG_HWCONFIG
#define HWCONFIG_BUFFER_SIZE		256

#define CONFIG_FSL_DEVICE_DISABLE

#define CONFIG_EXTRA_ENV_SETTINGS	\
	"bootargs=root=/dev/ram0 rw console=ttyS0,115200\0" \
"initrd_high=0xffffffff\0"	\
"fdt_high=0xffffffff\0"

/*
 * Miscellaneous configurable options
 */
#define CONFIG_CMD_GREPENV
#define CONFIG_CMD_MEMINFO

#define CONFIG_SYS_LOAD_ADDR		0x82000000

#define CONFIG_LS102XA_STREAM_ID

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

#ifdef CONFIG_SPL_BUILD
#define CONFIG_SYS_MONITOR_BASE CONFIG_SPL_TEXT_BASE
#else
/* start of monitor */
#define CONFIG_SYS_MONITOR_BASE CONFIG_SYS_TEXT_BASE
#endif

#define CONFIG_SYS_QE_FW_ADDR	0x67f40000

/*
 * Environment
 */

#define CONFIG_ENV_OVERWRITE

#if defined(CONFIG_SD_BOOT)
#define CONFIG_ENV_OFFSET		0x100000
#define CONFIG_SYS_MMC_ENV_DEV	0
#define CONFIG_ENV_SIZE			0x2000
#elif defined(CONFIG_QSPI_BOOT)
#define CONFIG_ENV_SIZE			0x2000
#define CONFIG_ENV_OFFSET		0x100000
#define CONFIG_ENV_SECT_SIZE	0x10000
#endif

#define CONFIG_OF_BOARD_SETUP
#define CONFIG_OF_STDOUT_VIA_ALIAS

#include <asm/fsl_secure_boot.h>

#endif
