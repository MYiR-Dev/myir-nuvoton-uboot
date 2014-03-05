/*
 * Copyright 2013 Broadcom Corporation.
 *
 * SPDX-License-Identifier:      GPL-2.0+
 */

#include <common.h>

/*
 * These weak functions are available to kona architectures that don't
 * require clock enables from the driver code.
 */
int __weak clk_sdio_enable(void *base, u32 rate, u32 *actual_ratep)
{
	return 0;
}

int __weak clk_bsc_enable(void *base, u32 rate, u32 *actual_ratep)
{
	return 0;
}
