/*
 * (C) Copyright 2000, 2001
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
 *
 */

#include <common.h>
#include <config.h>
#include <command.h>
#include <i2c.h>

#if (CONFIG_COMMANDS & CFG_CMD_EEPROM) || defined(CFG_ENV_IS_IN_EEPROM)

extern void eeprom_init  (void);
extern int  eeprom_read  (unsigned dev_addr, unsigned offset,
			  uchar *buffer, unsigned cnt);
extern int  eeprom_write (unsigned dev_addr, unsigned offset,
			  uchar *buffer, unsigned cnt);
#endif


#if defined(CFG_EEPROM_X40430)
	/* Maximum number of times to poll for acknowledge after write */
#define MAX_ACKNOWLEDGE_POLLS	10
#endif

/* ------------------------------------------------------------------------- */

#if (CONFIG_COMMANDS & CFG_CMD_EEPROM)
int do_eeprom ( cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	const char *const fmt =
		"\nEEPROM @0x%lX %s: addr %08lx  off %04lx  count %ld ... ";

#if defined(CFG_I2C_MULTI_EEPROMS)
	if (argc == 6) {
		ulong dev_addr = simple_strtoul (argv[2], NULL, 16);
		ulong addr = simple_strtoul (argv[3], NULL, 16);
		ulong off  = simple_strtoul (argv[4], NULL, 16);
		ulong cnt  = simple_strtoul (argv[5], NULL, 16);
#else
	if (argc == 5) {
		ulong dev_addr = CFG_DEF_EEPROM_ADDR;
		ulong addr = simple_strtoul (argv[2], NULL, 16);
		ulong off  = simple_strtoul (argv[3], NULL, 16);
		ulong cnt  = simple_strtoul (argv[4], NULL, 16);
#endif /* CFG_I2C_MULTI_EEPROMS */

# ifndef CONFIG_SPI
		eeprom_init ();
# endif /* !CONFIG_SPI */

		if (strcmp (argv[1], "read") == 0) {
			int rcode;

			printf (fmt, dev_addr, argv[1], addr, off, cnt);

			rcode = eeprom_read (dev_addr, off, (uchar *) addr, cnt);

			printf ("done\n");
			return rcode;
		} else if (strcmp (argv[1], "write") == 0) {
			int rcode;

			printf (fmt, dev_addr, argv[1], addr, off, cnt);

			rcode = eeprom_write (dev_addr, off, (uchar *) addr, cnt);

			printf ("done\n");
			return rcode;
		}
	}

	printf ("Usage:\n%s\n", cmdtp->usage);
	return 1;
}
#endif	/* CFG_CMD_EEPROM */

/*-----------------------------------------------------------------------
 *
 * for CFG_I2C_EEPROM_ADDR_LEN == 2 (16-bit EEPROM address) offset is
 *   0x000nxxxx for EEPROM address selectors at n, offset xxxx in EEPROM.
 *
 * for CFG_I2C_EEPROM_ADDR_LEN == 1 (8-bit EEPROM page address) offset is
 *   0x00000nxx for EEPROM address selectors and page number at n.
 */

#if (CONFIG_COMMANDS & CFG_CMD_EEPROM) || defined(CFG_ENV_IS_IN_EEPROM)

#ifndef CONFIG_SPI
#if !defined(CFG_I2C_EEPROM_ADDR_LEN) || CFG_I2C_EEPROM_ADDR_LEN < 1 || CFG_I2C_EEPROM_ADDR_LEN > 2
#error CFG_I2C_EEPROM_ADDR_LEN must be 1 or 2
#endif
#endif

int eeprom_read (unsigned dev_addr, unsigned offset, uchar *buffer, unsigned cnt)
{
	unsigned end = offset + cnt;
	unsigned blk_off;
	int rcode = 0;

	/* Read data until done or would cross a page boundary.
	 * We must write the address again when changing pages
	 * because the next page may be in a different device.
	 */
	while (offset < end) {
		unsigned alen, len, maxlen;
#if CFG_I2C_EEPROM_ADDR_LEN == 1 && !defined(CONFIG_SPI_X)
		uchar addr[2];

		blk_off = offset & 0xFF;	/* block offset */

		addr[0] = offset >> 8;		/* block number */
		addr[1] = blk_off;		/* block offset */
		alen	= 2;
#else
		uchar addr[3];

		blk_off = offset & 0xFF;	/* block offset */

		addr[0] = offset >> 16;		/* block number */
		addr[1] = offset >>  8;		/* upper address octet */
		addr[2] = blk_off;		/* lower address octet */
		alen	= 3;
#endif	/* CFG_I2C_EEPROM_ADDR_LEN, CONFIG_SPI_X */

		addr[0] |= dev_addr;		/* insert device address */

		maxlen = 0x100 - blk_off;
		if (maxlen > I2C_RXTX_LEN)
			maxlen = I2C_RXTX_LEN;
		len    = end - offset;
		if (len > maxlen)
			len = maxlen;
#ifdef CONFIG_SPI
		spi_read (addr, alen, buffer, len);
#else
		if (i2c_read (addr[0], offset, alen-1, buffer, len) != 0)
			rcode = 1;
#endif
		buffer += len;
		offset += len;
	}
	return rcode;
}

/*-----------------------------------------------------------------------
 *
 * for CFG_I2C_EEPROM_ADDR_LEN == 2 (16-bit EEPROM address) offset is
 *   0x000nxxxx for EEPROM address selectors at n, offset xxxx in EEPROM.
 *
 * for CFG_I2C_EEPROM_ADDR_LEN == 1 (8-bit EEPROM page address) offset is
 *   0x00000nxx for EEPROM address selectors and page number at n.
 */

int eeprom_write (unsigned dev_addr, unsigned offset, uchar *buffer, unsigned cnt)
{
	unsigned end = offset + cnt;
	unsigned blk_off;
	int rcode = 0;

#if defined(CFG_EEPROM_X40430)
	uchar	contr_r_addr[2];
	uchar	addr_void[2];
	uchar	contr_reg[2];
	uchar	ctrl_reg_v;
	int	i;
#endif

	/* Write data until done or would cross a write page boundary.
	 * We must write the address again when changing pages
	 * because the address counter only increments within a page.
	 */

	while (offset < end) {
		unsigned alen, len, maxlen;
#if CFG_I2C_EEPROM_ADDR_LEN == 1 && !defined(CONFIG_SPI_X)
		uchar addr[2];

		blk_off = offset & 0xFF;	/* block offset */

		addr[0] = offset >> 8;		/* block number */
		addr[1] = blk_off;		/* block offset */
		alen	= 2;
#else
		uchar addr[3];

		blk_off = offset & 0xFF;	/* block offset */

		addr[0] = offset >> 16;		/* block number */
		addr[1] = offset >>  8;		/* upper address octet */
		addr[2] = blk_off;		/* lower address octet */
		alen	= 3;
#endif	/* CFG_I2C_EEPROM_ADDR_LEN, CONFIG_SPI_X */

		addr[0] |= dev_addr;		/* insert device address */

#if defined(CFG_EEPROM_PAGE_WRITE_BITS)

#define	EEPROM_PAGE_SIZE	(1 << CFG_EEPROM_PAGE_WRITE_BITS)
#define	EEPROM_PAGE_OFFSET(x)	((x) & (EEPROM_PAGE_SIZE - 1))

		maxlen = EEPROM_PAGE_SIZE - EEPROM_PAGE_OFFSET(blk_off);
#else
		maxlen = 0x100 - blk_off;
#endif
		if (maxlen > I2C_RXTX_LEN)
			maxlen = I2C_RXTX_LEN;

		len = end - offset;
		if (len > maxlen)
			len = maxlen;
#ifdef CONFIG_SPI
		spi_write (addr, alen, buffer, len);
#else
#if defined(CFG_EEPROM_X40430)
		/* Get the value of the control register.
		 * Set current address (internal pointer in the x40430)
		 * to 0x1ff.
		 */
		contr_r_addr[0] = 9;
		contr_r_addr[1] = 0xff;
		addr_void[0]    = 0;
		addr_void[1]    = addr[1];
#ifdef CFG_I2C_EEPROM_ADDR
		contr_r_addr[0] |= CFG_I2C_EEPROM_ADDR;
		addr_void[0]    |= CFG_I2C_EEPROM_ADDR;
#endif
		contr_reg[0] = 0xff;
		if (i2c_read (contr_r_addr[0], contr_r_addr[1], 1, contr_reg, 1) != 0) {
			rcode = 1;
		}
		ctrl_reg_v = contr_reg[0];

		/* Are any of the eeprom blocks write protected?
		 */
		if (ctrl_reg_v & 0x18) {
			ctrl_reg_v &= ~0x18;   /* reset block protect bits  */
			ctrl_reg_v |=  0x02;   /* set write enable latch    */
			ctrl_reg_v &= ~0x04;   /* clear RWEL                */

			/* Set write enable latch.
			 */
			contr_reg[0] = 0x02;
			if (i2c_write (contr_r_addr[0], 0xff, 1, contr_reg, 1) != 0) {
				rcode = 1;
			}

			/* Set register write enable latch.
			 */
			contr_reg[0] = 0x06;
			if (i2c_write (contr_r_addr[0], 0xFF, 1, contr_reg, 1) != 0) {
				rcode = 1;
			}

			/* Modify ctrl register.
			 */
			contr_reg[0] = ctrl_reg_v;
			if (i2c_write (contr_r_addr[0], 0xFF, 1, contr_reg, 1) != 0) {
				rcode = 1;
			}

			/* The write (above) is an operation on NV memory.
			 * These can take some time (~5ms), and the device
			 * will not respond to further I2C messages till
			 * it's completed the write.
			 * So poll device for an I2C acknowledge.
			 * When we get one we know we can continue with other
			 * operations.
			 */
			contr_reg[0] = 0;
			for (i = 0; i < MAX_ACKNOWLEDGE_POLLS; i++) {
				if (i2c_read (addr_void[0], addr_void[1], 1, contr_reg, 1) == 0)
					break;	/* got ack */
#if defined(CFG_EEPROM_PAGE_WRITE_DELAY_MS)
				udelay(CFG_EEPROM_PAGE_WRITE_DELAY_MS * 1000);
#endif
			}
			if (i == MAX_ACKNOWLEDGE_POLLS) {
				printf("EEPROM poll acknowledge failed\n");
				rcode = 1;
			}
		}

		/* Is the write enable latch on?.
		 */
		else if (!(ctrl_reg_v & 0x02)) {
			/* Set write enable latch.
			 */
			contr_reg[0] = 0x02;
			if (i2c_write (contr_r_addr[0], 0xFF, 1, contr_reg, 1) != 0) {
			       rcode = 1;
			}
		}
		/* Write is enabled ... now write eeprom value.
		 */
#endif
		if (i2c_write (addr[0], offset, alen-1, buffer, len) != 0)
			rcode = 1;

#endif
		buffer += len;
		offset += len;

#if defined(CFG_EEPROM_PAGE_WRITE_DELAY_MS)
		udelay(CFG_EEPROM_PAGE_WRITE_DELAY_MS * 1000);
#endif
	}
	return rcode;
}

/*-----------------------------------------------------------------------
 * Set default values
 */
#ifndef	CFG_I2C_SPEED
#define	CFG_I2C_SPEED	50000
#endif

#ifndef	CFG_I2C_SLAVE
#define	CFG_I2C_SLAVE	0xFE
#endif

void eeprom_init  (void)
{
#if defined(CONFIG_SPI)
	spi_init_f ();
#endif
#if defined(CONFIG_HARD_I2C) || \
    defined(CONFIG_SOFT_I2C)
	i2c_init (CFG_I2C_SPEED, CFG_I2C_SLAVE);
#endif
}
/*-----------------------------------------------------------------------
 */
#endif	/* CFG_CMD_EEPROM */
