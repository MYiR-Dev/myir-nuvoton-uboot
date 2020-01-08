/* SPDX-License-Identifier: GPL-2.0+ OR BSD-3-Clause */
/*
 * Copyright (C) 2018, STMicroelectronics - All Rights Reserved
 */

#ifndef _MACH_STM32_H_
#define _MACH_STM32_H_

/*
 * Peripheral memory map
 * only address used before device tree parsing
 */
#define STM32_RCC_BASE			0x50000000
#define STM32_PWR_BASE			0x50001000
#define STM32_DBGMCU_BASE		0x50081000
#define STM32_TZC_BASE			0x5C006000
#define STM32_ETZPC_BASE		0x5C007000
#define STM32_STGEN_BASE		0x5C008000
#define STM32_TAMP_BASE			0x5C00A000

#define STM32_USART1_BASE		0x5C000000
#define STM32_USART2_BASE		0x4000E000
#define STM32_USART3_BASE		0x4000F000
#define STM32_UART4_BASE		0x40010000
#define STM32_UART5_BASE		0x40011000
#define STM32_USART6_BASE		0x44003000
#define STM32_UART7_BASE		0x40018000
#define STM32_UART8_BASE		0x40019000

#define STM32_SYSRAM_BASE		0x2FFC0000
#define STM32_SYSRAM_SIZE		SZ_256K

#define STM32_DDR_BASE			0xC0000000
#define STM32_DDR_SIZE			SZ_1G

#ifndef __ASSEMBLY__
/* enumerated used to identify the SYSCON driver instance */
enum {
	STM32MP_SYSCON_UNKNOWN,
	STM32MP_SYSCON_PWR,
	STM32MP_SYSCON_SYSCFG,
};

/*
 * enumerated for boot interface from Bootrom, used in TAMP_BOOT_CONTEXT
 * - boot device = bit 8:4
 * - boot instance = bit 3:0
 */
#define BOOT_TYPE_MASK		0xF0
#define BOOT_TYPE_SHIFT		4
#define BOOT_INSTANCE_MASK	0x0F
#define BOOT_INSTANCE_SHIFT	0

enum boot_device {
	BOOT_FLASH_SD = 0x10,
	BOOT_FLASH_SD_1 = 0x11,
	BOOT_FLASH_SD_2 = 0x12,
	BOOT_FLASH_SD_3 = 0x13,

	BOOT_FLASH_EMMC = 0x20,
	BOOT_FLASH_EMMC_1 = 0x21,
	BOOT_FLASH_EMMC_2 = 0x22,
	BOOT_FLASH_EMMC_3 = 0x23,

	BOOT_FLASH_NAND = 0x30,
	BOOT_FLASH_NAND_FMC = 0x31,

	BOOT_FLASH_NOR = 0x40,
	BOOT_FLASH_NOR_QSPI = 0x41,

	BOOT_SERIAL_UART = 0x50,
	BOOT_SERIAL_UART_1 = 0x51,
	BOOT_SERIAL_UART_2 = 0x52,
	BOOT_SERIAL_UART_3 = 0x53,
	BOOT_SERIAL_UART_4 = 0x54,
	BOOT_SERIAL_UART_5 = 0x55,
	BOOT_SERIAL_UART_6 = 0x56,
	BOOT_SERIAL_UART_7 = 0x57,
	BOOT_SERIAL_UART_8 = 0x58,

	BOOT_SERIAL_USB = 0x60,
	BOOT_SERIAL_USB_OTG = 0x62,
};

/* TAMP registers */
#define TAMP_BACKUP_REGISTER(x)		(STM32_TAMP_BASE + 0x100 + 4 * x)
#define TAMP_BACKUP_MAGIC_NUMBER	TAMP_BACKUP_REGISTER(4)
#define TAMP_BACKUP_BRANCH_ADDRESS	TAMP_BACKUP_REGISTER(5)
#define TAMP_COPRO_RSC_TBL_ADDRESS	TAMP_BACKUP_REGISTER(17)
#define TAMP_COPRO_STATE		TAMP_BACKUP_REGISTER(18)
#define TAMP_BOOT_CONTEXT		TAMP_BACKUP_REGISTER(20)
#define TAMP_BOOTCOUNT			TAMP_BACKUP_REGISTER(21)

#define TAMP_COPRO_STATE_OFF		0
#define TAMP_COPRO_STATE_INIT		1
#define TAMP_COPRO_STATE_CRUN		2
#define TAMP_COPRO_STATE_CSTOP		3
#define TAMP_COPRO_STATE_STANDBY	4
#define TAMP_COPRO_STATE_CRASH		5

#define TAMP_BOOT_MODE_MASK		GENMASK(15, 8)
#define TAMP_BOOT_MODE_SHIFT		8
#define TAMP_BOOT_DEVICE_MASK		GENMASK(7, 4)
#define TAMP_BOOT_INSTANCE_MASK		GENMASK(3, 0)
#define TAMP_BOOT_FORCED_MASK		GENMASK(7, 0)
#define TAMP_BOOT_DEBUG_ON		BIT(16)

enum forced_boot_mode {
	BOOT_NORMAL = 0x00,
	BOOT_FASTBOOT = 0x01,
	BOOT_RECOVERY = 0x02,
	BOOT_STM32PROG = 0x03,
	BOOT_UMS_MMC0 = 0x10,
	BOOT_UMS_MMC1 = 0x11,
	BOOT_UMS_MMC2 = 0x12,
};

/* offset used for BSEC driver: misc_read and misc_write */
#define STM32_BSEC_SHADOW_OFFSET	0x0
#define STM32_BSEC_SHADOW(id)		(STM32_BSEC_SHADOW_OFFSET + (id) * 4)
#define STM32_BSEC_OTP_OFFSET		0x80000000
#define STM32_BSEC_OTP(id)		(STM32_BSEC_OTP_OFFSET + (id) * 4)

#define BSEC_OTP_BOARD	59

#endif /* __ASSEMBLY__*/
#endif /* _MACH_STM32_H_ */
