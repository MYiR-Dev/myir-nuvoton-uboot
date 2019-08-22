/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2019 NXP
 */

#ifndef __LS1028A_RDB_H
#define __LS1028A_RDB_H

#include "ls1028a_common.h"

#define CONFIG_SYS_CLK_FREQ		100000000
#define CONFIG_DDR_CLK_FREQ		100000000
#define COUNTER_FREQUENCY_REAL		(CONFIG_SYS_CLK_FREQ / 4)

#define CONFIG_SYS_RTC_BUS_NUM         0

/* Store environment at top of flash */
#define CONFIG_ENV_SIZE			0x2000

#define CONFIG_DIMM_SLOTS_PER_CTLR          1

#define CONFIG_SYS_MONITOR_BASE CONFIG_SYS_TEXT_BASE

#define CONFIG_QIXIS_I2C_ACCESS

/*
 * QIXIS Definitions
 */
#define CONFIG_FSL_QIXIS

#ifdef CONFIG_FSL_QIXIS
#define QIXIS_BASE			0x7fb00000
#define QIXIS_BASE_PHYS			QIXIS_BASE
#define CONFIG_SYS_I2C_FPGA_ADDR	0x66
#define QIXIS_LBMAP_SWITCH		2
#define QIXIS_LBMAP_MASK		0xe0
#define QIXIS_LBMAP_SHIFT		0x5
#define QIXIS_LBMAP_DFLTBANK		0x00
#define QIXIS_LBMAP_ALTBANK		0x00
#define QIXIS_LBMAP_SD			0x00
#define QIXIS_LBMAP_EMMC		0x00
#define QIXIS_LBMAP_QSPI		0x00
#define QIXIS_RCW_SRC_SD		0xf8
#define QIXIS_RCW_SRC_EMMC		0xf9
#define QIXIS_RCW_SRC_QSPI		0xff
#define QIXIS_RST_CTL_RESET		0x31
#define QIXIS_RCFG_CTL_RECONFIG_IDLE	0x10
#define QIXIS_RCFG_CTL_RECONFIG_START	0x11
#define QIXIS_RCFG_CTL_WATCHDOG_ENBLE	0x08
#define QIXIS_RST_FORCE_MEM		0x01

#define CONFIG_SYS_FPGA_CSPR_EXT	(0x0)
#define CONFIG_SYS_FPGA_CSPR		(CSPR_PHYS_ADDR(QIXIS_BASE_PHYS) | \
					CSPR_PORT_SIZE_8 | \
					CSPR_MSEL_GPCM | \
					CSPR_V)
#define CONFIG_SYS_FPGA_CSOR		(CSOR_NOR_ADM_SHIFT(4) | \
					CSOR_NOR_NOR_MODE_AVD_NOR | \
					CSOR_NOR_TRHZ_80)
#endif

/* SATA */
#ifndef CONFIG_CMD_EXT2
#define CONFIG_CMD_EXT2
#endif
#define CONFIG_SYS_SCSI_MAX_SCSI_ID		1
#define CONFIG_SYS_SCSI_MAX_LUN			1
#define CONFIG_SYS_SCSI_MAX_DEVICE		(CONFIG_SYS_SCSI_MAX_SCSI_ID * \
						CONFIG_SYS_SCSI_MAX_LUN)
#define SCSI_VEND_ID 0x1b4b
#define SCSI_DEV_ID  0x9170
#define CONFIG_SCSI_DEV_LIST {SCSI_VEND_ID, SCSI_DEV_ID}
#define CONFIG_SCSI_AHCI_PLAT
#define CONFIG_SYS_SATA1                        AHCI_BASE_ADDR1

#endif /* __LS1028A_RDB_H */
