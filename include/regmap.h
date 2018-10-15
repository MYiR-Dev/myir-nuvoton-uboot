/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2015 Google, Inc
 * Written by Simon Glass <sjg@chromium.org>
 */

#ifndef __REGMAP_H
#define __REGMAP_H

/**
 * enum regmap_size_t - Access sizes for regmap reads and writes
 *
 * @REGMAP_SIZE_8: 8-bit read/write access size
 * @REGMAP_SIZE_16: 16-bit read/write access size
 * @REGMAP_SIZE_32: 32-bit read/write access size
 * @REGMAP_SIZE_64: 64-bit read/write access size
 */
enum regmap_size_t {
	REGMAP_SIZE_8 = 1,
	REGMAP_SIZE_16 = 2,
	REGMAP_SIZE_32 = 4,
	REGMAP_SIZE_64 = 8,
};

/**
 * struct regmap_range - a register map range
 *
 * @start:	Start address
 * @size:	Size in bytes
 */
struct regmap_range {
	ulong start;
	ulong size;
};

/**
 * struct regmap - a way of accessing hardware/bus registers
 *
 * @range_count:	Number of ranges available within the map
 * @ranges:		Array of ranges
 */
struct regmap {
	int range_count;
	struct regmap_range ranges[0];
};

/*
 * Interface to provide access to registers either through a direct memory
 * bus or through a peripheral bus like I2C, SPI.
 */

/**
 * regmap_write() - Write a 32-bit value to a regmap
 *
 * @map:	Regmap to write to
 * @offset:	Offset in the regmap to write to
 * @val:	Data to write to the regmap at the specified offset
 *
 * Note that this function will only write values of 32 bit width to the
 * regmap; if the size of data to be read is different, the regmap_raw_write
 * function can be used.
 *
 * Return: 0 if OK, -ve on error
 */
int regmap_write(struct regmap *map, uint offset, uint val);

/**
 * regmap_read() - Read a 32-bit value from a regmap
 *
 * @map:	Regmap to read from
 * @offset:	Offset in the regmap to read from
 * @valp:	Pointer to the buffer to receive the data read from the regmap
 *		at the specified offset
 *
 * Note that this function will only read values of 32 bit width from the
 * regmap; if the size of data to be read is different, the regmap_raw_read
 * function can be used.
 *
 * Return: 0 if OK, -ve on error
 */
int regmap_read(struct regmap *map, uint offset, uint *valp);

/**
 * regmap_raw_write() - Write a value of specified length to a regmap
 *
 * @map:	Regmap to write to
 * @offset:	Offset in the regmap to write to
 * @val:	Value to write to the regmap at the specified offset
 * @val_len:	Length of the data to be written to the regmap
 *
 * Note that this function will, as opposed to regmap_write, write data of
 * arbitrary length to the regmap, and not just 32-bit values, and is thus a
 * generalized version of regmap_write.
 *
 * Return: 0 if OK, -ve on error
 */
int regmap_raw_write(struct regmap *map, uint offset, const void *val,
		     size_t val_len);

/**
 * regmap_raw_read() - Read a value of specified length from a regmap
 *
 * @map:	Regmap to read from
 * @offset:	Offset in the regmap to read from
 * @valp:	Pointer to the buffer to receive the data read from the regmap
 *		at the specified offset
 * @val_len:	Length of the data to be read from the regmap
 *
 * Note that this function will, as opposed to regmap_read, read data of
 * arbitrary length from the regmap, and not just 32-bit values, and is thus a
 * generalized version of regmap_read.
 *
 * Return: 0 if OK, -ve on error
 */
int regmap_raw_read(struct regmap *map, uint offset, void *valp,
		    size_t val_len);

/**
 * regmap_raw_write_range() - Write a value of specified length to a range of a
 *			      regmap
 *
 * @map:	Regmap to write to
 * @range_num:	Number of the range in the regmap to write to
 * @offset:	Offset in the regmap to write to
 * @val:	Value to write to the regmap at the specified offset
 * @val_len:	Length of the data to be written to the regmap
 *
 * Return: 0 if OK, -ve on error
 */
int regmap_raw_write_range(struct regmap *map, uint range_num, uint offset,
			   const void *val, size_t val_len);

/**
 * regmap_raw_read_range() - Read a value of specified length from a range of a
 *			     regmap
 *
 * @map:	Regmap to read from
 * @range_num:	Number of the range in the regmap to write to
 * @offset:	Offset in the regmap to read from
 * @valp:	Pointer to the buffer to receive the data read from the regmap
 *		at the specified offset
 * @val_len:	Length of the data to be read from the regmap
 *
 * Return: 0 if OK, -ve on error
 */
int regmap_raw_read_range(struct regmap *map, uint range_num, uint offset,
			  void *valp, size_t val_len);

#define regmap_write32(map, ptr, member, val) \
	regmap_write(map, (uint32_t *)(ptr)->member - (uint32_t *)(ptr), val)

#define regmap_read32(map, ptr, member, valp) \
	regmap_read(map, (uint32_t *)(ptr)->member - (uint32_t *)(ptr), valp)

/**
 * regmap_update_bits() - Perform a read/modify/write using a mask
 *
 * @map:	The map returned by regmap_init_mem*()
 * @offset:	Offset of the memory
 * @mask:	Mask to apply to the read value
 * @val:	Value to apply to the value to write
 * Return: 0 if OK, -ve on error
 */
int regmap_update_bits(struct regmap *map, uint offset, uint mask, uint val);

/**
 * regmap_init_mem() - Set up a new register map that uses memory access
 *
 * @node:	Device node that uses this map
 * @mapp:	Returns allocated map
 * Return: 0 if OK, -ve on error
 *
 * Use regmap_uninit() to free it.
 */
int regmap_init_mem(ofnode node, struct regmap **mapp);

/**
 * regmap_init_mem_platdata() - Set up a new memory register map for
 *				of-platdata
 *
 * @dev:	Device that uses this map
 * @reg:	List of address, size pairs
 * @count:	Number of pairs (e.g. 1 if the regmap has a single entry)
 * @mapp:	Returns allocated map
 * Return: 0 if OK, -ve on error
 *
 * This creates a new regmap with a list of regions passed in, rather than
 * using the device tree. It only supports 32-bit machines.
 *
 * Use regmap_uninit() to free it.
 *
 */
int regmap_init_mem_platdata(struct udevice *dev, fdt_val_t *reg, int count,
			     struct regmap **mapp);

/**
 * regmap_get_range() - Obtain the base memory address of a regmap range
 *
 * @map:	Regmap to query
 * @range_num:	Range to look up
 * Return: Pointer to the range in question if OK, NULL on error
 */
void *regmap_get_range(struct regmap *map, unsigned int range_num);

/**
 * regmap_uninit() - free a previously inited regmap
 *
 * @map:	Regmap to free
 * Return: 0 if OK, -ve on error
 */
int regmap_uninit(struct regmap *map);

#endif
