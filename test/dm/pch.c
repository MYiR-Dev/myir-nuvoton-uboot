// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2018 Google LLC
 */

#include <common.h>
#include <dm.h>
#include <pch.h>
#include <asm/test.h>
#include <dm/test.h>
#include <test/ut.h>

/* Test that sandbox PCH works correctly */
static int dm_test_pch_base(struct unit_test_state *uts)
{
	struct udevice *dev;
	u32 gbase, iobase;
	ulong sbase;

	ut_assertok(uclass_first_device_err(UCLASS_PCH, &dev));
	ut_assertok(pch_get_spi_base(dev, &sbase));
	ut_asserteq(0x10, sbase);

	ut_asserteq(0, sandbox_get_pch_spi_protect(dev));
	ut_assertok(pch_set_spi_protect(dev, true));
	ut_asserteq(1, sandbox_get_pch_spi_protect(dev));

	ut_assertok(pch_get_gpio_base(dev, &gbase));
	ut_asserteq(0x20, gbase);

	ut_assertok(pch_get_io_base(dev, &iobase));
	ut_asserteq(0x30, iobase);

	return 0;
}
DM_TEST(dm_test_pch_base, DM_TESTF_SCAN_PDATA | DM_TESTF_SCAN_FDT);
