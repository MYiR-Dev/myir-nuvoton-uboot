/*
 * board.c
 *
 * Board functions for TI AM43XX based boards
 *
 * Copyright (C) 2013, Texas Instruments, Incorporated - http://www.ti.com/
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <i2c.h>
#include <asm/errno.h>
#include <spl.h>
#include <asm/arch/clock.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/mux.h>
#include "board.h"

DECLARE_GLOBAL_DATA_PTR;

/*
 * Read header information from EEPROM into global structure.
 */
static int read_eeprom(struct am43xx_board_id *header)
{
	/* Check if baseboard eeprom is available */
	if (i2c_probe(CONFIG_SYS_I2C_EEPROM_ADDR)) {
		printf("Could not probe the EEPROM at 0x%x\n",
		       CONFIG_SYS_I2C_EEPROM_ADDR);
		return -ENODEV;
	}

	/* read the eeprom using i2c */
	if (i2c_read(CONFIG_SYS_I2C_EEPROM_ADDR, 0, 2, (uchar *)header,
		     sizeof(struct am43xx_board_id))) {
		printf("Could not read the EEPROM\n");
		return -EIO;
	}

	if (header->magic != 0xEE3355AA) {
		/*
		 * read the eeprom using i2c again,
		 * but use only a 1 byte address
		 */
		if (i2c_read(CONFIG_SYS_I2C_EEPROM_ADDR, 0, 1, (uchar *)header,
			     sizeof(struct am43xx_board_id))) {
			printf("Could not read the EEPROM at 0x%x\n",
			       CONFIG_SYS_I2C_EEPROM_ADDR);
			return -EIO;
		}

		if (header->magic != 0xEE3355AA) {
			printf("Incorrect magic number (0x%x) in EEPROM\n",
			       header->magic);
			return -EINVAL;
		}
	}

	strncpy(am43xx_board_name, (char *)header->name, sizeof(header->name));
	am43xx_board_name[sizeof(header->name)] = 0;

	return 0;
}

#ifdef CONFIG_SPL_BUILD

#define NUM_OPPS	6

const struct dpll_params dpll_mpu[NUM_CRYSTAL_FREQ][NUM_OPPS] = {
	{	/* 19.2 MHz */
		{-1, -1, -1, -1, -1, -1, -1},	/* OPP 50 */
		{-1, -1, -1, -1, -1, -1, -1},	/* OPP RESERVED	*/
		{-1, -1, -1, -1, -1, -1, -1},	/* OPP 100 */
		{-1, -1, -1, -1, -1, -1, -1},	/* OPP 120 */
		{-1, -1, -1, -1, -1, -1, -1},	/* OPP TB */
		{-1, -1, -1, -1, -1, -1, -1}	/* OPP NT */
	},
	{	/* 24 MHz */
		{300, 23, 1, -1, -1, -1, -1},	/* OPP 50 */
		{-1, -1, -1, -1, -1, -1, -1},	/* OPP RESERVED	*/
		{600, 23, 1, -1, -1, -1, -1},	/* OPP 100 */
		{720, 23, 1, -1, -1, -1, -1},	/* OPP 120 */
		{800, 23, 1, -1, -1, -1, -1},	/* OPP TB */
		{1000, 23, 1, -1, -1, -1, -1}	/* OPP NT */
	},
	{	/* 25 MHz */
		{300, 24, 1, -1, -1, -1, -1},	/* OPP 50 */
		{-1, -1, -1, -1, -1, -1, -1},	/* OPP RESERVED	*/
		{600, 24, 1, -1, -1, -1, -1},	/* OPP 100 */
		{720, 24, 1, -1, -1, -1, -1},	/* OPP 120 */
		{800, 24, 1, -1, -1, -1, -1},	/* OPP TB */
		{1000, 24, 1, -1, -1, -1, -1}	/* OPP NT */
	},
	{	/* 26 MHz */
		{300, 25, 1, -1, -1, -1, -1},	/* OPP 50 */
		{-1, -1, -1, -1, -1, -1, -1},	/* OPP RESERVED	*/
		{600, 25, 1, -1, -1, -1, -1},	/* OPP 100 */
		{720, 25, 1, -1, -1, -1, -1},	/* OPP 120 */
		{800, 25, 1, -1, -1, -1, -1},	/* OPP TB */
		{1000, 25, 1, -1, -1, -1, -1}	/* OPP NT */
	},
};

const struct dpll_params dpll_core[NUM_CRYSTAL_FREQ] = {
		{-1, -1, -1, -1, -1, -1, -1},	/* 19.2 MHz */
		{1000, 23, -1, -1, 10, 8, 4},	/* 24 MHz */
		{1000, 24, -1, -1, 10, 8, 4},	/* 25 MHz */
		{1000, 25, -1, -1, 10, 8, 4}	/* 26 MHz */
};

const struct dpll_params dpll_per[NUM_CRYSTAL_FREQ] = {
		{-1, -1, -1, -1, -1, -1, -1},	/* 19.2 MHz */
		{960, 23, 5, -1, -1, -1, -1},	/* 24 MHz */
		{960, 24, 5, -1, -1, -1, -1},	/* 25 MHz */
		{960, 25, 5, -1, -1, -1, -1}	/* 26 MHz */
};

const struct dpll_params epos_evm_dpll_ddr = {
		266, 24, 1, -1, 1, -1, -1};

const struct dpll_params gp_evm_dpll_ddr = {
		400, 23, 1, -1, 1, -1, -1};

const struct dpll_params *get_dpll_ddr_params(void)
{
	struct am43xx_board_id header;

	enable_i2c0_pin_mux();
	i2c_init(CONFIG_SYS_OMAP24_I2C_SPEED, CONFIG_SYS_OMAP24_I2C_SLAVE);
	if (read_eeprom(&header) < 0)
		puts("Could not get board ID.\n");

	if (board_is_eposevm())
		return &epos_evm_dpll_ddr;
	else if (board_is_gpevm())
		return &gp_evm_dpll_ddr;

	puts(" Board not supported\n");
	return NULL;
}

/*
 * get_sys_clk_index : returns the index of the sys_clk read from
 *			ctrl status register. This value is either
 *			read from efuse or sysboot pins.
 */
static u32 get_sys_clk_index(void)
{
	struct ctrl_stat *ctrl = (struct ctrl_stat *)CTRL_BASE;
	u32 ind = readl(&ctrl->statusreg), src;

	src = (ind & CTRL_CRYSTAL_FREQ_SRC_MASK) >> CTRL_CRYSTAL_FREQ_SRC_SHIFT;
	if (src == CTRL_CRYSTAL_FREQ_SRC_EFUSE) /* Value read from EFUSE */
		return ((ind & CTRL_CRYSTAL_FREQ_SELECTION_MASK) >>
			CTRL_CRYSTAL_FREQ_SELECTION_SHIFT);
	else /* Value read from SYS BOOT pins */
		return ((ind & CTRL_SYSBOOT_15_14_MASK) >>
			CTRL_SYSBOOT_15_14_SHIFT);
}

/*
 * get_opp_offset:
 * Returns the index for safest OPP of the device to boot.
 * max_off:	Index of the MAX OPP in DEV ATTRIBUTE register.
 * min_off:	Index of the MIN OPP in DEV ATTRIBUTE register.
 * This data is read from dev_attribute register which is e-fused.
 * A'1' in bit indicates OPP disabled and not available, a '0' indicates
 * OPP available. Lowest OPP starts with min_off. So returning the
 * bit with rightmost '0'.
 */
static int get_opp_offset(int max_off, int min_off)
{
	struct ctrl_stat *ctrl = (struct ctrl_stat *)CTRL_BASE;
	int opp = readl(&ctrl->dev_attr), offset, i;

	for (i = max_off; i >= min_off; i--) {
		offset = opp & (1 << i);
		if (!offset)
			return i;
	}

	return min_off;
}

const struct dpll_params *get_dpll_mpu_params(void)
{
	int opp = get_opp_offset(DEV_ATTR_MAX_OFFSET, DEV_ATTR_MIN_OFFSET);
	u32 ind = get_sys_clk_index();

	return &dpll_mpu[ind][opp];
}

const struct dpll_params *get_dpll_core_params(void)
{
	int ind = get_sys_clk_index();

	return &dpll_core[ind];
}

const struct dpll_params *get_dpll_per_params(void)
{
	int ind = get_sys_clk_index();

	return &dpll_per[ind];
}

void set_uart_mux_conf(void)
{
	enable_uart0_pin_mux();
}

void set_mux_conf_regs(void)
{
	enable_board_pin_mux();
}

void sdram_init(void)
{
}
#endif

int board_init(void)
{
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;

	return 0;
}

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
#ifdef CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
	char safe_string[HDR_NAME_LEN + 1];
	struct am43xx_board_id header;

	if (read_eeprom(&header) < 0)
		puts("Could not get board ID.\n");

	/* Now set variables based on the header. */
	strncpy(safe_string, (char *)header.name, sizeof(header.name));
	safe_string[sizeof(header.name)] = 0;
	setenv("board_name", safe_string);

	strncpy(safe_string, (char *)header.version, sizeof(header.version));
	safe_string[sizeof(header.version)] = 0;
	setenv("board_rev", safe_string);
#endif
	return 0;
}
#endif
