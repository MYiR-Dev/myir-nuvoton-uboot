/*
 * (C) Copyright 2003
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
#include <command.h>
#include <pci.h>
#include <405gp_pci.h>


#if (CONFIG_COMMANDS & CFG_CMD_BSP)

#define IBM_VENDOR_ID    0x1014
#define PPC405_DEVICE_ID 0x0156


/*
 * Set device number on pci board
 */
int do_setdevice(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	int idx = 1;	  /* start at 1 (skip device 0) */
	pci_dev_t bdf = 0;
	u32 addr;

	while (bdf >= 0) {
		if ((bdf = pci_find_device(IBM_VENDOR_ID, PPC405_DEVICE_ID, idx++)) < 0) {
			break;
		}
		printf("Found device nr %d at %x!\n", idx-1, bdf);
		pci_read_config_dword(bdf, PCI_BASE_ADDRESS_1, &addr);
		addr &= ~0xf;
		*(u32 *)addr = (bdf & 0x0000f800) >> 11;
		printf("Wrote %x at %x!\n", (bdf & 0x0000f800) >> 11, addr);
	}

	return 0;
}
U_BOOT_CMD(
	setdevice,	1,	1,	do_setdevice,
	"setdevice - Set device number on pci adapter boards\n",
	NULL
);


/*
 * Get device number on pci board
 */
int do_getdevice(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	u32 device;
	char str[32];

	device = *(u32 *)0x0;
	device = 0x16 - device;      /* calculate vxworks bp slot id */
	sprintf(str, "%d", device);
	setenv("slot", str);
	printf("Variabel slot set to %x\n", device);

	return 0;
}
U_BOOT_CMD(
	getdevice,	1,	1,	do_getdevice,
	"getdevice - Get device number and set slot env variable\n",
	NULL
);

#endif
