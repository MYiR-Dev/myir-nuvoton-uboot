/*
 * Copyright (C) 2013 Google Inc.
 * Copyright (C) 2016 Bin Meng <bmeng.cn@gmail.com>
 *
 * Modified from coreboot src/soc/intel/baytrail/acpi/southcluster.asl
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

Device (PCI0)
{
	Name(_HID, EISAID("PNP0A08"))	/* PCIe */
	Name(_CID, EISAID("PNP0A03"))	/* PCI */

	Name(_ADR, 0)
	Name(_BBN, 0)

	Name(MCRS, ResourceTemplate()
	{
		/* Bus Numbers */
		WordBusNumber(ResourceProducer, MinFixed, MaxFixed, PosDecode,
				0x0000, 0x0000, 0x00ff, 0x0000, 0x0100, , , PB00)

		/* IO Region 0 */
		WordIO(ResourceProducer, MinFixed, MaxFixed, PosDecode, EntireRange,
				0x0000, 0x0000, 0x0cf7, 0x0000, 0x0cf8, , , PI00)

		/* PCI Config Space */
		IO(Decode16, 0x0cf8, 0x0cf8, 0x0001, 0x0008)

		/* IO Region 1 */
		WordIO(ResourceProducer, MinFixed, MaxFixed, PosDecode, EntireRange,
				0x0000, 0x0d00, 0xffff, 0x0000, 0xf300, , , PI01)

		/* VGA memory (0xa0000-0xbffff) */
		DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed,
				Cacheable, ReadWrite,
				0x00000000, 0x000a0000, 0x000bffff, 0x00000000,
				0x00020000, , , ASEG)

		/* OPROM reserved (0xc0000-0xc3fff) */
		DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed,
				Cacheable, ReadWrite,
				0x00000000, 0x000c0000, 0x000c3fff, 0x00000000,
				0x00004000, , , OPR0)

		/* OPROM reserved (0xc4000-0xc7fff) */
		DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed,
				Cacheable, ReadWrite,
				0x00000000, 0x000c4000, 0x000c7fff, 0x00000000,
				0x00004000, , , OPR1)

		/* OPROM reserved (0xc8000-0xcbfff) */
		DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed,
				Cacheable, ReadWrite,
				0x00000000, 0x000c8000, 0x000cbfff, 0x00000000,
				0x00004000, , , OPR2)

		/* OPROM reserved (0xcc000-0xcffff) */
		DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed,
				Cacheable, ReadWrite,
				0x00000000, 0x000cc000, 0x000cffff, 0x00000000,
				0x00004000, , , OPR3)

		/* OPROM reserved (0xd0000-0xd3fff) */
		DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed,
				Cacheable, ReadWrite,
				0x00000000, 0x000d0000, 0x000d3fff, 0x00000000,
				0x00004000, , , OPR4)

		/* OPROM reserved (0xd4000-0xd7fff) */
		DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed,
				Cacheable, ReadWrite,
				0x00000000, 0x000d4000, 0x000d7fff, 0x00000000,
				0x00004000, , , OPR5)

		/* OPROM reserved (0xd8000-0xdbfff) */
		DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed,
				Cacheable, ReadWrite,
				0x00000000, 0x000d8000, 0x000dbfff, 0x00000000,
				0x00004000, , , OPR6)

		/* OPROM reserved (0xdc000-0xdffff) */
		DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed,
				Cacheable, ReadWrite,
				0x00000000, 0x000dc000, 0x000dffff, 0x00000000,
				0x00004000, , , OPR7)

		/* BIOS Extension (0xe0000-0xe3fff) */
		DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed,
				Cacheable, ReadWrite,
				0x00000000, 0x000e0000, 0x000e3fff, 0x00000000,
				0x00004000, , , ESG0)

		/* BIOS Extension (0xe4000-0xe7fff) */
		DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed,
				Cacheable, ReadWrite,
				0x00000000, 0x000e4000, 0x000e7fff, 0x00000000,
				0x00004000, , , ESG1)

		/* BIOS Extension (0xe8000-0xebfff) */
		DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed,
				Cacheable, ReadWrite,
				0x00000000, 0x000e8000, 0x000ebfff, 0x00000000,
				0x00004000, , , ESG2)

		/* BIOS Extension (0xec000-0xeffff) */
		DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed,
				Cacheable, ReadWrite,
				0x00000000, 0x000ec000, 0x000effff, 0x00000000,
				0x00004000, , , ESG3)

		/* System BIOS (0xf0000-0xfffff) */
		DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed,
				Cacheable, ReadWrite,
				0x00000000, 0x000f0000, 0x000fffff, 0x00000000,
				0x00010000, , , FSEG)

		/* PCI Memory Region (TOLM-CONFIG_MMCONF_BASE_ADDRESS) */
		DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed,
				Cacheable, ReadWrite,
				0x00000000, 0x00000000, 0x00000000, 0x00000000,
				0x00000000, , , PMEM)

		/* High PCI Memory Region */
		QwordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed,
				Cacheable, ReadWrite,
				0x00000000, 0x00000000, 0x00000000, 0x00000000,
				0x00000000, , , UMEM)
	})

	Method(_CRS, 0, Serialized)
	{
		/* Update PCI resource area */
		CreateDwordField(MCRS, ^PMEM._MIN, PMIN)
		CreateDwordField(MCRS, ^PMEM._MAX, PMAX)
		CreateDwordField(MCRS, ^PMEM._LEN, PLEN)

		/*
		 * Hardcode TOLM to 2GB for now as BayTrail FSP uses this value.
		 *
		 * TODO: for generic usage, read TOLM value from register, or
		 * from global NVS (not implemented by U-Boot yet).
		 */
		Store(0x80000000, PMIN)
		Store(Subtract(MCFG_BASE_ADDRESS, 1), PMAX)
		Add(Subtract(PMAX, PMIN), 1, PLEN)

		/* Update High PCI resource area */
		CreateQwordField(MCRS, ^UMEM._MIN, UMIN)
		CreateQwordField(MCRS, ^UMEM._MAX, UMAX)
		CreateQwordField(MCRS, ^UMEM._LEN, ULEN)

		/* Set base address to 48GB and allocate 16GB for PCI space */
		Store(0xc00000000, UMIN)
		Store(0x400000000, ULEN)
		Add(UMIN, Subtract(ULEN, 1), UMAX)

		Return (MCRS)
	}

	/* Device Resource Consumption */
	Device (PDRC)
	{
		Name(_HID, EISAID("PNP0C02"))
		Name(_UID, 1)

		Name(PDRS, ResourceTemplate() {
			Memory32Fixed(ReadWrite, MCFG_BASE_ADDRESS, MCFG_BASE_SIZE)
			Memory32Fixed(ReadWrite, ABORT_BASE_ADDRESS, ABORT_BASE_SIZE)
			Memory32Fixed(ReadWrite, SPI_BASE_ADDRESS, SPI_BASE_SIZE)
			Memory32Fixed(ReadWrite, PMC_BASE_ADDRESS, PMC_BASE_SIZE)
			Memory32Fixed(ReadWrite, PUNIT_BASE_ADDRESS, PUNIT_BASE_SIZE)
			Memory32Fixed(ReadWrite, ILB_BASE_ADDRESS, ILB_BASE_SIZE)
			Memory32Fixed(ReadWrite, RCBA_BASE_ADDRESS, RCBA_BASE_SIZE)
			Memory32Fixed(ReadWrite, MPHY_BASE_ADDRESS, MPHY_BASE_SIZE)
		})

		/* Current Resource Settings */
		Method(_CRS, 0, Serialized)
		{
			Return (PDRS)
		}
	}

	Method(_OSC, 4)
	{
		/* Check for proper GUID */
		If (LEqual(Arg0, ToUUID("33DB4D5B-1FF7-401C-9657-7441C03DD766"))) {
			/* Let OS control everything */
			Return (Arg3)
		} Else {
			/* Unrecognized UUID */
			CreateDWordField(Arg3, 0, CDW1)
			Or(CDW1, 4, CDW1)
			Return (Arg3)
		}
	}

	/* LPC Bridge 0:1f.0 */
	#include "lpc.asl"

	/* USB EHCI 0:1d.0 */
	#include "usb.asl"

	/* USB XHCI 0:14.0 */
	#include "xhci.asl"

	/* IRQ routing for each PCI device */
	#include "irqroute.asl"
}
