/*
 * (C) Copyright 2015 Hans de Goede <hdegoede@redhat.com>
 * (C) Copyright 2007-2013
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Jerry Wang <wangflord@allwinnertech.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _SUNXI_CPU_SUN9I_H
#define _SUNXI_CPU_SUN9I_H

#define REGS_AHB0_BASE			0x01C00000
#define REGS_AHB1_BASE			0x00800000
#define REGS_AHB2_BASE			0x03000000
#define REGS_APB0_BASE			0x06000000
#define REGS_APB1_BASE			0x07000000
#define REGS_RCPUS_BASE			0x08000000

#define SUNXI_SRAM_D_BASE		0x08100000

/* AHB0 Module */
#define SUNXI_NFC_BASE			(REGS_AHB0_BASE + 0x3000)
#define SUNXI_TSC_BASE			(REGS_AHB0_BASE + 0x4000)

#define SUNXI_MMC0_BASE			(REGS_AHB0_BASE + 0x0f000)
#define SUNXI_MMC1_BASE			(REGS_AHB0_BASE + 0x10000)
#define SUNXI_MMC2_BASE			(REGS_AHB0_BASE + 0x11000)
#define SUNXI_MMC3_BASE			(REGS_AHB0_BASE + 0x12000)
#define SUNXI_MMC_COMMON_BASE		(REGS_AHB0_BASE + 0x13000)

#define SUNXI_SPI0_BASE			(REGS_AHB0_BASE + 0x1A000)
#define SUNXI_SPI1_BASE			(REGS_AHB0_BASE + 0x1B000)
#define SUNXI_SPI2_BASE			(REGS_AHB0_BASE + 0x1C000)
#define SUNXI_SPI3_BASE			(REGS_AHB0_BASE + 0x1D000)

#define SUNXI_GIC400_BASE		(REGS_AHB0_BASE + 0x40000)
#define SUNXI_ARMA9_GIC_BASE		(REGS_AHB0_BASE + 0x41000)
#define SUNXI_ARMA9_CPUIF_BASE		(REGS_AHB0_BASE + 0x42000)

/* AHB1 Module */
#define SUNXI_DMA_BASE			(REGS_AHB1_BASE + 0x002000)
#define SUNXI_USBOTG_BASE		(REGS_AHB1_BASE + 0x100000)
#define SUNXI_USBEHCI0_BASE		(REGS_AHB1_BASE + 0x200000)
#define SUNXI_USBEHCI1_BASE		(REGS_AHB1_BASE + 0x201000)
#define SUNXI_USBEHCI2_BASE		(REGS_AHB1_BASE + 0x202000)

/* AHB2 Module */
#define SUNXI_DE_SYS_BASE		(REGS_AHB2_BASE + 0x000000)
#define SUNXI_DISP_SYS_BASE		(REGS_AHB2_BASE + 0x010000)
#define SUNXI_DE_FE0_BASE		(REGS_AHB2_BASE + 0x100000)
#define SUNXI_DE_FE1_BASE		(REGS_AHB2_BASE + 0x140000)
#define SUNXI_DE_FE2_BASE		(REGS_AHB2_BASE + 0x180000)

#define SUNXI_DE_BE0_BASE		(REGS_AHB2_BASE + 0x200000)
#define SUNXI_DE_BE1_BASE		(REGS_AHB2_BASE + 0x240000)
#define SUNXI_DE_BE2_BASE		(REGS_AHB2_BASE + 0x280000)

#define SUNXI_DE_DEU0_BASE		(REGS_AHB2_BASE + 0x300000)
#define SUNXI_DE_DEU1_BASE		(REGS_AHB2_BASE + 0x340000)
#define SUNXI_DE_DRC0_BASE		(REGS_AHB2_BASE + 0x400000)
#define SUNXI_DE_DRC1_BASE		(REGS_AHB2_BASE + 0x440000)

#define SUNXI_LCD0_BASE			(REGS_AHB2_BASE + 0xC00000)
#define SUNXI_LCD1_BASE			(REGS_AHB2_BASE + 0xC10000)
#define SUNXI_LCD2_BASE			(REGS_AHB2_BASE + 0xC20000)
#define SUNXI_MIPI_DSI0_BASE		(REGS_AHB2_BASE + 0xC40000)
/* Also seen as SUNXI_MIPI_DSI0_DPHY_BASE 0x01ca1000 */
#define SUNXI_MIPI_DSI0_DPHY_BASE	(REGS_AHB2_BASE + 0xC40100)
#define SUNXI_HDMI_BASE			(REGS_AHB2_BASE + 0xD00000)

/* APB0 Module */
#define SUNXI_CCM_BASE			(REGS_APB0_BASE + 0x0000)
#define SUNXI_CCMMODULE_BASE		(REGS_APB0_BASE + 0x0400)
#define SUNXI_PIO_BASE			(REGS_APB0_BASE + 0x0800)
#define SUNXI_TIMER_BASE		(REGS_APB0_BASE + 0x0C00)
#define SUNXI_PWM_BASE			(REGS_APB0_BASE + 0x1400)
#define SUNXI_LRADC_BASE		(REGS_APB0_BASE + 0x1800)

/* APB1 Module */
#define SUNXI_UART0_BASE		(REGS_APB1_BASE + 0x0000)
#define SUNXI_UART1_BASE		(REGS_APB1_BASE + 0x0400)
#define SUNXI_UART2_BASE		(REGS_APB1_BASE + 0x0800)
#define SUNXI_UART3_BASE		(REGS_APB1_BASE + 0x0C00)
#define SUNXI_UART4_BASE		(REGS_APB1_BASE + 0x1000)
#define SUNXI_UART5_BASE		(REGS_APB1_BASE + 0x1400)
#define SUNXI_TWI0_BASE			(REGS_APB1_BASE + 0x2800)
#define SUNXI_TWI1_BASE			(REGS_APB1_BASE + 0x2C00)
#define SUNXI_TWI2_BASE			(REGS_APB1_BASE + 0x3000)
#define SUNXI_TWI3_BASE			(REGS_APB1_BASE + 0x3400)
#define SUNXI_TWI4_BASE			(REGS_APB1_BASE + 0x3800)

/* RCPUS Module */
#define SUNXI_PRCM_BASE			(REGS_RCPUS_BASE + 0x1400)
#define SUNXI_R_UART_BASE		(REGS_RCPUS_BASE + 0x2800)
#define SUNXI_R_PIO_BASE		(REGS_RCPUS_BASE + 0x2c00)
#define SUNXI_RSB_BASE			(REGS_RCPUS_BASE + 0x3400)

/* Misc. */
#define SUNXI_BROM_BASE			0xFFFF0000 /* 32K */
#define SUNXI_CPU_CFG			(SUNXI_TIMER_BASE + 0x13c)

#ifndef __ASSEMBLY__
void sunxi_board_init(void);
void sunxi_reset(void);
int sunxi_get_sid(unsigned int *sid);
#endif

#endif /* _SUNXI_CPU_SUN9I_H */
