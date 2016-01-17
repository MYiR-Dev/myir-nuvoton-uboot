/*
 * Copyright (C) 2014 Google, Inc
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <dm.h>
#include <errno.h>
#include <fdtdec.h>
#include <malloc.h>
#include <pch.h>
#include <asm/io.h>
#include <asm/lapic.h>
#include <asm/pci.h>
#include <asm/arch/bd82x6x.h>
#include <asm/arch/model_206ax.h>
#include <asm/arch/pch.h>
#include <asm/arch/sandybridge.h>

#define BIOS_CTRL	0xdc

static int pch_revision_id = -1;
static int pch_type = -1;

/**
 * pch_silicon_revision() - Read silicon revision ID from the PCH
 *
 * @dev:	PCH device
 * @return silicon revision ID
 */
static int pch_silicon_revision(struct udevice *dev)
{
	u8 val;

	if (pch_revision_id < 0) {
		dm_pci_read_config8(dev, PCI_REVISION_ID, &val);
		pch_revision_id = val;
	}

	return pch_revision_id;
}

int pch_silicon_type(struct udevice *dev)
{
	u8 val;

	if (pch_type < 0) {
		dm_pci_read_config8(dev, PCI_DEVICE_ID + 1, &val);
		pch_type = val;
	}

	return pch_type;
}

/**
 * pch_silicon_supported() - Check if a certain revision is supported
 *
 * @dev:	PCH device
 * @type:	PCH type
 * @rev:	Minimum required resion
 * @return 0 if not supported, 1 if supported
 */
static int pch_silicon_supported(struct udevice *dev, int type, int rev)
{
	int cur_type = pch_silicon_type(dev);
	int cur_rev = pch_silicon_revision(dev);

	switch (type) {
	case PCH_TYPE_CPT:
		/* CougarPoint minimum revision */
		if (cur_type == PCH_TYPE_CPT && cur_rev >= rev)
			return 1;
		/* PantherPoint any revision */
		if (cur_type == PCH_TYPE_PPT)
			return 1;
		break;

	case PCH_TYPE_PPT:
		/* PantherPoint minimum revision */
		if (cur_type == PCH_TYPE_PPT && cur_rev >= rev)
			return 1;
		break;
	}

	return 0;
}

#define IOBP_RETRY 1000
static inline int iobp_poll(void)
{
	unsigned try = IOBP_RETRY;
	u32 data;

	while (try--) {
		data = readl(RCB_REG(IOBPS));
		if ((data & 1) == 0)
			return 1;
		udelay(10);
	}

	printf("IOBP timeout\n");
	return 0;
}

void pch_iobp_update(struct udevice *dev, u32 address, u32 andvalue,
		     u32 orvalue)
{
	u32 data;

	/* Set the address */
	writel(address, RCB_REG(IOBPIRI));

	/* READ OPCODE */
	if (pch_silicon_supported(dev, PCH_TYPE_CPT, PCH_STEP_B0))
		writel(IOBPS_RW_BX, RCB_REG(IOBPS));
	else
		writel(IOBPS_READ_AX, RCB_REG(IOBPS));
	if (!iobp_poll())
		return;

	/* Read IOBP data */
	data = readl(RCB_REG(IOBPD));
	if (!iobp_poll())
		return;

	/* Check for successful transaction */
	if ((readl(RCB_REG(IOBPS)) & 0x6) != 0) {
		printf("IOBP read 0x%08x failed\n", address);
		return;
	}

	/* Update the data */
	data &= andvalue;
	data |= orvalue;

	/* WRITE OPCODE */
	if (pch_silicon_supported(dev, PCH_TYPE_CPT, PCH_STEP_B0))
		writel(IOBPS_RW_BX, RCB_REG(IOBPS));
	else
		writel(IOBPS_WRITE_AX, RCB_REG(IOBPS));
	if (!iobp_poll())
		return;

	/* Write IOBP data */
	writel(data, RCB_REG(IOBPD));
	if (!iobp_poll())
		return;
}

static int bd82x6x_probe(struct udevice *dev)
{
	const void *blob = gd->fdt_blob;
	int gma_node;
	int ret;

	if (!(gd->flags & GD_FLG_RELOC))
		return 0;

	/* Cause the SATA device to do its init */
	uclass_first_device(UCLASS_DISK, &dev);

	bd82x6x_usb_ehci_init(PCH_EHCI1_DEV);
	bd82x6x_usb_ehci_init(PCH_EHCI2_DEV);

	gma_node = fdtdec_next_compatible(blob, 0, COMPAT_INTEL_GMA);
	if (gma_node < 0) {
		debug("%s: Cannot find GMA node\n", __func__);
		return -EINVAL;
	}
	ret = dm_pci_bus_find_bdf(PCH_VIDEO_DEV, &dev);
	if (ret)
		return ret;
	ret = gma_func0_init(dev, blob, gma_node);
	if (ret)
		return ret;

	return 0;
}

static int bd82x6x_pch_get_sbase(struct udevice *dev, ulong *sbasep)
{
	u32 rcba;

	dm_pci_read_config32(dev, PCH_RCBA, &rcba);
	/* Bits 31-14 are the base address, 13-1 are reserved, 0 is enable */
	rcba = rcba & 0xffffc000;
	*sbasep = rcba + 0x3800;

	return 0;
}

static enum pch_version bd82x6x_pch_get_version(struct udevice *dev)
{
	return PCHV_9;
}

static int bd82x6x_set_spi_protect(struct udevice *dev, bool protect)
{
	uint8_t bios_cntl;

	/* Adjust the BIOS write protect and SMM BIOS Write Protect Disable */
	dm_pci_read_config8(dev, BIOS_CTRL, &bios_cntl);
	if (protect) {
		bios_cntl &= ~BIOS_CTRL_BIOSWE;
		bios_cntl |= BIT(5);
	} else {
		bios_cntl |= BIOS_CTRL_BIOSWE;
		bios_cntl &= ~BIT(5);
	}
	dm_pci_write_config8(dev, BIOS_CTRL, bios_cntl);

	return 0;
}

static const struct pch_ops bd82x6x_pch_ops = {
	.get_sbase	= bd82x6x_pch_get_sbase,
	.get_version	= bd82x6x_pch_get_version,
	.set_spi_protect = bd82x6x_set_spi_protect,
};

static const struct udevice_id bd82x6x_ids[] = {
	{ .compatible = "intel,bd82x6x" },
	{ }
};

U_BOOT_DRIVER(bd82x6x_drv) = {
	.name		= "bd82x6x",
	.id		= UCLASS_PCH,
	.of_match	= bd82x6x_ids,
	.probe		= bd82x6x_probe,
	.ops		= &bd82x6x_pch_ops,
};
