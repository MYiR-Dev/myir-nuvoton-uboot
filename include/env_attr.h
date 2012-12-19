/*
 * (C) Copyright 2012
 * Joe Hershberger, National Instruments, joe.hershberger@ni.com
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

#ifndef __ENV_ATTR_H__
#define __ENV_ATTR_H__

#define ENV_ATTR_LIST_DELIM	','
#define ENV_ATTR_SEP		':'

/*
 * env_attr_walk takes as input an "attr_list" that takes the form:
 *	attributes = [^,:\s]*
 *	entry = name[:attributes]
 *	list = entry[,list]
 * It will call the "callback" function with the "name" and attribute as "value"
 * The callback may return a non-0 to abort the list walk.
 * This return value will be passed through to the caller.
 * 0 is returned on success.
 */
extern int env_attr_walk(const char *attr_list,
	int (*callback)(const char *name, const char *value));

/*
 * env_attr_lookup takes as input an "attr_list" with the same form as above.
 * It also takes as input a "name" to look for.
 * If the name is found in the list, it's value is copied into "attributes".
 * There is no protection on attributes being too small for the value.
 * It returns -1 if attributes is NULL, 1 if "name" is not found, 2 if
 * "attr_list" is NULL.
 * Returns 0 on success.
 */
extern int env_attr_lookup(const char *attr_list, const char *name,
	char *attributes);

#endif /* __ENV_ATTR_H__ */
