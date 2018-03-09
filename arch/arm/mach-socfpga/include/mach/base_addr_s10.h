/*
 * Copyright (C) 2016-2017 Intel Corporation <www.intel.com>
 *
 * SPDX-License-Identifier:	GPL-2.0
 */

#ifndef _SOCFPGA_S10_BASE_HARDWARE_H_
#define _SOCFPGA_S10_BASE_HARDWARE_H_

#define SOCFPGA_SDR_SCHEDULER_ADDRESS		0xf8000400
#define SOCFPGA_HMC_MMR_IO48_ADDRESS		0xf8010000
#define SOCFPGA_SDR_ADDRESS			0xf8011000
#define SOCFPGA_SMMU_ADDRESS			0xfa000000
#define SOCFPGA_MAILBOX_ADDRESS			0xffa30000
#define SOCFPGA_UART0_ADDRESS			0xffc02000
#define SOCFPGA_UART1_ADDRESS			0xffc02100
#define SOCFPGA_SPTIMER0_ADDRESS		0xffc03000
#define SOCFPGA_SPTIMER1_ADDRESS		0xffc03100
#define SOCFPGA_SYSTIMER0_ADDRESS		0xffd00000
#define SOCFPGA_SYSTIMER1_ADDRESS		0xffd00100
#define SOCFPGA_GTIMER_SEC_ADDRESS		0xffd01000
#define SOCFPGA_GTIMER_NSEC_ADDRESS		0xffd02000
#define SOCFPGA_CLKMGR_ADDRESS			0xffd10000
#define SOCFPGA_RSTMGR_ADDRESS			0xffd11000
#define SOCFPGA_SYSMGR_ADDRESS			0xffd12000
#define SOCFPGA_PINMUX_DEDICATED_IO_ADDRESS	0xffd13000
#define SOCFPGA_DMANONSECURE_ADDRESS		0xffda0000
#define SOCFPGA_DMASECURE_ADDRESS		0xffda1000
#define SOCFPGA_OCRAM_ADDRESS			0xffe00000
#define GICD_BASE				0xfffc1000
#define GICC_BASE				0xfffc2000

#endif /* _SOCFPGA_S10_BASE_HARDWARE_H_ */
