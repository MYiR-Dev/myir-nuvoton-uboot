/*
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, d.mueller@elsoft.ch
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/* This code should work for both the S3C2400 and the S3C2410
 * as they seem to have the same I2C controller inside.
 * The different address mapping is handled by the s3c24xx.h files below.
 */

#include <common.h>
#include <fdtdec.h>
#if (defined CONFIG_EXYNOS4 || defined CONFIG_EXYNOS5)
#include <asm/arch/clk.h>
#include <asm/arch/cpu.h>
#include <asm/arch/pinmux.h>
#else
#include <asm/arch/s3c24x0_cpu.h>
#endif
#include <asm/io.h>
#include <i2c.h>
#include "s3c24x0_i2c.h"

#ifdef CONFIG_HARD_I2C

#define	I2C_WRITE	0
#define I2C_READ	1

#define I2C_OK		0
#define I2C_NOK		1
#define I2C_NACK	2
#define I2C_NOK_LA	3	/* Lost arbitration */
#define I2C_NOK_TOUT	4	/* time out */

#define I2CSTAT_BSY	0x20	/* Busy bit */
#define I2CSTAT_NACK	0x01	/* Nack bit */
#define I2CCON_ACKGEN	0x80	/* Acknowledge generation */
#define I2CCON_IRPND	0x10	/* Interrupt pending bit */
#define I2C_MODE_MT	0xC0	/* Master Transmit Mode */
#define I2C_MODE_MR	0x80	/* Master Receive Mode */
#define I2C_START_STOP	0x20	/* START / STOP */
#define I2C_TXRX_ENA	0x10	/* I2C Tx/Rx enable */

#define I2C_TIMEOUT_MS 1000		/* 1 second */


/*
 * For SPL boot some boards need i2c before SDRAM is initialised so force
 * variables to live in SRAM
 */
static unsigned int g_current_bus __attribute__((section(".data")));
#ifdef CONFIG_OF_CONTROL
static int i2c_busses __attribute__((section(".data")));
static struct s3c24x0_i2c_bus i2c_bus[CONFIG_MAX_I2C_NUM]
			__attribute__((section(".data")));
#endif

#if !(defined CONFIG_EXYNOS4 || defined CONFIG_EXYNOS5)
static int GetI2CSDA(void)
{
	struct s3c24x0_gpio *gpio = s3c24x0_get_base_gpio();

#ifdef CONFIG_S3C2410
	return (readl(&gpio->gpedat) & 0x8000) >> 15;
#endif
#ifdef CONFIG_S3C2400
	return (readl(&gpio->pgdat) & 0x0020) >> 5;
#endif
}

static void SetI2CSCL(int x)
{
	struct s3c24x0_gpio *gpio = s3c24x0_get_base_gpio();

#ifdef CONFIG_S3C2410
	writel((readl(&gpio->gpedat) & ~0x4000) |
					(x & 1) << 14, &gpio->gpedat);
#endif
#ifdef CONFIG_S3C2400
	writel((readl(&gpio->pgdat) & ~0x0040) | (x & 1) << 6, &gpio->pgdat);
#endif
}
#endif

/*
 * Wait til the byte transfer is completed.
 *
 * @param i2c- pointer to the appropriate i2c register bank.
 * @return I2C_OK, if transmission was ACKED
 *         I2C_NACK, if transmission was NACKED
 *         I2C_NOK_TIMEOUT, if transaction did not complete in I2C_TIMEOUT_MS
 */

static int WaitForXfer(struct s3c24x0_i2c *i2c)
{
	ulong start_time = get_timer(0);

	do {
		if (readl(&i2c->iiccon) & I2CCON_IRPND)
			return (readl(&i2c->iicstat) & I2CSTAT_NACK) ?
				I2C_NACK : I2C_OK;
	} while (get_timer(start_time) < I2C_TIMEOUT_MS);

	return I2C_NOK_TOUT;
}

static void ReadWriteByte(struct s3c24x0_i2c *i2c)
{
	writel(readl(&i2c->iiccon) & ~I2CCON_IRPND, &i2c->iiccon);
}

static struct s3c24x0_i2c *get_base_i2c(void)
{
#ifdef CONFIG_EXYNOS4
	struct s3c24x0_i2c *i2c = (struct s3c24x0_i2c *)(samsung_get_base_i2c()
							+ (EXYNOS4_I2C_SPACING
							* g_current_bus));
	return i2c;
#elif defined CONFIG_EXYNOS5
	struct s3c24x0_i2c *i2c = (struct s3c24x0_i2c *)(samsung_get_base_i2c()
							+ (EXYNOS5_I2C_SPACING
							* g_current_bus));
	return i2c;
#else
	return s3c24x0_get_base_i2c();
#endif
}

static void i2c_ch_init(struct s3c24x0_i2c *i2c, int speed, int slaveadd)
{
	ulong freq, pres = 16, div;
#if (defined CONFIG_EXYNOS4 || defined CONFIG_EXYNOS5)
	freq = get_i2c_clk();
#else
	freq = get_PCLK();
#endif
	/* calculate prescaler and divisor values */
	if ((freq / pres / (16 + 1)) > speed)
		/* set prescaler to 512 */
		pres = 512;

	div = 0;
	while ((freq / pres / (div + 1)) > speed)
		div++;

	/* set prescaler, divisor according to freq, also set ACKGEN, IRQ */
	writel((div & 0x0F) | 0xA0 | ((pres == 512) ? 0x40 : 0), &i2c->iiccon);

	/* init to SLAVE REVEIVE and set slaveaddr */
	writel(0, &i2c->iicstat);
	writel(slaveadd, &i2c->iicadd);
	/* program Master Transmit (and implicit STOP) */
	writel(I2C_MODE_MT | I2C_TXRX_ENA, &i2c->iicstat);
}

/*
 * MULTI BUS I2C support
 */

#ifdef CONFIG_I2C_MULTI_BUS
int i2c_set_bus_num(unsigned int bus)
{
	struct s3c24x0_i2c *i2c;

	if ((bus < 0) || (bus >= CONFIG_MAX_I2C_NUM)) {
		debug("Bad bus: %d\n", bus);
		return -1;
	}

	g_current_bus = bus;
	i2c = get_base_i2c();
	i2c_ch_init(i2c, CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);

	return 0;
}

unsigned int i2c_get_bus_num(void)
{
	return g_current_bus;
}
#endif

void i2c_init(int speed, int slaveadd)
{
	int i;
	struct s3c24x0_i2c *i2c;
#if !(defined CONFIG_EXYNOS4 || defined CONFIG_EXYNOS5)
	struct s3c24x0_gpio *gpio = s3c24x0_get_base_gpio();
#endif
	ulong start_time = get_timer(0);

	/* By default i2c channel 0 is the current bus */
	g_current_bus = 0;
	i2c = get_base_i2c();

	/*
	 * In case the previous transfer is still going, wait to give it a
	 * chance to finish.
	 */
	while (readl(&i2c->iicstat) & I2CSTAT_BSY) {
		if (get_timer(start_time) > I2C_TIMEOUT_MS) {
			printf("%s: I2C bus busy for %p\n", __func__,
			       &i2c->iicstat);
			return;
		}
	}

#if !(defined CONFIG_EXYNOS4 || defined CONFIG_EXYNOS5)
	if ((readl(&i2c->iicstat) & I2CSTAT_BSY) || GetI2CSDA() == 0) {
#ifdef CONFIG_S3C2410
		ulong old_gpecon = readl(&gpio->gpecon);
#endif
#ifdef CONFIG_S3C2400
		ulong old_gpecon = readl(&gpio->pgcon);
#endif
		/* bus still busy probably by (most) previously interrupted
		   transfer */

#ifdef CONFIG_S3C2410
		/* set I2CSDA and I2CSCL (GPE15, GPE14) to GPIO */
		writel((readl(&gpio->gpecon) & ~0xF0000000) | 0x10000000,
		       &gpio->gpecon);
#endif
#ifdef CONFIG_S3C2400
		/* set I2CSDA and I2CSCL (PG5, PG6) to GPIO */
		writel((readl(&gpio->pgcon) & ~0x00003c00) | 0x00001000,
		       &gpio->pgcon);
#endif

		/* toggle I2CSCL until bus idle */
		SetI2CSCL(0);
		udelay(1000);
		i = 10;
		while ((i > 0) && (GetI2CSDA() != 1)) {
			SetI2CSCL(1);
			udelay(1000);
			SetI2CSCL(0);
			udelay(1000);
			i--;
		}
		SetI2CSCL(1);
		udelay(1000);

		/* restore pin functions */
#ifdef CONFIG_S3C2410
		writel(old_gpecon, &gpio->gpecon);
#endif
#ifdef CONFIG_S3C2400
		writel(old_gpecon, &gpio->pgcon);
#endif
	}
#endif /* #if !(defined CONFIG_EXYNOS4 || defined CONFIG_EXYNOS5) */
	i2c_ch_init(i2c, speed, slaveadd);
}

/*
 * cmd_type is 0 for write, 1 for read.
 *
 * addr_len can take any value from 0-255, it is only limited
 * by the char, we could make it larger if needed. If it is
 * 0 we skip the address write cycle.
 */
static int i2c_transfer(struct s3c24x0_i2c *i2c,
			unsigned char cmd_type,
			unsigned char chip,
			unsigned char addr[],
			unsigned char addr_len,
			unsigned char data[],
			unsigned short data_len)
{
	int i = 0, result;
	ulong start_time = get_timer(0);

	if (data == 0 || data_len == 0) {
		/*Don't support data transfer of no length or to address 0 */
		debug("i2c_transfer: bad call\n");
		return I2C_NOK;
	}

	while (readl(&i2c->iicstat) & I2CSTAT_BSY) {
		if (get_timer(start_time) > I2C_TIMEOUT_MS)
			return I2C_NOK_TOUT;
	}

	writel(readl(&i2c->iiccon) | I2CCON_ACKGEN, &i2c->iiccon);

	/* Get the slave chip address going */
	writel(chip, &i2c->iicds);
	if ((cmd_type == I2C_WRITE) || (addr && addr_len))
		writel(I2C_MODE_MT | I2C_TXRX_ENA | I2C_START_STOP,
		       &i2c->iicstat);
	else
		writel(I2C_MODE_MR | I2C_TXRX_ENA | I2C_START_STOP,
		       &i2c->iicstat);

	/* Wait for chip address to transmit. */
	result = WaitForXfer(i2c);
	if (result != I2C_OK)
		goto bailout;

	/* If register address needs to be transmitted - do it now. */
	if (addr && addr_len) {
		while ((i < addr_len) && (result == I2C_OK)) {
			writel(addr[i++], &i2c->iicds);
			ReadWriteByte(i2c);
			result = WaitForXfer(i2c);
		}
		i = 0;
		if (result != I2C_OK)
			goto bailout;
	}

	switch (cmd_type) {
	case I2C_WRITE:
		while ((i < data_len) && (result == I2C_OK)) {
			writel(data[i++], &i2c->iicds);
			ReadWriteByte(i2c);
			result = WaitForXfer(i2c);
		}
		break;

	case I2C_READ:
		if (addr && addr_len) {
			/*
			 * Register address has been sent, now send slave chip
			 * address again to start the actual read transaction.
			 */
			writel(chip, &i2c->iicds);

			/* Generate a re-START. */
			writel(I2C_MODE_MR | I2C_TXRX_ENA | I2C_START_STOP,
				&i2c->iicstat);
			ReadWriteByte(i2c);
			result = WaitForXfer(i2c);

			if (result != I2C_OK)
				goto bailout;
		}

		while ((i < data_len) && (result == I2C_OK)) {
			/* disable ACK for final READ */
			if (i == data_len - 1)
				writel(readl(&i2c->iiccon)
				       & ~I2CCON_ACKGEN,
				       &i2c->iiccon);
			ReadWriteByte(i2c);
			result = WaitForXfer(i2c);
			data[i++] = readl(&i2c->iicds);
		}
		if (result == I2C_NACK)
			result = I2C_OK; /* Normal terminated read. */
		break;

	default:
		debug("i2c_transfer: bad call\n");
		result = I2C_NOK;
		break;
	}

bailout:
	/* Send STOP. */
	writel(I2C_MODE_MR | I2C_TXRX_ENA, &i2c->iicstat);
	ReadWriteByte(i2c);

	return result;
}

int i2c_probe(uchar chip)
{
	struct s3c24x0_i2c *i2c;
	uchar buf[1];

	i2c = get_base_i2c();
	buf[0] = 0;

	/*
	 * What is needed is to send the chip address and verify that the
	 * address was <ACK>ed (i.e. there was a chip at that address which
	 * drove the data line low).
	 */
	return i2c_transfer(i2c, I2C_READ, chip << 1, 0, 0, buf, 1) != I2C_OK;
}

int i2c_read(uchar chip, uint addr, int alen, uchar *buffer, int len)
{
	struct s3c24x0_i2c *i2c;
	uchar xaddr[4];
	int ret;

	if (alen > 4) {
		debug("I2C read: addr len %d not supported\n", alen);
		return 1;
	}

	if (alen > 0) {
		xaddr[0] = (addr >> 24) & 0xFF;
		xaddr[1] = (addr >> 16) & 0xFF;
		xaddr[2] = (addr >> 8) & 0xFF;
		xaddr[3] = addr & 0xFF;
	}

#ifdef CONFIG_SYS_I2C_EEPROM_ADDR_OVERFLOW
	/*
	 * EEPROM chips that implement "address overflow" are ones
	 * like Catalyst 24WC04/08/16 which has 9/10/11 bits of
	 * address and the extra bits end up in the "chip address"
	 * bit slots. This makes a 24WC08 (1Kbyte) chip look like
	 * four 256 byte chips.
	 *
	 * Note that we consider the length of the address field to
	 * still be one byte because the extra address bits are
	 * hidden in the chip address.
	 */
	if (alen > 0)
		chip |= ((addr >> (alen * 8)) &
			 CONFIG_SYS_I2C_EEPROM_ADDR_OVERFLOW);
#endif
	i2c = get_base_i2c();
	ret = i2c_transfer(i2c, I2C_READ, chip << 1, &xaddr[4 - alen], alen,
			buffer, len);
	if (ret != 0) {
		debug("I2c read: failed %d\n", ret);
		return 1;
	}
	return 0;
}

int i2c_write(uchar chip, uint addr, int alen, uchar *buffer, int len)
{
	struct s3c24x0_i2c *i2c;
	uchar xaddr[4];

	if (alen > 4) {
		debug("I2C write: addr len %d not supported\n", alen);
		return 1;
	}

	if (alen > 0) {
		xaddr[0] = (addr >> 24) & 0xFF;
		xaddr[1] = (addr >> 16) & 0xFF;
		xaddr[2] = (addr >> 8) & 0xFF;
		xaddr[3] = addr & 0xFF;
	}
#ifdef CONFIG_SYS_I2C_EEPROM_ADDR_OVERFLOW
	/*
	 * EEPROM chips that implement "address overflow" are ones
	 * like Catalyst 24WC04/08/16 which has 9/10/11 bits of
	 * address and the extra bits end up in the "chip address"
	 * bit slots. This makes a 24WC08 (1Kbyte) chip look like
	 * four 256 byte chips.
	 *
	 * Note that we consider the length of the address field to
	 * still be one byte because the extra address bits are
	 * hidden in the chip address.
	 */
	if (alen > 0)
		chip |= ((addr >> (alen * 8)) &
			 CONFIG_SYS_I2C_EEPROM_ADDR_OVERFLOW);
#endif
	i2c = get_base_i2c();
	return (i2c_transfer
		(i2c, I2C_WRITE, chip << 1, &xaddr[4 - alen], alen, buffer,
		 len) != 0);
}

#ifdef CONFIG_OF_CONTROL
void board_i2c_init(const void *blob)
{
	int i;
	int node_list[CONFIG_MAX_I2C_NUM];
	int count;

	count = fdtdec_find_aliases_for_id(blob, "i2c",
		COMPAT_SAMSUNG_S3C2440_I2C, node_list,
		CONFIG_MAX_I2C_NUM);

	for (i = 0; i < count; i++) {
		struct s3c24x0_i2c_bus *bus;
		int node = node_list[i];

		if (node <= 0)
			continue;
		bus = &i2c_bus[i];
		bus->regs = (struct s3c24x0_i2c *)
			fdtdec_get_addr(blob, node, "reg");
		bus->id = pinmux_decode_periph_id(blob, node);
		bus->node = node;
		bus->bus_num = i2c_busses++;
		exynos_pinmux_config(bus->id, 0);
	}
}

static struct s3c24x0_i2c_bus *get_bus(unsigned int bus_idx)
{
	if (bus_idx < i2c_busses)
		return &i2c_bus[bus_idx];

	debug("Undefined bus: %d\n", bus_idx);
	return NULL;
}

int i2c_get_bus_num_fdt(int node)
{
	int i;

	for (i = 0; i < i2c_busses; i++) {
		if (node == i2c_bus[i].node)
			return i;
	}

	debug("%s: Can't find any matched I2C bus\n", __func__);
	return -1;
}

int i2c_reset_port_fdt(const void *blob, int node)
{
	struct s3c24x0_i2c_bus *i2c;
	int bus;

	bus = i2c_get_bus_num_fdt(node);
	if (bus < 0) {
		debug("could not get bus for node %d\n", node);
		return -1;
	}

	i2c = get_bus(bus);
	if (!i2c) {
		debug("get_bus() failed for node node %d\n", node);
		return -1;
	}

	i2c_ch_init(i2c->regs, CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);

	return 0;
}
#endif

#endif /* CONFIG_HARD_I2C */
