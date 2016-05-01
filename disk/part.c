/*
 * (C) Copyright 2001
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <errno.h>
#include <ide.h>
#include <malloc.h>
#include <part.h>
#include <ubifs_uboot.h>

#undef	PART_DEBUG

#ifdef	PART_DEBUG
#define	PRINTF(fmt,args...)	printf (fmt ,##args)
#else
#define PRINTF(fmt,args...)
#endif

const struct block_drvr block_drvr[] = {
#if defined(CONFIG_CMD_IDE)
	{ .name = "ide", },
#endif
#if defined(CONFIG_CMD_SATA)
	{.name = "sata", .get_dev = sata_get_dev, },
#endif
#if defined(CONFIG_SCSI)
	{ .name = "scsi", .get_dev = scsi_get_dev, },
#endif
#if defined(CONFIG_CMD_USB) && defined(CONFIG_USB_STORAGE)
	{ .name = "usb", },
#endif
#if defined(CONFIG_MMC)
	{
		.name = "mmc",
		.select_hwpart = mmc_select_hwpart,
	},
#endif
#if defined(CONFIG_SYSTEMACE)
	{ .name = "ace", .get_dev = systemace_get_dev, },
#endif
#if defined(CONFIG_SANDBOX)
	{ .name = "host", .get_dev = host_get_dev, },
#endif
	{ },
};

DECLARE_GLOBAL_DATA_PTR;

#ifdef HAVE_BLOCK_DEVICE
static struct part_driver *part_driver_lookup_type(int part_type)
{
	struct part_driver *drv =
		ll_entry_start(struct part_driver, part_driver);
	const int n_ents = ll_entry_count(struct part_driver, part_driver);
	struct part_driver *entry;

	for (entry = drv; entry != drv + n_ents; entry++) {
		if (part_type == entry->part_type)
			return entry;
	}

	/* Not found */
	return NULL;
}

static struct blk_desc *get_dev_hwpart(const char *ifname, int dev, int hwpart)
{
	const struct block_drvr *drvr = block_drvr;
	int (*select_hwpart)(int dev_num, int hwpart);
	char *name;
	int ret;

	if (!ifname)
		return NULL;

	name = drvr->name;
#ifdef CONFIG_NEEDS_MANUAL_RELOC
	name += gd->reloc_off;
#endif
	while (drvr->name) {
		name = drvr->name;
		select_hwpart = drvr->select_hwpart;
#ifdef CONFIG_NEEDS_MANUAL_RELOC
		name += gd->reloc_off;
		if (select_hwpart)
			select_hwpart += gd->reloc_off;
#endif
		if (strncmp(ifname, name, strlen(name)) == 0) {
			struct blk_desc *dev_desc;

			dev_desc = blk_get_devnum_by_typename(name, dev);
			if (!dev_desc)
				return NULL;
			if (hwpart == 0 && !select_hwpart)
				return dev_desc;
			if (!select_hwpart)
				return NULL;
			ret = select_hwpart(dev_desc->devnum, hwpart);
			if (ret < 0)
				return NULL;
			return dev_desc;
		}
		drvr++;
	}
	return NULL;
}

struct blk_desc *blk_get_dev(const char *ifname, int dev)
{
	return get_dev_hwpart(ifname, dev, 0);
}
#else
struct blk_desc *get_dev_hwpart(const char *ifname, int dev, int hwpart)
{
	return NULL;
}

struct blk_desc *blk_get_dev(const char *ifname, int dev)
{
	return NULL;
}
#endif

#ifdef HAVE_BLOCK_DEVICE

/* ------------------------------------------------------------------------- */
/*
 * reports device info to the user
 */

#ifdef CONFIG_LBA48
typedef uint64_t lba512_t;
#else
typedef lbaint_t lba512_t;
#endif

/*
 * Overflowless variant of (block_count * mul_by / div_by)
 * when div_by > mul_by
 */
static lba512_t lba512_muldiv(lba512_t block_count, lba512_t mul_by, lba512_t div_by)
{
	lba512_t bc_quot, bc_rem;

	/* x * m / d == x / d * m + (x % d) * m / d */
	bc_quot = block_count / div_by;
	bc_rem  = block_count - div_by * bc_quot;
	return bc_quot * mul_by + (bc_rem * mul_by) / div_by;
}

void dev_print (struct blk_desc *dev_desc)
{
	lba512_t lba512; /* number of blocks if 512bytes block size */

	if (dev_desc->type == DEV_TYPE_UNKNOWN) {
		puts ("not available\n");
		return;
	}

	switch (dev_desc->if_type) {
	case IF_TYPE_SCSI:
		printf ("(%d:%d) Vendor: %s Prod.: %s Rev: %s\n",
			dev_desc->target,dev_desc->lun,
			dev_desc->vendor,
			dev_desc->product,
			dev_desc->revision);
		break;
	case IF_TYPE_ATAPI:
	case IF_TYPE_IDE:
	case IF_TYPE_SATA:
		printf ("Model: %s Firm: %s Ser#: %s\n",
			dev_desc->vendor,
			dev_desc->revision,
			dev_desc->product);
		break;
	case IF_TYPE_SD:
	case IF_TYPE_MMC:
	case IF_TYPE_USB:
		printf ("Vendor: %s Rev: %s Prod: %s\n",
			dev_desc->vendor,
			dev_desc->revision,
			dev_desc->product);
		break;
	case IF_TYPE_DOC:
		puts("device type DOC\n");
		return;
	case IF_TYPE_UNKNOWN:
		puts("device type unknown\n");
		return;
	default:
		printf("Unhandled device type: %i\n", dev_desc->if_type);
		return;
	}
	puts ("            Type: ");
	if (dev_desc->removable)
		puts ("Removable ");
	switch (dev_desc->type & 0x1F) {
	case DEV_TYPE_HARDDISK:
		puts ("Hard Disk");
		break;
	case DEV_TYPE_CDROM:
		puts ("CD ROM");
		break;
	case DEV_TYPE_OPDISK:
		puts ("Optical Device");
		break;
	case DEV_TYPE_TAPE:
		puts ("Tape");
		break;
	default:
		printf ("# %02X #", dev_desc->type & 0x1F);
		break;
	}
	puts ("\n");
	if (dev_desc->lba > 0L && dev_desc->blksz > 0L) {
		ulong mb, mb_quot, mb_rem, gb, gb_quot, gb_rem;
		lbaint_t lba;

		lba = dev_desc->lba;

		lba512 = (lba * (dev_desc->blksz/512));
		/* round to 1 digit */
		/* 2048 = (1024 * 1024) / 512 MB */
		mb = lba512_muldiv(lba512, 10, 2048);

		mb_quot	= mb / 10;
		mb_rem	= mb - (10 * mb_quot);

		gb = mb / 1024;
		gb_quot	= gb / 10;
		gb_rem	= gb - (10 * gb_quot);
#ifdef CONFIG_LBA48
		if (dev_desc->lba48)
			printf ("            Supports 48-bit addressing\n");
#endif
#if defined(CONFIG_SYS_64BIT_LBA)
		printf ("            Capacity: %ld.%ld MB = %ld.%ld GB (%Ld x %ld)\n",
			mb_quot, mb_rem,
			gb_quot, gb_rem,
			lba,
			dev_desc->blksz);
#else
		printf ("            Capacity: %ld.%ld MB = %ld.%ld GB (%ld x %ld)\n",
			mb_quot, mb_rem,
			gb_quot, gb_rem,
			(ulong)lba,
			dev_desc->blksz);
#endif
	} else {
		puts ("            Capacity: not available\n");
	}
}
#endif

#ifdef HAVE_BLOCK_DEVICE

void part_init(struct blk_desc *dev_desc)
{
	struct part_driver *drv =
		ll_entry_start(struct part_driver, part_driver);
	const int n_ents = ll_entry_count(struct part_driver, part_driver);
	struct part_driver *entry;

	blkcache_invalidate(dev_desc->if_type, dev_desc->devnum);

	dev_desc->part_type = PART_TYPE_UNKNOWN;
	for (entry = drv; entry != drv + n_ents; entry++) {
		int ret;

		ret = entry->test(dev_desc);
		debug("%s: try '%s': ret=%d\n", __func__, entry->name, ret);
		if (!ret) {
			dev_desc->part_type = entry->part_type;
			break;
		}
	}
}

static void print_part_header(const char *type, struct blk_desc *dev_desc)
{
#if defined(CONFIG_MAC_PARTITION) || \
	defined(CONFIG_DOS_PARTITION) || \
	defined(CONFIG_ISO_PARTITION) || \
	defined(CONFIG_AMIGA_PARTITION) || \
	defined(CONFIG_EFI_PARTITION)
	puts ("\nPartition Map for ");
	switch (dev_desc->if_type) {
	case IF_TYPE_IDE:
		puts ("IDE");
		break;
	case IF_TYPE_SATA:
		puts ("SATA");
		break;
	case IF_TYPE_SCSI:
		puts ("SCSI");
		break;
	case IF_TYPE_ATAPI:
		puts ("ATAPI");
		break;
	case IF_TYPE_USB:
		puts ("USB");
		break;
	case IF_TYPE_DOC:
		puts ("DOC");
		break;
	case IF_TYPE_MMC:
		puts ("MMC");
		break;
	case IF_TYPE_HOST:
		puts("HOST");
		break;
	default:
		puts ("UNKNOWN");
		break;
	}
	printf (" device %d  --   Partition Type: %s\n\n",
			dev_desc->devnum, type);
#endif /* any CONFIG_..._PARTITION */
}

void part_print(struct blk_desc *dev_desc)
{
	struct part_driver *drv;

	drv = part_driver_lookup_type(dev_desc->part_type);
	if (!drv) {
		printf("## Unknown partition table type %x\n",
		       dev_desc->part_type);
		return;
	}

	PRINTF("## Testing for valid %s partition ##\n", drv->name);
	print_part_header(drv->name, dev_desc);
	if (drv->print)
		drv->print(dev_desc);
}

#endif /* HAVE_BLOCK_DEVICE */

int part_get_info(struct blk_desc *dev_desc, int part,
		       disk_partition_t *info)
{
#ifdef HAVE_BLOCK_DEVICE
	struct part_driver *drv;

#ifdef CONFIG_PARTITION_UUIDS
	/* The common case is no UUID support */
	info->uuid[0] = 0;
#endif
#ifdef CONFIG_PARTITION_TYPE_GUID
	info->type_guid[0] = 0;
#endif

	drv = part_driver_lookup_type(dev_desc->part_type);
	if (!drv) {
		debug("## Unknown partition table type %x\n",
		      dev_desc->part_type);
		return -EPROTONOSUPPORT;
	}
	if (!drv->get_info) {
		PRINTF("## Driver %s does not have the get_info() method\n",
		       drv->name);
		return -ENOSYS;
	}
	if (drv->get_info(dev_desc, part, info) == 0) {
		PRINTF("## Valid %s partition found ##\n", drv->name);
		return 0;
	}
#endif /* HAVE_BLOCK_DEVICE */

	return -1;
}

int blk_get_device_by_str(const char *ifname, const char *dev_hwpart_str,
			  struct blk_desc **dev_desc)
{
	char *ep;
	char *dup_str = NULL;
	const char *dev_str, *hwpart_str;
	int dev, hwpart;

	hwpart_str = strchr(dev_hwpart_str, '.');
	if (hwpart_str) {
		dup_str = strdup(dev_hwpart_str);
		dup_str[hwpart_str - dev_hwpart_str] = 0;
		dev_str = dup_str;
		hwpart_str++;
	} else {
		dev_str = dev_hwpart_str;
		hwpart = 0;
	}

	dev = simple_strtoul(dev_str, &ep, 16);
	if (*ep) {
		printf("** Bad device specification %s %s **\n",
		       ifname, dev_str);
		dev = -1;
		goto cleanup;
	}

	if (hwpart_str) {
		hwpart = simple_strtoul(hwpart_str, &ep, 16);
		if (*ep) {
			printf("** Bad HW partition specification %s %s **\n",
			    ifname, hwpart_str);
			dev = -1;
			goto cleanup;
		}
	}

	*dev_desc = get_dev_hwpart(ifname, dev, hwpart);
	if (!(*dev_desc) || ((*dev_desc)->type == DEV_TYPE_UNKNOWN)) {
		printf("** Bad device %s %s **\n", ifname, dev_hwpart_str);
		dev = -1;
		goto cleanup;
	}

#ifdef HAVE_BLOCK_DEVICE
	/*
	 * Updates the partition table for the specified hw partition.
	 * Does not need to be done for hwpart 0 since it is default and
	 * already loaded.
	 */
	if(hwpart != 0)
		part_init(*dev_desc);
#endif

cleanup:
	free(dup_str);
	return dev;
}

#define PART_UNSPECIFIED -2
#define PART_AUTO -1
#define MAX_SEARCH_PARTITIONS 16
int blk_get_device_part_str(const char *ifname, const char *dev_part_str,
			     struct blk_desc **dev_desc,
			     disk_partition_t *info, int allow_whole_dev)
{
	int ret = -1;
	const char *part_str;
	char *dup_str = NULL;
	const char *dev_str;
	int dev;
	char *ep;
	int p;
	int part;
	disk_partition_t tmpinfo;

#ifdef CONFIG_SANDBOX
	/*
	 * Special-case a pseudo block device "hostfs", to allow access to the
	 * host's own filesystem.
	 */
	if (0 == strcmp(ifname, "hostfs")) {
		*dev_desc = NULL;
		info->start = 0;
		info->size = 0;
		info->blksz = 0;
		info->bootable = 0;
		strcpy((char *)info->type, BOOT_PART_TYPE);
		strcpy((char *)info->name, "Sandbox host");
#ifdef CONFIG_PARTITION_UUIDS
		info->uuid[0] = 0;
#endif
#ifdef CONFIG_PARTITION_TYPE_GUID
		info->type_guid[0] = 0;
#endif

		return 0;
	}
#endif

#ifdef CONFIG_CMD_UBIFS
	/*
	 * Special-case ubi, ubi goes through a mtd, rathen then through
	 * a regular block device.
	 */
	if (0 == strcmp(ifname, "ubi")) {
		if (!ubifs_is_mounted()) {
			printf("UBIFS not mounted, use ubifsmount to mount volume first!\n");
			return -1;
		}

		*dev_desc = NULL;
		memset(info, 0, sizeof(*info));
		strcpy((char *)info->type, BOOT_PART_TYPE);
		strcpy((char *)info->name, "UBI");
#ifdef CONFIG_PARTITION_UUIDS
		info->uuid[0] = 0;
#endif
		return 0;
	}
#endif

	/* If no dev_part_str, use bootdevice environment variable */
	if (!dev_part_str || !strlen(dev_part_str) ||
	    !strcmp(dev_part_str, "-"))
		dev_part_str = getenv("bootdevice");

	/* If still no dev_part_str, it's an error */
	if (!dev_part_str) {
		printf("** No device specified **\n");
		goto cleanup;
	}

	/* Separate device and partition ID specification */
	part_str = strchr(dev_part_str, ':');
	if (part_str) {
		dup_str = strdup(dev_part_str);
		dup_str[part_str - dev_part_str] = 0;
		dev_str = dup_str;
		part_str++;
	} else {
		dev_str = dev_part_str;
	}

	/* Look up the device */
	dev = blk_get_device_by_str(ifname, dev_str, dev_desc);
	if (dev < 0)
		goto cleanup;

	/* Convert partition ID string to number */
	if (!part_str || !*part_str) {
		part = PART_UNSPECIFIED;
	} else if (!strcmp(part_str, "auto")) {
		part = PART_AUTO;
	} else {
		/* Something specified -> use exactly that */
		part = (int)simple_strtoul(part_str, &ep, 16);
		/*
		 * Less than whole string converted,
		 * or request for whole device, but caller requires partition.
		 */
		if (*ep || (part == 0 && !allow_whole_dev)) {
			printf("** Bad partition specification %s %s **\n",
			    ifname, dev_part_str);
			goto cleanup;
		}
	}

	/*
	 * No partition table on device,
	 * or user requested partition 0 (entire device).
	 */
	if (((*dev_desc)->part_type == PART_TYPE_UNKNOWN) ||
	    (part == 0)) {
		if (!(*dev_desc)->lba) {
			printf("** Bad device size - %s %s **\n", ifname,
			       dev_str);
			goto cleanup;
		}

		/*
		 * If user specified a partition ID other than 0,
		 * or the calling command only accepts partitions,
		 * it's an error.
		 */
		if ((part > 0) || (!allow_whole_dev)) {
			printf("** No partition table - %s %s **\n", ifname,
			       dev_str);
			goto cleanup;
		}

		(*dev_desc)->log2blksz = LOG2((*dev_desc)->blksz);

		info->start = 0;
		info->size = (*dev_desc)->lba;
		info->blksz = (*dev_desc)->blksz;
		info->bootable = 0;
		strcpy((char *)info->type, BOOT_PART_TYPE);
		strcpy((char *)info->name, "Whole Disk");
#ifdef CONFIG_PARTITION_UUIDS
		info->uuid[0] = 0;
#endif
#ifdef CONFIG_PARTITION_TYPE_GUID
		info->type_guid[0] = 0;
#endif

		ret = 0;
		goto cleanup;
	}

	/*
	 * Now there's known to be a partition table,
	 * not specifying a partition means to pick partition 1.
	 */
	if (part == PART_UNSPECIFIED)
		part = 1;

	/*
	 * If user didn't specify a partition number, or did specify something
	 * other than "auto", use that partition number directly.
	 */
	if (part != PART_AUTO) {
		ret = part_get_info(*dev_desc, part, info);
		if (ret) {
			printf("** Invalid partition %d **\n", part);
			goto cleanup;
		}
	} else {
		/*
		 * Find the first bootable partition.
		 * If none are bootable, fall back to the first valid partition.
		 */
		part = 0;
		for (p = 1; p <= MAX_SEARCH_PARTITIONS; p++) {
			ret = part_get_info(*dev_desc, p, info);
			if (ret)
				continue;

			/*
			 * First valid partition, or new better partition?
			 * If so, save partition ID.
			 */
			if (!part || info->bootable)
				part = p;

			/* Best possible partition? Stop searching. */
			if (info->bootable)
				break;

			/*
			 * We now need to search further for best possible.
			 * If we what we just queried was the best so far,
			 * save the info since we over-write it next loop.
			 */
			if (part == p)
				tmpinfo = *info;
		}
		/* If we found any acceptable partition */
		if (part) {
			/*
			 * If we searched all possible partition IDs,
			 * return the first valid partition we found.
			 */
			if (p == MAX_SEARCH_PARTITIONS + 1)
				*info = tmpinfo;
		} else {
			printf("** No valid partitions found **\n");
			ret = -1;
			goto cleanup;
		}
	}
	if (strncmp((char *)info->type, BOOT_PART_TYPE, sizeof(info->type)) != 0) {
		printf("** Invalid partition type \"%.32s\""
			" (expect \"" BOOT_PART_TYPE "\")\n",
			info->type);
		ret  = -1;
		goto cleanup;
	}

	(*dev_desc)->log2blksz = LOG2((*dev_desc)->blksz);

	ret = part;
	goto cleanup;

cleanup:
	free(dup_str);
	return ret;
}
