/*
 * (C) Copyright 2007
 * Michael Schwingen, michael@schwingen.org
 *
 * Configuration settings for the AcTux-4 board.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_IXP425			1
#define CONFIG_ACTUX4			1

#define CONFIG_DISPLAY_CPUINFO		1
#define CONFIG_DISPLAY_BOARDINFO	1

#define CFG_IXP425_CONSOLE		IXP425_UART1
#define CONFIG_BAUDRATE			115200
#define CONFIG_BOOTDELAY		3
#define CONFIG_ZERO_BOOTDELAY_CHECK	/* check for keypress on bootdelay==0 */

/***************************************************************
 * U-boot generic defines start here.
 ***************************************************************/
#undef CONFIG_USE_IRQ

/* Size of malloc() pool */
#define CFG_MALLOC_LEN			(CFG_ENV_SIZE + 128*1024)
/* size in bytes reserved for initial data */
#define CFG_GBL_DATA_SIZE		128

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

/* Command line configuration */
#include <config_cmd_default.h>

#define CONFIG_CMD_ELF

#define CONFIG_BOOTCOMMAND		"run boot_flash"
/* enable passing of ATAGs */
#define CONFIG_CMDLINE_TAG		1
#define CONFIG_SETUP_MEMORY_TAGS	1
#define CONFIG_INITRD_TAG		1

#if defined(CONFIG_CMD_KGDB)
# define CONFIG_KGDB_BAUDRATE		230400
/* which serial port to use */
# define CONFIG_KGDB_SER_INDEX		1
#endif

/* Miscellaneous configurable options */
#define CFG_LONGHELP
#define CFG_PROMPT			"=> "
/* Console I/O Buffer Size */
#define CFG_CBSIZE			256
/* Print Buffer Size */
#define CFG_PBSIZE			(CFG_CBSIZE+sizeof(CFG_PROMPT)+16)
/* max number of command args */
#define CFG_MAXARGS			16
/* Boot Argument Buffer Size */
#define CFG_BARGSIZE			CFG_CBSIZE

#define CFG_MEMTEST_START		0x00400000
#define CFG_MEMTEST_END			0x00800000

/* everything, incl board info, in Hz */
#undef  CFG_CLKS_IN_HZ
/* spec says 66.666 MHz, but it appears to be 33 */
#define CFG_HZ				3333333

/* default load address */
#define CFG_LOAD_ADDR			0x00010000

/* valid baudrates */
#define CFG_BAUDRATE_TABLE		{ 9600, 19200, 38400, 57600, \
					  115200, 230400 }
#define CONFIG_SERIAL_RTS_ACTIVE	1

/*
 * Stack sizes
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE		(128*1024)	/* regular stack */
#ifdef CONFIG_USE_IRQ
# define CONFIG_STACKSIZE_IRQ		(4*1024)	/* IRQ stack */
# define CONFIG_STACKSIZE_FIQ		(4*1024)	/* FIQ stack */
#endif

/* Expansion bus settings */
#define CFG_EXP_CS0			0xbd113003

/* SDRAM settings */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM_1			0x00000000
#define CFG_DRAM_BASE			0x00000000

/* 32MB SDRAM */
#define CFG_SDR_CONFIG			0x18
#define PHYS_SDRAM_1_SIZE		0x02000000
#define CFG_SDRAM_REFRESH_CNT		0x81a
#define CFG_SDR_MODE_CONFIG		0x1
#define CFG_DRAM_SIZE			0x02000000

/* FLASH organization */
#define CFG_MAX_FLASH_BANKS		2
/* max # of sectors per chip */
#define CFG_MAX_FLASH_SECT		70
#define PHYS_FLASH_1			0x50000000
#define PHYS_FLASH_2			0x51000000
#define CFG_FLASH_BANKS_LIST		{ PHYS_FLASH_1, PHYS_FLASH_2 }

#define CFG_FLASH_BASE			PHYS_FLASH_1
#define CFG_MONITOR_BASE		PHYS_FLASH_1
#define CFG_MONITOR_LEN			(252 << 10)

/* Use common CFI driver */
#define CFG_FLASH_CFI
#define CFG_FLASH_CFI_DRIVER
/* board provides its own flash_init code */
#define CONFIG_FLASH_CFI_LEGACY		1
/* no byte writes on IXP4xx */
#define CFG_FLASH_CFI_WIDTH		FLASH_CFI_16BIT
/* SST 39VF020 etc. support */
#define CFG_FLASH_LEGACY_256Kx8	1

/* print 'E' for empty sector on flinfo */
#define CFG_FLASH_EMPTY_INFO

/* Ethernet */

/* include IXP4xx NPE support */
#define CONFIG_IXP4XX_NPE		1
/* use separate flash sector with ucode images */
#define CONFIG_IXP4XX_NPE_EXT_UCODE_BASE	0x51000000

#define CONFIG_NET_MULTI		1
/* NPE0 PHY address */
#define	CONFIG_PHY_ADDR			0x1C
/* MII PHY management */
#define CONFIG_MII			1
/* Number of ethernet rx buffers & descriptors */
#define CFG_RX_ETH_BUFFER		16

#define CONFIG_CMD_DHCP
#define CONFIG_CMD_NET
#define CONFIG_CMD_MII
#define CONFIG_CMD_PING
#undef  CONFIG_CMD_NFS

/* BOOTP options */
#define CONFIG_BOOTP_BOOTFILESIZE
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME

/* Cache Configuration */
#define CFG_CACHELINE_SIZE		32

/* environment organization: one complete 4k flash sector */
#define	CFG_ENV_IS_IN_FLASH		1
#define CFG_ENV_SIZE			0x1000
#define CFG_ENV_ADDR			(PHYS_FLASH_1 + 0x3f000)

#define CONFIG_EXTRA_ENV_SETTINGS					\
	"mtd=IXP4XX-Flash.0:252k(uboot),4k(uboot_env);"			\
	"IXP4XX-Flash.1:128k(ucode),1280k(linux),-(root)\0"		\
	"kerneladdr=51020000\0"						\
	"rootaddr=51160000\0"						\
	"loadaddr=10000\0"						\
	"updateboot_ser=mw.b 10000 ff 40000;"				\
	" loady ${loadaddr};"						\
	" run eraseboot writeboot\0"					\
	"updateboot_net=mw.b 10000 ff 40000;"				\
	" tftp ${loadaddr} u-boot.bin;"					\
	" run eraseboot writeboot\0"					\
	"eraseboot=protect off 50000000 5003efff;"			\
	" erase 50000000 +${filesize}\0"				\
	"writeboot=cp.b 10000 50000000 ${filesize}\0"			\
	"eraseenv=protect off 5003f000 5003ffff;"			\
	" erase 5003f000 5003ffff\0"					\
	"updateroot=tftp ${loadaddr} ${rootfile};"			\
	" era ${rootaddr} +${filesize};"				\
	" cp.b ${loadaddr} ${rootaddr} ${filesize}\0"			\
	"updatekern=tftp ${loadaddr} ${kernelfile};"			\
	" era ${kerneladdr} +${filesize};"				\
	" cp.b ${loadaddr} ${kerneladdr} ${filesize}\0"			\
	"flashargs=setenv bootargs mtdparts=${mtd} root=/dev/mtdblock4"	\
	" rootfstype=squashfs,jffs2 init=/etc/preinit\0"		\
	"netargs=setenv bootargs mtdparts=${mtd} root=/dev/mtdblock4"	\
	" rootfstype=squashfs,jffs2 init=/etc/preinit\0"		\
	"addtty=setenv bootargs ${bootargs} console=ttyS0,${baudrate}\0" \
	"addeth=setenv bootargs ${bootargs} ethaddr=${ethaddr}\0"	\
	"boot_flash=run flashargs addtty addeth;"			\
	" bootm ${kerneladdr}\0"					\
	"boot_net=run netargs addtty addeth;"				\
	" tftpboot ${loadaddr} ${kernelfile};"				\
	" bootm\0"

#endif /* __CONFIG_H */
