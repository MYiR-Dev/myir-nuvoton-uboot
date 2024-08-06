// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020 Nuvoton Technology Corporation.
 */

#include <common.h>
#include <dm.h>
#include <log.h>
#include <wdt.h>
#include <linux/bitops.h>
#include <asm-generic/gpio.h>
#include <linux/delay.h>

DECLARE_GLOBAL_DATA_PTR;

int user_led(void)
{
	unsigned int gpio_led=191;
	gpio_request(gpio_led, "user_led");

	gpio_direction_output(gpio_led, 0);

	gpio_set_value(gpio_led, 0);
}
int board_late_init(void)
{
	unsigned int gpio_net0=94,gpio_net1=181;

	gpio_request(gpio_net0, "gmac0_rst");
	gpio_request(gpio_net1, "gmac1_rst");

	gpio_direction_output(gpio_net0, 0);
	gpio_direction_output(gpio_net1, 0);
	

	gpio_set_value(gpio_net0, 1);
	gpio_set_value(gpio_net1, 1);
	mdelay(10);
	gpio_set_value(gpio_net0, 0);
	gpio_set_value(gpio_net1, 0);
	mdelay(10);
	gpio_set_value(gpio_net0, 1);
	gpio_set_value(gpio_net1, 1);
	mdelay(20);

	gpio_free(gpio_net0);
	gpio_free(gpio_net1);

	user_led();
	return 0;
}

int board_init(void)
{
	/* address of boot parameters */
	gd->bd->bi_boot_params = gd->ram_base + 0x100;

	debug("gd->fdt_blob is %p\n", gd->fdt_blob);
	return 0;
}
