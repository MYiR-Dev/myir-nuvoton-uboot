// SPDX-License-Identifier: GPL-2.0+
/*
 *  EFI application loader
 *
 *  Copyright (c) 2016 Alexander Graf
 */

#include <common.h>
#include <bootm.h>
#include <charset.h>
#include <command.h>
#include <dm.h>
#include <efi_loader.h>
#include <efi_selftest.h>
#include <errno.h>
#include <linux/libfdt.h>
#include <linux/libfdt_env.h>
#include <mapmem.h>
#include <memalign.h>
#include <asm/global_data.h>
#include <asm-generic/sections.h>
#include <asm-generic/unaligned.h>
#include <linux/linkage.h>

DECLARE_GLOBAL_DATA_PTR;

static struct efi_device_path *bootefi_image_path;
static struct efi_device_path *bootefi_device_path;

/*
 * Allow unaligned memory access.
 *
 * This routine is overridden by architectures providing this feature.
 */
void __weak allow_unaligned(void)
{
}

/*
 * Set the load options of an image from an environment variable.
 *
 * @handle:	the image handle
 * @env_var:	name of the environment variable
 * Return:	status code
 */
static efi_status_t set_load_options(efi_handle_t handle, const char *env_var)
{
	struct efi_loaded_image *loaded_image_info;
	size_t size;
	const char *env = env_get(env_var);
	u16 *pos;
	efi_status_t ret;

	ret = EFI_CALL(systab.boottime->open_protocol(
					handle,
					&efi_guid_loaded_image,
					(void **)&loaded_image_info,
					efi_root, NULL,
					EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL));
	if (ret != EFI_SUCCESS)
		return EFI_INVALID_PARAMETER;

	loaded_image_info->load_options = NULL;
	loaded_image_info->load_options_size = 0;
	if (!env)
		goto out;

	size = utf8_utf16_strlen(env) + 1;
	loaded_image_info->load_options = calloc(size, sizeof(u16));
	if (!loaded_image_info->load_options) {
		printf("ERROR: Out of memory\n");
		EFI_CALL(systab.boottime->close_protocol(handle,
							 &efi_guid_loaded_image,
							 efi_root, NULL));
		return EFI_OUT_OF_RESOURCES;
	}
	pos = loaded_image_info->load_options;
	utf8_utf16_strcpy(&pos, env);
	loaded_image_info->load_options_size = size * 2;

out:
	return EFI_CALL(systab.boottime->close_protocol(handle,
							&efi_guid_loaded_image,
							efi_root, NULL));
}

/**
 * copy_fdt() - Copy the device tree to a new location available to EFI
 *
 * The FDT is copied to a suitable location within the EFI memory map.
 * Additional 12 KiB are added to the space in case the device tree needs to be
 * expanded later with fdt_open_into().
 *
 * @fdtp:	On entry a pointer to the flattened device tree.
 *		On exit a pointer to the copy of the flattened device tree.
 *		FDT start
 * Return:	status code
 */
static efi_status_t copy_fdt(void **fdtp)
{
	unsigned long fdt_ram_start = -1L, fdt_pages;
	efi_status_t ret = 0;
	void *fdt, *new_fdt;
	u64 new_fdt_addr;
	uint fdt_size;
	int i;

	for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++) {
		u64 ram_start = gd->bd->bi_dram[i].start;
		u64 ram_size = gd->bd->bi_dram[i].size;

		if (!ram_size)
			continue;

		if (ram_start < fdt_ram_start)
			fdt_ram_start = ram_start;
	}

	/*
	 * Give us at least 12 KiB of breathing room in case the device tree
	 * needs to be expanded later.
	 */
	fdt = *fdtp;
	fdt_pages = efi_size_in_pages(fdt_totalsize(fdt) + 0x3000);
	fdt_size = fdt_pages << EFI_PAGE_SHIFT;

	/*
	 * Safe fdt location is at 127 MiB.
	 * On the sandbox convert from the sandbox address space.
	 */
	new_fdt_addr = (uintptr_t)map_sysmem(fdt_ram_start + 0x7f00000 +
					     fdt_size, 0);
	ret = efi_allocate_pages(EFI_ALLOCATE_MAX_ADDRESS,
				 EFI_BOOT_SERVICES_DATA, fdt_pages,
				 &new_fdt_addr);
	if (ret != EFI_SUCCESS) {
		/* If we can't put it there, put it somewhere */
		new_fdt_addr = (ulong)memalign(EFI_PAGE_SIZE, fdt_size);
		ret = efi_allocate_pages(EFI_ALLOCATE_MAX_ADDRESS,
					 EFI_BOOT_SERVICES_DATA, fdt_pages,
					 &new_fdt_addr);
		if (ret != EFI_SUCCESS) {
			printf("ERROR: Failed to reserve space for FDT\n");
			goto done;
		}
	}
	new_fdt = (void *)(uintptr_t)new_fdt_addr;
	memcpy(new_fdt, fdt, fdt_totalsize(fdt));
	fdt_set_totalsize(new_fdt, fdt_size);

	*fdtp = (void *)(uintptr_t)new_fdt_addr;
done:
	return ret;
}

/*
 * efi_carve_out_dt_rsv() - Carve out DT reserved memory ranges
 *
 * The mem_rsv entries of the FDT are added to the memory map. Any failures are
 * ignored because this is not critical and we would rather continue to try to
 * boot.
 *
 * @fdt: Pointer to device tree
 */
static void efi_carve_out_dt_rsv(void *fdt)
{
	int nr_rsv, i;
	uint64_t addr, size, pages;

	nr_rsv = fdt_num_mem_rsv(fdt);

	/* Look for an existing entry and add it to the efi mem map. */
	for (i = 0; i < nr_rsv; i++) {
		if (fdt_get_mem_rsv(fdt, i, &addr, &size) != 0)
			continue;

		/* Convert from sandbox address space. */
		addr = (uintptr_t)map_sysmem(addr, 0);

		pages = efi_size_in_pages(size + (addr & EFI_PAGE_MASK));
		addr &= ~EFI_PAGE_MASK;
		if (!efi_add_memory_map(addr, pages, EFI_RESERVED_MEMORY_TYPE,
					false))
			printf("FDT memrsv map %d: Failed to add to map\n", i);
	}
}

static efi_status_t efi_install_fdt(ulong fdt_addr)
{
	bootm_headers_t img = { 0 };
	efi_status_t ret;
	void *fdt;

	fdt = map_sysmem(fdt_addr, 0);
	if (fdt_check_header(fdt)) {
		printf("ERROR: invalid device tree\n");
		return EFI_INVALID_PARAMETER;
	}

	/* Create memory reservation as indicated by the device tree */
	efi_carve_out_dt_rsv(fdt);

	/* Prepare fdt for payload */
	ret = copy_fdt(&fdt);
	if (ret)
		return ret;

	if (image_setup_libfdt(&img, fdt, 0, NULL)) {
		printf("ERROR: failed to process device tree\n");
		return EFI_LOAD_ERROR;
	}

	/* Link to it in the efi tables */
	ret = efi_install_configuration_table(&efi_guid_fdt, fdt);
	if (ret != EFI_SUCCESS)
		return EFI_OUT_OF_RESOURCES;

	return ret;
}

/**
 * efi_process_fdt() - process fdt passed by a command argument
 * @fdt_opt:	pointer to argument
 * Return:	status code
 *
 * If specified, fdt will be installed as configuration table,
 * otherwise no fdt will be passed.
 */
static efi_status_t efi_process_fdt(const char *fdt_opt)
{
	unsigned long fdt_addr;
	efi_status_t ret;

	if (fdt_opt) {
		fdt_addr = simple_strtoul(fdt_opt, NULL, 16);
		if (!fdt_addr && *fdt_opt != '0')
			return EFI_INVALID_PARAMETER;

		/* Install device tree */
		ret = efi_install_fdt(fdt_addr);
		if (ret != EFI_SUCCESS) {
			printf("ERROR: failed to install device tree\n");
			return ret;
		}
	} else {
		/* Remove device tree. EFI_NOT_FOUND can be ignored here */
		efi_install_configuration_table(&efi_guid_fdt, NULL);
	}

	return EFI_SUCCESS;
}

static efi_status_t bootefi_run_prepare(const char *load_options_path,
		struct efi_device_path *device_path,
		struct efi_device_path *image_path,
		struct efi_loaded_image_obj **image_objp,
		struct efi_loaded_image **loaded_image_infop)
{
	efi_status_t ret;

	ret = efi_setup_loaded_image(device_path, image_path, image_objp,
				     loaded_image_infop);
	if (ret != EFI_SUCCESS)
		return ret;

	/* Transfer environment variable as load options */
	return set_load_options((efi_handle_t)*image_objp, load_options_path);
}

/**
 * bootefi_run_finish() - finish up after running an EFI test
 *
 * @loaded_image_info: Pointer to a struct which holds the loaded image info
 * @image_objj: Pointer to a struct which holds the loaded image object
 */
static void bootefi_run_finish(struct efi_loaded_image_obj *image_obj,
			       struct efi_loaded_image *loaded_image_info)
{
	efi_restore_gd();
	free(loaded_image_info->load_options);
	efi_delete_handle(&image_obj->header);
}

/**
 * do_bootefi_exec() - execute EFI binary
 *
 * @efi:		address of the binary
 * @device_path:	path of the device from which the binary was loaded
 * @image_path:		device path of the binary
 * Return:		status code
 *
 * Load the EFI binary into a newly assigned memory unwinding the relocation
 * information, install the loaded image protocol, and call the binary.
 */
static efi_status_t do_bootefi_exec(void *efi,
				    struct efi_device_path *device_path,
				    struct efi_device_path *image_path)
{
	efi_handle_t mem_handle = NULL;
	struct efi_device_path *memdp = NULL;
	efi_status_t ret;
	struct efi_loaded_image_obj *image_obj = NULL;
	struct efi_loaded_image *loaded_image_info = NULL;

	/*
	 * Special case for efi payload not loaded from disk, such as
	 * 'bootefi hello' or for example payload loaded directly into
	 * memory via JTAG, etc:
	 */
	if (!device_path && !image_path) {
		printf("WARNING: using memory device/image path, this may confuse some payloads!\n");
		/* actual addresses filled in after efi_load_pe() */
		memdp = efi_dp_from_mem(EFI_RESERVED_MEMORY_TYPE, 0, 0);
		device_path = image_path = memdp;
		/*
		 * Grub expects that the device path of the loaded image is
		 * installed on a handle.
		 */
		ret = efi_create_handle(&mem_handle);
		if (ret != EFI_SUCCESS)
			return ret; /* TODO: leaks device_path */
		ret = efi_add_protocol(mem_handle, &efi_guid_device_path,
				       device_path);
		if (ret != EFI_SUCCESS)
			goto err_add_protocol;
	} else {
		assert(device_path && image_path);
	}

	ret = bootefi_run_prepare("bootargs", device_path, image_path,
				  &image_obj, &loaded_image_info);
	if (ret)
		goto err_prepare;

	/* Load the EFI payload */
	ret = efi_load_pe(image_obj, efi, loaded_image_info);
	if (ret != EFI_SUCCESS)
		goto err_prepare;

	if (memdp) {
		struct efi_device_path_memory *mdp = (void *)memdp;
		mdp->memory_type = loaded_image_info->image_code_type;
		mdp->start_address = (uintptr_t)loaded_image_info->image_base;
		mdp->end_address = mdp->start_address +
				loaded_image_info->image_size;
	}

	/* we don't support much: */
	env_set("efi_8be4df61-93ca-11d2-aa0d-00e098032b8c_OsIndicationsSupported",
		"{ro,boot}(blob)0000000000000000");

	/* Call our payload! */
	debug("%s: Jumping to 0x%p\n", __func__, image_obj->entry);
	ret = EFI_CALL(efi_start_image(&image_obj->header, NULL, NULL));

err_prepare:
	/* image has returned, loaded-image obj goes *poof*: */
	bootefi_run_finish(image_obj, loaded_image_info);

err_add_protocol:
	if (mem_handle)
		efi_delete_handle(mem_handle);

	return ret;
}

#ifdef CONFIG_CMD_BOOTEFI_SELFTEST
/**
 * bootefi_test_prepare() - prepare to run an EFI test
 *
 * Prepare to run a test as if it were provided by a loaded image.
 *
 * @image_objp:		pointer to be set to the loaded image handle
 * @loaded_image_infop:	pointer to be set to the loaded image protocol
 * @path:		dummy file path used to construct the device path
 *			set in the loaded image protocol
 * @load_options_path:	name of a U-Boot environment variable. Its value is
 *			set as load options in the loaded image protocol.
 * Return:		status code
 */
static efi_status_t bootefi_test_prepare
		(struct efi_loaded_image_obj **image_objp,
		 struct efi_loaded_image **loaded_image_infop, const char *path,
		 const char *load_options_path)
{
	efi_status_t ret;

	/* Construct a dummy device path */
	bootefi_device_path = efi_dp_from_mem(EFI_RESERVED_MEMORY_TYPE, 0, 0);
	if (!bootefi_device_path)
		return EFI_OUT_OF_RESOURCES;

	bootefi_image_path = efi_dp_from_file(NULL, 0, path);
	if (!bootefi_image_path) {
		ret = EFI_OUT_OF_RESOURCES;
		goto failure;
	}

	ret = bootefi_run_prepare(load_options_path, bootefi_device_path,
				  bootefi_image_path, image_objp,
				  loaded_image_infop);
	if (ret == EFI_SUCCESS)
		return ret;

	efi_free_pool(bootefi_image_path);
	bootefi_image_path = NULL;
failure:
	efi_free_pool(bootefi_device_path);
	bootefi_device_path = NULL;
	return ret;
}

#endif /* CONFIG_CMD_BOOTEFI_SELFTEST */

static int do_bootefi_bootmgr_exec(void)
{
	struct efi_device_path *device_path, *file_path;
	void *addr;
	efi_status_t r;

	addr = efi_bootmgr_load(&device_path, &file_path);
	if (!addr)
		return 1;

	printf("## Starting EFI application at %p ...\n", addr);
	r = do_bootefi_exec(addr, device_path, file_path);
	printf("## Application terminated, r = %lu\n",
	       r & ~EFI_ERROR_MASK);

	if (r != EFI_SUCCESS)
		return 1;

	return 0;
}

/* Interpreter command to boot an arbitrary EFI image from memory */
static int do_bootefi(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned long addr;
	char *saddr;
	efi_status_t r;

	/* Allow unaligned memory access */
	allow_unaligned();

	switch_to_non_secure_mode();

	/* Initialize EFI drivers */
	r = efi_init_obj_list();
	if (r != EFI_SUCCESS) {
		printf("Error: Cannot set up EFI drivers, r = %lu\n",
		       r & ~EFI_ERROR_MASK);
		return CMD_RET_FAILURE;
	}

	if (argc < 2)
		return CMD_RET_USAGE;

	r = efi_process_fdt(argc > 2 ? argv[2] : NULL);
	if (r == EFI_INVALID_PARAMETER)
		return CMD_RET_USAGE;
	else if (r != EFI_SUCCESS)
		return CMD_RET_FAILURE;

#ifdef CONFIG_CMD_BOOTEFI_HELLO
	if (!strcmp(argv[1], "hello")) {
		ulong size = __efi_helloworld_end - __efi_helloworld_begin;

		saddr = env_get("loadaddr");
		if (saddr)
			addr = simple_strtoul(saddr, NULL, 16);
		else
			addr = CONFIG_SYS_LOAD_ADDR;
		memcpy(map_sysmem(addr, size), __efi_helloworld_begin, size);
	} else
#endif
#ifdef CONFIG_CMD_BOOTEFI_SELFTEST
	if (!strcmp(argv[1], "selftest")) {
		struct efi_loaded_image_obj *image_obj;
		struct efi_loaded_image *loaded_image_info;

		r = bootefi_test_prepare(&image_obj, &loaded_image_info,
					 "\\selftest", "efi_selftest");
		if (r != EFI_SUCCESS)
			return CMD_RET_FAILURE;

		/* Execute the test */
		r = EFI_CALL(efi_selftest(&image_obj->header, &systab));
		bootefi_run_finish(image_obj, loaded_image_info);
		return r != EFI_SUCCESS;
	} else
#endif
	if (!strcmp(argv[1], "bootmgr")) {
		return do_bootefi_bootmgr_exec();
	} else {
		saddr = argv[1];

		addr = simple_strtoul(saddr, NULL, 16);
		/* Check that a numeric value was passed */
		if (!addr && *saddr != '0')
			return CMD_RET_USAGE;

	}

	printf("## Starting EFI application at %08lx ...\n", addr);
	r = do_bootefi_exec(map_sysmem(addr, 0), bootefi_device_path,
			    bootefi_image_path);
	printf("## Application terminated, r = %lu\n",
	       r & ~EFI_ERROR_MASK);

	if (r != EFI_SUCCESS)
		return 1;
	else
		return 0;
}

#ifdef CONFIG_SYS_LONGHELP
static char bootefi_help_text[] =
	"<image address> [fdt address]\n"
	"  - boot EFI payload stored at address <image address>.\n"
	"    If specified, the device tree located at <fdt address> gets\n"
	"    exposed as EFI configuration table.\n"
#ifdef CONFIG_CMD_BOOTEFI_HELLO
	"bootefi hello\n"
	"  - boot a sample Hello World application stored within U-Boot\n"
#endif
#ifdef CONFIG_CMD_BOOTEFI_SELFTEST
	"bootefi selftest [fdt address]\n"
	"  - boot an EFI selftest application stored within U-Boot\n"
	"    Use environment variable efi_selftest to select a single test.\n"
	"    Use 'setenv efi_selftest list' to enumerate all tests.\n"
#endif
	"bootefi bootmgr [fdt addr]\n"
	"  - load and boot EFI payload based on BootOrder/BootXXXX variables.\n"
	"\n"
	"    If specified, the device tree located at <fdt address> gets\n"
	"    exposed as EFI configuration table.\n";
#endif

U_BOOT_CMD(
	bootefi, 3, 0, do_bootefi,
	"Boots an EFI payload from memory",
	bootefi_help_text
);

void efi_set_bootdev(const char *dev, const char *devnr, const char *path)
{
	struct efi_device_path *device, *image;
	efi_status_t ret;

	/* efi_set_bootdev is typically called repeatedly, recover memory */
	efi_free_pool(bootefi_device_path);
	efi_free_pool(bootefi_image_path);

	ret = efi_dp_from_name(dev, devnr, path, &device, &image);
	if (ret == EFI_SUCCESS) {
		bootefi_device_path = device;
		bootefi_image_path = image;
	} else {
		bootefi_device_path = NULL;
		bootefi_image_path = NULL;
	}
}
