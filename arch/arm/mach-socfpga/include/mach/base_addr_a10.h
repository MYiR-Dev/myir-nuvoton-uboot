/*
 * Copyright (C) 2014-2017 Altera Corporation <www.altera.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _SOCFPGA_A10_BASE_HARDWARE_H_
#define _SOCFPGA_A10_BASE_HARDWARE_H_

#define SOCFPGA_EMAC0_ADDRESS			0xff800000
#define SOCFPGA_EMAC1_ADDRESS			0xff802000
#define SOCFPGA_EMAC2_ADDRESS			0xff804000
#define SOCFPGA_SDMMC_ADDRESS			0xff808000
#define SOCFPGA_QSPIREGS_ADDRESS		0xff809000
#define SOCFPGA_QSPIDATA_ADDRESS		0xffa00000
#define SOCFPGA_UART1_ADDRESS			0xffc02100
#define SOCFPGA_HMC_MMR_IO48_ADDRESS		0xffcfa000
#define SOCFPGA_FPGAMGRDATA_ADDRESS		0xffcfe400
#define SOCFPGA_FPGAMGRREGS_ADDRESS		0xffd03000
#define SOCFPGA_L4WD0_ADDRESS			0xffd00200
#define SOCFPGA_SYSMGR_ADDRESS			0xffd06000
#define SOCFPGA_PINMUX_SHARED_3V_IO_ADDRESS	0xffd07000
#define SOCFPGA_PINMUX_DEDICATED_IO_ADDRESS	0xffd07200
#define SOCFPGA_PINMUX_DEDICATED_IO_CFG_ADDRESS	0xffd07300
#define SOCFPGA_PINMUX_FPGA_INTERFACE_ADDRESS	0xffd07400
#define SOCFPGA_DMANONSECURE_ADDRESS		0xffda0000
#define SOCFPGA_DMASECURE_ADDRESS		0xffda1000
#define SOCFPGA_MPUSCU_ADDRESS			0xffffc000
#define SOCFPGA_MPUL2_ADDRESS			0xfffff000
#define SOCFPGA_I2C0_ADDRESS			0xffc02200
#define SOCFPGA_I2C1_ADDRESS			0xffc02300
#define SOCFPGA_I2C2_ADDRESS			0xffc02400
#define SOCFPGA_I2C3_ADDRESS			0xffc02500
#define SOCFPGA_I2C4_ADDRESS			0xffc02600

#define SOCFPGA_ECC_OCRAM_ADDRESS		0xff8c3000
#define SOCFPGA_UART0_ADDRESS			0xffc02000
#define SOCFPGA_OSC1TIMER0_ADDRESS		0xffd00000
#define SOCFPGA_OSC1TIMER1_ADDRESS		0xffd00100
#define SOCFPGA_CLKMGR_ADDRESS			0xffd04000
#define SOCFPGA_RSTMGR_ADDRESS			0xffd05000

#define SOCFPGA_SDR_ADDRESS			0xffcfb000
#define SOCFPGA_NOC_L4_PRIV_FLT_OFST		0xffd11000
#define SOCFPGA_SDR_SCHEDULER_ADDRESS		0xffd12400
#define SOCFPGA_SDR_FIREWALL_OCRAM_ADDRESS	0xffd13200
#define SOCFPGA_SDR_FIREWALL_MPU_FPGA_ADDRESS	0xffd13300
#define SOCFPGA_SDR_FIREWALL_L3_ADDRESS		0xffd13400
#define SOCFPGA_NOC_FW_H2F_SCR_OFST		0xffd13500

#endif /* _SOCFPGA_A10_BASE_HARDWARE_H_ */
