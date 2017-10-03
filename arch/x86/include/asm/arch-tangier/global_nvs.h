/*
 * Copyright (c) 2017 Intel Corporation
 *
 * Partially based on global_nvs.h for other x86 platforms
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _GLOBAL_NVS_H_
#define _GLOBAL_NVS_H_

struct __packed acpi_global_nvs {
	u8	pcnt;		/* processor count */

	/*
	 * Add padding so sizeof(struct acpi_global_nvs) == 0x100.
	 * This must match the size defined in the global_nvs.asl.
	 */
	u8	rsvd[255];
};

#endif /* _GLOBAL_NVS_H_ */
