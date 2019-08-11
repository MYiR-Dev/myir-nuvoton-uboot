/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Common environment functions
 *
 * (C) Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 */

#ifndef __ENV_H
#define __ENV_H

#include <stdbool.h>
#include <linux/types.h>

/**
 * env_get_id() - Gets a sequence number for the environment
 *
 * This value increments every time the environment changes, so can be used an
 * an indication of this
 *
 * @return environment ID
 */
int env_get_id(void);

/**
 * env_init() - Set up the pre-relocation environment
 *
 * This locates the environment or uses the default if nothing is available.
 * This must be called before env_get() will work.
 *
 * @return 0 if OK, -ENODEV if no environment drivers are enabled
 */
int env_init(void);

/**
 * env_relocate() - Set up the post-relocation environment
 *
 * This loads the environment into RAM so that it can be modified. This is
 * called after relocation, before the environment is used
 */
void env_relocate(void);

/**
 * env_match() - Match a name / name=value pair
 *
 * This is used prior to relocation for finding envrionment variables
 *
 * @name: A simple 'name', or a 'name=value' pair.
 * @index: The environment index for a 'name2=value2' pair.
 * @return index for the value if the names match, else -1.
 */
int env_match(unsigned char *name, int index);

/**
 * env_get() - Look up the value of an environment variable
 *
 * In U-Boot proper this can be called before relocation (which is when the
 * environment is loaded from storage, i.e. GD_FLG_ENV_READY is 0). In that
 * case this function calls env_get_f().
 *
 * @varname:	Variable to look up
 * @return value of variable, or NULL if not found
 */
char *env_get(const char *varname);

/**
 * env_get_f() - Look up the value of an environment variable (early)
 *
 * This function is called from env_get() if the environment has not been
 * loaded yet (GD_FLG_ENV_READY flag is 0). Some environment locations will
 * support reading the value (slowly) and some will not.
 *
 * @varname:	Variable to look up
 * @return value of variable, or NULL if not found
 */
int env_get_f(const char *name, char *buf, unsigned int len);

/**
 * env_get_yesno() - Read an environment variable as a boolean
 *
 * @return 1 if yes/true (Y/y/T/t), -1 if variable does not exist (i.e. default
 *	to true), 0 if otherwise
 */
int env_get_yesno(const char *var);

/**
 * env_set() - set an environment variable
 *
 * This sets or deletes the value of an environment variable. For setting the
 * value the variable is created if it does not already exist.
 *
 * @varname: Variable to adjust
 * @value: Value to set for the variable, or NULL or "" to delete the variable
 * @return 0 if OK, 1 on error
 */
int env_set(const char *varname, const char *value);

/**
 * env_get_ulong() - Return an environment variable as an integer value
 *
 * Most U-Boot environment variables store hex values. For those which store
 * (e.g.) base-10 integers, this function can be used to read the value.
 *
 * @name:	Variable to look up
 * @base:	Base to use (e.g. 10 for base 10, 2 for binary)
 * @default_val: Default value to return if no value is found
 * @return the value found, or @default_val if none
 */
ulong env_get_ulong(const char *name, int base, ulong default_val);

/**
 * env_set_ulong() - set an environment variable to an integer
 *
 * @varname: Variable to adjust
 * @value: Value to set for the variable (will be converted to a string)
 * @return 0 if OK, 1 on error
 */
int env_set_ulong(const char *varname, ulong value);

/**
 * env_get_hex() - Return an environment variable as a hex value
 *
 * Decode an environment as a hex number (it may or may not have a 0x
 * prefix). If the environment variable cannot be found, or does not start
 * with hex digits, the default value is returned.
 *
 * @varname:		Variable to decode
 * @default_val:	Value to return on error
 */
ulong env_get_hex(const char *varname, ulong default_val);

/**
 * env_set_hex() - set an environment variable to a hex value
 *
 * @varname: Variable to adjust
 * @value: Value to set for the variable (will be converted to a hex string)
 * @return 0 if OK, 1 on error
 */
int env_set_hex(const char *varname, ulong value);

/**
 * env_set_addr - Set an environment variable to an address in hex
 *
 * @varname:	Environment variable to set
 * @addr:	Value to set it to
 * @return 0 if ok, 1 on error
 */
static inline int env_set_addr(const char *varname, const void *addr)
{
	return env_set_hex(varname, (ulong)addr);
}

/**
 * env_complete() - return an auto-complete for environment variables
 *
 * @var: partial name to auto-complete
 * @maxv: Maximum number of matches to return
 * @cmdv: Returns a list of possible matches
 * @maxsz: Size of buffer to use for matches
 * @buf: Buffer to use for matches
 * @dollar_comp: non-zero to wrap each match in ${...}
 * @return number of matches found (in @cmdv)
 */
int env_complete(char *var, int maxv, char *cmdv[], int maxsz, char *buf,
		 bool dollar_comp);

#endif
