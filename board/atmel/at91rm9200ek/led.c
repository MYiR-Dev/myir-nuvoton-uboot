/*
 * (C) Copyright 2006
 * Atmel Nordic AB <www.atmel.com>
 * Ulf Samuelsson <ulf@atmel.com>
 *
 * (C) Copyright 2010
 * Andreas Bießmann <andreas.devel@gmail.com>
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
#include <asm/arch/gpio.h>
#include <asm/arch/at91_pmc.h>

/* bit mask in PIO port B */
#define	GREEN_LED	(1<<0)
#define	YELLOW_LED	(1<<1)
#define	RED_LED		(1<<2)

void	green_LED_on(void)
{
	at91_pio_t *pio = (at91_pio_t *)AT91_PIO_BASE;
	writel(GREEN_LED, &pio->piob.codr);
}

void	 yellow_LED_on(void)
{
	at91_pio_t *pio = (at91_pio_t *)AT91_PIO_BASE;
	writel(YELLOW_LED, &pio->piob.codr);
}

void	 red_LED_on(void)
{
	at91_pio_t *pio = (at91_pio_t *)AT91_PIO_BASE;
	writel(RED_LED, &pio->piob.codr);
}

void	green_LED_off(void)
{
	at91_pio_t *pio = (at91_pio_t *)AT91_PIO_BASE;
	writel(GREEN_LED, &pio->piob.sodr);
}

void	yellow_LED_off(void)
{
	at91_pio_t *pio = (at91_pio_t *)AT91_PIO_BASE;
	writel(YELLOW_LED, &pio->piob.sodr);
}

void	red_LED_off(void)
{
	at91_pio_t *pio = (at91_pio_t *)AT91_PIO_BASE;
	writel(RED_LED, &pio->piob.sodr);
}

void coloured_LED_init (void)
{
	at91_pmc_t *pmc = (at91_pmc_t *)AT91_PMC_BASE;
	at91_pio_t *pio = (at91_pio_t *)AT91_PIO_BASE;

	/* Enable PIOB clock */
	writel(1 << AT91_ID_PIOB, &pmc->pcer);

	/* Disable peripherals on LEDs */
	writel(GREEN_LED | YELLOW_LED | RED_LED, &pio->piob.per);
	/* Enable pins as outputs */
	writel(GREEN_LED | YELLOW_LED | RED_LED, &pio->piob.oer);
	/* Turn all LEDs OFF */
	writel(GREEN_LED | YELLOW_LED | RED_LED, &pio->piob.sodr);
}
