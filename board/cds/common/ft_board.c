/*
 * Copyright 2004 Freescale Semiconductor.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>

#if defined(CONFIG_OF_FLAT_TREE)
#include <ft_build.h>
extern void ft_cpu_setup(void *blob, bd_t *bd);
#endif


#if defined(CONFIG_OF_FLAT_TREE) && defined(CONFIG_OF_BOARD_SETUP)
void
ft_board_setup(void *blob, bd_t *bd)
{
	u32 *p;
	int len;

	ft_cpu_setup(blob, bd);

	p = ft_get_prop(blob, "/memory/reg", &len);
	if (p != NULL) {
		*p++ = cpu_to_be32(bd->bi_memstart);
		*p = cpu_to_be32(bd->bi_memsize);
	}
}
#endif
