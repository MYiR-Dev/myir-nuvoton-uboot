/*
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

/*
 * Boot support
 */
#include <common.h>
#include <command.h>
#include <cmd_boot.h>
#include <cmd_autoscript.h>
#include <s_record.h>
#include <net.h>
#include <syscall.h>


#if (CONFIG_COMMANDS & CFG_CMD_LOADS)
static ulong load_serial (ulong offset);
static int read_record (char *buf, ulong len);
# if (CONFIG_COMMANDS & CFG_CMD_SAVES)
static int save_serial (ulong offset, ulong size);
static int write_record (char *buf);
# endif /* CFG_CMD_SAVES */

static int do_echo = 1;
#endif /* CFG_CMD_LOADS */


#if (CONFIG_COMMANDS & CFG_CMD_BDI)
static void print_num(const char *, ulong);

#ifndef CONFIG_ARM	/* PowerPC and other */

#ifdef CONFIG_PPC
static void print_str(const char *, const char *);

int do_bdinfo ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	DECLARE_GLOBAL_DATA_PTR;

	int i;
	bd_t *bd = gd->bd;
	char buf[32];

#ifdef DEBUG
	print_num ("bd address",    (ulong)bd		);
#endif
	print_num ("memstart",	    bd->bi_memstart	);
	print_num ("memsize",	    bd->bi_memsize	);
	print_num ("flashstart",    bd->bi_flashstart	);
	print_num ("flashsize",	    bd->bi_flashsize	);
	print_num ("flashoffset",   bd->bi_flashoffset	);
	print_num ("sramstart",	    bd->bi_sramstart	);
	print_num ("sramsize",	    bd->bi_sramsize	);
#if defined(CONFIG_5xx) || defined(CONFIG_8xx) || defined(CONFIG_8260)
	print_num ("immr_base",	    bd->bi_immr_base	);
#endif
	print_num ("bootflags",	    bd->bi_bootflags	);
#if defined(CONFIG_405GP) || defined(CONFIG_405CR) || defined(CONFIG_405EP)
	print_str ("procfreq",	    strmhz(buf, bd->bi_procfreq));
	print_str ("plb_busfreq",	    strmhz(buf, bd->bi_plb_busfreq));
#if defined(CONFIG_405GP) || defined(CONFIG_405EP)
	print_str ("pci_busfreq",	    strmhz(buf, bd->bi_pci_busfreq));
#endif
#else
#if defined(CONFIG_8260)
	print_str ("vco",	    strmhz(buf, bd->bi_vco));
	print_str ("sccfreq",	    strmhz(buf, bd->bi_sccfreq));
	print_str ("brgfreq",	    strmhz(buf, bd->bi_brgfreq));
#endif
	print_str ("intfreq",	    strmhz(buf, bd->bi_intfreq));
#if defined(CONFIG_8260)
	print_str ("cpmfreq",	    strmhz(buf, bd->bi_cpmfreq));
#endif
	print_str ("busfreq",	    strmhz(buf, bd->bi_busfreq));
#endif /* defined(CONFIG_405GP) || defined(CONFIG_405CR) */
	printf ("ethaddr     =");
	for (i=0; i<6; ++i) {
		printf ("%c%02X", i ? ':' : ' ', bd->bi_enetaddr[i]);
	}
#ifdef CONFIG_PN62
	printf ("\neth1addr    =");
	for (i=0; i<6; ++i) {
		printf ("%c%02X", i ? ':' : ' ', bd->bi_enet1addr[i]);
	}
#endif /* CONFIG_PN62 */
#ifdef CONFIG_HERMES
	print_str ("ethspeed",	    strmhz(buf, bd->bi_ethspeed));
#endif
	printf ("\nIP addr     = ");	print_IPaddr (bd->bi_ip_addr);
	printf ("\nbaudrate    = %6ld bps\n", bd->bi_baudrate   );
	return 0;
}

#else /* MIPS */

int do_bdinfo ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	DECLARE_GLOBAL_DATA_PTR;

	int i;
	bd_t *bd = gd->bd;

	print_num ("boot_params",	(ulong)bd->bi_boot_params);
	print_num ("memstart",		(ulong)bd->bi_memstart);
	print_num ("memsize",		(ulong)bd->bi_memsize);
	print_num ("flashstart",	(ulong)bd->bi_flashstart);
	print_num ("flashsize",		(ulong)bd->bi_flashsize);
	print_num ("flashoffset",	(ulong)bd->bi_flashoffset);

	printf ("ethaddr     =");
	for (i=0; i<6; ++i) {
		printf ("%c%02X", i ? ':' : ' ', bd->bi_enetaddr[i]);
	}
	printf ("\nip_addr     = ");
	print_IPaddr (bd->bi_ip_addr);
	printf ("\nbaudrate    = %d bps\n", bd->bi_baudrate);

	return 0;
}
#endif  /* MIPS */

#else	/* ARM */

int do_bdinfo ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	DECLARE_GLOBAL_DATA_PTR;

	int i;
	bd_t *bd = gd->bd;

	print_num ("arch_number",	bd->bi_arch_number);
	print_num ("env_t",		(ulong)bd->bi_env);
	print_num ("boot_params",	(ulong)bd->bi_boot_params);

	for (i=0; i<CONFIG_NR_DRAM_BANKS; ++i) {
		print_num("DRAM bank",	i);
		print_num("-> start",	bd->bi_dram[i].start);
		print_num("-> size",	bd->bi_dram[i].size);
	}

	printf ("ethaddr     =");
	for (i=0; i<6; ++i) {
		printf ("%c%02X", i ? ':' : ' ', bd->bi_enetaddr[i]);
	}
	printf ("\n"
		"ip_addr     = ");
	print_IPaddr (bd->bi_ip_addr);
	printf ("\n"
		"baudrate    = %d bps\n", bd->bi_baudrate);

	return 0;
}

#endif /* CONFIG_ARM XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX */

static void print_num(const char *name, ulong value)
{
	printf ("%-12s= 0x%08lX\n", name, value);
}

#ifdef CONFIG_PPC
static void print_str(const char *name, const char *str)
{
	printf ("%-12s= %6s MHz\n", name, str);
}
#endif	/* CONFIG_PPC */

#endif	/* CFG_CMD_BDI */

int do_go (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	ulong	addr, rc;
	int     rcode = 0;

	if (argc < 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	addr = simple_strtoul(argv[1], NULL, 16);

	printf ("## Starting application at 0x%08lX ...\n", addr);

	/*
	 * pass address parameter as argv[0] (aka command name),
	 * and all remaining args
	 */
	rc = ((ulong (*)(int, char *[]))addr) (--argc, &argv[1]);
	if (rc != 0) rcode = 1;

	printf ("## Application terminated, rc = 0x%lX\n", rc);
	return rcode;
}

#if (CONFIG_COMMANDS & CFG_CMD_LOADS)
int do_load_serial (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	ulong offset = 0;
	ulong addr;
	int i;
	char *env_echo;
	int rcode = 0;
#ifdef	CFG_LOADS_BAUD_CHANGE
	DECLARE_GLOBAL_DATA_PTR;
	int load_baudrate, current_baudrate;

	load_baudrate = current_baudrate = gd->baudrate;
#endif

	if (((env_echo = getenv("loads_echo")) != NULL) && (*env_echo == '1')) {
		do_echo = 1;
	} else {
		do_echo = 0;
	}

#ifdef	CFG_LOADS_BAUD_CHANGE
	if (argc >= 2) {
		offset = simple_strtoul(argv[1], NULL, 16);
	}
	if (argc == 3) {
		load_baudrate = (int)simple_strtoul(argv[2], NULL, 10);

		/* default to current baudrate */
		if (load_baudrate == 0)
			load_baudrate = current_baudrate;
	}
#else	/* ! CFG_LOADS_BAUD_CHANGE */
	if (argc == 2) {
		offset = simple_strtoul(argv[1], NULL, 16);
	}
#endif	/* CFG_LOADS_BAUD_CHANGE */

#ifdef	CFG_LOADS_BAUD_CHANGE
	if (load_baudrate != current_baudrate) {
		printf ("## Switch baudrate to %d bps and press ENTER ...\n",
			load_baudrate);
		udelay(50000);
		gd->baudrate = load_baudrate;
		serial_setbrg ();
		udelay(50000);
		for (;;) {
			if (getc() == '\r')
				break;
		}
	}
#endif	/* CFG_LOADS_BAUD_CHANGE */
	printf ("## Ready for S-Record download ...\n");

	addr = load_serial (offset);

	/*
	 * Gather any trailing characters (for instance, the ^D which
	 * is sent by 'cu' after sending a file), and give the
	 * box some time (100 * 1 ms)
	 */
	for (i=0; i<100; ++i) {
		if (serial_tstc()) {
			(void) serial_getc();
		}
		udelay(1000);
	}

	if (addr == ~0) {
		printf ("## S-Record download aborted\n");
		rcode = 1;
	} else {
		printf ("## Start Addr      = 0x%08lX\n", addr);
		load_addr = addr;
	}

#ifdef	CFG_LOADS_BAUD_CHANGE
	if (load_baudrate != current_baudrate) {
		printf ("## Switch baudrate to %d bps and press ESC ...\n",
			current_baudrate);
		udelay (50000);
		gd->baudrate = current_baudrate;
		serial_setbrg ();
		udelay (50000);
		for (;;) {
			if (getc() == 0x1B) /* ESC */
				break;
		}
	}
#endif
	return rcode;
}

static ulong
load_serial (ulong offset)
{
	char	record[SREC_MAXRECLEN + 1];	/* buffer for one S-Record	*/
	char	binbuf[SREC_MAXBINLEN];		/* buffer for binary data	*/
	int	binlen;				/* no. of data bytes in S-Rec.	*/
	int	type;				/* return code for record type	*/
	ulong	addr;				/* load address from S-Record	*/
	ulong	size;				/* number of bytes transferred	*/
	char	buf[32];
	ulong	store_addr;
	ulong	start_addr = ~0;
	ulong	end_addr   =  0;
	int	line_count =  0;

	while (read_record(record, SREC_MAXRECLEN + 1) >= 0) {
		type = srec_decode (record, &binlen, &addr, binbuf);

		if (type < 0) {
			return (~0);		/* Invalid S-Record		*/
		}

		switch (type) {
		case SREC_DATA2:
		case SREC_DATA3:
		case SREC_DATA4:
		    store_addr = addr + offset;
#ifndef CFG_NO_FLASH
		    if (addr2info(store_addr)) {
			int rc;

			rc = flash_write((uchar *)binbuf,store_addr,binlen);
			if (rc != 0) {
				flash_perror (rc);
				return (~0);
			}
		    } else
#endif
		    {
			memcpy ((char *)(store_addr), binbuf, binlen);
		    }
		    if ((store_addr) < start_addr)
			start_addr = store_addr;
		    if ((store_addr + binlen - 1) > end_addr)
			end_addr = store_addr + binlen - 1;
		    break;
		case SREC_END2:
		case SREC_END3:
		case SREC_END4:
		    udelay (10000);
		    size = end_addr - start_addr + 1;
		    printf ("\n"
			    "## First Load Addr = 0x%08lX\n"
			    "## Last  Load Addr = 0x%08lX\n"
			    "## Total Size      = 0x%08lX = %ld Bytes\n",
			    start_addr, end_addr, size, size
		    );
		    flush_cache (addr, size);
		    sprintf(buf, "%lX", size);
		    setenv("filesize", buf);
		    return (addr);
		case SREC_START:
		    break;
		default:
		    break;
		}
		if (!do_echo) {	/* print a '.' every 100 lines */
			if ((++line_count % 100) == 0)
				putc ('.');
		}
	}

	return (~0);			/* Download aborted		*/
}

static int
read_record (char *buf, ulong len)
{
	char *p;
	char c;

	--len;	/* always leave room for terminating '\0' byte */

	for (p=buf; p < buf+len; ++p) {
		c = serial_getc();		/* read character		*/
		if (do_echo)
			serial_putc (c);	/* ... and echo it		*/

		switch (c) {
		case '\r':
		case '\n':
			*p = '\0';
			return (p - buf);
		case '\0':
		case 0x03:			/* ^C - Control C		*/
			return (-1);
		default:
			*p = c;
		}

	    /* Check for the console hangup (if any different from serial) */
#ifdef CONFIG_PPC	/* we don't have syscall_tbl anywhere else */
	    if (syscall_tbl[SYSCALL_GETC] != serial_getc) {
		if (ctrlc()) {
		    return (-1);
		}
	    }
#endif
	}

	/* line too long - truncate */
	*p = '\0';
	return (p - buf);
}

#if (CONFIG_COMMANDS & CFG_CMD_SAVES)

int do_save_serial (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	ulong offset = 0;
	ulong size   = 0;
#ifdef	CFG_LOADS_BAUD_CHANGE
	DECLARE_GLOBAL_DATA_PTR;
	int save_baudrate, current_baudrate;

	save_baudrate = current_baudrate = gd->baudrate;
#endif

	if (argc >= 2) {
		offset = simple_strtoul(argv[1], NULL, 16);
	}
#ifdef	CFG_LOADS_BAUD_CHANGE
	if (argc >= 3) {
		size = simple_strtoul(argv[2], NULL, 16);
	}
	if (argc == 4) {
		save_baudrate = (int)simple_strtoul(argv[3], NULL, 10);

		/* default to current baudrate */
		if (save_baudrate == 0)
			save_baudrate = current_baudrate;
	}
#else	/* ! CFG_LOADS_BAUD_CHANGE */
	if (argc == 3) {
		size = simple_strtoul(argv[2], NULL, 16);
	}
#endif	/* CFG_LOADS_BAUD_CHANGE */

#ifdef	CFG_LOADS_BAUD_CHANGE
	if (save_baudrate != current_baudrate) {
		printf ("## Switch baudrate to %d bps and press ENTER ...\n",
			save_baudrate);
		udelay(50000);
		gd->baudrate = save_baudrate;
		serial_setbrg ();
		udelay(50000);
		for (;;) {
			if (getc() == '\r')
				break;
		}
	}
#endif	/* CFG_LOADS_BAUD_CHANGE */
	printf ("## Ready for S-Record upload, press ENTER to proceed ...\n");
	for (;;) {
		if (getc() == '\r')
			break;
	}
	if(save_serial (offset, size)) {
		printf ("## S-Record upload aborted\n");
	} else {
		printf ("## S-Record upload complete\n");
	}
#ifdef	CFG_LOADS_BAUD_CHANGE
	if (save_baudrate != current_baudrate) {
		printf ("## Switch baudrate to %d bps and press ESC ...\n",
			(int)current_baudrate);
		udelay (50000);
		gd->baudrate = current_baudrate;
		serial_setbrg ();
		udelay (50000);
		for (;;) {
			if (getc() == 0x1B) /* ESC */
				break;
		}
	}
#endif
	return 0;
}

#define SREC3_START				"S0030000FC\n"
#define SREC3_FORMAT			"S3%02X%08lX%s%02X\n"
#define SREC3_END				"S70500000000FA\n"
#define SREC_BYTES_PER_RECORD	16

static int save_serial (ulong address, ulong count)
{
	int i, c, reclen, checksum, length;
	char *hex = "0123456789ABCDEF";
	char	record[2*SREC_BYTES_PER_RECORD+16];	/* buffer for one S-Record	*/
	char	data[2*SREC_BYTES_PER_RECORD+1];	/* buffer for hex data	*/

	reclen = 0;
	checksum  = 0;

	if(write_record(SREC3_START))			/* write the header */
		return (-1);
	do {
		if(count) {						/* collect hex data in the buffer  */
			c = *(volatile uchar*)(address + reclen);	/* get one byte    */
			checksum += c;							/* accumulate checksum */
			data[2*reclen]   = hex[(c>>4)&0x0f];
			data[2*reclen+1] = hex[c & 0x0f];
			data[2*reclen+2] = '\0';
			++reclen;
			--count;
		}
		if(reclen == SREC_BYTES_PER_RECORD || count == 0) {
			/* enough data collected for one record: dump it */
			if(reclen) {	/* build & write a data record: */
				/* address + data + checksum */
				length = 4 + reclen + 1;

				/* accumulate length bytes into checksum */
				for(i = 0; i < 2; i++)
					checksum += (length >> (8*i)) & 0xff;

				/* accumulate address bytes into checksum: */
				for(i = 0; i < 4; i++)
					checksum += (address >> (8*i)) & 0xff;

				/* make proper checksum byte: */
				checksum = ~checksum & 0xff;

				/* output one record: */
				sprintf(record, SREC3_FORMAT, length, address, data, checksum);
				if(write_record(record))
					return (-1);
			}
			address  += reclen;  /* increment address */
			checksum  = 0;
			reclen    = 0;
		}
	}
	while(count);
	if(write_record(SREC3_END))	/* write the final record */
		return (-1);
	return(0);
}

static int
write_record (char *buf)
{
	char c;

	while((c = *buf++))
		serial_putc(c);

	/* Check for the console hangup (if any different from serial) */

	if (ctrlc()) {
	    return (-1);
	}
	return (0);
}
# endif /* CFG_CMD_SAVES */

#endif	/* CFG_CMD_LOADS */


#if (CONFIG_COMMANDS & CFG_CMD_LOADB)  /* loadb command (load binary) included */

#define XON_CHAR        17
#define XOFF_CHAR       19
#define START_CHAR      0x01
#define ETX_CHAR	0x03
#define END_CHAR        0x0D
#define SPACE           0x20
#define K_ESCAPE        0x23
#define SEND_TYPE       'S'
#define DATA_TYPE       'D'
#define ACK_TYPE        'Y'
#define NACK_TYPE       'N'
#define BREAK_TYPE      'B'
#define tochar(x) ((char) (((x) + SPACE) & 0xff))
#define untochar(x) ((int) (((x) - SPACE) & 0xff))

extern int os_data_count;
extern int os_data_header[8];

static void set_kerm_bin_mode(unsigned long *);
static int k_recv(void);
static ulong load_serial_bin (ulong offset);


char his_eol;        /* character he needs at end of packet */
int  his_pad_count;  /* number of pad chars he needs */
char his_pad_char;   /* pad chars he needs */
char his_quote;      /* quote chars he'll use */

int do_load_serial_bin (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	DECLARE_GLOBAL_DATA_PTR;

	ulong offset = 0;
	ulong addr;
	int load_baudrate, current_baudrate;
	int rcode = 0;
	char *s;

	/* pre-set offset from CFG_LOAD_ADDR */
	offset = CFG_LOAD_ADDR;

	/* pre-set offset from $loadaddr */
	if ((s = getenv("loadaddr")) != NULL) {
		offset = simple_strtoul(s, NULL, 16);
	}

	load_baudrate = current_baudrate = gd->baudrate;

	if (argc >= 2) {
		offset = simple_strtoul(argv[1], NULL, 16);
	}
	if (argc == 3) {
		load_baudrate = (int)simple_strtoul(argv[2], NULL, 10);

		/* default to current baudrate */
		if (load_baudrate == 0)
			load_baudrate = current_baudrate;
	}

	if (load_baudrate != current_baudrate) {
		printf ("## Switch baudrate to %d bps and press ENTER ...\n",
			load_baudrate);
		udelay(50000);
		gd->baudrate = load_baudrate;
		serial_setbrg ();
		udelay(50000);
		for (;;) {
			if (getc() == '\r')
				break;
		}
	}

	printf ("## Ready for binary (kermit) download "
		"to 0x%08lX at %d bps...\n",
		offset,
		current_baudrate);
	addr = load_serial_bin (offset);

	if (addr == ~0) {
		load_addr = 0;
		printf ("## Binary (kermit) download aborted\n");
		rcode = 1;
	} else {
		printf ("## Start Addr      = 0x%08lX\n", addr);
		load_addr = addr;
	}

	if (load_baudrate != current_baudrate) {
		printf ("## Switch baudrate to %d bps and press ESC ...\n",
			current_baudrate);
		udelay (50000);
		gd->baudrate = current_baudrate;
		serial_setbrg ();
		udelay (50000);
		for (;;) {
			if (getc() == 0x1B) /* ESC */
				break;
		}
	}

#ifdef CONFIG_AUTOSCRIPT
	if (load_addr) {
		char *s;

		if (((s = getenv("autoscript")) != NULL) && (strcmp(s,"yes") == 0)) {
			printf("Running autoscript at addr 0x%08lX ...\n", load_addr);
			rcode = autoscript (load_addr);
		}
	}
#endif
	return rcode;
}


static ulong load_serial_bin (ulong offset)
{
	int size, i;
	char buf[32];

	set_kerm_bin_mode ((ulong *) offset);
	size = k_recv ();

	/*
	 * Gather any trailing characters (for instance, the ^D which
	 * is sent by 'cu' after sending a file), and give the
	 * box some time (100 * 1 ms)
	 */
	for (i=0; i<100; ++i) {
		if (serial_tstc()) {
			(void) serial_getc();
		}
		udelay(1000);
	}
	
	flush_cache (offset, size);

	printf("## Total Size      = 0x%08x = %d Bytes\n", size, size);
	sprintf(buf, "%X", size);
	setenv("filesize", buf);

	return offset;
}

void send_pad (void)
{
	int count = his_pad_count;

	while (count-- > 0)
		serial_putc (his_pad_char);
}

/* converts escaped kermit char to binary char */
char ktrans (char in)
{
	if ((in & 0x60) == 0x40) {
		return (char) (in & ~0x40);
	} else if ((in & 0x7f) == 0x3f) {
		return (char) (in | 0x40);
	} else
		return in;
}

int chk1 (char *buffer)
{
	int total = 0;

	while (*buffer) {
		total += *buffer++;
	}
	return (int) ((total + ((total >> 6) & 0x03)) & 0x3f);
}

void s1_sendpacket (char *packet)
{
	send_pad ();
	while (*packet) {
		serial_putc (*packet++);
	}
}

static char a_b[24];
void send_ack (int n)
{
	a_b[0] = START_CHAR;
	a_b[1] = tochar (3);
	a_b[2] = tochar (n);
	a_b[3] = ACK_TYPE;
	a_b[4] = '\0';
	a_b[4] = tochar (chk1 (&a_b[1]));
	a_b[5] = his_eol;
	a_b[6] = '\0';
	s1_sendpacket (a_b);
}

void send_nack (int n)
{
	a_b[0] = START_CHAR;
	a_b[1] = tochar (3);
	a_b[2] = tochar (n);
	a_b[3] = NACK_TYPE;
	a_b[4] = '\0';
	a_b[4] = tochar (chk1 (&a_b[1]));
	a_b[5] = his_eol;
	a_b[6] = '\0';
	s1_sendpacket (a_b);
}



/* os_data_* takes an OS Open image and puts it into memory, and
   puts the boot header in an array named os_data_header

   if image is binary, no header is stored in os_data_header.
*/
void (*os_data_init) (void);
void (*os_data_char) (char new_char);
static int os_data_state, os_data_state_saved;
int os_data_count;
static int os_data_count_saved;
static char *os_data_addr, *os_data_addr_saved;
static char *bin_start_address;
int os_data_header[8];
static void bin_data_init (void)
{
	os_data_state = 0;
	os_data_count = 0;
	os_data_addr = bin_start_address;
}
static void os_data_save (void)
{
	os_data_state_saved = os_data_state;
	os_data_count_saved = os_data_count;
	os_data_addr_saved = os_data_addr;
}
static void os_data_restore (void)
{
	os_data_state = os_data_state_saved;
	os_data_count = os_data_count_saved;
	os_data_addr = os_data_addr_saved;
}
static void bin_data_char (char new_char)
{
	switch (os_data_state) {
	case 0:					/* data */
		*os_data_addr++ = new_char;
		--os_data_count;
		break;
	}
}
static void set_kerm_bin_mode (unsigned long *addr)
{
	bin_start_address = (char *) addr;
	os_data_init = bin_data_init;
	os_data_char = bin_data_char;
}


/* k_data_* simply handles the kermit escape translations */
static int k_data_escape, k_data_escape_saved;
void k_data_init (void)
{
	k_data_escape = 0;
	os_data_init ();
}
void k_data_save (void)
{
	k_data_escape_saved = k_data_escape;
	os_data_save ();
}
void k_data_restore (void)
{
	k_data_escape = k_data_escape_saved;
	os_data_restore ();
}
void k_data_char (char new_char)
{
	if (k_data_escape) {
		/* last char was escape - translate this character */
		os_data_char (ktrans (new_char));
		k_data_escape = 0;
	} else {
		if (new_char == his_quote) {
			/* this char is escape - remember */
			k_data_escape = 1;
		} else {
			/* otherwise send this char as-is */
			os_data_char (new_char);
		}
	}
}

#define SEND_DATA_SIZE  20
char send_parms[SEND_DATA_SIZE];
char *send_ptr;

/* handle_send_packet interprits the protocol info and builds and
   sends an appropriate ack for what we can do */
void handle_send_packet (int n)
{
	int length = 3;
	int bytes;

	/* initialize some protocol parameters */
	his_eol = END_CHAR;		/* default end of line character */
	his_pad_count = 0;
	his_pad_char = '\0';
	his_quote = K_ESCAPE;

	/* ignore last character if it filled the buffer */
	if (send_ptr == &send_parms[SEND_DATA_SIZE - 1])
		--send_ptr;
	bytes = send_ptr - send_parms;	/* how many bytes we'll process */
	do {
		if (bytes-- <= 0)
			break;
		/* handle MAXL - max length */
		/* ignore what he says - most I'll take (here) is 94 */
		a_b[++length] = tochar (94);
		if (bytes-- <= 0)
			break;
		/* handle TIME - time you should wait for my packets */
		/* ignore what he says - don't wait for my ack longer than 1 second */
		a_b[++length] = tochar (1);
		if (bytes-- <= 0)
			break;
		/* handle NPAD - number of pad chars I need */
		/* remember what he says - I need none */
		his_pad_count = untochar (send_parms[2]);
		a_b[++length] = tochar (0);
		if (bytes-- <= 0)
			break;
		/* handle PADC - pad chars I need */
		/* remember what he says - I need none */
		his_pad_char = ktrans (send_parms[3]);
		a_b[++length] = 0x40;	/* He should ignore this */
		if (bytes-- <= 0)
			break;
		/* handle EOL - end of line he needs */
		/* remember what he says - I need CR */
		his_eol = untochar (send_parms[4]);
		a_b[++length] = tochar (END_CHAR);
		if (bytes-- <= 0)
			break;
		/* handle QCTL - quote control char he'll use */
		/* remember what he says - I'll use '#' */
		his_quote = send_parms[5];
		a_b[++length] = '#';
		if (bytes-- <= 0)
			break;
		/* handle QBIN - 8-th bit prefixing */
		/* ignore what he says - I refuse */
		a_b[++length] = 'N';
		if (bytes-- <= 0)
			break;
		/* handle CHKT - the clock check type */
		/* ignore what he says - I do type 1 (for now) */
		a_b[++length] = '1';
		if (bytes-- <= 0)
			break;
		/* handle REPT - the repeat prefix */
		/* ignore what he says - I refuse (for now) */
		a_b[++length] = 'N';
		if (bytes-- <= 0)
			break;
		/* handle CAPAS - the capabilities mask */
		/* ignore what he says - I only do long packets - I don't do windows */
		a_b[++length] = tochar (2);	/* only long packets */
		a_b[++length] = tochar (0);	/* no windows */
		a_b[++length] = tochar (94);	/* large packet msb */
		a_b[++length] = tochar (94);	/* large packet lsb */
	} while (0);

	a_b[0] = START_CHAR;
	a_b[1] = tochar (length);
	a_b[2] = tochar (n);
	a_b[3] = ACK_TYPE;
	a_b[++length] = '\0';
	a_b[length] = tochar (chk1 (&a_b[1]));
	a_b[++length] = his_eol;
	a_b[++length] = '\0';
	s1_sendpacket (a_b);
}

/* k_recv receives a OS Open image file over kermit line */
static int k_recv (void)
{
	char new_char;
	char k_state, k_state_saved;
	int sum;
	int done;
	int length;
	int n, last_n;
	int z = 0;
	int len_lo, len_hi;

	/* initialize some protocol parameters */
	his_eol = END_CHAR;		/* default end of line character */
	his_pad_count = 0;
	his_pad_char = '\0';
	his_quote = K_ESCAPE;

	/* initialize the k_recv and k_data state machine */
	done = 0;
	k_state = 0;
	k_data_init ();
	k_state_saved = k_state;
	k_data_save ();
	n = 0;				/* just to get rid of a warning */
	last_n = -1;

	/* expect this "type" sequence (but don't check):
	   S: send initiate
	   F: file header
	   D: data (multiple)
	   Z: end of file
	   B: break transmission
	 */

	/* enter main loop */
	while (!done) {
		/* set the send packet pointer to begining of send packet parms */
		send_ptr = send_parms;

		/* With each packet, start summing the bytes starting with the length.
		   Save the current sequence number.
		   Note the type of the packet.
		   If a character less than SPACE (0x20) is received - error.
		 */

#if 0
		/* OLD CODE, Prior to checking sequence numbers */
		/* first have all state machines save current states */
		k_state_saved = k_state;
		k_data_save ();
#endif

		/* get a packet */
		/* wait for the starting character or ^C */
		for (;;) {
			switch (serial_getc ()) {
			case START_CHAR:	/* start packet */
				goto START;
			case ETX_CHAR:		/* ^C waiting for packet */
				return (0);
			default:
				;
			}
		}
START:
		/* get length of packet */
		sum = 0;
		new_char = serial_getc ();
		if ((new_char & 0xE0) == 0)
			goto packet_error;
		sum += new_char & 0xff;
		length = untochar (new_char);
		/* get sequence number */
		new_char = serial_getc ();
		if ((new_char & 0xE0) == 0)
			goto packet_error;
		sum += new_char & 0xff;
		n = untochar (new_char);
		--length;

		/* NEW CODE - check sequence numbers for retried packets */
		/* Note - this new code assumes that the sequence number is correctly
		 * received.  Handling an invalid sequence number adds another layer
		 * of complexity that may not be needed - yet!  At this time, I'm hoping
		 * that I don't need to buffer the incoming data packets and can write
		 * the data into memory in real time.
		 */
		if (n == last_n) {
			/* same sequence number, restore the previous state */
			k_state = k_state_saved;
			k_data_restore ();
		} else {
			/* new sequence number, checkpoint the download */
			last_n = n;
			k_state_saved = k_state;
			k_data_save ();
		}
		/* END NEW CODE */

		/* get packet type */
		new_char = serial_getc ();
		if ((new_char & 0xE0) == 0)
			goto packet_error;
		sum += new_char & 0xff;
		k_state = new_char;
		--length;
		/* check for extended length */
		if (length == -2) {
			/* (length byte was 0, decremented twice) */
			/* get the two length bytes */
			new_char = serial_getc ();
			if ((new_char & 0xE0) == 0)
				goto packet_error;
			sum += new_char & 0xff;
			len_hi = untochar (new_char);
			new_char = serial_getc ();
			if ((new_char & 0xE0) == 0)
				goto packet_error;
			sum += new_char & 0xff;
			len_lo = untochar (new_char);
			length = len_hi * 95 + len_lo;
			/* check header checksum */
			new_char = serial_getc ();
			if ((new_char & 0xE0) == 0)
				goto packet_error;
			if (new_char != tochar ((sum + ((sum >> 6) & 0x03)) & 0x3f))
				goto packet_error;
			sum += new_char & 0xff;
/* --length; */ /* new length includes only data and block check to come */
		}
		/* bring in rest of packet */
		while (length > 1) {
			new_char = serial_getc ();
			if ((new_char & 0xE0) == 0)
				goto packet_error;
			sum += new_char & 0xff;
			--length;
			if (k_state == DATA_TYPE) {
				/* pass on the data if this is a data packet */
				k_data_char (new_char);
			} else if (k_state == SEND_TYPE) {
				/* save send pack in buffer as is */
				*send_ptr++ = new_char;
				/* if too much data, back off the pointer */
				if (send_ptr >= &send_parms[SEND_DATA_SIZE])
					--send_ptr;
			}
		}
		/* get and validate checksum character */
		new_char = serial_getc ();
		if ((new_char & 0xE0) == 0)
			goto packet_error;
		if (new_char != tochar ((sum + ((sum >> 6) & 0x03)) & 0x3f))
			goto packet_error;
		/* get END_CHAR */
		new_char = serial_getc ();
		if (new_char != END_CHAR) {
		  packet_error:
			/* restore state machines */
			k_state = k_state_saved;
			k_data_restore ();
			/* send a negative acknowledge packet in */
			send_nack (n);
		} else if (k_state == SEND_TYPE) {
			/* crack the protocol parms, build an appropriate ack packet */
			handle_send_packet (n);
		} else {
			/* send simple acknowledge packet in */
			send_ack (n);
			/* quit if end of transmission */
			if (k_state == BREAK_TYPE)
				done = 1;
		}
		++z;
	}
	return ((ulong) os_data_addr - (ulong) bin_start_address);
}
#endif	/* CFG_CMD_LOADB */
#if (CONFIG_COMMANDS & CFG_CMD_HWFLOW)
int do_hwflow (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	extern int hwflow_onoff(int);

	if (argc == 2) {
		if (strcmp(argv[1], "off") == 0)
			hwflow_onoff(-1);
		else
			if (strcmp(argv[1], "on") == 0)
				hwflow_onoff(1);
			else
				printf("Usage: %s\n", cmdtp->usage);
	}
	printf("RTS/CTS hardware flow control: %s\n", hwflow_onoff(0) ? "on" : "off");
	return 0;
}
#endif /* CFG_CMD_HWFLOW */
