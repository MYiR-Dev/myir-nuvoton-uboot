/* (C) Copyright 2002
 * Detlev Zundel, DENX Software Engineering, dzu@denx.de.
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
 * Bitmap display support
 */
#ifndef	_CMD_BMP_H
#define	_CMD_BMP_H

#include <common.h>
#include <command.h>


#if (CONFIG_COMMANDS & CFG_CMD_BMP)

#define CMD_TBL_BMP	MK_CMD_TBL_ENTRY(				\
	"bmp", 3,	3,	1,	do_bmp,				\
	"bmp     - manipulate BMP image data\n",			\
	"info <imageAddr>    - display image info\n"			\
	"bmp display <imageAddr> - display image\n"			\
),
int do_bmp (cmd_tbl_t *cmdtp,  int flag, int argc, char *argv[]);

#else
#define CMD_TBL_BMP
#endif

#endif	/* _CMD_BMP_H */
