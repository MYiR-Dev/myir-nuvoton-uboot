/*
 * (C) Copyright 2000
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
#include <mpc8xx.h>

/* ------------------------------------------------------------------------- */

static long int dram_size (long int, long int *, long int);

/* ------------------------------------------------------------------------- */

#define	_NOT_USED_	0xFFFFCC25

const uint sdram_table[] =
{
	/*
	 * Single Read. (Offset 00h in UPMA RAM)
	 */
	0x0F03CC04, 0x00ACCC24, 0x1FF74C20, _NOT_USED_,
	_NOT_USED_, _NOT_USED_, _NOT_USED_, _NOT_USED_,

	/*
	 * Burst Read. (Offset 08h in UPMA RAM)
	 */
	0x0F03CC04, 0x00ACCC24, 0x00FFCC20, 0x00FFCC20,
	0x01FFCC20, 0x1FF74C20, _NOT_USED_, _NOT_USED_,
	_NOT_USED_, _NOT_USED_, _NOT_USED_, _NOT_USED_,
	_NOT_USED_, _NOT_USED_, _NOT_USED_, _NOT_USED_,

	/*
	 * Single Write. (Offset 18h in UPMA RAM)
	 */
	0x0F03CC02, 0x00AC0C24, 0x1FF74C25, _NOT_USED_,
	_NOT_USED_, _NOT_USED_, _NOT_USED_, _NOT_USED_,

	/*
	 * Burst Write. (Offset 20h in UPMA RAM)
	 */
	0x0F03CC00, 0x00AC0C20, 0x00FFFC20, 0x00FFFC22,
	0x01FFFC24, 0x1FF74C25, _NOT_USED_, _NOT_USED_,
	_NOT_USED_, _NOT_USED_, _NOT_USED_, _NOT_USED_,
	_NOT_USED_, _NOT_USED_, _NOT_USED_, _NOT_USED_,

	/*
	 * Refresh. (Offset 30h in UPMA RAM)
	 * (Initialization code at 0x36)
	 */
	0x0FF0CC24, 0xFFFFCC24, _NOT_USED_, _NOT_USED_,
	_NOT_USED_, _NOT_USED_, 0xEFFB8C34, 0x0FF74C34,
	0x0FFACCB4, 0x0FF5CC34, 0x0FFCC34, 0x0FFFCCB4,

	/*
	 * Exception. (Offset 3Ch in UPMA RAM)
	 */
	0x0FEA8C34, 0x1FB54C34, 0xFFFFCC34, _NOT_USED_
};

/* ------------------------------------------------------------------------- */


/*
 * Check Board Identity:
 */

int checkboard (void)
{
	puts ("Board: RMU\n") ;
	return (0) ;
}

/* ------------------------------------------------------------------------- */

long int initdram (int board_type)
{
	volatile immap_t *immap = (immap_t *) CFG_IMMR;
	volatile memctl8xx_t *memctl = &immap->im_memctl;
	long int size9;

	upmconfig (UPMA, (uint *) sdram_table,
		   sizeof (sdram_table) / sizeof (uint));

	/* Refresh clock prescalar */
	memctl->memc_mptpr = CFG_MPTPR;

	memctl->memc_mar = 0x00000088;

	/* Map controller banks 1 to the SDRAM bank */
	memctl->memc_or1 = CFG_OR1_PRELIM;
	memctl->memc_br1 = CFG_BR1_PRELIM;

	memctl->memc_mamr = CFG_MAMR_9COL & (~(MAMR_PTAE));	/* no refresh yet */

	udelay (200);

	/* perform SDRAM initializsation sequence */

	memctl->memc_mcr = 0x80002136;	/* SDRAM bank 0 */
	udelay (1);

	memctl->memc_mamr |= MAMR_PTAE;	/* enable refresh */

	udelay (1000);

	/* Check Bank 0 Memory Size,
	 * 9 column mode
	 */

	size9 = dram_size (CFG_MAMR_9COL, (ulong *) SDRAM_BASE_PRELIM,
			   SDRAM_MAX_SIZE);

	/*
	 * Final mapping:
	 */

	memctl->memc_or1 = ((-size9) & 0xFFFF0000) | CFG_OR_TIMING_SDRAM;
	udelay (1000);

	return (size9);
}

/* ------------------------------------------------------------------------- */

/*
 * Check memory range for valid RAM. A simple memory test determines
 * the actually available RAM size between addresses `base' and
 * `base + maxsize'. Some (not all) hardware errors are detected:
 * - short between address lines
 * - short between data lines
 */

static long int dram_size (long int mamr_value, long int *base,
			   long int maxsize)
{
	volatile immap_t *immap = (immap_t *) CFG_IMMR;
	volatile memctl8xx_t *memctl = &immap->im_memctl;

	memctl->memc_mamr = mamr_value;

	return (get_ram_size(base, maxsize));
}
