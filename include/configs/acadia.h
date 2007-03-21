/*
 * (C) Copyright 2007
 * Stefan Roese, DENX Software Engineering, sr@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/************************************************************************
 * acadia.h - configuration for AMCC Acadia (405EZ)
 ***********************************************************************/

#ifndef __CONFIG_H
#define __CONFIG_H

/*-----------------------------------------------------------------------
 * High Level Configuration Options
 *----------------------------------------------------------------------*/
#define CONFIG_ACADIA			1	/* Board is Acadia	*/
#define CONFIG_4xx			1	/* ... PPC4xx family	*/
#define CONFIG_405EZ			1	/* Specifc 405EZ support*/
#undef	CFG_DRAM_TEST				/* Disable-takes long time */
#define CONFIG_SYS_CLK_FREQ	66666666	/* external freq to pll	*/

#define CONFIG_BOARD_EARLY_INIT_F 1	/* Call board_pre_init		*/
#define	CONFIG_MISC_INIT_F	1	/* Use misc_init_f()		*/

#define CONFIG_NO_SERIAL_EEPROM
/*#undef CONFIG_NO_SERIAL_EEPROM*/

#ifdef CONFIG_NO_SERIAL_EEPROM

/*----------------------------------------------------------------------------
 * PLL settings for 266MHz CPU, 133MHz PLB/SDRAM, 66MHz EBC, 33MHz PCI,
 * assuming a 66MHz input clock to the 405EZ.
 *---------------------------------------------------------------------------*/
/* #define PLLMR0_100_100_12 */
#define PLLMR0_200_133_66
/* #define PLLMR0_266_160_80 */
/* #define PLLMR0_333_166_83 */
#endif

/*-----------------------------------------------------------------------
 * Base addresses -- Note these are effective addresses where the
 * actual resources get mapped (not physical addresses)
 *----------------------------------------------------------------------*/
#define CFG_SDRAM_BASE		0x00000000
#define CFG_FLASH_BASE		0xFE000000
#define CFG_MONITOR_LEN		(256 * 1024)/* Reserve 256 kB for Monitor	*/
#define CFG_MALLOC_LEN		(384 * 1024)/* Reserve 128 kB for malloc()	*/
#define CFG_MONITOR_BASE	TEXT_BASE
#define CFG_USB_HOST		0xef603000	/* USB OHCI 1.1 controller	*/

/*
 * Define here the location of the environment variables (FLASH).
 * Note: DENX encourages to use redundant environment in FLASH. NVRAM is only
 *       supported for backward compatibility.
 */
#if !defined(CONFIG_NAND_U_BOOT) && !defined(CONFIG_NAND_SPL)
   #define CFG_ENV_IS_IN_FLASH	1 		/* use FLASH for environment vars	*/
#else
   #define CFG_ENV_IS_IN_NAND	1		/* use NAND for environment vars	*/
#endif

#define CONFIG_PREBOOT	"echo;"	\
	"echo Type \"run flash_nfs\" to mount root filesystem over NFS;" \
	"echo"

#undef	CONFIG_BOOTARGS

#define	CONFIG_EXTRA_ENV_SETTINGS					\
	"netdev=eth0\0"							\
	"hostname=acadia\0"						\
	"nfsargs=setenv bootargs root=/dev/nfs rw "			\
		"nfsroot=${serverip}:${rootpath}\0"			\
	"ramargs=setenv bootargs root=/dev/ram rw\0"			\
	"addip=setenv bootargs ${bootargs} "				\
		"ip=${ipaddr}:${serverip}:${gatewayip}:${netmask}"	\
		":${hostname}:${netdev}:off panic=1\0"			\
	"addtty=setenv bootargs ${bootargs} console=ttyS0,${baudrate}\0"\
	"flash_nfs=run nfsargs addip addtty;"				\
		"bootm ${kernel_addr}\0"				\
	"flash_self=run ramargs addip addtty;"				\
		"bootm ${kernel_addr} ${ramdisk_addr}\0"		\
	"net_nfs=tftp 200000 ${bootfile};run nfsargs addip addtty;"     \
		"bootm\0"						\
	"rootpath=/opt/eldk/ppc_4xx\0"				\
	"bootfile=acadia/uImage\0"					\
	"kernel_addr=fff10000\0"					\
	"ramdisk_addr=fff20000\0"					\
	"initrd_high=30000000\0"					\
	"load=tftp 200000 acadia/u-boot.bin\0"				\
	"update=protect off fffc0000 ffffffff;era fffc0000 ffffffff;"	\
		"cp.b ${fileaddr} fffc0000 ${filesize};"		\
		"setenv filesize;saveenv\0"				\
	"upd=run load;run update\0"					\
	"kozio=bootm ffc60000\0"					\
	""
#define CONFIG_BOOTCOMMAND	"run flash_self"

#if 0
#define CONFIG_BOOTDELAY	-1	/* autoboot disabled		*/
#else
#define CONFIG_BOOTDELAY	5	/* autoboot after 5 seconds	*/
#endif

#define CONFIG_LOADS_ECHO	1	/* echo on for serial download	*/
#define CFG_LOADS_BAUD_CHANGE	1	/* allow baudrate change	*/

#define CONFIG_MII		1	/* MII PHY management		*/
#define	CONFIG_PHY_ADDR		0	/* PHY address			*/
#define CONFIG_NET_MULTI	1
#define CFG_RX_ETH_BUFFER	16	/* Number of ethernet rx buffers & descriptors */

#define CONFIG_NETCONSOLE		/* include NetConsole support	*/

#define CONFIG_USB_OHCI
#define CONFIG_USB_STORAGE

#if 0 /* test-only */
#define TEST_ONLY_NAND
#endif

#ifdef TEST_ONLY_NAND
#define CMD_NAND		CFG_CMD_NAND
#else
#define CMD_NAND		0
#endif

/* Partitions */
#define CONFIG_MAC_PARTITION
#define CONFIG_DOS_PARTITION
#define CONFIG_ISO_PARTITION

#define CONFIG_SUPPORT_VFAT

#define CONFIG_COMMANDS       (CONFIG_CMD_DFL	|	\
			       CFG_CMD_ASKENV	|	\
			       CFG_CMD_DHCP	|	\
			       CFG_CMD_DTT	|	\
			       CFG_CMD_DIAG	|	\
			       CFG_CMD_EEPROM	|	\
			       CFG_CMD_ELF	|	\
			       CFG_CMD_FAT	|	\
			       CFG_CMD_I2C	|	\
			       CFG_CMD_IRQ	|	\
			       CFG_CMD_MII	|	\
			       CMD_NAND		|	\
			       CFG_CMD_NET	|	\
			       CFG_CMD_NFS	|	\
			       CFG_CMD_PCI	|	\
			       CFG_CMD_PING	|	\
			       CFG_CMD_REGINFO	|	\
			       CFG_CMD_SDRAM	|	\
			       CFG_CMD_USB)

/* this must be included AFTER the definition of CONFIG_COMMANDS (if any) */
#include <cmd_confdefs.h>

#undef CONFIG_WATCHDOG					/* watchdog disabled		*/

/*
 * Miscellaneous configurable options
 */
#define CFG_LONGHELP			/* undef to save memory		*/
#define CFG_PROMPT	"=> "		/* Monitor Command Prompt	*/
#if (CONFIG_COMMANDS & CFG_CMD_KGDB)
#define	CFG_CBSIZE	1024		/* Console I/O Buffer Size	*/
#else
#define	CFG_CBSIZE	256		/* Console I/O Buffer Size	*/
#endif
#define CFG_PBSIZE (CFG_CBSIZE+sizeof(CFG_PROMPT)+16) /* Print Buffer Size */
#define CFG_MAXARGS	16		/* max number of command args	*/
#define CFG_BARGSIZE	CFG_CBSIZE	/* Boot Argument Buffer Size	*/

#define CFG_MEMTEST_START	0x0400000	/* memtest works on	*/
#define CFG_MEMTEST_END		0x0C00000	/* 4 ... 12 MB in DRAM	*/

#define CFG_LOAD_ADDR		0x100000	/* default load address */
#define CFG_EXTBDINFO		1	/* To use extended board_into (bd_t) */

#define	CFG_HZ		1000		/* decrementer freq: 1 ms ticks	*/

#define CONFIG_CMDLINE_EDITING	1	/* add command line history	*/
#define CONFIG_LOOPW            1       /* enable loopw command		*/
#define CONFIG_MX_CYCLIC        1       /* enable mdc/mwc commands	*/
#define CONFIG_ZERO_BOOTDELAY_CHECK	/* check for keypress on bootdelay==0 */
#define CONFIG_VERSION_VARIABLE 1	/* include version env variable */

/*-----------------------------------------------------------------------
 * Serial Port
 *----------------------------------------------------------------------*/
#undef	CFG_EXT_SERIAL_CLOCK			/* external serial clock */
#define CFG_BASE_BAUD		691200
#define CONFIG_BAUDRATE		115200

/* The following table includes the supported baudrates */
#define CFG_BAUDRATE_TABLE	\
	{300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400}

/*-----------------------------------------------------------------------
 * I2C
 *----------------------------------------------------------------------*/
#define CONFIG_HARD_I2C		1		/* I2C with hardware support	*/
#undef	CONFIG_SOFT_I2C				/* I2C bit-banged		*/
#define CFG_I2C_SPEED		400000		/* I2C speed and slave address	*/
#define CFG_I2C_SLAVE		0x7F

#define CFG_I2C_MULTI_EEPROMS
#define CFG_I2C_EEPROM_ADDR	(0xa8>>1)
#define CFG_I2C_EEPROM_ADDR_LEN 1
#define CFG_EEPROM_PAGE_WRITE_ENABLE
#define CFG_EEPROM_PAGE_WRITE_BITS 3
#define CFG_EEPROM_PAGE_WRITE_DELAY_MS 10

/* I2C SYSMON (LM75, AD7414 is almost compatible)			*/
#define CONFIG_DTT_LM75		1		/* ON Semi's LM75	*/
#define CONFIG_DTT_AD7414	1		/* use AD7414		*/
#define CONFIG_DTT_SENSORS	{0}		/* Sensor addresses	*/
#define CFG_DTT_MAX_TEMP	70
#define CFG_DTT_LOW_TEMP	-30
#define CFG_DTT_HYSTERESIS	3

#if 0 /* test-only... */
/*-----------------------------------------------------------------------
 * SPI stuff - Define to include SPI control
 *-----------------------------------------------------------------------
 */
#define CONFIG_SPI
#endif

/*
 * For booting Linux, the board info and command line data
 * have to be in the first 8 MB of memory, since this is
 * the maximum mapped by the Linux kernel during initialization.
 */
#define CFG_BOOTMAPSZ		(8 << 20)	/* Initial Memory map for Linux */

/*-----------------------------------------------------------------------
 * FLASH related
 *----------------------------------------------------------------------*/
#define CFG_FLASH_CFI
#define CFG_FLASH_CFI_DRIVER
#define CFG_FLASH_EMPTY_INFO		/* print 'E' for empty sector on flinfo */
#define CFG_FLASH_USE_BUFFER_WRITE 1	/* use buffered writes (20x faster)	*/

#define CFG_FLASH_BANKS_LIST    {CFG_FLASH_BASE}
#define CFG_MAX_FLASH_BANKS     1		    /* number of banks	    */
#define CFG_MAX_FLASH_SECT	1024		    /* sectors per device   */

#undef	CFG_FLASH_CHECKSUM
#define CFG_FLASH_ERASE_TOUT	120000	/* Timeout for Flash Erase (in ms)	*/
#define CFG_FLASH_WRITE_TOUT	500	/* Timeout for Flash Write (in ms)	*/

#ifdef CFG_ENV_IS_IN_FLASH
#define CFG_ENV_SECT_SIZE	0x40000 /* size of one complete sector	*/
#define CFG_ENV_ADDR		(CFG_MONITOR_BASE-CFG_ENV_SECT_SIZE)
#define	CFG_ENV_SIZE		0x4000	/* Total Size of Environment Sector	*/

/* Address and size of Redundant Environment Sector	*/
#define CFG_ENV_ADDR_REDUND	(CFG_ENV_ADDR-CFG_ENV_SECT_SIZE)
#define CFG_ENV_SIZE_REDUND	(CFG_ENV_SIZE)
#endif

#ifdef TEST_ONLY_NAND
/*-----------------------------------------------------------------------
 * NAND FLASH
 *----------------------------------------------------------------------*/
#define CFG_MAX_NAND_DEVICE	1
#define NAND_MAX_CHIPS		1
#define CFG_NAND_BASE		(CFG_NAND + CFG_NAND_CS)
#define CFG_NAND_SELECT_DEVICE  1	/* nand driver supports mutipl. chips	*/
#endif

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CFG_DCACHE_SIZE		16384		/* For AMCC 405EZ CPU		*/
#define CFG_CACHELINE_SIZE	32		/* ...				*/
#if (CONFIG_COMMANDS & CFG_CMD_KGDB)
#define CFG_CACHELINE_SHIFT	5		/* log base 2 of the above value*/
#endif

/*-----------------------------------------------------------------------
 * Definitions for initial stack pointer and data area (in data cache)
 */
/* use on chip memory ( OCM ) for temperary stack until sdram is tested */
#define CFG_TEMP_STACK_OCM	1

/* On Chip Memory location */
#define CFG_OCM_DATA_ADDR	0xF8000000
#define CFG_OCM_DATA_SIZE	0x4000			/* 16K of onchip SRAM		*/
#define CFG_INIT_RAM_ADDR	CFG_OCM_DATA_ADDR	/* inside of SRAM		*/
#define CFG_INIT_RAM_END	CFG_OCM_DATA_SIZE	/* End of used area in RAM	*/

#define CFG_GBL_DATA_SIZE	128			/* size for initial data	*/
#define CFG_GBL_DATA_OFFSET	(CFG_INIT_RAM_END - CFG_GBL_DATA_SIZE)
#define CFG_INIT_SP_OFFSET	CFG_GBL_DATA_OFFSET

/*-----------------------------------------------------------------------
 * External Bus Controller (EBC) Setup
 */
#define CFG_NAND		0xd0000000
#define CFG_NAND_CS		0		/* NAND chip connected to CSx	*/

/* Memory Bank 0 (Flash) initialization */
#define CFG_EBC_PB0AP		0x03337200
#define CFG_EBC_PB0CR		0xfe0bc000 	/* BAS=0xFE0,BS=32MB,BU=R/W,BW=32bit	*/

/* Memory Bank 1 (CRAM) initialization */
#define CFG_EBC_PB1AP		0x030400c0
#define CFG_EBC_PB1CR		0x000bc000

/* Memory Bank 2 (CRAM) initialization */
#define CFG_EBC_PB2AP		0x030400c0
#define CFG_EBC_PB2CR		0x020bc000

/* Memory Bank 3 (NAND-FLASH) initialization					*/
#define CFG_EBC_PB3AP		0x018003c0
#define CFG_EBC_PB3CR		(CFG_NAND | 0x1c000)

/* Memory Bank 4 (CPLD) initialization */
#define CFG_EBC_PB4AP		0x04006000
#define CFG_EBC_PB4CR		0x80018000 	/* BAS=0x000,BS=16MB,BU=R/W,BW=32bit	*/

#define CFG_EBC_CFG		0xf8400000

/*-----------------------------------------------------------------------
 * Definitions for GPIO_0 setup (PPC405EZ specific)
 *
 * GPIO0[0-3]	- External Bus Controller CS_4 - CS_7 Outputs
 * GPIO0[4]	- External Bus Controller Hold Input
 * GPIO0[5]	- External Bus Controller Priority Input
 * GPIO0[6]	- External Bus Controller HLDA Output
 * GPIO0[7]	- External Bus Controller Bus Request Output
 * GPIO0[8]	- CRAM Clk Output
 * GPIO0[9]	- External Bus Controller Ready Input
 * GPIO0[10]	- CRAM Adv Output
 * GPIO0[11-24]	- NAND Flash Control Data -> Bypasses GPIO when enabled
 * GPIO0[25]	- External DMA Request Input
 * GPIO0[26]	- External DMA EOT I/O
 * GPIO0[25]	- External DMA Ack_n Output
 * GPIO0[17-23]	- External Interrupts IRQ0 - IRQ6 inputs
 * GPIO0[28-30]	- Trace Outputs / PWM Inputs
 * GPIO0[31]	- PWM_8 I/O
 */
#define CFG_GPIO0_TCR		0xC0000000
#define CFG_GPIO0_OSRL		0x50000000
#define CFG_GPIO0_OSRH		0x00000055
#define CFG_GPIO0_ISR1L		0x00000000
#define CFG_GPIO0_ISR1H		0x00000055
#define CFG_GPIO0_TSRL		0x00000000
#define CFG_GPIO0_TSRH		0x00000055

/*-----------------------------------------------------------------------
 * Definitions for GPIO_1 setup (PPC405EZ specific)
 *
 * GPIO1[0-6]	- PWM_9 to PWM_15 I/O
 * GPIO1[7]	- PWM_DIV_CLK (Out) / IRQ4 Input
 * GPIO1[8]	- TS5 Output / DAC_IP_TRIG Input
 * GPIO1[9]	- TS6 Output / ADC_IP_TRIG Input
 * GPIO1[10-12]	- UART0 Control Inputs
 * GPIO1[13]	- UART0_DTR_N Output/IEEE_1588_TS Output/TMRCLK Input
 * GPIO1[14]	- UART0_RTS_N Output/SPI_SS_2_N Output
 * GPIO1[15]	- SPI_SS_3_N Output/UART0_RI_N Input
 * GPIO1[16]	- SPI_SS_1_N Output
 * GPIO1[17-20]	- Trace Output/External Interrupts IRQ0 - IRQ3 inputs
 */
#define CFG_GPIO1_OSRH		0x55455555
#define CFG_GPIO1_OSRL		0x40000110
#define CFG_GPIO1_ISR1H		0x00000000
#define CFG_GPIO1_ISR1L		0x15555445
#define CFG_GPIO1_TSRH		0x00000000
#define CFG_GPIO1_TSRL		0x00000000
#define CFG_GPIO1_TCR		0xFFFF8014

/*-----------------------------------------------------------------------
 * EPLD Regs.
 */
#define	EPLD_BASE	0x80000000
#define	EPLD_ETHRSTBOOT	0x10
#define	EPLD_CTRL	0x14
#define	EPLD_MUXOE	0x16

/*
 * State definations
 */
#define	LOAK_INIT	0x494e4954	/* ASCII "INIT" */
#define	LOAK_NONE	0x4e4f4e45	/* ASCII "NONE" */
#define	LOAK_CRAM	0x4352414d	/* ASCII "CRAM" */
#define	LOAK_PSRAM	0x50535241	/* ASCII "PSRA" - PSRAM */
#define	LOAK_OCM	0x4f434d20	/* ASCII "OCM " */
#define	LOAK_ZERO	0x5a45524f	/* ASCII "ZERO" */
#define	LOAK_SPL	0x53504c20	/* ASCII "SPL" */

/*
 * Internal Definitions
 *
 * Boot Flags
 */
#define BOOTFLAG_COLD	0x01		/* Normal Power-On: Boot from FLASH	*/
#define BOOTFLAG_WARM	0x02		/* Software reboot			*/

#if (CONFIG_COMMANDS & CFG_CMD_KGDB)
  #define CONFIG_KGDB_BAUDRATE	230400	/* speed to run kgdb serial port */
  #define CONFIG_KGDB_SER_INDEX	2	/* which serial port to use */
#endif

#endif	/* __CONFIG_H */
