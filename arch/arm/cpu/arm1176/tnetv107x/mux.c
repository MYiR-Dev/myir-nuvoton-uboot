/*
 * TNETV107X: Pinmux configuration
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/mux.h>

#define MUX_MODE_1		0x00
#define MUX_MODE_2		0x04
#define MUX_MODE_3		0x0c
#define MUX_MODE_4		0x1c

#define MUX_DEBUG		0

static const struct pin_config pin_table[] = {
	/*		  reg	shift	mode	*/
	TNETV107X_MUX_CFG(0,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(0,	0,	MUX_MODE_2),
	TNETV107X_MUX_CFG(0,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(0,	5,	MUX_MODE_2),
	TNETV107X_MUX_CFG(0,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(0,	10,	MUX_MODE_2),
	TNETV107X_MUX_CFG(0,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(0,	15,	MUX_MODE_2),
	TNETV107X_MUX_CFG(0,	20,	MUX_MODE_1),
	TNETV107X_MUX_CFG(0,	20,	MUX_MODE_2),
	TNETV107X_MUX_CFG(0,	25,	MUX_MODE_1),
	TNETV107X_MUX_CFG(0,	25,	MUX_MODE_2),
	TNETV107X_MUX_CFG(1,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(1,	0,	MUX_MODE_2),
	TNETV107X_MUX_CFG(1,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(1,	5,	MUX_MODE_2),
	TNETV107X_MUX_CFG(1,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(1,	10,	MUX_MODE_2),
	TNETV107X_MUX_CFG(1,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(1,	15,	MUX_MODE_2),
	TNETV107X_MUX_CFG(1,	20,	MUX_MODE_1),
	TNETV107X_MUX_CFG(1,	20,	MUX_MODE_2),
	TNETV107X_MUX_CFG(1,	25,	MUX_MODE_1),
	TNETV107X_MUX_CFG(1,	25,	MUX_MODE_2),
	TNETV107X_MUX_CFG(2,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(2,	0,	MUX_MODE_2),
	TNETV107X_MUX_CFG(2,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(2,	5,	MUX_MODE_2),
	TNETV107X_MUX_CFG(2,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(2,	10,	MUX_MODE_2),
	TNETV107X_MUX_CFG(2,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(2,	15,	MUX_MODE_2),
	TNETV107X_MUX_CFG(2,	20,	MUX_MODE_1),
	TNETV107X_MUX_CFG(2,	20,	MUX_MODE_2),
	TNETV107X_MUX_CFG(2,	25,	MUX_MODE_1),
	TNETV107X_MUX_CFG(2,	25,	MUX_MODE_2),
	TNETV107X_MUX_CFG(3,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(3,	0,	MUX_MODE_2),
	TNETV107X_MUX_CFG(3,	0,	MUX_MODE_4),
	TNETV107X_MUX_CFG(3,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(3,	5,	MUX_MODE_2),
	TNETV107X_MUX_CFG(3,	5,	MUX_MODE_4),
	TNETV107X_MUX_CFG(3,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(3,	10,	MUX_MODE_2),
	TNETV107X_MUX_CFG(3,	10,	MUX_MODE_4),
	TNETV107X_MUX_CFG(3,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(3,	15,	MUX_MODE_2),
	TNETV107X_MUX_CFG(3,	15,	MUX_MODE_4),
	TNETV107X_MUX_CFG(3,	20,	MUX_MODE_1),
	TNETV107X_MUX_CFG(3,	20,	MUX_MODE_2),
	TNETV107X_MUX_CFG(3,	20,	MUX_MODE_4),
	TNETV107X_MUX_CFG(3,	25,	MUX_MODE_1),
	TNETV107X_MUX_CFG(3,	25,	MUX_MODE_2),
	TNETV107X_MUX_CFG(3,	25,	MUX_MODE_4),
	TNETV107X_MUX_CFG(4,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(4,	0,	MUX_MODE_2),
	TNETV107X_MUX_CFG(4,	0,	MUX_MODE_4),
	TNETV107X_MUX_CFG(4,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(4,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(4,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(4,	15,	MUX_MODE_4),
	TNETV107X_MUX_CFG(4,	20,	MUX_MODE_1),
	TNETV107X_MUX_CFG(4,	20,	MUX_MODE_3),
	TNETV107X_MUX_CFG(4,	25,	MUX_MODE_1),
	TNETV107X_MUX_CFG(4,	25,	MUX_MODE_4),
	TNETV107X_MUX_CFG(5,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(5,	0,	MUX_MODE_4),
	TNETV107X_MUX_CFG(5,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(5,	5,	MUX_MODE_4),
	TNETV107X_MUX_CFG(5,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(5,	10,	MUX_MODE_4),
	TNETV107X_MUX_CFG(5,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(5,	15,	MUX_MODE_4),
	TNETV107X_MUX_CFG(5,	20,	MUX_MODE_1),
	TNETV107X_MUX_CFG(5,	20,	MUX_MODE_4),
	TNETV107X_MUX_CFG(5,	25,	MUX_MODE_1),
	TNETV107X_MUX_CFG(5,	25,	MUX_MODE_4),
	TNETV107X_MUX_CFG(6,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(6,	0,	MUX_MODE_4),
	TNETV107X_MUX_CFG(6,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(6,	5,	MUX_MODE_4),
	TNETV107X_MUX_CFG(6,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(6,	10,	MUX_MODE_4),
	TNETV107X_MUX_CFG(6,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(6,	15,	MUX_MODE_4),
	TNETV107X_MUX_CFG(6,	20,	MUX_MODE_1),
	TNETV107X_MUX_CFG(6,	20,	MUX_MODE_4),
	TNETV107X_MUX_CFG(6,	25,	MUX_MODE_1),
	TNETV107X_MUX_CFG(6,	25,	MUX_MODE_4),
	TNETV107X_MUX_CFG(7,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(7,	0,	MUX_MODE_4),
	TNETV107X_MUX_CFG(7,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(7,	5,	MUX_MODE_4),
	TNETV107X_MUX_CFG(7,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(7,	10,	MUX_MODE_4),
	TNETV107X_MUX_CFG(7,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(7,	15,	MUX_MODE_2),
	TNETV107X_MUX_CFG(7,	20,	MUX_MODE_1),
	TNETV107X_MUX_CFG(7,	20,	MUX_MODE_2),
	TNETV107X_MUX_CFG(7,	25,	MUX_MODE_1),
	TNETV107X_MUX_CFG(7,	25,	MUX_MODE_2),
	TNETV107X_MUX_CFG(8,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(8,	0,	MUX_MODE_2),
	TNETV107X_MUX_CFG(8,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(8,	5,	MUX_MODE_2),
	TNETV107X_MUX_CFG(8,	5,	MUX_MODE_4),
	TNETV107X_MUX_CFG(8,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(8,	10,	MUX_MODE_2),
	TNETV107X_MUX_CFG(9,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(9,	0,	MUX_MODE_2),
	TNETV107X_MUX_CFG(9,	0,	MUX_MODE_4),
	TNETV107X_MUX_CFG(9,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(9,	5,	MUX_MODE_2),
	TNETV107X_MUX_CFG(9,	5,	MUX_MODE_4),
	TNETV107X_MUX_CFG(9,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(9,	10,	MUX_MODE_2),
	TNETV107X_MUX_CFG(9,	10,	MUX_MODE_4),
	TNETV107X_MUX_CFG(9,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(9,	15,	MUX_MODE_2),
	TNETV107X_MUX_CFG(9,	15,	MUX_MODE_4),
	TNETV107X_MUX_CFG(9,	20,	MUX_MODE_1),
	TNETV107X_MUX_CFG(9,	20,	MUX_MODE_2),
	TNETV107X_MUX_CFG(9,	20,	MUX_MODE_4),
	TNETV107X_MUX_CFG(10,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(10,	0,	MUX_MODE_2),
	TNETV107X_MUX_CFG(10,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(10,	5,	MUX_MODE_2),
	TNETV107X_MUX_CFG(10,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(10,	10,	MUX_MODE_2),
	TNETV107X_MUX_CFG(10,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(10,	15,	MUX_MODE_2),
	TNETV107X_MUX_CFG(10,	20,	MUX_MODE_1),
	TNETV107X_MUX_CFG(10,	20,	MUX_MODE_2),
	TNETV107X_MUX_CFG(10,	25,	MUX_MODE_1),
	TNETV107X_MUX_CFG(10,	25,	MUX_MODE_2),
	TNETV107X_MUX_CFG(11,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(11,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(12,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(12,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(12,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(12,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(12,	20,	MUX_MODE_1),
	TNETV107X_MUX_CFG(12,	25,	MUX_MODE_1),
	TNETV107X_MUX_CFG(13,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(13,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(13,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(13,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(14,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(14,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(14,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(14,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(14,	20,	MUX_MODE_1),
	TNETV107X_MUX_CFG(14,	25,	MUX_MODE_1),
	TNETV107X_MUX_CFG(15,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(15,	0,	MUX_MODE_2),
	TNETV107X_MUX_CFG(15,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(15,	5,	MUX_MODE_2),
	TNETV107X_MUX_CFG(15,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(15,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(15,	20,	MUX_MODE_1),
	TNETV107X_MUX_CFG(15,	25,	MUX_MODE_1),
	TNETV107X_MUX_CFG(16,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(16,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(16,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(16,	10,	MUX_MODE_2),
	TNETV107X_MUX_CFG(16,	10,	MUX_MODE_3),
	TNETV107X_MUX_CFG(16,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(16,	15,	MUX_MODE_2),
	TNETV107X_MUX_CFG(17,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(17,	0,	MUX_MODE_2),
	TNETV107X_MUX_CFG(17,	0,	MUX_MODE_3),
	TNETV107X_MUX_CFG(17,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(17,	5,	MUX_MODE_2),
	TNETV107X_MUX_CFG(17,	5,	MUX_MODE_3),
	TNETV107X_MUX_CFG(17,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(17,	10,	MUX_MODE_2),
	TNETV107X_MUX_CFG(17,	10,	MUX_MODE_3),
	TNETV107X_MUX_CFG(17,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(17,	15,	MUX_MODE_2),
	TNETV107X_MUX_CFG(17,	15,	MUX_MODE_3),
	TNETV107X_MUX_CFG(18,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(18,	0,	MUX_MODE_2),
	TNETV107X_MUX_CFG(18,	0,	MUX_MODE_3),
	TNETV107X_MUX_CFG(18,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(18,	5,	MUX_MODE_2),
	TNETV107X_MUX_CFG(18,	5,	MUX_MODE_3),
	TNETV107X_MUX_CFG(18,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(18,	10,	MUX_MODE_2),
	TNETV107X_MUX_CFG(18,	10,	MUX_MODE_3),
	TNETV107X_MUX_CFG(18,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(18,	15,	MUX_MODE_2),
	TNETV107X_MUX_CFG(18,	15,	MUX_MODE_3),
	TNETV107X_MUX_CFG(19,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(19,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(19,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(19,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(19,	20,	MUX_MODE_1),
	TNETV107X_MUX_CFG(19,	25,	MUX_MODE_1),
	TNETV107X_MUX_CFG(20,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(20,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(20,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(20,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(20,	15,	MUX_MODE_3),
	TNETV107X_MUX_CFG(20,	20,	MUX_MODE_1),
	TNETV107X_MUX_CFG(20,	25,	MUX_MODE_1),
	TNETV107X_MUX_CFG(21,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(21,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(21,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(21,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(21,	20,	MUX_MODE_1),
	TNETV107X_MUX_CFG(21,	25,	MUX_MODE_1),
	TNETV107X_MUX_CFG(22,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(22,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(22,	5,	MUX_MODE_3),
	TNETV107X_MUX_CFG(22,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(22,	10,	MUX_MODE_3),
	TNETV107X_MUX_CFG(22,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(22,	15,	MUX_MODE_2),
	TNETV107X_MUX_CFG(22,	15,	MUX_MODE_3),
	TNETV107X_MUX_CFG(22,	20,	MUX_MODE_1),
	TNETV107X_MUX_CFG(22,	20,	MUX_MODE_3),
	TNETV107X_MUX_CFG(22,	25,	MUX_MODE_1),
	TNETV107X_MUX_CFG(22,	25,	MUX_MODE_3),
	TNETV107X_MUX_CFG(23,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(23,	0,	MUX_MODE_3),
	TNETV107X_MUX_CFG(23,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(23,	5,	MUX_MODE_3),
	TNETV107X_MUX_CFG(23,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(23,	10,	MUX_MODE_3),
	TNETV107X_MUX_CFG(24,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(24,	0,	MUX_MODE_2),
	TNETV107X_MUX_CFG(24,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(24,	5,	MUX_MODE_2),
	TNETV107X_MUX_CFG(24,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(24,	10,	MUX_MODE_2),
	TNETV107X_MUX_CFG(24,	10,	MUX_MODE_3),
	TNETV107X_MUX_CFG(24,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(24,	15,	MUX_MODE_2),
	TNETV107X_MUX_CFG(24,	15,	MUX_MODE_3),
	TNETV107X_MUX_CFG(24,	20,	MUX_MODE_1),
	TNETV107X_MUX_CFG(24,	20,	MUX_MODE_2),
	TNETV107X_MUX_CFG(24,	25,	MUX_MODE_1),
	TNETV107X_MUX_CFG(24,	25,	MUX_MODE_2),
	TNETV107X_MUX_CFG(25,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(25,	0,	MUX_MODE_2),
	TNETV107X_MUX_CFG(25,	0,	MUX_MODE_3),
	TNETV107X_MUX_CFG(25,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(25,	5,	MUX_MODE_2),
	TNETV107X_MUX_CFG(25,	5,	MUX_MODE_3),
	TNETV107X_MUX_CFG(25,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(25,	10,	MUX_MODE_2),
	TNETV107X_MUX_CFG(25,	10,	MUX_MODE_3),
	TNETV107X_MUX_CFG(25,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(25,	15,	MUX_MODE_2),
	TNETV107X_MUX_CFG(25,	15,	MUX_MODE_3),
	TNETV107X_MUX_CFG(25,	15,	MUX_MODE_4),
	TNETV107X_MUX_CFG(26,	0,	MUX_MODE_1),
	TNETV107X_MUX_CFG(26,	5,	MUX_MODE_1),
	TNETV107X_MUX_CFG(26,	10,	MUX_MODE_1),
	TNETV107X_MUX_CFG(26,	10,	MUX_MODE_2),
	TNETV107X_MUX_CFG(26,	15,	MUX_MODE_1),
	TNETV107X_MUX_CFG(26,	15,	MUX_MODE_2),
	TNETV107X_MUX_CFG(26,	20,	MUX_MODE_1),
	TNETV107X_MUX_CFG(26,	20,	MUX_MODE_2),
	TNETV107X_MUX_CFG(26,	25,	MUX_MODE_1),
	TNETV107X_MUX_CFG(26,	25,	MUX_MODE_2),
};

const int pin_table_size = sizeof(pin_table) / sizeof(pin_table[0]);

int mux_select_pin(short index)
{
	const struct pin_config *cfg;
	unsigned long mask, mode, reg;

	if (index >= pin_table_size)
		return 0;

	cfg = &pin_table[index];

	mask = 0x1f << cfg->mask_offset;
	mode = cfg->mode << cfg->mask_offset;

	reg = __raw_readl(TNETV107X_PINMUX(cfg->reg_index));
	reg = (reg & ~mask) | mode;
	__raw_writel(reg, TNETV107X_PINMUX(cfg->reg_index));

	return 1;
}

int mux_select_pins(const short *pins)
{
	int i, ret = 1;

	for (i = 0; pins[i] >= 0; i++)
		ret &= mux_select_pin(pins[i]);

	return ret;
}
