/*
 * (C) Copyright 2003, Psyent Corporation <www.psyent.com>
 * Scott McNutt <smcnutt@psyent.com>
 *
 * (C) Copyright 2000-2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <stdio_dev.h>
#include <watchdog.h>
#include <malloc.h>
#include <net.h>
#ifdef CONFIG_STATUS_LED
#include <status_led.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

/*
 * All attempts to come up with a "common" initialization sequence
 * that works for all boards and architectures failed: some of the
 * requirements are just _too_ different. To get rid of the resulting
 * mess of board dependend #ifdef'ed code we now make the whole
 * initialization sequence configurable to the user.
 *
 * The requirements for any new initalization function is simple: it
 * receives a pointer to the "global data" structure as it's only
 * argument, and returns an integer return code, where 0 means
 * "continue" and != 0 means "fatal error, hang the system".
 */


extern void malloc_bin_reloc (void);
typedef int (init_fnc_t) (void);

/*
 * The Malloc area is immediately below the monitor copy in RAM
 */
static void mem_malloc_init(ulong start, ulong size)
{
	mem_malloc_start = start;
	mem_malloc_end = start + size;
	mem_malloc_brk = start;

	memset ((void *)mem_malloc_start, 0, size);
}


/************************************************************************
 * Initialization sequence						*
 ***********************************************************************/

init_fnc_t *init_sequence[] = {

#if defined(CONFIG_BOARD_EARLY_INIT_F)
	board_early_init_f,	/* Call board-specific init code early.*/
#endif

	env_init,
	serial_init,
	console_init_f,
	display_options,
	checkcpu,
	checkboard,
	NULL,			/* Terminate this list */
};


/***********************************************************************/
void board_init (void)
{
	bd_t *bd;
	init_fnc_t **init_fnc_ptr;
	char *s, *e;
	int i;

	/* Pointer is writable since we allocated a register for it.
	 * Nios treats CONFIG_SYS_GBL_DATA_OFFSET as an address.
	 */
	gd = (gd_t *)CONFIG_SYS_GBL_DATA_OFFSET;
	/* compiler optimization barrier needed for GCC >= 3.4 */
	__asm__ __volatile__("": : :"memory");

	memset( gd, 0, CONFIG_SYS_GBL_DATA_SIZE );

	gd->bd = (bd_t *)(gd+1);	/* At end of global data */
	gd->baudrate = CONFIG_BAUDRATE;
	gd->cpu_clk = CONFIG_SYS_CLK_FREQ;

	bd = gd->bd;
	bd->bi_memstart	= CONFIG_SYS_SDRAM_BASE;
	bd->bi_memsize = CONFIG_SYS_SDRAM_SIZE;
	bd->bi_flashstart = CONFIG_SYS_FLASH_BASE;
#if	defined(CONFIG_SYS_SRAM_BASE) && defined(CONFIG_SYS_SRAM_SIZE)
	bd->bi_sramstart= CONFIG_SYS_SRAM_BASE;
	bd->bi_sramsize	= CONFIG_SYS_SRAM_SIZE;
#endif
	bd->bi_baudrate	= CONFIG_BAUDRATE;

	for (init_fnc_ptr = init_sequence; *init_fnc_ptr; ++init_fnc_ptr) {
		WATCHDOG_RESET ();
		if ((*init_fnc_ptr) () != 0) {
			hang ();
		}
	}

	WATCHDOG_RESET ();
	mem_malloc_init(CONFIG_SYS_MALLOC_BASE, CONFIG_SYS_MALLOC_LEN);
	malloc_bin_reloc();

	WATCHDOG_RESET ();
	bd->bi_flashsize = flash_init();

	WATCHDOG_RESET ();
	env_relocate();

	bd->bi_ip_addr = getenv_IPaddr ("ipaddr");

	WATCHDOG_RESET ();
	stdio_init();
	jumptable_init();
	console_init_r();
	/*
	 */

	WATCHDOG_RESET ();
	interrupt_init ();

#ifdef CONFIG_STATUS_LED
	status_led_set(STATUS_LED_BOOT, STATUS_LED_BLINKING);
#endif

	/* main_loop */
	for (;;) {
		WATCHDOG_RESET ();
		main_loop ();
	}
}


/***********************************************************************/

void hang (void)
{
#ifdef CONFIG_STATUS_LED
	status_led_set(STATUS_LED_BOOT, STATUS_LED_OFF);
	status_led_set(STATUS_LED_RED, STATUS_LED_BLINKING);
#endif
	puts("### ERROR ### Please reset board ###\n");
	for (;;);
}

unsigned long do_go_exec (ulong (*entry)(int, char *[]), int argc, char *argv[])
{
	/*
	 * x86 does not use a dedicated register to pass the pointer
	 * to the global_data
	 */
	argv[-1] = (char *)gd;
	return entry (argc, argv);
}
