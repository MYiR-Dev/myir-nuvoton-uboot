/*
 * (C) Copyright 2000
 * Sangmoon Kim, Etin Systems. dogoil@etinsys.com.
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
#include <mpc824x.h>
#include <pci.h>

int checkboard (void)
{
	/*TODO: Check processor type */

	puts (	"Board: Debris "
#ifdef CONFIG_MPC8240
		"8240"
#endif
#ifdef CONFIG_MPC8245
		"8245"
#endif
		" ##Test not implemented yet##\n");
	return 0;
}

#if 0 	/* NOT USED */
int checkflash (void)
{
	/* TODO: XXX XXX XXX */
	printf ("## Test not implemented yet ##\n");

	return (0);
}
#endif

long int initdram (int board_type)
{
	long size;
#if 0
	long new_bank0_end;
	long mear1;
	long emear1;
#endif

	size = get_ram_size(CFG_SDRAM_BASE, CFG_MAX_RAM_SIZE);

#if 0
	new_bank0_end = size - 1;
	mear1 = mpc824x_mpc107_getreg(MEAR1);
	emear1 = mpc824x_mpc107_getreg(EMEAR1);
	mear1 = (mear1  & 0xFFFFFF00) |
		((new_bank0_end & MICR_ADDR_MASK) >> MICR_ADDR_SHIFT);
	emear1 = (emear1 & 0xFFFFFF00) |
		((new_bank0_end & MICR_ADDR_MASK) >> MICR_EADDR_SHIFT);
	mpc824x_mpc107_setreg(MEAR1, mear1);
	mpc824x_mpc107_setreg(EMEAR1, emear1);
#endif

	return (size);
}

/*
 * Initialize PCI Devices, report devices found.
 */
#ifndef CONFIG_PCI_PNP
static struct pci_config_table pci_debris_config_table[] = {
	{ PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID, 0x0f, PCI_ANY_ID,
	  pci_cfgfunc_config_device, { PCI_ENET0_IOADDR,
				       PCI_ENET0_MEMADDR,
				       PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER }},
	{ PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID, PCI_ANY_ID, 0x10, PCI_ANY_ID,
	  pci_cfgfunc_config_device, { PCI_ENET1_IOADDR,
				       PCI_ENET1_MEMADDR,
				       PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER }},
	{ }
};
#endif

struct pci_controller hose = {
#ifndef CONFIG_PCI_PNP
	config_table: pci_debris_config_table,
#endif
};

void pci_init_board(void)
{
	pci_mpc824x_init(&hose);
}

void *nvram_read(void *dest, const long src, size_t count)
{
	volatile uchar *d = (volatile uchar*) dest;
	volatile uchar *s = (volatile uchar*) src;
	while(count--) {
		*d++ = *s++;
		asm volatile("sync");
	}
	return dest;
}

void nvram_write(long dest, const void *src, size_t count)
{
	volatile uchar *d = (volatile uchar*)dest;
	volatile uchar *s = (volatile uchar*)src;
	while(count--) {
		*d++ = *s++;
		asm volatile("sync");
	}
}

int misc_init_r(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	/* Write ethernet addr in NVRAM for VxWorks */
	nvram_write(CFG_ENV_ADDR + CFG_NVRAM_VXWORKS_OFFS,
			(char*)&gd->bd->bi_enetaddr[0], 6);
	return 0;
}
