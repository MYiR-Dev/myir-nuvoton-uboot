/* SPDX-License-Identifier: GPL-2.0 */
/*
 * (C) Copyright 2015 Google, Inc
 * Copyright 2014 Rockchip Inc.
 */

#ifndef _ASM_ARCH_GRF_RK3288_H
#define _ASM_ARCH_GRF_RK3288_H

struct rk3288_grf_gpio_lh {
	u32 l;
	u32 h;
};

struct rk3288_grf {
	u32 reserved[3];
	u32 gpio1d_iomux;
	u32 gpio2a_iomux;
	u32 gpio2b_iomux;

	u32 gpio2c_iomux;
	u32 reserved2;
	u32 gpio3a_iomux;
	u32 gpio3b_iomux;

	u32 gpio3c_iomux;
	u32 gpio3dl_iomux;
	u32 gpio3dh_iomux;
	u32 gpio4al_iomux;

	u32 gpio4ah_iomux;
	u32 gpio4bl_iomux;
	u32 reserved3;
	u32 gpio4c_iomux;

	u32 gpio4d_iomux;
	u32 reserved4;
	u32 gpio5b_iomux;
	u32 gpio5c_iomux;

	u32 reserved5;
	u32 gpio6a_iomux;
	u32 gpio6b_iomux;
	u32 gpio6c_iomux;
	u32 reserved6;
	u32 gpio7a_iomux;
	u32 gpio7b_iomux;
	u32 gpio7cl_iomux;
	u32 gpio7ch_iomux;
	u32 reserved7;
	u32 gpio8a_iomux;
	u32 gpio8b_iomux;
	u32 reserved8[30];
	struct rk3288_grf_gpio_lh gpio_sr[8];
	u32 gpio1_p[8][4];
	u32 gpio1_e[8][4];
	u32 gpio_smt;
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
	u32 soc_con12;
	u32 soc_con13;
	u32 soc_con14;
	u32 soc_status[22];
	u32 reserved9[2];
	u32 peridmac_con[4];
	u32 ddrc0_con0;
	u32 ddrc1_con0;
	u32 cpu_con[5];
	u32 reserved10[3];
	u32 cpu_status0;
	u32 reserved11;
	u32 uoc0_con[5];
	u32 uoc1_con[5];
	u32 uoc2_con[4];
	u32 uoc3_con[2];
	u32 uoc4_con[2];
	u32 pvtm_con[3];
	u32 pvtm_status[3];
	u32 io_vsel;
	u32 saradc_testbit;
	u32 tsadc_testbit_l;
	u32 tsadc_testbit_h;
	u32 os_reg[4];
	u32 reserved12;
	u32 soc_con15;
	u32 soc_con16;
};

struct rk3288_sgrf {
	u32 soc_con0;
	u32 soc_con1;
	u32 soc_con2;
	u32 soc_con3;
	u32 soc_con4;
	u32 soc_con5;
	u32 reserved1[(0x20-0x18)/4];
	u32 busdmac_con[2];
	u32 reserved2[(0x40-0x28)/4];
	u32 cpu_con[3];
	u32 reserved3[(0x50-0x4c)/4];
	u32 soc_con6;
	u32 soc_con7;
	u32 soc_con8;
	u32 soc_con9;
	u32 soc_con10;
	u32 soc_con11;
	u32 soc_con12;
	u32 soc_con13;
	u32 soc_con14;
	u32 soc_con15;
	u32 soc_con16;
	u32 soc_con17;
	u32 soc_con18;
	u32 soc_con19;
	u32 soc_con20;
	u32 soc_con21;
	u32 reserved4[(0x100-0x90)/4];
	u32 soc_status[2];
	u32 reserved5[(0x120-0x108)/4];
	u32 fast_boot_addr;
};

/* GRF_GPIO1D_IOMUX */
enum {
	GPIO1D3_SHIFT		= 6,
	GPIO1D3_MASK		= 1,
	GPIO1D3_GPIO		= 0,
	GPIO1D3_LCDC0_DCLK,

	GPIO1D2_SHIFT		= 4,
	GPIO1D2_MASK		= 1,
	GPIO1D2_GPIO		= 0,
	GPIO1D2_LCDC0_DEN,

	GPIO1D1_SHIFT		= 2,
	GPIO1D1_MASK		= 1,
	GPIO1D1_GPIO		= 0,
	GPIO1D1_LCDC0_VSYNC,

	GPIO1D0_SHIFT		= 0,
	GPIO1D0_MASK		= 1,
	GPIO1D0_GPIO		= 0,
	GPIO1D0_LCDC0_HSYNC,
};

/* GRF_GPIO2C_IOMUX */
enum {
	GPIO2C1_SHIFT		= 2,
	GPIO2C1_MASK		= 1,
	GPIO2C1_GPIO		= 0,
	GPIO2C1_I2C3CAM_SDA,

	GPIO2C0_SHIFT		= 0,
	GPIO2C0_MASK		= 1,
	GPIO2C0_GPIO		= 0,
	GPIO2C0_I2C3CAM_SCL,
};

/* GRF_GPIO3A_IOMUX */
enum {
	GPIO3A7_SHIFT		= 14,
	GPIO3A7_MASK		= 3,
	GPIO3A7_GPIO		= 0,
	GPIO3A7_FLASH0_DATA7,
	GPIO3A7_EMMC_DATA7,

	GPIO3A6_SHIFT		= 12,
	GPIO3A6_MASK		= 3,
	GPIO3A6_GPIO		= 0,
	GPIO3A6_FLASH0_DATA6,
	GPIO3A6_EMMC_DATA6,

	GPIO3A5_SHIFT		= 10,
	GPIO3A5_MASK		= 3,
	GPIO3A5_GPIO		= 0,
	GPIO3A5_FLASH0_DATA5,
	GPIO3A5_EMMC_DATA5,

	GPIO3A4_SHIFT		= 8,
	GPIO3A4_MASK		= 3,
	GPIO3A4_GPIO		= 0,
	GPIO3A4_FLASH0_DATA4,
	GPIO3A4_EMMC_DATA4,

	GPIO3A3_SHIFT		= 6,
	GPIO3A3_MASK		= 3,
	GPIO3A3_GPIO		= 0,
	GPIO3A3_FLASH0_DATA3,
	GPIO3A3_EMMC_DATA3,

	GPIO3A2_SHIFT		= 4,
	GPIO3A2_MASK		= 3,
	GPIO3A2_GPIO		= 0,
	GPIO3A2_FLASH0_DATA2,
	GPIO3A2_EMMC_DATA2,

	GPIO3A1_SHIFT		= 2,
	GPIO3A1_MASK		= 3,
	GPIO3A1_GPIO		= 0,
	GPIO3A1_FLASH0_DATA1,
	GPIO3A1_EMMC_DATA1,

	GPIO3A0_SHIFT		= 0,
	GPIO3A0_MASK		= 3,
	GPIO3A0_GPIO		= 0,
	GPIO3A0_FLASH0_DATA0,
	GPIO3A0_EMMC_DATA0,
};

/* GRF_GPIO3B_IOMUX */
enum {
	GPIO3B7_SHIFT		= 14,
	GPIO3B7_MASK		= 1,
	GPIO3B7_GPIO		= 0,
	GPIO3B7_FLASH0_CSN1,

	GPIO3B6_SHIFT		= 12,
	GPIO3B6_MASK		= 1,
	GPIO3B6_GPIO		= 0,
	GPIO3B6_FLASH0_CSN0,

	GPIO3B5_SHIFT		= 10,
	GPIO3B5_MASK		= 1,
	GPIO3B5_GPIO		= 0,
	GPIO3B5_FLASH0_WRN,

	GPIO3B4_SHIFT		= 8,
	GPIO3B4_MASK		= 1,
	GPIO3B4_GPIO		= 0,
	GPIO3B4_FLASH0_CLE,

	GPIO3B3_SHIFT		= 6,
	GPIO3B3_MASK		= 1,
	GPIO3B3_GPIO		= 0,
	GPIO3B3_FLASH0_ALE,

	GPIO3B2_SHIFT		= 4,
	GPIO3B2_MASK		= 1,
	GPIO3B2_GPIO		= 0,
	GPIO3B2_FLASH0_RDN,

	GPIO3B1_SHIFT		= 2,
	GPIO3B1_MASK		= 3,
	GPIO3B1_GPIO		= 0,
	GPIO3B1_FLASH0_WP,
	GPIO3B1_EMMC_PWREN,

	GPIO3B0_SHIFT		= 0,
	GPIO3B0_MASK		= 1,
	GPIO3B0_GPIO		= 0,
	GPIO3B0_FLASH0_RDY,
};

/* GRF_GPIO3C_IOMUX */
enum {
	GPIO3C2_SHIFT		= 4,
	GPIO3C2_MASK		= 3,
	GPIO3C2_GPIO		= 0,
	GPIO3C2_FLASH0_DQS,
	GPIO3C2_EMMC_CLKOUT,

	GPIO3C1_SHIFT		= 2,
	GPIO3C1_MASK		= 3,
	GPIO3C1_GPIO		= 0,
	GPIO3C1_FLASH0_CSN3,
	GPIO3C1_EMMC_RSTNOUT,

	GPIO3C0_SHIFT		= 0,
	GPIO3C0_MASK		= 3,
	GPIO3C0_GPIO		= 0,
	GPIO3C0_FLASH0_CSN2,
	GPIO3C0_EMMC_CMD,
};

/* GRF_GPIO3DL_IOMUX */
enum {
	GPIO3D3_SHIFT		= 12,
	GPIO3D3_MASK		= 7,
	GPIO3D3_GPIO		= 0,
	GPIO3D3_FLASH1_DATA3,
	GPIO3D3_HOST_DOUT3,
	GPIO3D3_MAC_RXD3,
	GPIO3D3_SDIO1_DATA3,

	GPIO3D2_SHIFT		= 8,
	GPIO3D2_MASK		= 7,
	GPIO3D2_GPIO		= 0,
	GPIO3D2_FLASH1_DATA2,
	GPIO3D2_HOST_DOUT2,
	GPIO3D2_MAC_RXD2,
	GPIO3D2_SDIO1_DATA2,

	GPIO3D1_SHIFT		= 4,
	GPIO3D1_MASK		= 7,
	GPIO3D1_GPIO		= 0,
	GPIO3DL1_FLASH1_DATA1,
	GPIO3D1_HOST_DOUT1,
	GPIO3D1_MAC_TXD3,
	GPIO3D1_SDIO1_DATA1,

	GPIO3D0_SHIFT		= 0,
	GPIO3D0_MASK		= 7,
	GPIO3D0_GPIO		= 0,
	GPIO3D0_FLASH1_DATA0,
	GPIO3D0_HOST_DOUT0,
	GPIO3D0_MAC_TXD2,
	GPIO3D0_SDIO1_DATA0,
};

/* GRF_GPIO3HL_IOMUX */
enum {
	GPIO3D7_SHIFT		= 12,
	GPIO3D7_MASK		= 7,
	GPIO3D7_GPIO		= 0,
	GPIO3D7_FLASH1_DATA7,
	GPIO3D7_HOST_DOUT7,
	GPIO3D7_MAC_RXD1,
	GPIO3D7_SDIO1_INTN,

	GPIO3D6_SHIFT		= 8,
	GPIO3D6_MASK		= 7,
	GPIO3D6_GPIO		= 0,
	GPIO3D6_FLASH1_DATA6,
	GPIO3D6_HOST_DOUT6,
	GPIO3D6_MAC_RXD0,
	GPIO3D6_SDIO1_BKPWR,

	GPIO3D5_SHIFT		= 4,
	GPIO3D5_MASK		= 7,
	GPIO3D5_GPIO		= 0,
	GPIO3D5_FLASH1_DATA5,
	GPIO3D5_HOST_DOUT5,
	GPIO3D5_MAC_TXD1,
	GPIO3D5_SDIO1_WRPRT,

	GPIO3D4_SHIFT		= 0,
	GPIO3D4_MASK		= 7,
	GPIO3D4_GPIO		= 0,
	GPIO3D4_FLASH1_DATA4,
	GPIO3D4_HOST_DOUT4,
	GPIO3D4_MAC_TXD0,
	GPIO3D4_SDIO1_DETECTN,
};

/* GRF_GPIO4AL_IOMUX */
enum {
	GPIO4A3_SHIFT		= 12,
	GPIO4A3_MASK		= 7,
	GPIO4A3_GPIO		= 0,
	GPIO4A3_FLASH1_ALE,
	GPIO4A3_HOST_DOUT9,
	GPIO4A3_MAC_CLK,
	GPIO4A3_FLASH0_CSN6,

	GPIO4A2_SHIFT		= 8,
	GPIO4A2_MASK		= 7,
	GPIO4A2_GPIO		= 0,
	GPIO4A2_FLASH1_RDN,
	GPIO4A2_HOST_DOUT8,
	GPIO4A2_MAC_RXER,
	GPIO4A2_FLASH0_CSN5,

	GPIO4A1_SHIFT		= 4,
	GPIO4A1_MASK		= 7,
	GPIO4A1_GPIO		= 0,
	GPIO4A1_FLASH1_WP,
	GPIO4A1_HOST_CKOUTN,
	GPIO4A1_MAC_TXDV,
	GPIO4A1_FLASH0_CSN4,

	GPIO4A0_SHIFT		= 0,
	GPIO4A0_MASK		= 3,
	GPIO4A0_GPIO		= 0,
	GPIO4A0_FLASH1_RDY,
	GPIO4A0_HOST_CKOUTP,
	GPIO4A0_MAC_MDC,
};

/* GRF_GPIO4AH_IOMUX */
enum {
	GPIO4A7_SHIFT		= 12,
	GPIO4A7_MASK		= 7,
	GPIO4A7_GPIO		= 0,
	GPIO4A7_FLASH1_CSN1,
	GPIO4A7_HOST_DOUT13,
	GPIO4A7_MAC_CSR,
	GPIO4A7_SDIO1_CLKOUT,

	GPIO4A6_SHIFT		= 8,
	GPIO4A6_MASK		= 7,
	GPIO4A6_GPIO		= 0,
	GPIO4A6_FLASH1_CSN0,
	GPIO4A6_HOST_DOUT12,
	GPIO4A6_MAC_RXCLK,
	GPIO4A6_SDIO1_CMD,

	GPIO4A5_SHIFT		= 4,
	GPIO4A5_MASK		= 3,
	GPIO4A5_GPIO		= 0,
	GPIO4A5_FLASH1_WRN,
	GPIO4A5_HOST_DOUT11,
	GPIO4A5_MAC_MDIO,

	GPIO4A4_SHIFT		= 0,
	GPIO4A4_MASK		= 7,
	GPIO4A4_GPIO		= 0,
	GPIO4A4_FLASH1_CLE,
	GPIO4A4_HOST_DOUT10,
	GPIO4A4_MAC_TXEN,
	GPIO4A4_FLASH0_CSN7,
};

/* GRF_GPIO4BL_IOMUX */
enum {
	GPIO4B1_SHIFT		= 4,
	GPIO4B1_MASK		= 7,
	GPIO4B1_GPIO		= 0,
	GPIO4B1_FLASH1_CSN2,
	GPIO4B1_HOST_DOUT15,
	GPIO4B1_MAC_TXCLK,
	GPIO4B1_SDIO1_PWREN,

	GPIO4B0_SHIFT		= 0,
	GPIO4B0_MASK		= 7,
	GPIO4B0_GPIO		= 0,
	GPIO4B0_FLASH1_DQS,
	GPIO4B0_HOST_DOUT14,
	GPIO4B0_MAC_COL,
	GPIO4B0_FLASH1_CSN3,
};

/* GRF_GPIO4C_IOMUX */
enum {
	GPIO4C7_SHIFT		= 14,
	GPIO4C7_MASK		= 1,
	GPIO4C7_GPIO		= 0,
	GPIO4C7_SDIO0_DATA3,

	GPIO4C6_SHIFT		= 12,
	GPIO4C6_MASK		= 1,
	GPIO4C6_GPIO		= 0,
	GPIO4C6_SDIO0_DATA2,

	GPIO4C5_SHIFT		= 10,
	GPIO4C5_MASK		= 1,
	GPIO4C5_GPIO		= 0,
	GPIO4C5_SDIO0_DATA1,

	GPIO4C4_SHIFT		= 8,
	GPIO4C4_MASK		= 1,
	GPIO4C4_GPIO		= 0,
	GPIO4C4_SDIO0_DATA0,

	GPIO4C3_SHIFT		= 6,
	GPIO4C3_MASK		= 1,
	GPIO4C3_GPIO		= 0,
	GPIO4C3_UART0BT_RTSN,

	GPIO4C2_SHIFT		= 4,
	GPIO4C2_MASK		= 1,
	GPIO4C2_GPIO		= 0,
	GPIO4C2_UART0BT_CTSN,

	GPIO4C1_SHIFT		= 2,
	GPIO4C1_MASK		= 1,
	GPIO4C1_GPIO		= 0,
	GPIO4C1_UART0BT_SOUT,

	GPIO4C0_SHIFT		= 0,
	GPIO4C0_MASK		= 1,
	GPIO4C0_GPIO		= 0,
	GPIO4C0_UART0BT_SIN,
};

/* GRF_GPIO5B_IOMUX */
enum {
	GPIO5B7_SHIFT		= 14,
	GPIO5B7_MASK		= 3,
	GPIO5B7_GPIO		= 0,
	GPIO5B7_SPI0_RXD,
	GPIO5B7_TS0_DATA7,
	GPIO5B7_UART4EXP_SIN,

	GPIO5B6_SHIFT		= 12,
	GPIO5B6_MASK		= 3,
	GPIO5B6_GPIO		= 0,
	GPIO5B6_SPI0_TXD,
	GPIO5B6_TS0_DATA6,
	GPIO5B6_UART4EXP_SOUT,

	GPIO5B5_SHIFT		= 10,
	GPIO5B5_MASK		= 3,
	GPIO5B5_GPIO		= 0,
	GPIO5B5_SPI0_CSN0,
	GPIO5B5_TS0_DATA5,
	GPIO5B5_UART4EXP_RTSN,

	GPIO5B4_SHIFT		= 8,
	GPIO5B4_MASK		= 3,
	GPIO5B4_GPIO		= 0,
	GPIO5B4_SPI0_CLK,
	GPIO5B4_TS0_DATA4,
	GPIO5B4_UART4EXP_CTSN,

	GPIO5B3_SHIFT		= 6,
	GPIO5B3_MASK		= 3,
	GPIO5B3_GPIO		= 0,
	GPIO5B3_UART1BB_RTSN,
	GPIO5B3_TS0_DATA3,

	GPIO5B2_SHIFT		= 4,
	GPIO5B2_MASK		= 3,
	GPIO5B2_GPIO		= 0,
	GPIO5B2_UART1BB_CTSN,
	GPIO5B2_TS0_DATA2,

	GPIO5B1_SHIFT		= 2,
	GPIO5B1_MASK		= 3,
	GPIO5B1_GPIO		= 0,
	GPIO5B1_UART1BB_SOUT,
	GPIO5B1_TS0_DATA1,

	GPIO5B0_SHIFT		= 0,
	GPIO5B0_MASK		= 3,
	GPIO5B0_GPIO		= 0,
	GPIO5B0_UART1BB_SIN,
	GPIO5B0_TS0_DATA0,
};

/* GRF_GPIO5C_IOMUX */
enum {
	GPIO5C3_SHIFT		= 6,
	GPIO5C3_MASK		= 1,
	GPIO5C3_GPIO		= 0,
	GPIO5C3_TS0_ERR,

	GPIO5C2_SHIFT		= 4,
	GPIO5C2_MASK		= 1,
	GPIO5C2_GPIO		= 0,
	GPIO5C2_TS0_CLK,

	GPIO5C1_SHIFT		= 2,
	GPIO5C1_MASK		= 1,
	GPIO5C1_GPIO		= 0,
	GPIO5C1_TS0_VALID,

	GPIO5C0_SHIFT		= 0,
	GPIO5C0_MASK		= 3,
	GPIO5C0_GPIO		= 0,
	GPIO5C0_SPI0_CSN1,
	GPIO5C0_TS0_SYNC,
};

/* GRF_GPIO6A_IOMUX */
enum {
	GPIO6A7_SHIFT		= 0xe,
	GPIO6A7_MASK		= 1,
	GPIO6A7_GPIO		= 0,
	GPIO6A7_I2S_SDO3,

	GPIO6A6_SHIFT		= 0xc,
	GPIO6A6_MASK		= 1,
	GPIO6A6_GPIO		= 0,
	GPIO6A6_I2S_SDO2,

	GPIO6A5_SHIFT		= 0xa,
	GPIO6A5_MASK		= 1,
	GPIO6A5_GPIO		= 0,
	GPIO6A5_I2S_SDO1,

	GPIO6A4_SHIFT		= 8,
	GPIO6A4_MASK		= 1,
	GPIO6A4_GPIO		= 0,
	GPIO6A4_I2S_SDO0,

	GPIO6A3_SHIFT		= 6,
	GPIO6A3_MASK		= 1,
	GPIO6A3_GPIO		= 0,
	GPIO6A3_I2S_SDI,

	GPIO6A2_SHIFT		= 4,
	GPIO6A2_MASK		= 1,
	GPIO6A2_GPIO		= 0,
	GPIO6A2_I2S_LRCKTX,

	GPIO6A1_SHIFT		= 2,
	GPIO6A1_MASK		= 1,
	GPIO6A1_GPIO		= 0,
	GPIO6A1_I2S_LRCKRX,

	GPIO6A0_SHIFT		= 0,
	GPIO6A0_MASK		= 1,
	GPIO6A0_GPIO		= 0,
	GPIO6A0_I2S_SCLK,
};

/* GRF_GPIO6B_IOMUX */
enum {
	GPIO6B3_SHIFT		= 6,
	GPIO6B3_MASK		= 1,
	GPIO6B3_GPIO		= 0,
	GPIO6B3_SPDIF_TX,

	GPIO6B2_SHIFT		= 4,
	GPIO6B2_MASK		= 1,
	GPIO6B2_GPIO		= 0,
	GPIO6B2_I2C1AUDIO_SCL,

	GPIO6B1_SHIFT		= 2,
	GPIO6B1_MASK		= 1,
	GPIO6B1_GPIO		= 0,
	GPIO6B1_I2C1AUDIO_SDA,

	GPIO6B0_SHIFT		= 0,
	GPIO6B0_MASK		= 1,
	GPIO6B0_GPIO		= 0,
	GPIO6B0_I2S_CLK,
};

/* GRF_GPIO6C_IOMUX */
enum {
	GPIO6C6_SHIFT		= 12,
	GPIO6C6_MASK		= 1,
	GPIO6C6_GPIO		= 0,
	GPIO6C6_SDMMC0_DECTN,

	GPIO6C5_SHIFT		= 10,
	GPIO6C5_MASK		= 1,
	GPIO6C5_GPIO		= 0,
	GPIO6C5_SDMMC0_CMD,

	GPIO6C4_SHIFT		= 8,
	GPIO6C4_MASK		= 3,
	GPIO6C4_GPIO		= 0,
	GPIO6C4_SDMMC0_CLKOUT,
	GPIO6C4_JTAG_TDO,

	GPIO6C3_SHIFT		= 6,
	GPIO6C3_MASK		= 3,
	GPIO6C3_GPIO		= 0,
	GPIO6C3_SDMMC0_DATA3,
	GPIO6C3_JTAG_TCK,

	GPIO6C2_SHIFT		= 4,
	GPIO6C2_MASK		= 3,
	GPIO6C2_GPIO		= 0,
	GPIO6C2_SDMMC0_DATA2,
	GPIO6C2_JTAG_TDI,

	GPIO6C1_SHIFT		= 2,
	GPIO6C1_MASK		= 3,
	GPIO6C1_GPIO		= 0,
	GPIO6C1_SDMMC0_DATA1,
	GPIO6C1_JTAG_TRSTN,

	GPIO6C0_SHIFT		= 0,
	GPIO6C0_MASK		= 3,
	GPIO6C0_GPIO		= 0,
	GPIO6C0_SDMMC0_DATA0,
	GPIO6C0_JTAG_TMS,
};

/* GRF_GPIO7A_IOMUX */
enum {
	GPIO7A7_SHIFT		= 14,
	GPIO7A7_MASK		= 3,
	GPIO7A7_GPIO		= 0,
	GPIO7A7_UART3GPS_SIN,
	GPIO7A7_GPS_MAG,
	GPIO7A7_HSADCT1_DATA0,

	GPIO7A1_SHIFT		= 2,
	GPIO7A1_MASK		= 1,
	GPIO7A1_GPIO		= 0,
	GPIO7A1_PWM_1,

	GPIO7A0_SHIFT		= 0,
	GPIO7A0_MASK		= 3,
	GPIO7A0_GPIO		= 0,
	GPIO7A0_PWM_0,
	GPIO7A0_VOP0_PWM,
	GPIO7A0_VOP1_PWM,
};

/* GRF_GPIO7B_IOMUX */
enum {
	GPIO7B7_SHIFT		= 14,
	GPIO7B7_MASK		= 3,
	GPIO7B7_GPIO		= 0,
	GPIO7B7_ISP_SHUTTERTRIG,
	GPIO7B7_SPI1_TXD,

	GPIO7B6_SHIFT		= 12,
	GPIO7B6_MASK		= 3,
	GPIO7B6_GPIO		= 0,
	GPIO7B6_ISP_PRELIGHTTRIG,
	GPIO7B6_SPI1_RXD,

	GPIO7B5_SHIFT		= 10,
	GPIO7B5_MASK		= 3,
	GPIO7B5_GPIO		= 0,
	GPIO7B5_ISP_FLASHTRIGOUT,
	GPIO7B5_SPI1_CSN0,

	GPIO7B4_SHIFT		= 8,
	GPIO7B4_MASK		= 3,
	GPIO7B4_GPIO		= 0,
	GPIO7B4_ISP_SHUTTEREN,
	GPIO7B4_SPI1_CLK,

	GPIO7B3_SHIFT		= 6,
	GPIO7B3_MASK		= 3,
	GPIO7B3_GPIO		= 0,
	GPIO7B3_USB_DRVVBUS1,
	GPIO7B3_EDP_HOTPLUG,

	GPIO7B2_SHIFT		= 4,
	GPIO7B2_MASK		= 3,
	GPIO7B2_GPIO		= 0,
	GPIO7B2_UART3GPS_RTSN,
	GPIO7B2_USB_DRVVBUS0,

	GPIO7B1_SHIFT		= 2,
	GPIO7B1_MASK		= 3,
	GPIO7B1_GPIO		= 0,
	GPIO7B1_UART3GPS_CTSN,
	GPIO7B1_GPS_RFCLK,
	GPIO7B1_GPST1_CLK,

	GPIO7B0_SHIFT		= 0,
	GPIO7B0_MASK		= 3,
	GPIO7B0_GPIO		= 0,
	GPIO7B0_UART3GPS_SOUT,
	GPIO7B0_GPS_SIG,
	GPIO7B0_HSADCT1_DATA1,
};

/* GRF_GPIO7CL_IOMUX */
enum {
	GPIO7C3_SHIFT		= 12,
	GPIO7C3_MASK		= 3,
	GPIO7C3_GPIO		= 0,
	GPIO7C3_I2C5HDMI_SDA,
	GPIO7C3_EDPHDMII2C_SDA,

	GPIO7C2_SHIFT		= 8,
	GPIO7C2_MASK		= 1,
	GPIO7C2_GPIO		= 0,
	GPIO7C2_I2C4TP_SCL,

	GPIO7C1_SHIFT		= 4,
	GPIO7C1_MASK		= 1,
	GPIO7C1_GPIO		= 0,
	GPIO7C1_I2C4TP_SDA,

	GPIO7C0_SHIFT		= 0,
	GPIO7C0_MASK		= 3,
	GPIO7C0_GPIO		= 0,
	GPIO7C0_ISP_FLASHTRIGIN,
	GPIO7C0_EDPHDMI_CECINOUTT1,
};

/* GRF_GPIO7CH_IOMUX */
enum {
	GPIO7C7_SHIFT		= 12,
	GPIO7C7_MASK		= 7,
	GPIO7C7_GPIO		= 0,
	GPIO7C7_UART2DBG_SOUT,
	GPIO7C7_UART2DBG_SIROUT,
	GPIO7C7_PWM_3,
	GPIO7C7_EDPHDMI_CECINOUT,

	GPIO7C6_SHIFT		= 8,
	GPIO7C6_MASK		= 3,
	GPIO7C6_GPIO		= 0,
	GPIO7C6_UART2DBG_SIN,
	GPIO7C6_UART2DBG_SIRIN,
	GPIO7C6_PWM_2,

	GPIO7C4_SHIFT		= 0,
	GPIO7C4_MASK		= 3,
	GPIO7C4_GPIO		= 0,
	GPIO7C4_I2C5HDMI_SCL,
	GPIO7C4_EDPHDMII2C_SCL,
};

/* GRF_GPIO8A_IOMUX */
enum {
	GPIO8A7_SHIFT		= 14,
	GPIO8A7_MASK		= 3,
	GPIO8A7_GPIO		= 0,
	GPIO8A7_SPI2_CSN0,
	GPIO8A7_SC_DETECT,
	GPIO8A7_RESERVE,

	GPIO8A6_SHIFT		= 12,
	GPIO8A6_MASK		= 3,
	GPIO8A6_GPIO		= 0,
	GPIO8A6_SPI2_CLK,
	GPIO8A6_SC_IO,
	GPIO8A6_RESERVE,

	GPIO8A5_SHIFT		= 10,
	GPIO8A5_MASK		= 3,
	GPIO8A5_GPIO		= 0,
	GPIO8A5_I2C2SENSOR_SCL,
	GPIO8A5_SC_CLK,

	GPIO8A4_SHIFT		= 8,
	GPIO8A4_MASK		= 3,
	GPIO8A4_GPIO		= 0,
	GPIO8A4_I2C2SENSOR_SDA,
	GPIO8A4_SC_RST,

	GPIO8A3_SHIFT		= 6,
	GPIO8A3_MASK		= 3,
	GPIO8A3_GPIO		= 0,
	GPIO8A3_SPI2_CSN1,
	GPIO8A3_SC_IOT1,

	GPIO8A2_SHIFT		= 4,
	GPIO8A2_MASK		= 1,
	GPIO8A2_GPIO		= 0,
	GPIO8A2_SC_DETECTT1,

	GPIO8A1_SHIFT		= 2,
	GPIO8A1_MASK		= 3,
	GPIO8A1_GPIO		= 0,
	GPIO8A1_PS2_DATA,
	GPIO8A1_SC_VCC33V,

	GPIO8A0_SHIFT		= 0,
	GPIO8A0_MASK		= 3,
	GPIO8A0_GPIO		= 0,
	GPIO8A0_PS2_CLK,
	GPIO8A0_SC_VCC18V,
};

/* GRF_GPIO8B_IOMUX */
enum {
	GPIO8B1_SHIFT		= 2,
	GPIO8B1_MASK		= 3,
	GPIO8B1_GPIO		= 0,
	GPIO8B1_SPI2_TXD,
	GPIO8B1_SC_CLK,

	GPIO8B0_SHIFT		= 0,
	GPIO8B0_MASK		= 3,
	GPIO8B0_GPIO		= 0,
	GPIO8B0_SPI2_RXD,
	GPIO8B0_SC_RST,
};

/* GRF_SOC_CON0 */
enum {
	PAUSE_MMC_PERI_SHIFT	= 0xf,
	PAUSE_MMC_PERI_MASK	= 1,

	PAUSE_EMEM_PERI_SHIFT	= 0xe,
	PAUSE_EMEM_PERI_MASK	= 1,

	PAUSE_USB_PERI_SHIFT	= 0xd,
	PAUSE_USB_PERI_MASK	= 1,

	GRF_FORCE_JTAG_SHIFT	= 0xc,
	GRF_FORCE_JTAG_MASK	= 1,

	GRF_CORE_IDLE_REQ_MODE_SEL1_SHIFT = 0xb,
	GRF_CORE_IDLE_REQ_MODE_SEL1_MASK = 1,

	GRF_CORE_IDLE_REQ_MODE_SEL0_SHIFT = 0xa,
	GRF_CORE_IDLE_REQ_MODE_SEL0_MASK = 1,

	DDR1_16BIT_EN_SHIFT	= 9,
	DDR1_16BIT_EN_MASK	= 1,

	DDR0_16BIT_EN_SHIFT	= 8,
	DDR0_16BIT_EN_MASK	= 1,

	VCODEC_SHIFT		= 7,
	VCODEC_MASK		= 1,
	VCODEC_SELECT_VEPU_ACLK	= 0,
	VCODEC_SELECT_VDPU_ACLK,

	UPCTL1_C_ACTIVE_IN_SHIFT = 6,
	UPCTL1_C_ACTIVE_IN_MASK	= 1,
	UPCTL1_C_ACTIVE_IN_MAY	= 0,
	UPCTL1_C_ACTIVE_IN_WILL,

	UPCTL0_C_ACTIVE_IN_SHIFT = 5,
	UPCTL0_C_ACTIVE_IN_MASK	= 1,
	UPCTL0_C_ACTIVE_IN_MAY	= 0,
	UPCTL0_C_ACTIVE_IN_WILL,

	MSCH1_MAINDDR3_SHIFT	= 4,
	MSCH1_MAINDDR3_MASK	= 1,
	MSCH1_MAINDDR3_DDR3	= 1,

	MSCH0_MAINDDR3_SHIFT	= 3,
	MSCH0_MAINDDR3_MASK	= 1,
	MSCH0_MAINDDR3_DDR3	= 1,

	MSCH1_MAINPARTIALPOP_SHIFT = 2,
	MSCH1_MAINPARTIALPOP_MASK = 1,

	MSCH0_MAINPARTIALPOP_SHIFT = 1,
	MSCH0_MAINPARTIALPOP_MASK = 1,
};

/* GRF_SOC_CON1 */
enum {
	RK3288_RMII_MODE_SHIFT = 14,
	RK3288_RMII_MODE_MASK  = (1 << RK3288_RMII_MODE_SHIFT),
	RK3288_RMII_MODE       = (1 << RK3288_RMII_MODE_SHIFT),

	RK3288_GMAC_CLK_SEL_SHIFT = 12,
	RK3288_GMAC_CLK_SEL_MASK  = (3 << RK3288_GMAC_CLK_SEL_SHIFT),
	RK3288_GMAC_CLK_SEL_125M  = (0 << RK3288_GMAC_CLK_SEL_SHIFT),
	RK3288_GMAC_CLK_SEL_25M   = (3 << RK3288_GMAC_CLK_SEL_SHIFT),
	RK3288_GMAC_CLK_SEL_2_5M  = (2 << RK3288_GMAC_CLK_SEL_SHIFT),

	RK3288_RMII_CLK_SEL_SHIFT = 11,
	RK3288_RMII_CLK_SEL_MASK  = (1 << RK3288_RMII_CLK_SEL_SHIFT),
	RK3288_RMII_CLK_SEL_2_5M  = (0 << RK3288_RMII_CLK_SEL_SHIFT),
	RK3288_RMII_CLK_SEL_25M   = (1 << RK3288_RMII_CLK_SEL_SHIFT),

	GMAC_SPEED_SHIFT	= 0xa,
	GMAC_SPEED_MASK		= 1,
	GMAC_SPEED_10M		= 0,
	GMAC_SPEED_100M,

	GMAC_FLOWCTRL_SHIFT	= 0x9,
	GMAC_FLOWCTRL_MASK	= 1,

	RK3288_GMAC_PHY_INTF_SEL_SHIFT = 6,
	RK3288_GMAC_PHY_INTF_SEL_MASK  = (7 << RK3288_GMAC_PHY_INTF_SEL_SHIFT),
	RK3288_GMAC_PHY_INTF_SEL_RGMII = (1 << RK3288_GMAC_PHY_INTF_SEL_SHIFT),
	RK3288_GMAC_PHY_INTF_SEL_RMII  = (4 << RK3288_GMAC_PHY_INTF_SEL_SHIFT),

	HOST_REMAP_SHIFT	= 0x5,
	HOST_REMAP_MASK		= 1
};

/* GRF_SOC_CON2 */
enum {
	UPCTL1_LPDDR3_ODT_EN_SHIFT = 0xd,
	UPCTL1_LPDDR3_ODT_EN_MASK = 1,
	UPCTL1_LPDDR3_ODT_EN_ODT = 1,

	UPCTL1_BST_DIABLE_SHIFT	= 0xc,
	UPCTL1_BST_DIABLE_MASK	= 1,
	UPCTL1_BST_DIABLE_DISABLE = 1,

	LPDDR3_EN1_SHIFT	= 0xb,
	LPDDR3_EN1_MASK		= 1,
	LPDDR3_EN1_LPDDR3	= 1,

	UPCTL0_LPDDR3_ODT_EN_SHIFT = 0xa,
	UPCTL0_LPDDR3_ODT_EN_MASK = 1,
	UPCTL0_LPDDR3_ODT_EN_ODT_ENABLE = 1,

	UPCTL0_BST_DIABLE_SHIFT	= 9,
	UPCTL0_BST_DIABLE_MASK	= 1,
	UPCTL0_BST_DIABLE_DISABLE = 1,

	LPDDR3_EN0_SHIFT	= 8,
	LPDDR3_EN0_MASK		= 1,
	LPDDR3_EN0_LPDDR3	= 1,

	GRF_POC_FLASH0_CTRL_SHIFT = 7,
	GRF_POC_FLASH0_CTRL_MASK = 1,
	GRF_POC_FLASH0_CTRL_GPIO3C_3 = 0,
	GRF_POC_FLASH0_CTRL_GRF_IO_VSEL,

	SIMCARD_MUX_SHIFT	= 6,
	SIMCARD_MUX_MASK	= 1,
	SIMCARD_MUX_USE_A	= 1,
	SIMCARD_MUX_USE_B	= 0,

	GRF_SPDIF_2CH_EN_SHIFT	= 1,
	GRF_SPDIF_2CH_EN_MASK	= 1,
	GRF_SPDIF_2CH_EN_8CH	= 0,
	GRF_SPDIF_2CH_EN_2CH,

	PWM_SHIFT		= 0,
	PWM_MASK		= 1,
	PWM_RK			= 1,
	PWM_PWM			= 0,
};

/* GRF_SOC_CON3 */
enum {
	RK3288_RXCLK_DLY_ENA_GMAC_SHIFT = 0xf,
	RK3288_RXCLK_DLY_ENA_GMAC_MASK =
		(1 << RK3288_RXCLK_DLY_ENA_GMAC_SHIFT),
	RK3288_RXCLK_DLY_ENA_GMAC_DISABLE = 0,
	RK3288_RXCLK_DLY_ENA_GMAC_ENABLE =
		(1 << RK3288_RXCLK_DLY_ENA_GMAC_SHIFT),

	RK3288_TXCLK_DLY_ENA_GMAC_SHIFT = 0xe,
	RK3288_TXCLK_DLY_ENA_GMAC_MASK =
		(1 << RK3288_TXCLK_DLY_ENA_GMAC_SHIFT),
	RK3288_TXCLK_DLY_ENA_GMAC_DISABLE = 0,
	RK3288_TXCLK_DLY_ENA_GMAC_ENABLE =
		(1 << RK3288_TXCLK_DLY_ENA_GMAC_SHIFT),

	RK3288_CLK_RX_DL_CFG_GMAC_SHIFT = 0x7,
	RK3288_CLK_RX_DL_CFG_GMAC_MASK =
		(0x7f << RK3288_CLK_RX_DL_CFG_GMAC_SHIFT),

	RK3288_CLK_TX_DL_CFG_GMAC_SHIFT = 0x0,
	RK3288_CLK_TX_DL_CFG_GMAC_MASK =
		(0x7f << RK3288_CLK_TX_DL_CFG_GMAC_SHIFT),
};

/* GRF_SOC_CON6 */
enum GRF_SOC_CON6 {
	RK3288_HDMI_EDP_SEL_SHIFT = 0xf,
	RK3288_HDMI_EDP_SEL_MASK =
		1 << RK3288_HDMI_EDP_SEL_SHIFT,
	RK3288_HDMI_EDP_SEL_EDP = 0,
	RK3288_HDMI_EDP_SEL_HDMI,

	RK3288_DSI0_DPICOLORM_SHIFT = 0x8,
	RK3288_DSI0_DPICOLORM_MASK =
		1 << RK3288_DSI0_DPICOLORM_SHIFT,

	RK3288_DSI0_DPISHUTDN_SHIFT = 0x7,
	RK3288_DSI0_DPISHUTDN_MASK =
		1 << RK3288_DSI0_DPISHUTDN_SHIFT,

	RK3288_DSI0_LCDC_SEL_SHIFT = 0x6,
	RK3288_DSI0_LCDC_SEL_MASK =
		1 << RK3288_DSI0_LCDC_SEL_SHIFT,
	RK3288_DSI0_LCDC_SEL_BIG = 0,
	RK3288_DSI0_LCDC_SEL_LIT = 1,

	RK3288_EDP_LCDC_SEL_SHIFT = 0x5,
	RK3288_EDP_LCDC_SEL_MASK =
		1 << RK3288_EDP_LCDC_SEL_SHIFT,
	RK3288_EDP_LCDC_SEL_BIG = 0,
	RK3288_EDP_LCDC_SEL_LIT = 1,

	RK3288_HDMI_LCDC_SEL_SHIFT = 0x4,
	RK3288_HDMI_LCDC_SEL_MASK =
		1 << RK3288_HDMI_LCDC_SEL_SHIFT,
	RK3288_HDMI_LCDC_SEL_BIG = 0,
	RK3288_HDMI_LCDC_SEL_LIT = 1,

	RK3288_LVDS_LCDC_SEL_SHIFT = 0x3,
	RK3288_LVDS_LCDC_SEL_MASK =
		1 << RK3288_LVDS_LCDC_SEL_SHIFT,
	RK3288_LVDS_LCDC_SEL_BIG = 0,
	RK3288_LVDS_LCDC_SEL_LIT = 1,
};

/* RK3288_SOC_CON8 */
enum GRF_SOC_CON8 {
	RK3288_DPHY_TX0_RXMODE_SHIFT = 4,
	RK3288_DPHY_TX0_RXMODE_MASK =
	   0xf << RK3288_DPHY_TX0_RXMODE_SHIFT,
	RK3288_DPHY_TX0_RXMODE_EN = 0xf,
	RK3288_DPHY_TX0_RXMODE_DIS = 0,

	RK3288_DPHY_TX0_TXSTOPMODE_SHIFT = 0x8,
	RK3288_DPHY_TX0_TXSTOPMODE_MASK =
	   0xf << RK3288_DPHY_TX0_TXSTOPMODE_SHIFT,
	RK3288_DPHY_TX0_TXSTOPMODE_EN = 0xf,
	RK3288_DPHY_TX0_TXSTOPMODE_DIS = 0,

	RK3288_DPHY_TX0_TURNREQUEST_SHIFT = 0,
	RK3288_DPHY_TX0_TURNREQUEST_MASK =
	   0xf << RK3288_DPHY_TX0_TURNREQUEST_SHIFT,
	RK3288_DPHY_TX0_TURNREQUEST_EN = 0xf,
	RK3288_DPHY_TX0_TURNREQUEST_DIS = 0,
};

/* GRF_IO_VSEL */
enum {
	GPIO1830_V18SEL_SHIFT		= 9,
	GPIO1830_V18SEL_MASK		= 1,
	GPIO1830_V18SEL_3_3V		= 0,
	GPIO1830_V18SEL_1_8V,

	GPIO30_V18SEL_SHIFT	= 8,
	GPIO30_V18SEL_MASK	= 1,
	GPIO30_V18SEL_3_3V	= 0,
	GPIO30_V18SEL_1_8V,

	SDCARD_V18SEL_SHIFT	= 7,
	SDCARD_V18SEL_MASK	= 1,
	SDCARD_V18SEL_3_3V	= 0,
	SDCARD_V18SEL_1_8V,

	AUDIO_V18SEL_SHIFT	= 6,
	AUDIO_V18SEL_MASK	= 1,
	AUDIO_V18SEL_3_3V	= 0,
	AUDIO_V18SEL_1_8V,

	BB_V18SEL_SHIFT		= 5,
	BB_V18SEL_MASK		= 1,
	BB_V18SEL_3_3V		= 0,
	BB_V18SEL_1_8V,

	WIFI_V18SEL_SHIFT	= 4,
	WIFI_V18SEL_MASK	= 1,
	WIFI_V18SEL_3_3V	= 0,
	WIFI_V18SEL_1_8V,

	FLASH1_V18SEL_SHIFT	= 3,
	FLASH1_V18SEL_MASK	= 1,
	FLASH1_V18SEL_3_3V	= 0,
	FLASH1_V18SEL_1_8V,

	FLASH0_V18SEL_SHIFT	= 2,
	FLASH0_V18SEL_MASK	= 1,
	FLASH0_V18SEL_3_3V	= 0,
	FLASH0_V18SEL_1_8V,

	DVP_V18SEL_SHIFT	= 1,
	DVP_V18SEL_MASK		= 1,
	DVP_V18SEL_3_3V		= 0,
	DVP_V18SEL_1_8V,

	LCDC_V18SEL_SHIFT	= 0,
	LCDC_V18SEL_MASK	= 1,
	LCDC_V18SEL_3_3V	= 0,
	LCDC_V18SEL_1_8V,
};

/* GPIO Bias settings */
enum GPIO_BIAS {
	GPIO_BIAS_2MA = 0,
	GPIO_BIAS_4MA,
	GPIO_BIAS_8MA,
	GPIO_BIAS_12MA,
};

#define GPIO_BIAS_MASK	0x3
#define GPIO_BIAS_SHIFT(x)  ((x) * 2)

#define GPIO_PULL_MASK	0x3
#define GPIO_PULL_SHIFT(x)  ((x) * 2)

#endif
