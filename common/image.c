/*
 * (C) Copyright 2008 Semihalf
 *
 * (C) Copyright 2000-2006
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
#ifndef USE_HOSTCC
# include <common.h>
# include <watchdog.h>
#else
# include "mkimage.h"
#endif

#include <image.h>

unsigned long crc32 (unsigned long, const unsigned char *, unsigned int);

int image_check_hcrc (image_header_t *hdr)
{
	ulong hcrc;
	ulong len = image_get_header_size ();
	image_header_t header;

	/* Copy header so we can blank CRC field for re-calculation */
	memmove (&header, (char *)hdr, image_get_header_size ());
	image_set_hcrc (&header, 0);

	hcrc = crc32 (0, (unsigned char *)&header, len);

	return (hcrc == image_get_hcrc (hdr));
}

int image_check_dcrc (image_header_t *hdr)
{
	ulong data = image_get_data (hdr);
	ulong len = image_get_data_size (hdr);
	ulong dcrc = crc32 (0, (unsigned char *)data, len);

	return (dcrc == image_get_dcrc (hdr));
}

#ifndef USE_HOSTCC
int image_check_dcrc_wd (image_header_t *hdr, ulong chunksz)
{
	ulong dcrc = 0;
	ulong len = image_get_data_size (hdr);
	ulong data = image_get_data (hdr);

#if defined(CONFIG_HW_WATCHDOG) || defined(CONFIG_WATCHDOG)
	ulong cdata = data;
	ulong edata = cdata + len;

	while (cdata < edata) {
		ulong chunk = edata - cdata;

		if (chunk > chunksz)
			chunk = chunksz;
		dcrc = crc32 (dcrc, (unsigned char *)cdata, chunk);
		cdata += chunk;

		WATCHDOG_RESET ();
	}
#else
	dcrc = crc32 (0, (unsigned char *)data, len);
#endif

	return (dcrc == image_get_dcrc (hdr));
}

int getenv_verify (void)
{
	char *s = getenv ("verify");
	return (s && (*s == 'n')) ? 0 : 1;
}

void memmove_wd (void *to, void *from, size_t len, ulong chunksz)
{
#if defined(CONFIG_HW_WATCHDOG) || defined(CONFIG_WATCHDOG)
	while (len > 0) {
		size_t tail = (len > chunksz) ? chunksz : len;
		WATCHDOG_RESET ();
		memmove (to, from, tail);
		to += tail;
		from += tail;
		len -= tail;
	}
#else	/* !(CONFIG_HW_WATCHDOG || CONFIG_WATCHDOG) */
	memmove (to, from, len);
#endif	/* CONFIG_HW_WATCHDOG || CONFIG_WATCHDOG */
}
#endif /* USE_HOSTCC */

/**
 * image_multi_count - get component (sub-image) count
 * @hdr: pointer to the header of the multi component image
 *
 * image_multi_count() returns number of components in a multi
 * component image.
 *
 * Note: no checking of the image type is done, caller must pass
 * a valid multi component image.
 *
 * returns:
 *     number of components
 */
ulong image_multi_count (image_header_t *hdr)
{
	ulong i, count = 0;
	ulong *size;

	/* get start of the image payload, which in case of multi
	 * component images that points to a table of component sizes */
	size = (ulong *)image_get_data (hdr);

	/* count non empty slots */
	for (i = 0; size[i]; ++i)
		count++;

	return count;
}

/**
 * image_multi_getimg - get component data address and size
 * @hdr: pointer to the header of the multi component image
 * @idx: index of the requested component
 * @data: pointer to a ulong variable, will hold component data address
 * @len: pointer to a ulong variable, will hold component size
 *
 * image_multi_getimg() returns size and data address for the requested
 * component in a multi component image.
 *
 * Note: no checking of the image type is done, caller must pass
 * a valid multi component image.
 *
 * returns:
 *     data address and size of the component, if idx is valid
 *     0 in data and len, if idx is out of range
 */
void image_multi_getimg (image_header_t *hdr, ulong idx,
			ulong *data, ulong *len)
{
	int i;
	ulong *size;
	ulong offset, tail, count, img_data;

	/* get number of component */
	count = image_multi_count (hdr);

	/* get start of the image payload, which in case of multi
	 * component images that points to a table of component sizes */
	size = (ulong *)image_get_data (hdr);

	/* get address of the proper component data start, which means
	 * skipping sizes table (add 1 for last, null entry) */
	img_data = image_get_data (hdr) + (count + 1) * sizeof (ulong);

	if (idx < count) {
		*len = size[idx];
		offset = 0;
		tail = 0;

		/* go over all indices preceding requested component idx */
		for (i = 0; i < idx; i++) {
			/* add up i-th component size */
			offset += size[i];

			/* add up alignment for i-th component */
			tail += (4 - size[i] % 4);
		}

		/* calculate idx-th component data address */
		*data = img_data + offset + tail;
	} else {
		*len = 0;
		*data = 0;
	}
}
