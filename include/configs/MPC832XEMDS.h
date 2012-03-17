/*
 * Copyright (C) 2006 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
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

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * High Level Configuration Options
 */
#define CONFIG_E300		1	/* E300 family */
#define CONFIG_QE		1	/* Has QE */
#define CONFIG_MPC83xx		1	/* MPC83xx family */
#define CONFIG_MPC832x		1	/* MPC832x CPU specific */
#define CONFIG_MPC832XEMDS	1	/* MPC832XEMDS board specific */

#define	CONFIG_SYS_TEXT_BASE	0xFE000000

/*
 * System Clock Setup
 */
#ifdef CONFIG_PCISLAVE
#define CONFIG_83XX_PCICLK	66000000	/* in HZ */
#else
#define CONFIG_83XX_CLKIN	66000000	/* in Hz */
#endif

#ifndef CONFIG_SYS_CLK_FREQ
#define CONFIG_SYS_CLK_FREQ	66000000
#endif

/*
 * Hardware Reset Configuration Word
 */
#define CONFIG_SYS_HRCW_LOW (\
	HRCWL_LCL_BUS_TO_SCB_CLK_1X1 |\
	HRCWL_DDR_TO_SCB_CLK_2X1 |\
	HRCWL_VCO_1X2 |\
	HRCWL_CSB_TO_CLKIN_2X1 |\
	HRCWL_CORE_TO_CSB_2X1 |\
	HRCWL_CE_PLL_VCO_DIV_2 |\
	HRCWL_CE_PLL_DIV_1X1 |\
	HRCWL_CE_TO_PLL_1X3)

#ifdef CONFIG_PCISLAVE
#define CONFIG_SYS_HRCW_HIGH (\
	HRCWH_PCI_AGENT |\
	HRCWH_PCI1_ARBITER_DISABLE |\
	HRCWH_CORE_ENABLE |\
	HRCWH_FROM_0XFFF00100 |\
	HRCWH_BOOTSEQ_DISABLE |\
	HRCWH_SW_WATCHDOG_DISABLE |\
	HRCWH_ROM_LOC_LOCAL_16BIT |\
	HRCWH_BIG_ENDIAN |\
	HRCWH_LALE_NORMAL)
#else
#define CONFIG_SYS_HRCW_HIGH (\
	HRCWH_PCI_HOST |\
	HRCWH_PCI1_ARBITER_ENABLE |\
	HRCWH_CORE_ENABLE |\
	HRCWH_FROM_0X00000100 |\
	HRCWH_BOOTSEQ_DISABLE |\
	HRCWH_SW_WATCHDOG_DISABLE |\
	HRCWH_ROM_LOC_LOCAL_16BIT |\
	HRCWH_BIG_ENDIAN |\
	HRCWH_LALE_NORMAL)
#endif

/*
 * System IO Config
 */
#define CONFIG_SYS_SICRL		0x00000000

#define CONFIG_BOARD_EARLY_INIT_F	/* call board_pre_init */
#define CONFIG_BOARD_EARLY_INIT_R

/*
 * IMMR new address
 */
#define CONFIG_SYS_IMMR		0xE0000000

/*
 * DDR Setup
 */
#define CONFIG_SYS_DDR_BASE	0x00000000	/* DDR is system memory */
#define CONFIG_SYS_SDRAM_BASE	CONFIG_SYS_DDR_BASE
#define CONFIG_SYS_DDR_SDRAM_BASE	CONFIG_SYS_DDR_BASE
#define CONFIG_SYS_DDRCDR	0x73000002	/* DDR II voltage is 1.8V */

#undef CONFIG_SPD_EEPROM
#if defined(CONFIG_SPD_EEPROM)
/* Determine DDR configuration from I2C interface
 */
#define SPD_EEPROM_ADDRESS	0x51	/* DDR SODIMM */
#else
/* Manually set up DDR parameters
 */
#define CONFIG_SYS_DDR_SIZE		128	/* MB */
#define CONFIG_SYS_DDR_CS0_CONFIG	(CSCONFIG_EN \
					| CSCONFIG_AP \
					| CSCONFIG_ODT_WR_CFG \
					| CSCONFIG_ROW_BIT_13 \
					| CSCONFIG_COL_BIT_10)
					/* 0x80840102 */
#define CONFIG_SYS_DDR_TIMING_0		((0 << TIMING_CFG0_RWT_SHIFT) \
					| (0 << TIMING_CFG0_WRT_SHIFT) \
					| (0 << TIMING_CFG0_RRT_SHIFT) \
					| (0 << TIMING_CFG0_WWT_SHIFT) \
					| (2 << TIMING_CFG0_ACT_PD_EXIT_SHIFT) \
					| (2 << TIMING_CFG0_PRE_PD_EXIT_SHIFT) \
					| (8 << TIMING_CFG0_ODT_PD_EXIT_SHIFT) \
					| (2 << TIMING_CFG0_MRS_CYC_SHIFT))
					/* 0x00220802 */
#define CONFIG_SYS_DDR_TIMING_1		((3 << TIMING_CFG1_PRETOACT_SHIFT) \
					| (9 << TIMING_CFG1_ACTTOPRE_SHIFT) \
					| (3 << TIMING_CFG1_ACTTORW_SHIFT) \
					| (5 << TIMING_CFG1_CASLAT_SHIFT) \
					| (13 << TIMING_CFG1_REFREC_SHIFT) \
					| (3 << TIMING_CFG1_WRREC_SHIFT) \
					| (2 << TIMING_CFG1_ACTTOACT_SHIFT) \
					| (2 << TIMING_CFG1_WRTORD_SHIFT))
					/* 0x3935D322 */
#define CONFIG_SYS_DDR_TIMING_2		((0 << TIMING_CFG2_ADD_LAT_SHIFT) \
				| (31 << TIMING_CFG2_CPO_SHIFT) \
				| (2 << TIMING_CFG2_WR_LAT_DELAY_SHIFT) \
				| (2 << TIMING_CFG2_RD_TO_PRE_SHIFT) \
				| (2 << TIMING_CFG2_WR_DATA_DELAY_SHIFT) \
				| (3 << TIMING_CFG2_CKE_PLS_SHIFT) \
				| (10 << TIMING_CFG2_FOUR_ACT_SHIFT))
				/* 0x0F9048CA */
#define CONFIG_SYS_DDR_TIMING_3		0x00000000
#define CONFIG_SYS_DDR_CLK_CNTL		DDR_SDRAM_CLK_CNTL_CLK_ADJUST_05
					/* 0x02000000 */
#define CONFIG_SYS_DDR_MODE		((0x4440 << SDRAM_MODE_ESD_SHIFT) \
					| (0x0232 << SDRAM_MODE_SD_SHIFT))
					/* 0x44400232 */
#define CONFIG_SYS_DDR_MODE2		0x8000c000
#define CONFIG_SYS_DDR_INTERVAL		((800 << SDRAM_INTERVAL_REFINT_SHIFT) \
					| (100 << SDRAM_INTERVAL_BSTOPRE_SHIFT))
					/* 0x03200064 */
#define CONFIG_SYS_DDR_CS0_BNDS		0x00000007
#define CONFIG_SYS_DDR_SDRAM_CFG	(SDRAM_CFG_SREN \
					| SDRAM_CFG_SDRAM_TYPE_DDR2 \
					| SDRAM_CFG_32_BE)
					/* 0x43080000 */
#define CONFIG_SYS_DDR_SDRAM_CFG2	0x00401000
#endif

/*
 * Memory test
 */
#undef CONFIG_SYS_DRAM_TEST		/* memory test, takes time */
#define CONFIG_SYS_MEMTEST_START	0x00000000	/* memtest region */
#define CONFIG_SYS_MEMTEST_END		0x00100000

/*
 * The reserved memory
 */
#define CONFIG_SYS_MONITOR_BASE	CONFIG_SYS_TEXT_BASE	/* start of monitor */

#if (CONFIG_SYS_MONITOR_BASE < CONFIG_SYS_FLASH_BASE)
#define CONFIG_SYS_RAMBOOT
#else
#undef  CONFIG_SYS_RAMBOOT
#endif

/* CONFIG_SYS_MONITOR_LEN must be a multiple of CONFIG_ENV_SECT_SIZE */
#define CONFIG_SYS_MONITOR_LEN	(384 * 1024)	/* Reserve 384 kB for Mon */
#define CONFIG_SYS_MALLOC_LEN	(256 * 1024)	/* Reserved for malloc */

/*
 * Initial RAM Base Address Setup
 */
#define CONFIG_SYS_INIT_RAM_LOCK	1
#define CONFIG_SYS_INIT_RAM_ADDR	0xE6000000	/* Initial RAM addr */
#define CONFIG_SYS_INIT_RAM_SIZE	0x1000	/* Size of used area in RAM */
#define CONFIG_SYS_GBL_DATA_OFFSET	\
			(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)

/*
 * Local Bus Configuration & Clock Setup
 */
#define CONFIG_SYS_LCRR_DBYP		LCRR_DBYP
#define CONFIG_SYS_LCRR_CLKDIV		LCRR_CLKDIV_2
#define CONFIG_SYS_LBC_LBCR		0x00000000

/*
 * FLASH on the Local Bus
 */
#define CONFIG_SYS_FLASH_CFI		/* use the Common Flash Interface */
#define CONFIG_FLASH_CFI_DRIVER		/* use the CFI driver */
#define CONFIG_SYS_FLASH_BASE	0xFE000000	/* FLASH base address */
#define CONFIG_SYS_FLASH_SIZE	16	/* FLASH size is 16M */
#define CONFIG_SYS_FLASH_PROTECTION	1	/* Use h/w Flash protection. */

					/* Window base at flash base */
#define CONFIG_SYS_LBLAWBAR0_PRELIM	CONFIG_SYS_FLASH_BASE
#define CONFIG_SYS_LBLAWAR0_PRELIM	(LBLAWAR_EN | LBLAWAR_32MB)

#define CONFIG_SYS_BR0_PRELIM	(CONFIG_SYS_FLASH_BASE \
				| BR_PS_16	/* 16 bit port */ \
				| BR_MS_GPCM	/* MSEL = GPCM */ \
				| BR_V)		/* valid */
#define CONFIG_SYS_OR0_PRELIM	(MEG_TO_AM(CONFIG_SYS_FLASH_SIZE) \
				| OR_GPCM_XAM \
				| OR_GPCM_CSNT \
				| OR_GPCM_ACS_DIV2 \
				| OR_GPCM_XACS \
				| OR_GPCM_SCY_15 \
				| OR_GPCM_TRLX_SET \
				| OR_GPCM_EHTR_SET \
				| OR_GPCM_EAD)
				/* 0xfe006ff7 */

#define CONFIG_SYS_MAX_FLASH_BANKS	1	/* number of banks */
#define CONFIG_SYS_MAX_FLASH_SECT	128	/* sectors per device */

#undef CONFIG_SYS_FLASH_CHECKSUM

/*
 * BCSR on the Local Bus
 */
#define CONFIG_SYS_BCSR			0xF8000000
					/* Access window base at BCSR base */
#define CONFIG_SYS_LBLAWBAR1_PRELIM	CONFIG_SYS_BCSR
#define CONFIG_SYS_LBLAWAR1_PRELIM	(LBLAWAR_EN | LBLAWAR_32KB)

#define CONFIG_SYS_BR1_PRELIM		(CONFIG_SYS_BCSR \
					| BR_PS_8 \
					| BR_MS_GPCM \
					| BR_V)
#define CONFIG_SYS_OR1_PRELIM		(OR_AM_32KB \
					| OR_GPCM_XAM \
					| OR_GPCM_CSNT \
					| OR_GPCM_XACS \
					| OR_GPCM_SCY_15 \
					| OR_GPCM_TRLX_SET \
					| OR_GPCM_EHTR_SET \
					| OR_GPCM_EAD)
					/* 0xFFFFE9F7 */

/*
 * Windows to access PIB via local bus
 */
					/* PIB window base 0xF8008000 */
#define CONFIG_SYS_PIB_BASE		0xF8008000
#define CONFIG_SYS_PIB_WINDOW_SIZE	(32 * 1024)
#define CONFIG_SYS_LBLAWBAR3_PRELIM	CONFIG_SYS_PIB_BASE
#define CONFIG_SYS_LBLAWAR3_PRELIM	(LBLAWAR_EN | LBLAWAR_64KB)

/*
 * CS2 on Local Bus, to PIB
 */
#define CONFIG_SYS_BR2_PRELIM	(CONFIG_SYS_PIB_BASE \
				| BR_PS_8 \
				| BR_MS_GPCM \
				| BR_V)
				/* 0xF8008801 */
#define CONFIG_SYS_OR2_PRELIM	(P2SZ_TO_AM(CONFIG_SYS_PIB_WINDOW_SIZE) \
				| OR_GPCM_XAM \
				| OR_GPCM_CSNT \
				| OR_GPCM_XACS \
				| OR_GPCM_SCY_15 \
				| OR_GPCM_TRLX_SET \
				| OR_GPCM_EHTR_SET \
				| OR_GPCM_EAD)
				/* 0xffffe9f7 */

/*
 * CS3 on Local Bus, to PIB
 */
#define CONFIG_SYS_BR3_PRELIM	((CONFIG_SYS_PIB_BASE + \
					CONFIG_SYS_PIB_WINDOW_SIZE) \
				| BR_PS_8 \
				| BR_MS_GPCM \
				| BR_V)
				/* 0xF8010801 */
#define CONFIG_SYS_OR3_PRELIM	(P2SZ_TO_AM(CONFIG_SYS_PIB_WINDOW_SIZE) \
				| OR_GPCM_XAM \
				| OR_GPCM_CSNT \
				| OR_GPCM_XACS \
				| OR_GPCM_SCY_15 \
				| OR_GPCM_TRLX_SET \
				| OR_GPCM_EHTR_SET \
				| OR_GPCM_EAD)
				/* 0xffffe9f7 */

/*
 * Serial Port
 */
#define CONFIG_CONS_INDEX	1
#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE	1
#define CONFIG_SYS_NS16550_CLK		get_bus_freq(0)

#define CONFIG_SYS_BAUDRATE_TABLE  \
		{300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 115200}

#define CONFIG_SYS_NS16550_COM1	(CONFIG_SYS_IMMR+0x4500)
#define CONFIG_SYS_NS16550_COM2	(CONFIG_SYS_IMMR+0x4600)

#define CONFIG_CMDLINE_EDITING	1	/* add command line history	*/
#define CONFIG_AUTO_COMPLETE		/* add autocompletion support   */
/* Use the HUSH parser */
#define CONFIG_SYS_HUSH_PARSER
#ifdef CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2 "> "
#endif

/* pass open firmware flat tree */
#define CONFIG_OF_LIBFDT	1
#define CONFIG_OF_BOARD_SETUP	1
#define CONFIG_OF_STDOUT_VIA_ALIAS	1

/* I2C */
#define CONFIG_HARD_I2C		/* I2C with hardware support */
#undef CONFIG_SOFT_I2C		/* I2C bit-banged */
#define CONFIG_FSL_I2C
#define CONFIG_SYS_I2C_SPEED	400000	/* I2C speed and slave address */
#define CONFIG_SYS_I2C_SLAVE	0x7F
#define CONFIG_SYS_I2C_NOPROBES	{0x51}	/* Don't probe these addrs */
#define CONFIG_SYS_I2C_OFFSET	0x3000

/*
 * Config on-board RTC
 */
#define CONFIG_RTC_DS1374		/* use ds1374 rtc via i2c */
#define CONFIG_SYS_I2C_RTC_ADDR	0x68	/* at address 0x68 */

/*
 * General PCI
 * Addresses are mapped 1-1.
 */
#define CONFIG_SYS_PCI1_MEM_BASE	0x80000000
#define CONFIG_SYS_PCI1_MEM_PHYS	CONFIG_SYS_PCI1_MEM_BASE
#define CONFIG_SYS_PCI1_MEM_SIZE	0x10000000	/* 256M */
#define CONFIG_SYS_PCI1_MMIO_BASE	0x90000000
#define CONFIG_SYS_PCI1_MMIO_PHYS	CONFIG_SYS_PCI1_MMIO_BASE
#define CONFIG_SYS_PCI1_MMIO_SIZE	0x10000000	/* 256M */
#define CONFIG_SYS_PCI1_IO_BASE		0x00000000
#define CONFIG_SYS_PCI1_IO_PHYS		0xE0300000
#define CONFIG_SYS_PCI1_IO_SIZE		0x100000	/* 1M */

#define CONFIG_SYS_PCI_SLV_MEM_LOCAL	CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_PCI_SLV_MEM_BUS	0x00000000
#define CONFIG_SYS_PCI_SLV_MEM_SIZE	0x80000000


#ifdef CONFIG_PCI

#define CONFIG_PCI_PNP		/* do pci plug-and-play */
#define CONFIG_83XX_PCI_STREAMING

#undef CONFIG_EEPRO100
#undef CONFIG_PCI_SCAN_SHOW	/* show pci devices on startup */
#define CONFIG_SYS_PCI_SUBSYS_VENDORID	0x1957	/* Freescale */

#endif	/* CONFIG_PCI */

/*
 * QE UEC ethernet configuration
 */
#define CONFIG_UEC_ETH
#define CONFIG_ETHPRIME		"UEC0"

#define CONFIG_UEC_ETH1		/* ETH3 */

#ifdef CONFIG_UEC_ETH1
#define CONFIG_SYS_UEC1_UCC_NUM	2	/* UCC3 */
#define CONFIG_SYS_UEC1_RX_CLK		QE_CLK9
#define CONFIG_SYS_UEC1_TX_CLK		QE_CLK10
#define CONFIG_SYS_UEC1_ETH_TYPE	FAST_ETH
#define CONFIG_SYS_UEC1_PHY_ADDR	3
#define CONFIG_SYS_UEC1_INTERFACE_TYPE	PHY_INTERFACE_MODE_MII
#define CONFIG_SYS_UEC1_INTERFACE_SPEED	100
#endif

#define CONFIG_UEC_ETH2		/* ETH4 */

#ifdef CONFIG_UEC_ETH2
#define CONFIG_SYS_UEC2_UCC_NUM	3	/* UCC4 */
#define CONFIG_SYS_UEC2_RX_CLK		QE_CLK7
#define CONFIG_SYS_UEC2_TX_CLK		QE_CLK8
#define CONFIG_SYS_UEC2_ETH_TYPE	FAST_ETH
#define CONFIG_SYS_UEC2_PHY_ADDR	4
#define CONFIG_SYS_UEC2_INTERFACE_TYPE	PHY_INTERFACE_MODE_MII
#define CONFIG_SYS_UEC2_INTERFACE_SPEED	100
#endif

/*
 * Environment
 */
#ifndef CONFIG_SYS_RAMBOOT
	#define CONFIG_ENV_IS_IN_FLASH	1
	#define CONFIG_ENV_ADDR		\
			(CONFIG_SYS_MONITOR_BASE + CONFIG_SYS_MONITOR_LEN)
	#define CONFIG_ENV_SECT_SIZE	0x20000
	#define CONFIG_ENV_SIZE		0x2000
#else
	#define CONFIG_SYS_NO_FLASH	1	/* Flash is not usable now */
	#define CONFIG_ENV_IS_NOWHERE	1	/* Store ENV in memory only */
	#define CONFIG_ENV_ADDR		(CONFIG_SYS_MONITOR_BASE - 0x1000)
	#define CONFIG_ENV_SIZE		0x2000
#endif

#define CONFIG_LOADS_ECHO	1	/* echo on for serial download */
#define CONFIG_SYS_LOADS_BAUD_CHANGE	1	/* allow baudrate change */

/*
 * BOOTP options
 */
#define CONFIG_BOOTP_BOOTFILESIZE
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME


/*
 * Command line configuration.
 */
#include <config_cmd_default.h>

#define CONFIG_CMD_PING
#define CONFIG_CMD_I2C
#define CONFIG_CMD_ASKENV

#if defined(CONFIG_PCI)
    #define CONFIG_CMD_PCI
#endif

#if defined(CONFIG_SYS_RAMBOOT)
    #undef CONFIG_CMD_SAVEENV
    #undef CONFIG_CMD_LOADS
#endif


#undef CONFIG_WATCHDOG		/* watchdog disabled */

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP	/* undef to save memory */
#define CONFIG_SYS_LOAD_ADDR	0x2000000	/* default load address */
#define CONFIG_SYS_PROMPT	"=> "	/* Monitor Command Prompt */

#if defined(CONFIG_CMD_KGDB)
	#define CONFIG_SYS_CBSIZE	1024	/* Console I/O Buffer Size */
#else
	#define CONFIG_SYS_CBSIZE	256	/* Console I/O Buffer Size */
#endif

				/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16)
#define CONFIG_SYS_MAXARGS	16	/* max number of command args */
				/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE
#define CONFIG_SYS_HZ		1000	/* decrementer freq: 1ms ticks */

/*
 * For booting Linux, the board info and command line data
 * have to be in the first 256 MB of memory, since this is
 * the maximum mapped by the Linux kernel during initialization.
 */
					/* Initial Memory map for Linux */
#define CONFIG_SYS_BOOTMAPSZ		(256 << 20)

/*
 * Core HID Setup
 */
#define CONFIG_SYS_HID0_INIT	0x000000000
#define CONFIG_SYS_HID0_FINAL	(HID0_ENABLE_MACHINE_CHECK | \
				 HID0_ENABLE_INSTRUCTION_CACHE)
#define CONFIG_SYS_HID2		HID2_HBE

/*
 * MMU Setup
 */

#define CONFIG_HIGH_BATS	1	/* High BATs supported */

/* DDR: cache cacheable */
#define CONFIG_SYS_IBAT0L	(CONFIG_SYS_SDRAM_BASE \
				| BATL_PP_RW \
				| BATL_MEMCOHERENCE)
#define CONFIG_SYS_IBAT0U	(CONFIG_SYS_SDRAM_BASE \
				| BATU_BL_256M \
				| BATU_VS \
				| BATU_VP)
#define CONFIG_SYS_DBAT0L	CONFIG_SYS_IBAT0L
#define CONFIG_SYS_DBAT0U	CONFIG_SYS_IBAT0U

/* IMMRBAR & PCI IO: cache-inhibit and guarded */
#define CONFIG_SYS_IBAT1L	(CONFIG_SYS_IMMR \
				| BATL_PP_RW \
				| BATL_CACHEINHIBIT \
				| BATL_GUARDEDSTORAGE)
#define CONFIG_SYS_IBAT1U	(CONFIG_SYS_IMMR \
				| BATU_BL_4M \
				| BATU_VS \
				| BATU_VP)
#define CONFIG_SYS_DBAT1L	CONFIG_SYS_IBAT1L
#define CONFIG_SYS_DBAT1U	CONFIG_SYS_IBAT1U

/* BCSR: cache-inhibit and guarded */
#define CONFIG_SYS_IBAT2L	(CONFIG_SYS_BCSR \
				| BATL_PP_RW \
				| BATL_CACHEINHIBIT \
				| BATL_GUARDEDSTORAGE)
#define CONFIG_SYS_IBAT2U	(CONFIG_SYS_BCSR \
				| BATU_BL_128K \
				| BATU_VS \
				| BATU_VP)
#define CONFIG_SYS_DBAT2L	CONFIG_SYS_IBAT2L
#define CONFIG_SYS_DBAT2U	CONFIG_SYS_IBAT2U

/* FLASH: icache cacheable, but dcache-inhibit and guarded */
#define CONFIG_SYS_IBAT3L	(CONFIG_SYS_FLASH_BASE \
				| BATL_PP_RW \
				| BATL_MEMCOHERENCE)
#define CONFIG_SYS_IBAT3U	(CONFIG_SYS_FLASH_BASE \
				| BATU_BL_32M \
				| BATU_VS \
				| BATU_VP)
#define CONFIG_SYS_DBAT3L	(CONFIG_SYS_FLASH_BASE \
				| BATL_PP_RW \
				| BATL_CACHEINHIBIT \
				| BATL_GUARDEDSTORAGE)
#define CONFIG_SYS_DBAT3U	CONFIG_SYS_IBAT3U

#define CONFIG_SYS_IBAT4L	(0)
#define CONFIG_SYS_IBAT4U	(0)
#define CONFIG_SYS_DBAT4L	CONFIG_SYS_IBAT4L
#define CONFIG_SYS_DBAT4U	CONFIG_SYS_IBAT4U

/* Stack in dcache: cacheable, no memory coherence */
#define CONFIG_SYS_IBAT5L	(CONFIG_SYS_INIT_RAM_ADDR | BATL_PP_RW)
#define CONFIG_SYS_IBAT5U	(CONFIG_SYS_INIT_RAM_ADDR \
				| BATU_BL_128K \
				| BATU_VS \
				| BATU_VP)
#define CONFIG_SYS_DBAT5L	CONFIG_SYS_IBAT5L
#define CONFIG_SYS_DBAT5U	CONFIG_SYS_IBAT5U

#ifdef CONFIG_PCI
/* PCI MEM space: cacheable */
#define CONFIG_SYS_IBAT6L	(CONFIG_SYS_PCI1_MEM_PHYS \
				| BATL_PP_RW \
				| BATL_MEMCOHERENCE)
#define CONFIG_SYS_IBAT6U	(CONFIG_SYS_PCI1_MEM_PHYS \
				| BATU_BL_256M \
				| BATU_VS \
				| BATU_VP)
#define CONFIG_SYS_DBAT6L	CONFIG_SYS_IBAT6L
#define CONFIG_SYS_DBAT6U	CONFIG_SYS_IBAT6U
/* PCI MMIO space: cache-inhibit and guarded */
#define CONFIG_SYS_IBAT7L	(CONFIG_SYS_PCI1_MMIO_PHYS \
				| BATL_PP_RW \
				| BATL_CACHEINHIBIT \
				| BATL_GUARDEDSTORAGE)
#define CONFIG_SYS_IBAT7U	(CONFIG_SYS_PCI1_MMIO_PHYS \
				| BATU_BL_256M \
				| BATU_VS \
				| BATU_VP)
#define CONFIG_SYS_DBAT7L	CONFIG_SYS_IBAT7L
#define CONFIG_SYS_DBAT7U	CONFIG_SYS_IBAT7U
#else
#define CONFIG_SYS_IBAT6L	(0)
#define CONFIG_SYS_IBAT6U	(0)
#define CONFIG_SYS_IBAT7L	(0)
#define CONFIG_SYS_IBAT7U	(0)
#define CONFIG_SYS_DBAT6L	CONFIG_SYS_IBAT6L
#define CONFIG_SYS_DBAT6U	CONFIG_SYS_IBAT6U
#define CONFIG_SYS_DBAT7L	CONFIG_SYS_IBAT7L
#define CONFIG_SYS_DBAT7U	CONFIG_SYS_IBAT7U
#endif

#if defined(CONFIG_CMD_KGDB)
#define CONFIG_KGDB_BAUDRATE	230400	/* speed of kgdb serial port */
#define CONFIG_KGDB_SER_INDEX	2	/* which serial port to use */
#endif

/*
 * Environment Configuration
 */ #define CONFIG_ENV_OVERWRITE

#if defined(CONFIG_UEC_ETH)
#define CONFIG_HAS_ETH0
#define CONFIG_HAS_ETH1
#endif

#define CONFIG_BAUDRATE	115200

#define CONFIG_LOADADDR	800000	/* default location for tftp and bootm */

#define CONFIG_BOOTDELAY 6	/* -1 disables auto-boot */
#undef  CONFIG_BOOTARGS		/* the boot command will set bootargs */

#define CONFIG_EXTRA_ENV_SETTINGS					\
	"netdev=eth0\0"							\
	"consoledev=ttyS0\0"						\
	"ramdiskaddr=1000000\0"						\
	"ramdiskfile=ramfs.83xx\0"					\
	"fdtaddr=780000\0"						\
	"fdtfile=mpc832x_mds.dtb\0"					\
	""

#define CONFIG_NFSBOOTCOMMAND						\
	"setenv bootargs root=/dev/nfs rw "				\
		"nfsroot=$serverip:$rootpath "				\
		"ip=$ipaddr:$serverip:$gatewayip:$netmask:$hostname:"	\
							"$netdev:off "	\
		"console=$consoledev,$baudrate $othbootargs;"		\
	"tftp $loadaddr $bootfile;"					\
	"tftp $fdtaddr $fdtfile;"					\
	"bootm $loadaddr - $fdtaddr"

#define CONFIG_RAMBOOTCOMMAND						\
	"setenv bootargs root=/dev/ram rw "				\
		"console=$consoledev,$baudrate $othbootargs;"		\
	"tftp $ramdiskaddr $ramdiskfile;"				\
	"tftp $loadaddr $bootfile;"					\
	"tftp $fdtaddr $fdtfile;"					\
	"bootm $loadaddr $ramdiskaddr $fdtaddr"


#define CONFIG_BOOTCOMMAND CONFIG_NFSBOOTCOMMAND

#endif	/* __CONFIG_H */
