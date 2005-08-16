/*
 * (C) Copyright 2000, 2001
 * Stefan Roese, esd gmbh germany, stefan.roese@esd-electronics.com
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
#include "du405.h"
#include <asm/processor.h>
#include <ppc4xx.h>
#include <405gp_i2c.h>
#include <command.h>

/*cmd_boot.c*/

extern int do_reset (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);
extern void lxt971_no_sleep(void);


#if 0
#define FPGA_DEBUG
#endif

#if 0
#define FPGA_DEBUG2
#endif

/* fpga configuration data - generated by bin2cc */
const unsigned char fpgadata[] = {
#include "fpgadata.c"
};

/*
 * include common fpga code (for esd boards)
 */
#include "../common/fpga.c"


int board_early_init_f (void)
{
	DECLARE_GLOBAL_DATA_PTR;

	int index, len, i;
	int status;

#ifdef FPGA_DEBUG
	/* set up serial port with default baudrate */
	(void) get_clocks ();
	gd->baudrate = CONFIG_BAUDRATE;
	serial_init ();
	console_init_f ();
#endif

	/*
	 * Boot onboard FPGA
	 */
	status = fpga_boot ((unsigned char *) fpgadata, sizeof (fpgadata));
	if (status != 0) {
		/* booting FPGA failed */
#ifndef FPGA_DEBUG
		/* set up serial port with default baudrate */
		(void) get_clocks ();
		gd->baudrate = CONFIG_BAUDRATE;
		serial_init ();
		console_init_f ();
#endif
		printf ("\nFPGA: Booting failed ");
		switch (status) {
		case ERROR_FPGA_PRG_INIT_LOW:
			printf ("(Timeout: INIT not low after asserting PROGRAM*)\n ");
			break;
		case ERROR_FPGA_PRG_INIT_HIGH:
			printf ("(Timeout: INIT not high after deasserting PROGRAM*)\n ");
			break;
		case ERROR_FPGA_PRG_DONE:
			printf ("(Timeout: DONE not high after programming FPGA)\n ");
			break;
		}

		/* display infos on fpgaimage */
		index = 15;
		for (i = 0; i < 4; i++) {
			len = fpgadata[index];
			printf ("FPGA: %s\n", &(fpgadata[index + 1]));
			index += len + 3;
		}
		putc ('\n');
		/* delayed reboot */
		for (i = 20; i > 0; i--) {
			printf ("Rebooting in %2d seconds \r", i);
			for (index = 0; index < 1000; index++)
				udelay (1000);
		}
		putc ('\n');
		do_reset (NULL, 0, 0, NULL);
	}

	/*
	 * IRQ 0-15  405GP internally generated; active high; level sensitive
	 * IRQ 16    405GP internally generated; active low; level sensitive
	 * IRQ 17-24 RESERVED
	 * IRQ 25 (EXT IRQ 0) CAN0; active low; level sensitive
	 * IRQ 26 (EXT IRQ 1) DUART_A; active high; level sensitive
	 * IRQ 27 (EXT IRQ 2) DUART_B; active high; level sensitive
	 * IRQ 28 (EXT IRQ 3) unused; active low; level sensitive
	 * IRQ 29 (EXT IRQ 4) unused; active low; level sensitive
	 * IRQ 30 (EXT IRQ 5) unused; active low; level sensitive
	 * IRQ 31 (EXT IRQ 6) COMPACT FLASH; active high; level sensitive
	 */
	mtdcr (uicsr, 0xFFFFFFFF);	/* clear all ints */
	mtdcr (uicer, 0x00000000);	/* disable all ints */
	mtdcr (uiccr, 0x00000000);	/* set all to be non-critical */
	mtdcr (uicpr, 0xFFFFFFB1);	/* set int polarities */
	mtdcr (uictr, 0x10000000);	/* set int trigger levels */
	mtdcr (uicvcr, 0x00000001);	/* set vect base=0,INT0 highest priority */
	mtdcr (uicsr, 0xFFFFFFFF);	/* clear all ints */

	/*
	 * EBC Configuration Register: set ready timeout to 100 us
	 */
	mtebc (epcr, 0xb8400000);

	return 0;
}


int misc_init_r (void)
{
	unsigned long cntrl0Reg;

	/*
	 * Setup UART1 handshaking: use CTS instead of DSR
	 */
	cntrl0Reg = mfdcr(cntrl0);
	mtdcr(cntrl0, cntrl0Reg | 0x00001000);

	return (0);
}


/*
 * Check Board Identity:
 */
int checkboard (void)
{
	int index;
	int len;
	unsigned char str[64];
	int i = getenv_r ("serial#", str, sizeof (str));

	puts ("Board: ");

	if (i == -1) {
		puts ("### No HW ID - assuming DU405");
	} else {
		puts (str);
	}

	puts ("\nFPGA:  ");

	/* display infos on fpgaimage */
	index = 15;
	for (i = 0; i < 4; i++) {
		len = fpgadata[index];
		printf ("%s ", &(fpgadata[index + 1]));
		index += len + 3;
	}

	putc ('\n');

	/*
	 * Reset external DUART via FPGA
	 */
	*(volatile unsigned char *) FPGA_MODE_REG = 0xff;	/* reset high active */
	*(volatile unsigned char *) FPGA_MODE_REG = 0x00;	/* low again */

	/*
	 * Disable sleep mode in LXT971
	 */
	lxt971_no_sleep();

	return 0;
}


long int initdram (int board_type)
{
	return (16 * 1024 * 1024);
}


int testdram (void)
{
	/* TODO: XXX XXX XXX */
	printf ("test: 16 MB - ok\n");

	return (0);
}
