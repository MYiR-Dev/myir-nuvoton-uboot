// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2015 Google, Inc
 */

#include <common.h>
#include <debug_uart.h>
#include <dm.h>
#include <fdtdec.h>
#include <i2c.h>
#include <led.h>
#include <malloc.h>
#include <ram.h>
#include <spl.h>
#include <asm/gpio.h>
#include <asm/io.h>
#include <asm/arch-rockchip/bootrom.h>
#include <asm/arch-rockchip/clock.h>
#include <asm/arch-rockchip/hardware.h>
#include <asm/arch-rockchip/periph.h>
#include <asm/arch-rockchip/pmu_rk3288.h>
#include <asm/arch-rockchip/sdram.h>
#include <asm/arch-rockchip/sdram_common.h>
#include <asm/arch-rockchip/sys_proto.h>
#include <dm/root.h>
#include <dm/test.h>
#include <dm/util.h>

DECLARE_GLOBAL_DATA_PTR;

void board_return_to_bootrom(void)
{
	back_to_bootrom(BROM_BOOT_NEXTSTAGE);
}

u32 spl_boot_device(void)
{
#if !CONFIG_IS_ENABLED(OF_PLATDATA)
	const void *blob = gd->fdt_blob;
	struct udevice *dev;
	const char *bootdev;
	int node;
	int ret;

	bootdev = fdtdec_get_config_string(blob, "u-boot,boot0");
	debug("Boot device %s\n", bootdev);
	if (!bootdev)
		goto fallback;

	node = fdt_path_offset(blob, bootdev);
	if (node < 0) {
		debug("node=%d\n", node);
		goto fallback;
	}
	ret = device_get_global_by_ofnode(offset_to_ofnode(node), &dev);
	if (ret) {
		debug("device at node %s/%d not found: %d\n", bootdev, node,
		      ret);
		goto fallback;
	}
	debug("Found device %s\n", dev->name);
	switch (device_get_uclass_id(dev)) {
	case UCLASS_SPI_FLASH:
		return BOOT_DEVICE_SPI;
	case UCLASS_MMC:
		return BOOT_DEVICE_MMC1;
	default:
		debug("Booting from device uclass '%s' not supported\n",
		      dev_get_uclass_name(dev));
	}

fallback:
#elif defined(CONFIG_TARGET_CHROMEBOOK_JERRY) || \
		defined(CONFIG_TARGET_CHROMEBIT_MICKEY) || \
		defined(CONFIG_TARGET_CHROMEBOOK_MINNIE) || \
		defined(CONFIG_TARGET_CHROMEBOOK_SPEEDY)
	return BOOT_DEVICE_SPI;
#endif
	return BOOT_DEVICE_MMC1;
}

__weak int arch_cpu_init(void)
{
	return 0;
}

#define TIMER_LOAD_COUNT_L	0x00
#define TIMER_LOAD_COUNT_H	0x04
#define TIMER_CONTROL_REG	0x10
#define TIMER_EN	0x1
#define	TIMER_FMODE	BIT(0)
#define	TIMER_RMODE	BIT(1)

void rockchip_stimer_init(void)
{
	/* If Timer already enabled, don't re-init it */
	u32 reg = readl(CONFIG_ROCKCHIP_STIMER_BASE + TIMER_CONTROL_REG);

	if (reg & TIMER_EN)
		return;

	asm volatile("mcr p15, 0, %0, c14, c0, 0"
		     : : "r"(COUNTER_FREQUENCY));

	writel(0, CONFIG_ROCKCHIP_STIMER_BASE + TIMER_CONTROL_REG);
	writel(0xffffffff, CONFIG_ROCKCHIP_STIMER_BASE);
	writel(0xffffffff, CONFIG_ROCKCHIP_STIMER_BASE + 4);
	writel(TIMER_EN | TIMER_FMODE, CONFIG_ROCKCHIP_STIMER_BASE +
	       TIMER_CONTROL_REG);
}

void board_init_f(ulong dummy)
{
	struct udevice *dev;
	int ret;

#ifdef CONFIG_DEBUG_UART
	/*
	 * Debug UART can be used from here if required:
	 *
	 * debug_uart_init();
	 * printch('a');
	 * printhex8(0x1234);
	 * printascii("string");
	 */
	debug_uart_init();
	debug("\nspl:debug uart enabled in %s\n", __func__);
#endif
	ret = spl_early_init();
	if (ret) {
		debug("spl_early_init() failed: %d\n", ret);
		hang();
	}

	/* Init secure timer */
	rockchip_stimer_init();
	/* Init ARM arch timer in arch/arm/cpu/armv7/arch_timer.c */
	timer_init();

	arch_cpu_init();

	preloader_console_init();

	ret = rockchip_get_clk(&dev);
	if (ret) {
		debug("CLK init failed: %d\n", ret);
		return;
	}

#if !defined(CONFIG_SUPPORT_TPL)
	debug("\nspl:init dram\n");
	ret = uclass_get_device(UCLASS_RAM, 0, &dev);
	if (ret) {
		debug("DRAM init failed: %d\n", ret);
		return;
	}
#endif
}
