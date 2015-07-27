/*
 * Copyright (C) 2015, Bin Meng <bmeng.cn@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * board/config.h - configuration options, board specific
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <configs/x86-common.h>

#define CONFIG_SYS_MONITOR_LEN		(1 << 20)
#define CONFIG_ARCH_MISC_INIT

#define CONFIG_PCI_CONFIG_HOST_BRIDGE
#define CONFIG_SYS_EARLY_PCI_INIT
#define CONFIG_PCI_PNP
#define CONFIG_E1000

#define CONFIG_STD_DEVICES_SETTINGS	"stdin=serial,vga,usbkbd\0" \
					"stdout=serial,vga\0" \
					"stderr=serial,vga\0"

#define CONFIG_SCSI_DEV_LIST		\
	{PCI_VENDOR_ID_INTEL, PCI_DEVICE_ID_INTEL_VALLEYVIEW_SATA}

#define CONFIG_MMC
#define CONFIG_SDHCI
#define CONFIG_GENERIC_MMC
#define CONFIG_MMC_SDMA
#define CONFIG_CMD_MMC

/* BayTrail IGD support */
#define CONFIG_VGA_AS_SINGLE_DEVICE

/* Environment configuration */
#define CONFIG_ENV_SECT_SIZE		0x1000
#define CONFIG_ENV_OFFSET		0x006ff000

#endif	/* __CONFIG_H */
