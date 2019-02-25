// SPDX-License-Identifier: GPL-2.0+
/*
 *  UEFI Shell-like command
 *
 *  Copyright (c) 2018 AKASHI Takahiro, Linaro Limited
 */

#include <charset.h>
#include <common.h>
#include <command.h>
#include <efi_loader.h>
#include <environment.h>
#include <exports.h>
#include <malloc.h>
#include <search.h>
#include <linux/ctype.h>

#define RT systab.runtime

/**
 * do_efi_boot_add() - set UEFI load option
 *
 * @cmdtp:	Command table
 * @flag:	Command flag
 * @argc:	Number of arguments
 * @argv:	Argument array
 * Return:	CMD_RET_SUCCESS on success,
 *		CMD_RET_USAGE or CMD_RET_RET_FAILURE on failure
 *
 * Implement efidebug "boot add" sub-command.
 * Create or change UEFI load option.
 *   - boot add <id> <label> <interface> <devnum>[:<part>] <file> <options>
 */
static int do_efi_boot_add(cmd_tbl_t *cmdtp, int flag,
			   int argc, char * const argv[])
{
	int id;
	char *endp;
	char var_name[9];
	u16 var_name16[9], *p;
	efi_guid_t guid;
	size_t label_len, label_len16;
	u16 *label;
	struct efi_device_path *device_path = NULL, *file_path = NULL;
	struct efi_load_option lo;
	void *data = NULL;
	efi_uintn_t size;
	int ret;

	if (argc < 6 || argc > 7)
		return CMD_RET_USAGE;

	id = (int)simple_strtoul(argv[1], &endp, 16);
	if (*endp != '\0' || id > 0xffff)
		return CMD_RET_FAILURE;

	sprintf(var_name, "Boot%04X", id);
	p = var_name16;
	utf8_utf16_strncpy(&p, var_name, 9);

	guid = efi_global_variable_guid;

	/* attributes */
	lo.attributes = LOAD_OPTION_ACTIVE; /* always ACTIVE */

	/* label */
	label_len = strlen(argv[2]);
	label_len16 = utf8_utf16_strnlen(argv[2], label_len);
	label = malloc((label_len16 + 1) * sizeof(u16));
	if (!label)
		return CMD_RET_FAILURE;
	lo.label = label; /* label will be changed below */
	utf8_utf16_strncpy(&label, argv[2], label_len);

	/* file path */
	ret = efi_dp_from_name(argv[3], argv[4], argv[5], &device_path,
			       &file_path);
	if (ret != EFI_SUCCESS) {
		printf("Cannot create device path for \"%s %s\"\n",
		       argv[3], argv[4]);
		ret = CMD_RET_FAILURE;
		goto out;
	}
	lo.file_path = file_path;
	lo.file_path_length = efi_dp_size(file_path)
				+ sizeof(struct efi_device_path); /* for END */

	/* optional data */
	lo.optional_data = (u8 *)(argc == 6 ? "" : argv[6]);

	size = efi_serialize_load_option(&lo, (u8 **)&data);
	if (!size) {
		ret = CMD_RET_FAILURE;
		goto out;
	}

	ret = EFI_CALL(RT->set_variable(var_name16, &guid,
					EFI_VARIABLE_BOOTSERVICE_ACCESS |
					EFI_VARIABLE_RUNTIME_ACCESS,
					size, data));
	ret = (ret == EFI_SUCCESS ? CMD_RET_SUCCESS : CMD_RET_FAILURE);
out:
	free(data);
	efi_free_pool(device_path);
	efi_free_pool(file_path);
	free(lo.label);

	return ret;
}

/**
 * do_efi_boot_rm() - delete UEFI load options
 *
 * @cmdtp:	Command table
 * @flag:	Command flag
 * @argc:	Number of arguments
 * @argv:	Argument array
 * Return:	CMD_RET_SUCCESS on success, CMD_RET_RET_FAILURE on failure
 *
 * Implement efidebug "boot rm" sub-command.
 * Delete UEFI load options.
 *   - boot rm <id> ...
 */
static int do_efi_boot_rm(cmd_tbl_t *cmdtp, int flag,
			  int argc, char * const argv[])
{
	efi_guid_t guid;
	int id, i;
	char *endp;
	char var_name[9];
	u16 var_name16[9];
	efi_status_t ret;

	if (argc == 1)
		return CMD_RET_USAGE;

	guid = efi_global_variable_guid;
	for (i = 1; i < argc; i++, argv++) {
		id = (int)simple_strtoul(argv[1], &endp, 16);
		if (*endp != '\0' || id > 0xffff)
			return CMD_RET_FAILURE;

		sprintf(var_name, "Boot%04X", id);
		utf8_utf16_strncpy((u16 **)&var_name16, var_name, 9);

		ret = EFI_CALL(RT->set_variable(var_name16, &guid, 0, 0, NULL));
		if (ret) {
			printf("cannot remove Boot%04X", id);
			return CMD_RET_FAILURE;
		}
	}

	return CMD_RET_SUCCESS;
}

/**
 * show_efi_boot_opt_data() - dump UEFI load option
 *
 * @id:		Load option number
 * @data:	Value of UEFI load option variable
 *
 * Decode the value of UEFI load option variable and print information.
 */
static void show_efi_boot_opt_data(int id, void *data)
{
	struct efi_load_option lo;
	char *label, *p;
	size_t label_len16, label_len;
	u16 *dp_str;

	efi_deserialize_load_option(&lo, data);

	label_len16 = u16_strlen(lo.label);
	label_len = utf16_utf8_strnlen(lo.label, label_len16);
	label = malloc(label_len + 1);
	if (!label)
		return;
	p = label;
	utf16_utf8_strncpy(&p, lo.label, label_len16);

	printf("Boot%04X:\n", id);
	printf("\tattributes: %c%c%c (0x%08x)\n",
	       /* ACTIVE */
	       lo.attributes & LOAD_OPTION_ACTIVE ? 'A' : '-',
	       /* FORCE RECONNECT */
	       lo.attributes & LOAD_OPTION_FORCE_RECONNECT ? 'R' : '-',
	       /* HIDDEN */
	       lo.attributes & LOAD_OPTION_HIDDEN ? 'H' : '-',
	       lo.attributes);
	printf("\tlabel: %s\n", label);

	dp_str = efi_dp_str(lo.file_path);
	printf("\tfile_path: %ls\n", dp_str);
	efi_free_pool(dp_str);

	printf("\tdata: %s\n", lo.optional_data);

	free(label);
}

/**
 * show_efi_boot_opt() - dump UEFI load option
 *
 * @id:		Load option number
 *
 * Dump information defined by UEFI load option.
 */
static void show_efi_boot_opt(int id)
{
	char var_name[9];
	u16 var_name16[9], *p;
	efi_guid_t guid;
	void *data = NULL;
	efi_uintn_t size;
	int ret;

	sprintf(var_name, "Boot%04X", id);
	p = var_name16;
	utf8_utf16_strncpy(&p, var_name, 9);
	guid = efi_global_variable_guid;

	size = 0;
	ret = EFI_CALL(RT->get_variable(var_name16, &guid, NULL, &size, NULL));
	if (ret == (int)EFI_BUFFER_TOO_SMALL) {
		data = malloc(size);
		ret = EFI_CALL(RT->get_variable(var_name16, &guid, NULL, &size,
						data));
	}
	if (ret == EFI_SUCCESS)
		show_efi_boot_opt_data(id, data);
	else if (ret == EFI_NOT_FOUND)
		printf("Boot%04X: not found\n", id);

	free(data);
}

/**
 * show_efi_boot_dump() - dump all UEFI load options
 *
 * @cmdtp:	Command table
 * @flag:	Command flag
 * @argc:	Number of arguments
 * @argv:	Argument array
 * Return:	CMD_RET_SUCCESS on success, CMD_RET_RET_FAILURE on failure
 *
 * Implement efidebug "boot dump" sub-command.
 * Dump information of all UEFI load options defined.
 *   - boot dump
 */
static int do_efi_boot_dump(cmd_tbl_t *cmdtp, int flag,
			    int argc, char * const argv[])
{
	char regex[256];
	char * const regexlist[] = {regex};
	char *variables = NULL, *boot, *value;
	int len;
	int id;

	if (argc > 1)
		return CMD_RET_USAGE;

	snprintf(regex, 256, "efi_.*-.*-.*-.*-.*_Boot[0-9A-F]+");

	/* TODO: use GetNextVariableName? */
	len = hexport_r(&env_htab, '\n', H_MATCH_REGEX | H_MATCH_KEY,
			&variables, 0, 1, regexlist);

	if (!len)
		return CMD_RET_SUCCESS;

	if (len < 0)
		return CMD_RET_FAILURE;

	boot = variables;
	while (*boot) {
		value = strstr(boot, "Boot") + 4;
		id = (int)simple_strtoul(value, NULL, 16);
		show_efi_boot_opt(id);
		boot = strchr(boot, '\n');
		if (!*boot)
			break;
		boot++;
	}
	free(variables);

	return CMD_RET_SUCCESS;
}

/**
 * show_efi_boot_order() - show order of UEFI load options
 *
 * Return:	CMD_RET_SUCCESS on success, CMD_RET_RET_FAILURE on failure
 *
 * Show order of UEFI load options defined by BootOrder variable.
 */
static int show_efi_boot_order(void)
{
	efi_guid_t guid;
	u16 *bootorder = NULL;
	efi_uintn_t size;
	int num, i;
	char var_name[9];
	u16 var_name16[9], *p16;
	void *data;
	struct efi_load_option lo;
	char *label, *p;
	size_t label_len16, label_len;
	efi_status_t ret;

	guid = efi_global_variable_guid;
	size = 0;
	ret = EFI_CALL(RT->get_variable(L"BootOrder", &guid, NULL, &size,
					NULL));
	if (ret == EFI_BUFFER_TOO_SMALL) {
		bootorder = malloc(size);
		ret = EFI_CALL(RT->get_variable(L"BootOrder", &guid, NULL,
						&size, bootorder));
	}
	if (ret == EFI_NOT_FOUND) {
		printf("BootOrder not defined\n");
		ret = CMD_RET_SUCCESS;
		goto out;
	} else if (ret != EFI_SUCCESS) {
		ret = CMD_RET_FAILURE;
		goto out;
	}

	num = size / sizeof(u16);
	for (i = 0; i < num; i++) {
		sprintf(var_name, "Boot%04X", bootorder[i]);
		p16 = var_name16;
		utf8_utf16_strncpy(&p16, var_name, 9);

		size = 0;
		ret = EFI_CALL(RT->get_variable(var_name16, &guid, NULL, &size,
						NULL));
		if (ret != EFI_BUFFER_TOO_SMALL) {
			printf("%2d: Boot%04X: (not defined)\n",
			       i + 1, bootorder[i]);
			continue;
		}

		data = malloc(size);
		if (!data) {
			ret = CMD_RET_FAILURE;
			goto out;
		}
		ret = EFI_CALL(RT->get_variable(var_name16, &guid, NULL, &size,
						data));
		if (ret != EFI_SUCCESS) {
			free(data);
			ret = CMD_RET_FAILURE;
			goto out;
		}

		efi_deserialize_load_option(&lo, data);

		label_len16 = u16_strlen(lo.label);
		label_len = utf16_utf8_strnlen(lo.label, label_len16);
		label = malloc(label_len + 1);
		if (!label) {
			free(data);
			ret = CMD_RET_FAILURE;
			goto out;
		}
		p = label;
		utf16_utf8_strncpy(&p, lo.label, label_len16);
		printf("%2d: Boot%04X: %s\n", i + 1, bootorder[i], label);
		free(label);

		free(data);
	}
out:
	free(bootorder);

	return ret;
}

/**
 * do_efi_boot_next() - manage UEFI BootNext variable
 *
 * @cmdtp:	Command table
 * @flag:	Command flag
 * @argc:	Number of arguments
 * @argv:	Argument array
 * Return:	CMD_RET_SUCCESS on success,
 *		CMD_RET_USAGE or CMD_RET_RET_FAILURE on failure
 *
 * Implement efidebug "boot next" sub-command.
 * Set BootNext variable.
 *   - boot next <id>
 */
static int do_efi_boot_next(cmd_tbl_t *cmdtp, int flag,
			    int argc, char * const argv[])
{
	u16 bootnext;
	efi_uintn_t size;
	char *endp;
	efi_guid_t guid;
	efi_status_t ret;

	if (argc != 2)
		return CMD_RET_USAGE;

	bootnext = (u16)simple_strtoul(argv[1], &endp, 16);
	if (*endp != '\0' || bootnext > 0xffff) {
		printf("invalid value: %s\n", argv[1]);
		ret = CMD_RET_FAILURE;
		goto out;
	}

	guid = efi_global_variable_guid;
	size = sizeof(u16);
	ret = EFI_CALL(RT->set_variable(L"BootNext", &guid,
					EFI_VARIABLE_BOOTSERVICE_ACCESS |
					EFI_VARIABLE_RUNTIME_ACCESS,
					size, &bootnext));
	ret = (ret == EFI_SUCCESS ? CMD_RET_SUCCESS : CMD_RET_FAILURE);
out:
	return ret;
}

/**
 * do_efi_boot_order() - manage UEFI BootOrder variable
 *
 * @cmdtp:	Command table
 * @flag:	Command flag
 * @argc:	Number of arguments
 * @argv:	Argument array
 * Return:	CMD_RET_SUCCESS on success, CMD_RET_RET_FAILURE on failure
 *
 * Implement efidebug "boot order" sub-command.
 * Show order of UEFI load options, or change it in BootOrder variable.
 *   - boot order [<id> ...]
 */
static int do_efi_boot_order(cmd_tbl_t *cmdtp, int flag,
			     int argc, char * const argv[])
{
	u16 *bootorder = NULL;
	efi_uintn_t size;
	int id, i;
	char *endp;
	efi_guid_t guid;
	efi_status_t ret;

	if (argc == 1)
		return show_efi_boot_order();

	argc--;
	argv++;

	size = argc * sizeof(u16);
	bootorder = malloc(size);
	if (!bootorder)
		return CMD_RET_FAILURE;

	for (i = 0; i < argc; i++) {
		id = (int)simple_strtoul(argv[i], &endp, 16);
		if (*endp != '\0' || id > 0xffff) {
			printf("invalid value: %s\n", argv[i]);
			ret = CMD_RET_FAILURE;
			goto out;
		}

		bootorder[i] = (u16)id;
	}

	guid = efi_global_variable_guid;
	ret = EFI_CALL(RT->set_variable(L"BootOrder", &guid,
					EFI_VARIABLE_BOOTSERVICE_ACCESS |
					EFI_VARIABLE_RUNTIME_ACCESS,
					size, bootorder));
	ret = (ret == EFI_SUCCESS ? CMD_RET_SUCCESS : CMD_RET_FAILURE);
out:
	free(bootorder);

	return ret;
}

static cmd_tbl_t cmd_efidebug_boot_sub[] = {
	U_BOOT_CMD_MKENT(add, CONFIG_SYS_MAXARGS, 1, do_efi_boot_add, "", ""),
	U_BOOT_CMD_MKENT(rm, CONFIG_SYS_MAXARGS, 1, do_efi_boot_rm, "", ""),
	U_BOOT_CMD_MKENT(dump, CONFIG_SYS_MAXARGS, 1, do_efi_boot_dump, "", ""),
	U_BOOT_CMD_MKENT(next, CONFIG_SYS_MAXARGS, 1, do_efi_boot_next, "", ""),
	U_BOOT_CMD_MKENT(order, CONFIG_SYS_MAXARGS, 1, do_efi_boot_order,
			 "", ""),
};

/**
 * do_efi_boot_opt() - manage UEFI load options
 *
 * @cmdtp:	Command table
 * @flag:	Command flag
 * @argc:	Number of arguments
 * @argv:	Argument array
 * Return:	CMD_RET_SUCCESS on success,
 *		CMD_RET_USAGE or CMD_RET_RET_FAILURE on failure
 *
 * Implement efidebug "boot" sub-command.
 * See above for details of sub-commands.
 */
static int do_efi_boot_opt(cmd_tbl_t *cmdtp, int flag,
			   int argc, char * const argv[])
{
	cmd_tbl_t *cp;

	if (argc < 2)
		return CMD_RET_USAGE;

	argc--; argv++;

	cp = find_cmd_tbl(argv[0], cmd_efidebug_boot_sub,
			  ARRAY_SIZE(cmd_efidebug_boot_sub));
	if (!cp)
		return CMD_RET_USAGE;

	return cp->cmd(cmdtp, flag, argc, argv);
}

static cmd_tbl_t cmd_efidebug_sub[] = {
	U_BOOT_CMD_MKENT(boot, CONFIG_SYS_MAXARGS, 1, do_efi_boot_opt, "", ""),
};

/**
 * do_efidebug() - display and configure UEFI environment
 *
 * @cmdtp:	Command table
 * @flag:	Command flag
 * @argc:	Number of arguments
 * @argv:	Argument array
 * Return:	CMD_RET_SUCCESS on success,
 *		CMD_RET_USAGE or CMD_RET_RET_FAILURE on failure
 *
 * Implement efidebug command which allows us to display and
 * configure UEFI environment.
 * See above for details of sub-commands.
 */
static int do_efidebug(cmd_tbl_t *cmdtp, int flag,
		       int argc, char * const argv[])
{
	cmd_tbl_t *cp;
	efi_status_t r;

	if (argc < 2)
		return CMD_RET_USAGE;

	argc--; argv++;

	/* Initialize UEFI drivers */
	r = efi_init_obj_list();
	if (r != EFI_SUCCESS) {
		printf("Error: Cannot initialize UEFI sub-system, r = %lu\n",
		       r & ~EFI_ERROR_MASK);
		return CMD_RET_FAILURE;
	}

	cp = find_cmd_tbl(argv[0], cmd_efidebug_sub,
			  ARRAY_SIZE(cmd_efidebug_sub));
	if (!cp)
		return CMD_RET_USAGE;

	return cp->cmd(cmdtp, flag, argc, argv);
}

#ifdef CONFIG_SYS_LONGHELP
static char efidebug_help_text[] =
	"  - UEFI Shell-like interface to configure UEFI environment\n"
	"\n"
	"efidebug boot add <bootid> <label> <interface> <devnum>[:<part>] <file path> [<load options>]\n"
	"  - set UEFI BootXXXX variable\n"
	"    <load options> will be passed to UEFI application\n"
	"efidebug boot rm <bootid#1> [<bootid#2> [<bootid#3> [...]]]\n"
	"  - delete UEFI BootXXXX variables\n"
	"efidebug boot dump\n"
	"  - dump all UEFI BootXXXX variables\n"
	"efidebug boot next <bootid>\n"
	"  - set UEFI BootNext variable\n"
	"efidebug boot order [<bootid#1> [<bootid#2> [<bootid#3> [...]]]]\n"
	"  - set/show UEFI boot order\n"
	"\n";
#endif

U_BOOT_CMD(
	efidebug, 10, 0, do_efidebug,
	"Configure UEFI environment",
	efidebug_help_text
);
