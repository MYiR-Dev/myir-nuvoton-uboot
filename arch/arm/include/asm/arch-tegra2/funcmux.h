/*
 * Copyright (c) 2011 The Chromium OS Authors.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/* Tegra2 high-level function multiplexing */

#ifndef __FUNCMUX_H
#define __FUNCMUX_H

/**
 * Select a config for a particular peripheral.
 *
 * Each peripheral can operate through a number of configurations,
 * which are sets of pins that it uses to bring out its signals.
 * The basic config is 0, and higher numbers indicate different
 * pinmux settings to bring the peripheral out on other pins,
 *
 * @param id		Peripheral id
 * @param config	Configuration to use (generally 0)
 * @return 0 if ok, -1 on error (e.g. incorrect id or config)
 */
int funcmux_select(enum periph_id id, int config);

#endif
