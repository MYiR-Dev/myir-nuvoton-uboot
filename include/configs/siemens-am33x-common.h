/*
 * siemens am33x common board options
 * (C) Copyright 2013 Siemens Schweiz AG
 * (C) Heiko Schocher, DENX Software Engineering, hs@denx.de.
 *
 * Based on:
 * U-Boot file:/include/configs/am335x_evm.h
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_SIEMENS_AM33X_COMMON_H
#define __CONFIG_SIEMENS_AM33X_COMMON_H

#define CONFIG_AM33XX
#define CONFIG_OMAP
#define CONFIG_OMAP_COMMON

#include <asm/arch/omap.h>

#define CONFIG_DMA_COHERENT
#define CONFIG_DMA_COHERENT_SIZE	(1 << 20)

#define CONFIG_ENV_SIZE			(0x2000)
#define CONFIG_SYS_MALLOC_LEN		(16 * 1024 * 1024)
#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_SYS_HUSH_PARSER		/* use "hush" command parser */
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#define CONFIG_BOARD_LATE_INIT
#define CONFIG_SYS_NO_FLASH
#ifdef CONFIG_SIEMENS_MACH_TYPE
#define CONFIG_MACH_TYPE		CONFIG_SIEMENS_MACH_TYPE
#endif

#define CONFIG_CMDLINE_TAG		/* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG

#define CONFIG_SYS_CACHELINE_SIZE       64

/* commands to include */
#define CONFIG_CMD_ASKENV
#define CONFIG_CMD_CACHE
#define CONFIG_CMD_TIME


#define CONFIG_ENV_VARS_UBOOT_CONFIG
#ifndef CONFIG_SPL_BUILD
#define CONFIG_ROOTPATH		"/opt/eldk"
#endif

#define CONFIG_ENV_OVERWRITE		1
#define CONFIG_ENV_IS_NOWHERE

#define CONFIG_SYS_LONGHELP
#define CONFIG_CMDLINE_EDITING
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_AUTOLOAD	"yes"

/* Clock Defines */
#define V_OSCK				24000000  /* Clock output from T2 */
#define V_SCLK				(V_OSCK)

/* We set the max number of command args high to avoid HUSH bugs. */
#define CONFIG_SYS_MAXARGS		32

/* Console I/O Buffer Size */
#define CONFIG_SYS_CBSIZE		1024

/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE \
					+ sizeof(CONFIG_SYS_PROMPT) + 16)

/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE

/*
 * memtest works on 8 MB in DRAM after skipping 32MB from
 * start addr of ram disk
 */
#define CONFIG_SYS_MEMTEST_START	(PHYS_DRAM_1 + (64 * 1024 * 1024))
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START \
					+ (8 * 1024 * 1024))

#define CONFIG_SYS_LOAD_ADDR		0x81000000 /* Default load address */

#define CONFIG_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_OMAP_HSMMC
#define CONFIG_CMD_MMC
#define CONFIG_DOS_PARTITION
#define CONFIG_CMD_FAT
#define CONFIG_CMD_EXT2

#define CONFIG_SPI
#define CONFIG_OMAP3_SPI
#define CONFIG_MTD_DEVICE
#define CONFIG_CMD_SF
#define CONFIG_SF_DEFAULT_SPEED		(75000000)

 /* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS		1		/*  1 bank of DRAM */
#define PHYS_DRAM_1			0x80000000	/* DRAM Bank #1 */

#define CONFIG_SYS_SDRAM_BASE		PHYS_DRAM_1
#define CONFIG_SYS_INIT_SP_ADDR         (NON_SECURE_SRAM_END - \
						GENERATED_GBL_DATA_SIZE)
 /* Platform/Board specific defs */
#define CONFIG_SYS_TIMERBASE		0x48040000	/* Use Timer2 */
#define CONFIG_SYS_PTV			2	/* Divisor: 2^(PTV+1) => 8 */

/* NS16550 Configuration */
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE	(-4)
#define CONFIG_SYS_NS16550_CLK		(48000000)
#define CONFIG_SYS_NS16550_COM1		0x44e09000
#define CONFIG_SYS_NS16550_COM4		0x481a6000

#define CONFIG_BAUDRATE		115200

#define CONFIG_SYS_CONSOLE_INFO_QUIET
#define CONFIG_SERIAL1                  1
#define CONFIG_CONS_INDEX               1

/* I2C Configuration */
#define CONFIG_I2C
#define CONFIG_CMD_I2C
#define CONFIG_SYS_I2C
#define CONFIG_SYS_OMAP24_I2C_SPEED	100000
#define CONFIG_SYS_OMAP24_I2C_SLAVE	1
#define CONFIG_SYS_I2C_OMAP24XX

/* Defines for SPL */
#define CONFIG_SPL_FRAMEWORK
#define CONFIG_SPL_TEXT_BASE		0x402F0400
#define CONFIG_SPL_MAX_SIZE		(101 * 1024)

#define CONFIG_SPL_BSS_START_ADDR	0x80000000
#define CONFIG_SPL_BSS_MAX_SIZE		0x80000		/* 512 KB */

#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR	0x300 /* address 0x60000 */
#define CONFIG_SYS_MMCSD_FS_BOOT_PARTITION	1
#define CONFIG_SPL_FS_LOAD_PAYLOAD_NAME	"u-boot.img"
#define CONFIG_SPL_MMC_SUPPORT
#define CONFIG_SPL_FAT_SUPPORT
#define CONFIG_FS_FAT
#define CONFIG_SPL_I2C_SUPPORT

#define CONFIG_SPL_LIBCOMMON_SUPPORT
#define CONFIG_SPL_LIBDISK_SUPPORT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPL_YMODEM_SUPPORT

#define CONFIG_SPL_GPIO_SUPPORT
#define CONFIG_SPL_WATCHDOG_SUPPORT

#define CONFIG_SPL_SPI_SUPPORT
#define CONFIG_SPL_SPI_FLASH_SUPPORT
#define CONFIG_SPL_SPI_LOAD
#define CONFIG_SYS_SPI_U_BOOT_OFFS	0x20000

#define CONFIG_SPL_LDSCRIPT		"$(CPUDIR)/am33xx/u-boot-spl.lds"

#define CONFIG_SPL_BOARD_INIT
#define CONFIG_SPL_NAND_AM33XX_BCH
#define CONFIG_SPL_NAND_SUPPORT
#define CONFIG_SPL_NAND_BASE
#define CONFIG_SPL_NAND_DRIVERS
#define CONFIG_SPL_NAND_ECC
#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_NAND_PAGE_COUNT	(CONFIG_SYS_NAND_BLOCK_SIZE / \
					 CONFIG_SYS_NAND_PAGE_SIZE)
#define CONFIG_SYS_NAND_PAGE_SIZE	2048
#define CONFIG_SYS_NAND_OOBSIZE		64
#define CONFIG_SYS_NAND_BLOCK_SIZE	(128*1024)
#define CONFIG_SYS_NAND_BAD_BLOCK_POS	NAND_LARGE_BADBLOCK_POS
#define CONFIG_SYS_NAND_ECCPOS		{ 2, 3, 4, 5, 6, 7, 8, 9, \
					 10, 11, 12, 13, 14, 15, 16, 17, \
					 18, 19, 20, 21, 22, 23, 24, 25, \
					 26, 27, 28, 29, 30, 31, 32, 33, \
					 34, 35, 36, 37, 38, 39, 40, 41, \
					 42, 43, 44, 45, 46, 47, 48, 49, \
					 50, 51, 52, 53, 54, 55, 56, 57, }

#define CONFIG_SYS_NAND_ECCSIZE		512
#define CONFIG_SYS_NAND_ECCBYTES	14
#define CONFIG_NAND_OMAP_ECCSCHEME	OMAP_ECC_BCH8_CODE_HW

#define CONFIG_SYS_NAND_ECCSTEPS	4
#define	CONFIG_SYS_NAND_ECCTOTAL	(CONFIG_SYS_NAND_ECCBYTES * \
						CONFIG_SYS_NAND_ECCSTEPS)

#define	CONFIG_SYS_NAND_U_BOOT_START	CONFIG_SYS_TEXT_BASE

#define CONFIG_SYS_NAND_U_BOOT_OFFS	0x80000

/*
 * 1MB into the SDRAM to allow for SPL's bss at the beginning of SDRAM
 * 64 bytes before this address should be set aside for u-boot.img's
 * header. That is 0x800FFFC0--0x80100000 should not be used for any
 * other needs.
 */
#define CONFIG_SYS_TEXT_BASE		0x80100000
#define CONFIG_SYS_SPL_MALLOC_START	0x80208000
#define CONFIG_SYS_SPL_MALLOC_SIZE	0x100000

/*
 * Since SPL did pll and ddr initialization for us,
 * we don't need to do it twice.
 */
#ifndef CONFIG_SPL_BUILD
#define CONFIG_SKIP_LOWLEVEL_INIT
#endif

#ifndef CONFIG_SPL_BUILD
/*
 * USB configuration
 */
#define CONFIG_USB_MUSB_DSPS
#define CONFIG_ARCH_MISC_INIT
#define CONFIG_USB_MUSB_PIO_ONLY
#define CONFIG_USB_MUSB_DISABLE_BULK_COMBINE_SPLIT
#undef CONFIG_USB_GADGET_DUALSPEED
#define CONFIG_USB_MUSB_HOST

#define CONFIG_AM335X_USB0
#define CONFIG_AM335X_USB0_MODE	MUSB_PERIPHERAL
#define CONFIG_AM335X_USB1
#define CONFIG_AM335X_USB1_MODE MUSB_HOST
#ifdef CONFIG_USB_MUSB_HOST
#define CONFIG_CMD_USB
#define CONFIG_USB_STORAGE
#endif

#ifdef CONFIG_USB_MUSB_GADGET
#define CONFIG_USB_ETHER
#define CONFIG_USB_ETH_RNDIS
#define CONFIG_USBNET_HOST_ADDR	"de:ad:be:af:00:00"
#endif /* CONFIG_USB_MUSB_GADGET */

/* USB DRACO ID as default */
#define CONFIG_USBD_HS
#define CONFIG_G_DNL_VENDOR_NUM 0x0908
#define CONFIG_G_DNL_PRODUCT_NUM 0x02d2
#define CONFIG_G_DNL_MANUFACTURER "Siemens AG"

/* USB Device Firmware Update support */
#define CONFIG_USB_FUNCTION_DFU
#define CONFIG_DFU_NAND
#define CONFIG_CMD_DFU
#define CONFIG_SYS_DFU_DATA_BUF_SIZE	(1 << 20)
#define DFU_MANIFEST_POLL_TIMEOUT	25000

#endif /* CONFIG_SPL_BUILD */

/*
 * Default to using SPI for environment, etc.  We have multiple copies
 * of SPL as the ROM will check these locations.
 * 0x0 - 0x20000 : First copy of SPL
 * 0x20000 - 0x40000 : Second copy of SPL
 * 0x40000 - 0x60000 : Third copy of SPL
 * 0x60000 - 0x80000 : Fourth copy of SPL
 * 0x80000 - 0xDF000 : U-Boot
 * 0xDF000 - 0xE0000 : U-Boot Environment
 * 0xE0000 - 0x442000 : Linux Kernel
 * 0x442000 - 0x800000 : Userland
 */
#if defined(CONFIG_SPI_BOOT)
# undef CONFIG_ENV_IS_NOWHERE
# define CONFIG_ENV_IS_IN_SPI_FLASH
# define CONFIG_ENV_SPI_MAX_HZ		CONFIG_SF_DEFAULT_SPEED
# define CONFIG_ENV_OFFSET		(892 << 10) /* 892 KiB in */
# define CONFIG_ENV_SECT_SIZE		(4 << 10) /* 4 KB sectors */
#endif /* SPI support */

/* Unsupported features */
#undef CONFIG_USE_IRQ

#define CONFIG_CMD_DHCP
#define CONFIG_CMD_PING
#define CONFIG_DRIVER_TI_CPSW
#define CONFIG_MII
#define CONFIG_PHY_GIGE
#define CONFIG_PHYLIB
#define CONFIG_CMD_MII
#define CONFIG_BOOTP_DEFAULT
#define CONFIG_BOOTP_DNS
#define CONFIG_BOOTP_DNS2
#define CONFIG_BOOTP_SEND_HOSTNAME
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_SUBNETMASK
#define CONFIG_NET_RETRY_COUNT         10

#define CONFIG_NAND
/* NAND support */
#ifdef CONFIG_NAND
#define CONFIG_CMD_NAND

/* UBI Support */
#ifndef CONFIG_SPL_BUILD
#define CONFIG_CMD_MTDPARTS
#define CONFIG_MTD_PARTITIONS
#define CONFIG_MTD_DEVICE
#define CONFIG_RBTREE
#define CONFIG_LZO
#define CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS
#endif

/* Commen environment */
#define CONFIG_PREBOOT
#define COMMON_ENV_DFU_ARGS	"dfu_args=run bootargs_defaults;" \
				"setenv bootargs ${bootargs};" \
				"mtdparts default;" \
				"draco_led 1;" \
				"dfu 0 nand 0;" \
				"draco_led 0;\0" \

#define COMMON_ENV_NAND_BOOT \
		"nand_boot=echo Booting from nand; " \
		"if test ${upgrade_available} -eq 1; then " \
			"if test ${bootcount} -gt ${bootlimit}; " \
				"then " \
				"setenv upgrade_available 0;" \
				"setenv ${partitionset_active} true;" \
				"if test -n ${A}; then " \
					"setenv partitionset_active B; " \
					"env delete A; " \
				"fi;" \
				"if test -n ${B}; then " \
					"setenv partitionset_active A; " \
					"env delete B; " \
				"fi;" \
				"saveenv; " \
			"fi;" \
		"fi;" \
		"echo set ${partitionset_active}...;" \
		"run nand_args; "

#define COMMON_ENV_NAND_CMDS	"flash_self=run nand_boot\0" \
				"flash_self_test=setenv testargs test; " \
					"run nand_boot\0" \
				"dfu_start=echo Preparing for dfu mode ...; " \
				"run dfu_args; \0"

#define COMMON_ENV_SETTINGS \
	"verify=no \0" \
	"project_dir=targetdir\0" \
	"upgrade_available=0\0" \
	"altbootcmd=run bootcmd\0" \
	"bootlimit=3\0" \
	"partitionset_active=A\0" \
	"loadaddr=0x82000000\0" \
	"kloadaddr=0x81000000\0" \
	"script_addr=0x81900000\0" \
	"console=console=ttyMTD,mtdoops console=ttyO0,115200n8 panic=5\0" \
	"nfsopts=nolock rw\0" \
	"ip_method=none\0" \
	"bootenv=uEnv.txt\0" \
	"bootargs_defaults=setenv bootargs " \
		"console=${console} " \
		"${testargs} " \
		"${optargs}\0" \
	"siemens_help=echo; "\
		"echo Type 'run flash_self' to use kernel and root " \
		"filesystem on memory; echo Type 'run flash_self_test' to " \
		"use kernel and root filesystem on memory, boot in test " \
		"mode; echo Not ready yet: 'run flash_nfs' to use kernel " \
		"from memory and root filesystem over NFS; echo Type " \
		"'run net_nfs' to get Kernel over TFTP and mount root " \
		"filesystem over NFS; " \
		"echo Set partitionset_active variable to 'A' " \
		"or 'B' to select kernel and rootfs partition; " \
		"echo" \
		"\0"

/*
 * Variant 1 partition layout
 * chip-size = 256MiB
 *|         name |        size |           address area |
 *-------------------------------------------------------
 *|          spl | 128.000 KiB | 0x       0..0x   1ffff |
 *|  spl.backup1 | 128.000 KiB | 0x   20000..0x   3ffff |
 *|  spl.backup2 | 128.000 KiB | 0x   40000..0x   5ffff |
 *|  spl.backup3 | 128.000 KiB | 0x   60000..0x   7ffff |
 *|       u-boot |   1.875 MiB | 0x   80000..0x  25ffff |
 *|    uboot.env | 128.000 KiB | 0x  260000..0x  27ffff |
 *|     kernel_a |   5.000 MiB | 0x  280000..0x  77ffff |
 *|     kernel_b |   5.000 MiB | 0x  780000..0x  c7ffff |
 *|      mtdoops |   8.000 MiB | 0x  c80000..0x 147ffff |
 *|       rootfs | 235.500 MiB | 0x 1480000..0x fffffff |
 *-------------------------------------------------------
 */
#define MTDIDS_NAME_STR		"omap2-nand.0"
#define MTDIDS_DEFAULT		"nand0=" MTDIDS_NAME_STR
#define MTDPARTS_DEFAULT_V1	"mtdparts=" MTDIDS_NAME_STR ":" \
					"128k(spl),"		\
					"128k(spl.backup1),"	\
					"128k(spl.backup2),"	\
					"128k(spl.backup3),"	\
					"1920k(u-boot),"	\
					"128k(uboot.env),"	\
					"5120k(kernel_a),"	\
					"5120k(kernel_b),"	\
					"8192k(mtdoops),"	\
					"-(rootfs)"

#define DFU_ALT_INFO_NAND_V1 \
	"spl part 0 1;" \
	"spl.backup1 part 0 2;" \
	"spl.backup2 part 0 3;" \
	"spl.backup3 part 0 4;" \
	"u-boot part 0 5;" \
	"u-boot.env part 0 6;" \
	"kernel_a part 0 7;" \
	"kernel_b part 0 8;" \
	"rootfs partubi 0 10"

#define CONFIG_ENV_SETTINGS_NAND_V1 \
	"nand_active_ubi_vol=rootfs_a\0" \
	"nand_active_ubi_vol_A=rootfs_a\0" \
	"nand_active_ubi_vol_B=rootfs_b\0" \
	"nand_root_fs_type=ubifs rootwait=1\0" \
	"nand_src_addr=0x280000\0" \
	"nand_src_addr_A=0x280000\0" \
	"nand_src_addr_B=0x780000\0" \
	"nand_args=run bootargs_defaults;" \
		"mtdparts default;" \
		"setenv ${partitionset_active} true;" \
		"if test -n ${A}; then " \
			"setenv nand_active_ubi_vol ${nand_active_ubi_vol_A};" \
			"setenv nand_src_addr ${nand_src_addr_A};" \
		"fi;" \
		"if test -n ${B}; then " \
			"setenv nand_active_ubi_vol ${nand_active_ubi_vol_B};" \
			"setenv nand_src_addr ${nand_src_addr_B};" \
		"fi;" \
		"setenv nand_root ubi0:${nand_active_ubi_vol} rw " \
		"ubi.mtd=9,2048;" \
		"setenv bootargs ${bootargs} " \
		"root=${nand_root} noinitrd ${mtdparts} " \
		"rootfstype=${nand_root_fs_type} ip=${ip_method} " \
		"console=ttyMTD,mtdoops console=ttyO0,115200n8 mtdoops.mtddev" \
		"=mtdoops\0" \
	COMMON_ENV_DFU_ARGS \
		"dfu_alt_info=" DFU_ALT_INFO_NAND_V1 "\0" \
	COMMON_ENV_NAND_BOOT \
		"nand read.i ${kloadaddr} ${nand_src_addr} " \
		"${nand_img_size}; bootm ${kloadaddr}\0" \
	COMMON_ENV_NAND_CMDS

#define CONFIG_ENV_SETTINGS_V1 \
		COMMON_ENV_SETTINGS \
	"net_args=run bootargs_defaults;" \
		"mtdparts default;" \
		"setenv bootfile ${project_dir}/kernel/uImage;" \
		"setenv rootpath /home/projects/${project_dir}/rootfs;" \
		"setenv bootargs ${bootargs} " \
		"root=/dev/nfs ${mtdparts} " \
		"nfsroot=${serverip}:${rootpath},${nfsopts} " \
		"ip=${ipaddr}:${serverip}:" \
		"${gatewayip}:${netmask}:${hostname}:eth0:off\0" \
	"net_nfs=echo Booting from network ...; " \
		"run net_args; " \
		"tftpboot ${kloadaddr} ${serverip}:${bootfile}; " \
		"bootm ${kloadaddr}\0"

/*
 * Variant 2 partition layout
 * chip-size = 256MiB or 512 MiB
 *|         name |        size |           address area |
 *-------------------------------------------------------
 *|          spl | 128.000 KiB | 0x       0..0x   1ffff |
 *|  spl.backup1 | 128.000 KiB | 0x   20000..0x   3ffff |
 *|  spl.backup2 | 128.000 KiB | 0x   40000..0x   5ffff |
 *|  spl.backup3 | 128.000 KiB | 0x   60000..0x   7ffff |
 *|       u-boot |   1.875 MiB | 0x   80000..0x  25ffff |
 *|   uboot.env0 | 512.000 KiB | 0x  260000..0x  2Dffff |
 *|   uboot.env1 | 512.000 KiB | 0x  2E0000..0x  35ffff |
 *|      mtdoops | 512.000 KiB | 0x  360000..0x  3dffff |
 *| (256) rootfs | 252.125 MiB | 0x  3E0000..0x fffffff |
 *| (512) rootfs | 508.125 MiB | 0x  3E0000..0x1fffffff |
 *-------------------------------------------------------
 */

#define MTDPARTS_DEFAULT_V2	"mtdparts=" MTDIDS_NAME_STR ":" \
					"128k(spl)," \
					"128k(spl.backup1)," \
					"128k(spl.backup2)," \
					"128k(spl.backup3)," \
					"1920k(u-boot)," \
					"512k(u-boot.env0)," \
					"512k(u-boot.env1)," \
					"512k(mtdoops)," \
					"-(rootfs)"


#define DFU_ALT_INFO_NAND_V2 \
	"spl part 0 1;" \
	"spl.backup1 part 0 2;" \
	"spl.backup2 part 0 3;" \
	"spl.backup3 part 0 4;" \
	"u-boot part 0 5;" \
	"u-boot.env0 part 0 6;" \
	"u-boot.env1 part 0 7;" \
	"rootfs partubi 0 9" \

#define CONFIG_ENV_SETTINGS_NAND_V2 \
	"nand_active_ubi_vol=rootfs_a\0" \
	"rootfs_name=rootfs\0" \
	"kernel_name=uImage\0"\
	"nand_root_fs_type=ubifs rootwait=1\0" \
	"nand_args=run bootargs_defaults;" \
		"mtdparts default;" \
		"setenv ${partitionset_active} true;" \
		"if test -n ${A}; then " \
			"setenv nand_active_ubi_vol ${rootfs_name}_a;" \
		"fi;" \
		"if test -n ${B}; then " \
			"setenv nand_active_ubi_vol ${rootfs_name}_b;" \
		"fi;" \
		"setenv nand_root ubi0:${nand_active_ubi_vol} rw " \
		"ubi.mtd=rootfs,2048;" \
		"setenv bootargs ${bootargs} " \
		"root=${nand_root} noinitrd ${mtdparts} " \
		"rootfstype=${nand_root_fs_type} ip=${ip_method} " \
		"console=ttyMTD,mtdoops console=ttyO0,115200n8 mtdoops.mtddev" \
		"=mtdoops\0" \
	COMMON_ENV_DFU_ARGS \
		"dfu_alt_info=" DFU_ALT_INFO_NAND_V2 "\0" \
	COMMON_ENV_NAND_BOOT \
		"ubi part rootfs 2048;" \
		"ubifsmount ubi0:${nand_active_ubi_vol};" \
		"ubifsload ${kloadaddr} boot/${kernel_name};" \
		"ubifsload ${loadaddr} boot/${dtb_name}.dtb;" \
		"bootm ${kloadaddr} - ${loadaddr}\0" \
	"nand_boot_backup=ubifsload ${loadaddr} boot/am335x-draco.dtb;" \
		"bootm ${kloadaddr} - ${loadaddr}\0" \
	COMMON_ENV_NAND_CMDS

#define CONFIG_ENV_SETTINGS_V2 \
		COMMON_ENV_SETTINGS \
	"net_args=run bootargs_defaults;" \
		"mtdparts default;" \
		"setenv bootfile ${project_dir}/kernel/uImage;" \
		"setenv bootdtb ${project_dir}/kernel/dtb;" \
		"setenv rootpath /home/projects/${project_dir}/rootfs;" \
		"setenv bootargs ${bootargs} " \
		"root=/dev/nfs ${mtdparts} " \
		"nfsroot=${serverip}:${rootpath},${nfsopts} " \
		"ip=${ipaddr}:${serverip}:" \
		"${gatewayip}:${netmask}:${hostname}:eth0:off\0" \
	"net_nfs=echo Booting from network ...; " \
		"run net_args; " \
		"tftpboot ${kloadaddr} ${serverip}:${bootfile}; " \
		"tftpboot ${loadaddr} ${serverip}:${bootdtb}; " \
		"bootm ${kloadaddr} - ${loadaddr}\0"

/*
 * Variant 3 partition layout
 * chip-size = 512MiB
 *|         name |        size |           address area |
 *-------------------------------------------------------
 *|          spl | 128.000 KiB | 0x       0..0x   1ffff |
 *|  spl.backup1 | 128.000 KiB | 0x   20000..0x   3ffff |
 *|  spl.backup2 | 128.000 KiB | 0x   40000..0x   5ffff |
 *|  spl.backup3 | 128.000 KiB | 0x   60000..0x   7ffff |
 *|       u-boot |   1.875 MiB | 0x   80000..0x  25ffff |
 *|   uboot.env0 | 512.000 KiB | 0x  260000..0x  2Dffff |
 *|   uboot.env1 | 512.000 KiB | 0x  2E0000..0x  35ffff |
 *|       rootfs | 300.000 MiB | 0x  360000..0x12f5ffff |
 *|      mtdoops | 512.000 KiB | 0x12f60000..0x12fdffff |
 *|configuration | 104.125 MiB | 0x12fe0000..0x1fffffff |
 *-------------------------------------------------------
 */

#define MTDPARTS_DEFAULT_V3	"mtdparts=" MTDIDS_NAME_STR ":" \
					"128k(spl),"		\
					"128k(spl.backup1),"	\
					"128k(spl.backup2),"	\
					"128k(spl.backup3),"	\
					"1920k(u-boot),"	\
					"512k(u-boot.env0),"	\
					"512k(u-boot.env1),"	\
					"300m(rootfs),"		\
					"512k(mtdoops),"	\
					"-(configuration)"


#define CONFIG_NAND_OMAP_GPMC
#define CONFIG_NAND_OMAP_ELM
#define CONFIG_SYS_NAND_BASE		(0x08000000)	/* physical address */
							/* to access nand at */
							/* CS0 */
#define CONFIG_SYS_MAX_NAND_DEVICE	1		/* Max number of NAND
							   devices */
#if !defined(CONFIG_SPI_BOOT)
#undef CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_IS_IN_NAND
#define CONFIG_ENV_OFFSET		0x260000 /* environment starts here */
#define CONFIG_SYS_ENV_SECT_SIZE	(128 << 10)	/* 128 KiB */
#endif
#endif

#define CONFIG_OMAP_GPIO

/* Gpio cmd support */

/* Watchdog */
#define CONFIG_HW_WATCHDOG

/* Reboot after 60 sec if bootcmd fails */
#define CONFIG_RESET_TO_RETRY
#define CONFIG_BOOT_RETRY_TIME 60

#define CONFIG_BOOTCOUNT_LIMIT
#define CONFIG_BOOTCOUNT_ENV

/* Enable Device-Tree (FDT) support */
#define CONFIG_CMD_FDT

#endif	/* ! __CONFIG_SIEMENS_AM33X_COMMON_H */
