/*
 * (C) Copyright 2016 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#ifndef _ASM_ARCH_GRF_RV1108_H
#define _ASM_ARCH_GRF_RV1108_H

#include <common.h>

struct rv1108_grf {
	u32 reserved[4];
	u32 gpio1a_iomux;
	u32 gpio1b_iomux;
	u32 gpio1c_iomux;
	u32 gpio1d_iomux;
	u32 gpio2a_iomux;
	u32 gpio2b_iomux;
	u32 gpio2c_iomux;
	u32 gpio2d_iomux;
	u32 gpio3a_iomux;
	u32 gpio3b_iomux;
	u32 gpio3c_iomux;
	u32 gpio3d_iomux;
	u32 reserved1[52];
	u32 gpio1a_pull;
	u32 gpio1b_pull;
	u32 gpio1c_pull;
	u32 gpio1d_pull;
	u32 gpio2a_pull;
	u32 gpio2b_pull;
	u32 gpio2c_pull;
	u32 gpio2d_pull;
	u32 gpio3a_pull;
	u32 gpio3b_pull;
	u32 gpio3c_pull;
	u32 gpio3d_pull;
	u32 reserved2[52];
	u32 gpio1a_drv;
	u32 gpio1b_drv;
	u32 gpio1c_drv;
	u32 gpio1d_drv;
	u32 gpio2a_drv;
	u32 gpio2b_drv;
	u32 gpio2c_drv;
	u32 gpio2d_drv;
	u32 gpio3a_drv;
	u32 gpio3b_drv;
	u32 gpio3c_drv;
	u32 gpio3d_drv;
	u32 reserved3[50];
	u32 gpio1l_sr;
	u32 gpio1h_sr;
	u32 gpio2l_sr;
	u32 gpio2h_sr;
	u32 gpio3l_sr;
	u32 gpio3h_sr;
	u32 reserved4[26];
	u32 gpio1l_smt;
	u32 gpio1h_smt;
	u32 gpio2l_smt;
	u32 gpio2h_smt;
	u32 gpio3l_smt;
	u32 gpio3h_smt;
	u32 reserved5[24];
	u32 soc_con0;
	u32 soc_con1;
	u32 soc_con2;
	u32 soc_con3;
	u32 soc_con4;
	u32 soc_con5;
	u32 soc_con6;
	u32 soc_con7;
	u32 soc_con8;
	u32 soc_con9;
	u32 soc_con10;
	u32 soc_con11;
	u32 reserved6[20];
	u32 soc_status0;
	u32 soc_status1;
	u32 reserved7[30];
	u32 cpu_con0;
	u32 cpu_con1;
	u32 reserved8[30];
	u32 os_reg0;
	u32 os_reg1;
	u32 os_reg2;
	u32 os_reg3;
	u32 reserved9[29];
	u32 ddr_status;
	u32 reserved10[30];
	u32 sig_det_con;
	u32 reserved11[3];
	u32 sig_det_status;
	u32 reserved12[3];
	u32 sig_det_clr;
	u32 reserved13[23];
	u32 host_con0;
	u32 host_con1;
	u32 reserved14[2];
	u32 dma_con0;
	u32 dma_con1;
	u32 reserved15[59];
	u32 uoc_status;
	u32 reserved16[2];
	u32 host_status;
	u32 reserved17[59];
	u32 gmac_con0;
	u32 reserved18[191];
	u32 chip_id;
};

check_member(rv1108_grf, chip_id, 0x0c00);

/* GRF_GPIO1B_IOMUX */
enum {
	GPIO1B7_SHIFT		= 14,
	GPIO1B7_MASK		= 3 << GPIO1B7_SHIFT,
	GPIO1B7_GPIO		= 0,
	GPIO1B7_LCDC_D12,
	GPIO1B7_I2S_SDIO2_M0,
	GPIO1B7_GMAC_RXDV,

	GPIO1B6_SHIFT		= 12,
	GPIO1B6_MASK		= 3 << GPIO1B6_SHIFT,
	GPIO1B6_GPIO		= 0,
	GPIO1B6_LCDC_D13,
	GPIO1B6_I2S_LRCLKTX_M0,
	GPIO1B6_GMAC_RXD1,

	GPIO1B5_SHIFT		= 10,
	GPIO1B5_MASK		= 3 << GPIO1B5_SHIFT,
	GPIO1B5_GPIO		= 0,
	GPIO1B5_LCDC_D14,
	GPIO1B5_I2S_SDIO1_M0,
	GPIO1B5_GMAC_RXD0,

	GPIO1B4_SHIFT		= 8,
	GPIO1B4_MASK		= 3 << GPIO1B4_SHIFT,
	GPIO1B4_GPIO		= 0,
	GPIO1B4_LCDC_D15,
	GPIO1B4_I2S_MCLK_M0,
	GPIO1B4_GMAC_TXEN,

	GPIO1B3_SHIFT		= 6,
	GPIO1B3_MASK		= 3 << GPIO1B3_SHIFT,
	GPIO1B3_GPIO		= 0,
	GPIO1B3_LCDC_D16,
	GPIO1B3_I2S_SCLK_M0,
	GPIO1B3_GMAC_TXD1,

	GPIO1B2_SHIFT		= 4,
	GPIO1B2_MASK		= 3 << GPIO1B2_SHIFT,
	GPIO1B2_GPIO		= 0,
	GPIO1B2_LCDC_D17,
	GPIO1B2_I2S_SDIO_M0,
	GPIO1B2_GMAC_TXD0,

	GPIO1B1_SHIFT		= 2,
	GPIO1B1_MASK		= 3 << GPIO1B1_SHIFT,
	GPIO1B1_GPIO		= 0,
	GPIO1B1_LCDC_D9,
	GPIO1B1_PWM7,

	GPIO1B0_SHIFT		= 0,
	GPIO1B0_MASK		= 3,
	GPIO1B0_GPIO		= 0,
	GPIO1B0_LCDC_D8,
	GPIO1B0_PWM6,
};

/* GRF_GPIO1C_IOMUX */
enum {
	GPIO1C7_SHIFT		= 14,
	GPIO1C7_MASK		= 3 << GPIO1C7_SHIFT,
	GPIO1C7_GPIO		= 0,
	GPIO1C7_CIF_D5,
	GPIO1C7_I2S_SDIO2_M1,

	GPIO1C6_SHIFT		= 12,
	GPIO1C6_MASK		= 3 << GPIO1C6_SHIFT,
	GPIO1C6_GPIO		= 0,
	GPIO1C6_CIF_D4,
	GPIO1C6_I2S_LRCLKTX_M1,

	GPIO1C5_SHIFT		= 10,
	GPIO1C5_MASK		= 3 << GPIO1C5_SHIFT,
	GPIO1C5_GPIO		= 0,
	GPIO1C5_LCDC_CLK,
	GPIO1C5_GMAC_CLK,

	GPIO1C4_SHIFT		= 8,
	GPIO1C4_MASK		= 3 << GPIO1C4_SHIFT,
	GPIO1C4_GPIO		= 0,
	GPIO1C4_LCDC_HSYNC,
	GPIO1C4_GMAC_MDC,

	GPIO1C3_SHIFT		= 6,
	GPIO1C3_MASK		= 3 << GPIO1C3_SHIFT,
	GPIO1C3_GPIO		= 0,
	GPIO1C3_LCDC_VSYNC,
	GPIO1C3_GMAC_MDIO,

	GPIO1C2_SHIFT		= 4,
	GPIO1C2_MASK		= 3 << GPIO1C2_SHIFT,
	GPIO1C2_GPIO		= 0,
	GPIO1C2_LCDC_EN,
	GPIO1C2_I2S_SDIO3_M0,
	GPIO1C2_GMAC_RXER,

	GPIO1C1_SHIFT		= 2,
	GPIO1C1_MASK		= 3 << GPIO1C1_SHIFT,
	GPIO1C1_GPIO		= 0,
	GPIO1C1_LCDC_D10,
	GPIO1C1_I2S_SDI_M0,
	GPIO1C1_PWM4,

	GPIO1C0_SHIFT           = 0,
	GPIO1C0_MASK		= 3,
	GPIO1C0_GPIO		= 0,
	GPIO1C0_LCDC_D11,
	GPIO1C0_I2S_LRCLKRX_M0,
};

/* GRF_GPIO1D_OIMUX */
enum {
	GPIO1D7_SHIFT		= 14,
	GPIO1D7_MASK		= 3 << GPIO1D7_SHIFT,
	GPIO1D7_GPIO		= 0,
	GPIO1D7_HDMI_CEC,
	GPIO1D7_DSP_RTCK,

	GPIO1D6_SHIFT		= 12,
	GPIO1D6_MASK		= 1 << GPIO1D6_SHIFT,
	GPIO1D6_GPIO		= 0,
	GPIO1D6_HDMI_HPD_M0,

	GPIO1D5_SHIFT		= 10,
	GPIO1D5_MASK		= 3 << GPIO1D5_SHIFT,
	GPIO1D5_GPIO		= 0,
	GPIO1D5_UART2_RTSN,
	GPIO1D5_HDMI_SDA_M0,

	GPIO1D4_SHIFT		= 8,
	GPIO1D4_MASK		= 3 << GPIO1D4_SHIFT,
	GPIO1D4_GPIO		= 0,
	GPIO1D4_UART2_CTSN,
	GPIO1D4_HDMI_SCL_M0,

	GPIO1D3_SHIFT		= 6,
	GPIO1D3_MASK		= 3 << GPIO1D3_SHIFT,
	GPIO1D3_GPIO		= 0,
	GPIO1D3_UART0_SOUT,
	GPIO1D3_SPI_TXD_M0,

	GPIO1D2_SHIFT		= 4,
	GPIO1D2_MASK		= 3 << GPIO1D2_SHIFT,
	GPIO1D2_GPIO		= 0,
	GPIO1D2_UART0_SIN,
	GPIO1D2_SPI_RXD_M0,
	GPIO1D2_DSP_TDI,

	GPIO1D1_SHIFT		= 2,
	GPIO1D1_MASK		= 3 << GPIO1D1_SHIFT,
	GPIO1D1_GPIO		= 0,
	GPIO1D1_UART0_RTSN,
	GPIO1D1_SPI_CSN0_M0,
	GPIO1D1_DSP_TMS,

	GPIO1D0_SHIFT		= 0,
	GPIO1D0_MASK		= 3,
	GPIO1D0_GPIO		= 0,
	GPIO1D0_UART0_CTSN,
	GPIO1D0_SPI_CLK_M0,
	GPIO1D0_DSP_TCK,
};

/* GRF_GPIO2A_IOMUX */
enum {
	GPIO2A7_SHIFT		= 14,
	GPIO2A7_MASK		= 3 << GPIO2A7_SHIFT,
	GPIO2A7_GPIO		= 0,
	GPIO2A7_FLASH_D7,
	GPIO2A7_EMMC_D7,

	GPIO2A6_SHIFT		= 12,
	GPIO2A6_MASK		= 3 << GPIO2A6_SHIFT,
	GPIO2A6_GPIO		= 0,
	GPIO2A6_FLASH_D6,
	GPIO2A6_EMMC_D6,

	GPIO2A5_SHIFT           = 10,
	GPIO2A5_MASK            = 3 << GPIO2A5_SHIFT,
	GPIO2A5_GPIO            = 0,
	GPIO2A5_FLASH_D5,
	GPIO2A5_EMMC_D5,

	GPIO2A4_SHIFT           = 8,
	GPIO2A4_MASK            = 3 << GPIO2A4_SHIFT,
	GPIO2A4_GPIO            = 0,
	GPIO2A4_FLASH_D4,
	GPIO2A4_EMMC_D4,

	GPIO2A3_SHIFT           = 6,
	GPIO2A3_MASK            = 3 << GPIO2A3_SHIFT,
	GPIO2A3_GPIO            = 0,
	GPIO2A3_FLASH_D3,
	GPIO2A3_EMMC_D3,
	GPIO2A3_SFC_HOLD_IO3,

	GPIO2A2_SHIFT           = 4,
	GPIO2A2_MASK            = 3 << GPIO2A2_SHIFT,
	GPIO2A2_GPIO            = 0,
	GPIO2A2_FLASH_D2,
	GPIO2A2_EMMC_D2,
	GPIO2A2_SFC_WP_IO2,

	GPIO2A1_SHIFT           = 2,
	GPIO2A1_MASK            = 3 << GPIO2A1_SHIFT,
	GPIO2A1_GPIO            = 0,
	GPIO2A1_FLASH_D1,
	GPIO2A1_EMMC_D1,
	GPIO2A1_SFC_SO_IO1,

	GPIO2A0_SHIFT           = 0,
	GPIO2A0_MASK            = 3 << GPIO2A0_SHIFT,
	GPIO2A0_GPIO            = 0,
	GPIO2A0_FLASH_D0,
	GPIO2A0_EMMC_D0,
	GPIO2A0_SFC_SI_IO0,
};

/* GRF_GPIO2D_IOMUX */
enum {
	GPIO2B7_SHIFT		= 14,
	GPIO2B7_MASK		= 3 << GPIO2B7_SHIFT,
	GPIO2B7_GPIO		= 0,
	GPIO2B7_FLASH_CS1,
	GPIO2B7_SFC_CLK,

	GPIO2B6_SHIFT           = 12,
	GPIO2B6_MASK            = 1 << GPIO2B6_SHIFT,
	GPIO2B6_GPIO            = 0,
	GPIO2B6_EMMC_CLKO,

	GPIO2B5_SHIFT           = 10,
	GPIO2B5_MASK            = 1 << GPIO2B5_SHIFT,
	GPIO2B5_GPIO            = 0,
	GPIO2B5_FLASH_CS0,

	GPIO2B4_SHIFT           = 8,
	GPIO2B4_MASK            = 3 << GPIO2B4_SHIFT,
	GPIO2B4_GPIO            = 0,
	GPIO2B4_FLASH_RDY,
	GPIO2B4_EMMC_CMD,
	GPIO2B4_SFC_CSN0,

	GPIO2B3_SHIFT           = 6,
	GPIO2B3_MASK            = 1 << GPIO2B3_SHIFT,
	GPIO2B3_GPIO            = 0,
	GPIO2B3_FLASH_RDN,

	GPIO2B2_SHIFT           = 4,
	GPIO2B2_MASK            = 1 << GPIO2B2_SHIFT,
	GPIO2B2_GPIO            = 0,
	GPIO2B2_FLASH_WRN,

	GPIO2B1_SHIFT           = 2,
	GPIO2B1_MASK            = 1 << GPIO2B1_SHIFT,
	GPIO2B1_GPIO            = 0,
	GPIO2B1_FLASH_CLE,

	GPIO2B0_SHIFT           = 0,
	GPIO2B0_MASK            = 1 << GPIO2B0_SHIFT,
	GPIO2B0_GPIO            = 0,
	GPIO2B0_FLASH_ALE,
};

/* GRF_GPIO2D_IOMUX */
enum {
	GPIO2D7_SHIFT		= 14,
	GPIO2D7_MASK		= 1 << GPIO2D7_SHIFT,
	GPIO2D7_GPIO		= 0,
	GPIO2D7_SDIO_D0,

	GPIO2D6_SHIFT		= 12,
	GPIO2D6_MASK		= 1 << GPIO2D6_SHIFT,
	GPIO2D6_GPIO		= 0,
	GPIO2D6_SDIO_CMD,

	GPIO2D5_SHIFT		= 10,
	GPIO2D5_MASK		= 1 << GPIO2D5_SHIFT,
	GPIO2D5_GPIO		= 0,
	GPIO2D5_SDIO_CLKO,

	GPIO2D4_SHIFT		= 8,
	GPIO2D4_MASK		= 1 << GPIO2D4_SHIFT,
	GPIO2D4_GPIO		= 0,
	GPIO2D4_I2C1_SCL,

	GPIO2D3_SHIFT		= 6,
	GPIO2D3_MASK		= 1 << GPIO2D3_SHIFT,
	GPIO2D3_GPIO		= 0,
	GPIO2D3_I2C1_SDA,

	GPIO2D2_SHIFT		= 4,
	GPIO2D2_MASK		= 3 << GPIO2D2_SHIFT,
	GPIO2D2_GPIO		= 0,
	GPIO2D2_UART2_SOUT_M0,
	GPIO2D2_JTAG_TCK,

	GPIO2D1_SHIFT		= 2,
	GPIO2D1_MASK		= 3 << GPIO2D1_SHIFT,
	GPIO2D1_GPIO		= 0,
	GPIO2D1_UART2_SIN_M0,
	GPIO2D1_JTAG_TMS,
	GPIO2D1_DSP_TMS,

	GPIO2D0_SHIFT		= 0,
	GPIO2D0_MASK		= 3,
	GPIO2D0_GPIO		= 0,
	GPIO2D0_UART0_CTSN,
	GPIO2D0_SPI_CLK_M0,
	GPIO2D0_DSP_TCK,
};

/* GRF_GPIO3A_IOMUX */
enum {
	GPIO3A7_SHIFT		= 14,
	GPIO3A7_MASK		= 1 << GPIO3A7_SHIFT,
	GPIO3A7_GPIO		= 0,

	GPIO3A6_SHIFT		= 12,
	GPIO3A6_MASK		= 1 << GPIO3A6_SHIFT,
	GPIO3A6_GPIO		= 0,
	GPIO3A6_UART1_SOUT,

	GPIO3A5_SHIFT		= 10,
	GPIO3A5_MASK		= 1 << GPIO3A5_SHIFT,
	GPIO3A5_GPIO		= 0,
	GPIO3A5_UART1_SIN,

	GPIO3A4_SHIFT		= 8,
	GPIO3A4_MASK		= 1 << GPIO3A4_SHIFT,
	GPIO3A4_GPIO		= 0,
	GPIO3A4_UART1_CTSN,

	GPIO3A3_SHIFT		= 6,
	GPIO3A3_MASK		= 1 << GPIO3A3_SHIFT,
	GPIO3A3_GPIO		= 0,
	GPIO3A3_UART1_RTSN,

	GPIO3A2_SHIFT		= 4,
	GPIO3A2_MASK		= 1 << GPIO3A2_SHIFT,
	GPIO3A2_GPIO		= 0,
	GPIO3A2_SDIO_D3,

	GPIO3A1_SHIFT		= 2,
	GPIO3A1_MASK		= 1 << GPIO3A1_SHIFT,
	GPIO3A1_GPIO		= 0,
	GPIO3A1_SDIO_D2,

	GPIO3A0_SHIFT		= 0,
	GPIO3A0_MASK		= 1,
	GPIO3A0_GPIO		= 0,
	GPIO3A0_SDIO_D1,
};

/* GRF_GPIO3C_IOMUX */
enum {
	GPIO3C7_SHIFT		= 14,
	GPIO3C7_MASK		= 1 << GPIO3C7_SHIFT,
	GPIO3C7_GPIO		= 0,
	GPIO3C7_CIF_CLKI,

	GPIO3C6_SHIFT		= 12,
	GPIO3C6_MASK		= 1 << GPIO3C6_SHIFT,
	GPIO3C6_GPIO		= 0,
	GPIO3C6_CIF_VSYNC,

	GPIO3C5_SHIFT		= 10,
	GPIO3C5_MASK		= 1 << GPIO3C5_SHIFT,
	GPIO3C5_GPIO		= 0,
	GPIO3C5_SDMMC_CMD,

	GPIO3C4_SHIFT		= 8,
	GPIO3C4_MASK		= 1 << GPIO3C4_SHIFT,
	GPIO3C4_GPIO		= 0,
	GPIO3C4_SDMMC_CLKO,

	GPIO3C3_SHIFT		= 6,
	GPIO3C3_MASK		= 3 << GPIO3C3_SHIFT,
	GPIO3C3_GPIO		= 0,
	GPIO3C3_SDMMC_D0,
	GPIO3C3_UART2_SOUT_M1,

	GPIO3C2_SHIFT		= 4,
	GPIO3C2_MASK		= 3 << GPIO3C2_SHIFT,
	GPIO3C2_GPIO		= 0,
	GPIO3C2_SDMMC_D1,
	GPIO3C2_UART2_SIN_M1,

	GPIOC1_SHIFT		= 2,
	GPIOC1_MASK		= 1 << GPIOC1_SHIFT,
	GPIOC1_GPIO		= 0,
	GPIOC1_SDMMC_D2,

	GPIOC0_SHIFT		= 0,
	GPIOC0_MASK		= 1,
	GPIO3C0_GPIO		= 0,
	GPIO3C0_SDMMC_D3,
};
#endif
