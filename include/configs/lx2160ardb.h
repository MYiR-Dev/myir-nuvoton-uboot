/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2018 NXP
 */

#ifndef __LX2_RDB_H
#define __LX2_RDB_H

#include "lx2160a_common.h"

/* Qixis */
#define QIXIS_XMAP_MASK			0x07
#define QIXIS_XMAP_SHIFT		5
#define QIXIS_RST_CTL_RESET_EN		0x30
#define QIXIS_LBMAP_DFLTBANK		0x00
#define QIXIS_LBMAP_ALTBANK		0x20
#define QIXIS_LBMAP_QSPI		0x00
#define QIXIS_RCW_SRC_QSPI		0xff
#define QIXIS_RST_CTL_RESET		0x31
#define QIXIS_RCFG_CTL_RECONFIG_IDLE	0x20
#define QIXIS_RCFG_CTL_RECONFIG_START	0x21
#define QIXIS_RCFG_CTL_WATCHDOG_ENBLE	0x08
#define QIXIS_LBMAP_MASK		0x0f
#define QIXIS_LBMAP_SD
#define QIXIS_RCW_SRC_SD           0x08
#define NON_EXTENDED_DUTCFG

/* VID */

#define I2C_MUX_CH_VOL_MONITOR		0xA
/* Voltage monitor on channel 2*/
#define I2C_VOL_MONITOR_ADDR		0x63
#define I2C_VOL_MONITOR_BUS_V_OFFSET	0x2
#define I2C_VOL_MONITOR_BUS_V_OVF	0x1
#define I2C_VOL_MONITOR_BUS_V_SHIFT	3
#define CONFIG_VID_FLS_ENV		"lx2160ardb_vdd_mv"
#define CONFIG_VID

/* The lowest and highest voltage allowed*/
#define VDD_MV_MIN			775
#define VDD_MV_MAX			855

/* PM Bus commands code for LTC3882*/
#define PMBUS_CMD_PAGE                  0x0
#define PMBUS_CMD_READ_VOUT             0x8B
#define PMBUS_CMD_PAGE_PLUS_WRITE       0x05
#define PMBUS_CMD_VOUT_COMMAND          0x21
#define PWM_CHANNEL0                    0x0

#define CONFIG_VOL_MONITOR_LTC3882_SET
#define CONFIG_VOL_MONITOR_LTC3882_READ

/* RTC */
#define CONFIG_SYS_RTC_BUS_NUM		4

/* MAC/PHY configuration */
#if defined(CONFIG_FSL_MC_ENET)
#define CONFIG_MII
#define CONFIG_ETHPRIME		"DPMAC1@xgmii"

#define AQR107_PHY_ADDR1	0x04
#define AQR107_PHY_ADDR2	0x05

#define CORTINA_NO_FW_UPLOAD
#define CORTINA_PHY_ADDR1	0x0
#define INPHI_PHY_ADDR1		0x0

#define RGMII_PHY_ADDR1		0x01
#define RGMII_PHY_ADDR2		0x02

#endif

/* EEPROM */
#define CONFIG_ID_EEPROM
#define CONFIG_SYS_I2C_EEPROM_NXID
#define CONFIG_SYS_EEPROM_BUS_NUM	           0
#define CONFIG_SYS_I2C_EEPROM_ADDR	           0x57
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN	    1
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS     3
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS 5

/* Initial environment variables */
#define CONFIG_EXTRA_ENV_SETTINGS		\
	"hwconfig=fsl_ddr:bank_intlv=auto\0"	\
	"scriptaddr=0x80800000\0"		\
	"kernel_addr_r=0x81000000\0"		\
	"pxefile_addr_r=0x81000000\0"		\
	"fdt_addr_r=0x88000000\0"		\
	"ramdisk_addr_r=0x89000000\0"		\
	"loadaddr=0x80100000\0"			\
	"kernel_addr=0x100000\0"		\
	"ramdisk_addr=0x800000\0"		\
	"ramdisk_size=0x2000000\0"		\
	"fdt_high=0xa0000000\0"			\
	"initrd_high=0xffffffffffffffff\0"	\
	"kernel_start=0x21000000\0"		\
	"lx2160ardb_vdd_mv=800\0"		\
	"mcmemsize=0x40000000\0"

#include <asm/fsl_secure_boot.h>

#endif /* __LX2_RDB_H */
