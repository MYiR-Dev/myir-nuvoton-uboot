/*
 * (C) Copyright 2007 Michal Simek
 *
 * Michal  SIMEK <monstr@monstr.eu>
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

/* FSL macros */
#define NGET(val, fslnum) \
	__asm__ __volatile__ ("nget %0, rfsl" #fslnum :"=r" (val));
#define GET(val, fslnum) \
	__asm__ __volatile__ ("get %0, rfsl" #fslnum :"=r" (val));
#define NPUT(val, fslnum) \
	__asm__ __volatile__ ("nput %0, rfsl" #fslnum ::"r" (val));
#define PUT(val, fslnum) \
	__asm__ __volatile__ ("put %0, rfsl" #fslnum ::"r" (val));
								
/* CPU dependent */
#define RMSR(val) \
	__asm__ __volatile__ ("mfs %0,rmsr":"=r" (val));
