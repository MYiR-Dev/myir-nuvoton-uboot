/*
 * iPAQ h2200 board configuration
 *
 * Copyright (C) 2012 Lukasz Dalek <luk0104@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define MACH_TYPE_H2200			341
#define CONFIG_MACH_TYPE		MACH_TYPE_H2200

#define CONFIG_CPU_PXA25X		1
#define CONFIG_BOARD_H2200

#define CONFIG_SYS_NO_FLASH

#define CONFIG_SYS_HZ			1000

#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM_1			0xa0000000 /* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE		0x04000000 /* 64 MB */

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM_1
#define CONFIG_SYS_SDRAM_SIZE		PHYS_SDRAM_1_SIZE

#define CONFIG_SYS_INIT_SP_ADDR		0xfffff800

#define CONFIG_ENV_SIZE			0x00040000
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 128*1024)

#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_SYS_MAXARGS		16
#define CONFIG_SYS_LOAD_ADDR		0xa3000000 /* default load address */

/*
 * iPAQ 1st stage bootloader loads 2nd stage bootloader
 * at address 0xa0040000 but bootloader requires header
 * which is 0x1000 long.
 *
 * --- Header begin ---
 *	.word 0xea0003fe ; b 0x1000
 *
 *	.org 0x40
 *	.ascii "ECEC"
 *
 *	.org 0x1000
 * --- Header end ---
 */

#define CONFIG_SYS_TEXT_BASE		0xa0041000

/*
 * Static chips
 */

#define CONFIG_SYS_MSC0_VAL		0x246c7ffc
#define CONFIG_SYS_MSC1_VAL		0x7ff07ff0
#define CONFIG_SYS_MSC2_VAL		0x7ff07ff0

/*
 * PCMCIA and CF Interfaces
 */

#define CONFIG_SYS_MECR_VAL		0x00000000
#define CONFIG_SYS_MCMEM0_VAL		0x00000000
#define CONFIG_SYS_MCMEM1_VAL		0x00000000
#define CONFIG_SYS_MCATT0_VAL		0x00000000
#define CONFIG_SYS_MCATT1_VAL		0x00000000
#define CONFIG_SYS_MCIO0_VAL		0x00000000
#define CONFIG_SYS_MCIO1_VAL		0x00000000

#define CONFIG_SYS_FLYCNFG_VAL		0x00000000
#define CONFIG_SYS_SXCNFG_VAL		0x00040004

#define CONFIG_SYS_MDREFR_VAL		0x0099E018
#define CONFIG_SYS_MDCNFG_VAL		0x01C801CB
#define CONFIG_SYS_MDMRS_VAL		0x00220022

#define CONFIG_SYS_PSSR_VAL		0x00000000
#define CONFIG_SYS_CKEN			0x00004840
#define CONFIG_SYS_CCCR			0x00000161

/*
 * GPIOs
 */

#define CONFIG_SYS_GPSR0_VAL		0x01000000
#define CONFIG_SYS_GPSR1_VAL		0x00000000
#define CONFIG_SYS_GPSR2_VAL		0x00010000

#define CONFIG_SYS_GPCR0_VAL		0x00000000
#define CONFIG_SYS_GPCR1_VAL		0x00000000
#define CONFIG_SYS_GPCR2_VAL		0x00000000

#define CONFIG_SYS_GPDR0_VAL		0xF7E38C00
#define CONFIG_SYS_GPDR1_VAL		0xBCFFBF83
#define CONFIG_SYS_GPDR2_VAL		0x000157FF

#define CONFIG_SYS_GAFR0_L_VAL		0x80401000
#define CONFIG_SYS_GAFR0_U_VAL		0x00000112
#define CONFIG_SYS_GAFR1_L_VAL		0x600A9550
#define CONFIG_SYS_GAFR1_U_VAL		0x0005AAAA
#define CONFIG_SYS_GAFR2_L_VAL		0x20000000
#define CONFIG_SYS_GAFR2_U_VAL		0x00000000

/*
 * Serial port
 */

#define CONFIG_PXA_SERIAL
#define CONFIG_FFUART
#define CONFIG_CONS_INDEX		3

#define CONFIG_BAUDRATE			115200
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 38400, 115200 }

#define CONFIG_CMD_IMPORTENV		1
#define CONFIG_CMD_LOADB
#define CONFIG_CMD_SOURCE
#define CONFIG_CMD_RUN
#define CONFIG_CMD_IMI

#define CONFIG_FIT
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_CMDLINE_TAG
#define CONFIG_INITRD_TAG

/* Monitor Command Prompt */
#define CONFIG_SYS_PROMPT		"> "
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2	"$ "

/* Console I/O Buffer Size */
#define CONFIG_SYS_CBSIZE		256

/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)

#define CONFIG_BOOTARGS "root=/dev/ram0 ro console=ttyS0,115200n8"

#define CONFIG_SYS_CONSOLE_IS_IN_ENV
#define CONFIG_USB_DEV_PULLUP_GPIO	33
/* USB VBUS GPIO 3 */

#define CONFIG_CMD_NET
#define CONFIG_CMD_PING

#define CONFIG_BOOTDELAY		2
#define CONFIG_BOOTCOMMAND		\
	"setenv downloaded 0 ; while test $downloaded -eq 0 ; do " \
	"if bootp ; then setenv downloaded 1 ; fi ; done ; " \
	"source :script ; " \
	"bootm ; "

#define CONFIG_USB_GADGET_PXA2XX
#define CONFIG_USB_ETHER
#define CONFIG_USB_ETH_SUBSET

#define CONFIG_USBNET_DEV_ADDR		"de:ad:be:ef:00:01"
#define CONFIG_USBNET_HOST_ADDR	"de:ad:be:ef:00:02"
#define CONFIG_EXTRA_ENV_SETTINGS \
	"stdin=serial\0" \
	"stdout=serial\0" \
	"stderr=serial\0"

#endif /* __CONFIG_H */
