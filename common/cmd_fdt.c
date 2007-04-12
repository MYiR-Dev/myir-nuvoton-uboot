/*
 * (C) Copyright 2007
 * Gerald Van Baren, Custom IDEAS, vanbaren@cideas.com
 * Based on code written by:
 *   Pantelis Antoniou <pantelis.antoniou@gmail.com> and
 *   Matthew McClintock <msm@freescale.com>
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

#include <common.h>
#include <command.h>
#include <linux/ctype.h>
#include <linux/types.h>

#ifdef CONFIG_OF_LIBFDT
#include <asm/global_data.h>
#include <fdt.h>
#include <libfdt.h>

#define MAX_LEVEL	32		/* how deeply nested we will go */
#define SCRATCHPAD	1024	/* bytes of scratchpad memory */

/*
 * Global data (for the gd->bd)
 */
DECLARE_GLOBAL_DATA_PTR;

/*
 * Scratchpad memory.
 */
static char data[SCRATCHPAD];


/*
 * Function prototypes/declarations.
 */
static int fdt_valid(void);
static void print_data(const void *data, int len);
static int fdt_chosen(void *fdt, ulong initrd_start, ulong initrd_end);
static int fdt_env(void *fdt);
static int fdt_bd_t(void *fdt);


/*
 * Flattened Device Tree command, see the help for parameter definitions.
 */
int do_fdt (cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	char		op;

	if (argc < 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	/*
	 * Figure out which subcommand was given
	 */
	op = argv[1][0];
	/********************************************************************
	 * Set the address of the fdt
	 ********************************************************************/
	if (op == 'a') {
		/*
		 * Set the address [and length] of the fdt.
		 */
		fdt = (struct fdt_header *)simple_strtoul(argv[2], NULL, 16);

		if (!fdt_valid()) {
			return 1;
		}

		if (argc >= 4) {
			int  len;
			int  err;
			/*
			 * Optional new length
			 */
			len =  simple_strtoul(argv[3], NULL, 16);
			if (len < fdt_totalsize(fdt)) {
				printf ("New length %d < existing length %d, ignoring.\n",
					len, fdt_totalsize(fdt));
			} else {
				/*
				 * Open in place with a new length.
				 */
				err = fdt_open_into(fdt, fdt, len);
				if (err != 0) {
					printf ("libfdt: %s\n", fdt_strerror(err));
				}
			}
		}

	/********************************************************************
	 * Move the fdt
	 ********************************************************************/
	} else if (op == 'm') {
		struct fdt_header *newaddr;
		int  len;
		int  err;

		if (argc != 5) {
			printf ("Usage:\n%s\n", cmdtp->usage);
			return 1;
		}

		/*
		 * Set the address and length of the fdt.
		 */
		fdt = (struct fdt_header *)simple_strtoul(argv[2], NULL, 16);
		if (!fdt_valid()) {
			return 1;
		}

		newaddr = (struct fdt_header *)simple_strtoul(argv[3], NULL, 16);
		len     =  simple_strtoul(argv[4], NULL, 16);
		if (len < fdt_totalsize(fdt)) {
			printf ("New length %d < existing length %d, aborting.\n",
				len, fdt_totalsize(fdt));
			return 1;
		}

		/*
		 * Copy to the new location.
		 */
		err = fdt_open_into(fdt, newaddr, len);
		if (err != 0) {
			printf ("libfdt: %s\n", fdt_strerror(err));
			return 1;
		}
		fdt = newaddr;

	/********************************************************************
	 * Set the value of a node in the fdt.
	 ********************************************************************/
	} else if (op == 's') {
		char *pathp;		/* path */
		char *prop;			/* property */
		struct fdt_property *nodep;	/* node struct pointer */
		char *newval;		/* value from the user (as a string) */
		char *vp;			/* temporary value pointer */
		char *cp;			/* temporary char pointer */
		int  nodeoffset;	/* node offset from libfdt */
		int  len;			/* new length of the property */
		int  oldlen;		/* original length of the property */
		unsigned long tmp;	/* holds converted values */
		int  ret;			/* return value */

		/*
		 * Parameters: Node path, property, value.
		 */
		if (argc < 5) {
			printf ("Usage:\n%s\n", cmdtp->usage);
			return 1;
		}

		pathp  = argv[2];
		prop   = argv[3];
		newval = argv[4];

		if (strcmp(pathp, "/") == 0) {
			nodeoffset = 0;
		} else {
			nodeoffset = fdt_path_offset (fdt, pathp);
			if (nodeoffset < 0) {
				/*
			 	 * Not found or something else bad happened.
			 	 */
				printf ("libfdt: %s\n", fdt_strerror(nodeoffset));
				return 1;
			}
		}
		nodep = fdt_getprop (fdt, nodeoffset, prop, &oldlen);
		if (oldlen < 0) {
			printf ("libfdt %s\n", fdt_strerror(oldlen));
			return 1;
		} else if (oldlen == 0) {
			/*
			 * The specified property has no value
			 */
			printf("%s has no value, cannot set one (yet).\n", prop);
			return 1;
		} else {
			/*
			 * Convert the new property
			 */
			vp = data;
			if (*newval == '<') {
				/*
				 * Bigger values than bytes.
				 */
				len = 0;
				newval++;
				while ((*newval != '>') && (*newval != '\0')) {
					cp = newval;
					tmp = simple_strtoul(cp, &newval, 16);
					if ((newval - cp) <= 2) {
						*vp = tmp & 0xFF;
						vp  += 1;
						len += 1;
					} else if ((newval - cp) <= 4) {
						*(uint16_t *)vp = __cpu_to_be16(tmp);
						vp  += 2;
						len += 2;
					} else if ((newval - cp) <= 8) {
						*(uint32_t *)vp = __cpu_to_be32(tmp);
						vp  += 4;
						len += 4;
					} else {
						printf("Sorry, I could not convert \"%s\"\n", cp);
						return 1;
					}
					while (*newval == ' ')
						newval++;
				}
				if (*newval != '>') {
					printf("Unexpected character '%c'\n", *newval);
					return 1;
				}
			} else if (*newval == '[') {
				/*
				 * Byte stream.  Convert the values.
				 */
				len = 0;
				newval++;
				while ((*newval != ']') && (*newval != '\0')) {
					tmp = simple_strtoul(newval, &newval, 16);
					*vp++ = tmp & 0xFF;
					len++;
					while (*newval == ' ')
						newval++;
				}
				if (*newval != ']') {
					printf("Unexpected character '%c'\n", *newval);
					return 1;
				}
			} else {
				/*
				 * Assume it is a string.  Copy it into our data area for
				 * convenience (including the terminating '\0').
				 */
				len = strlen(newval) + 1;
				strcpy(data, newval);
			}

			ret = fdt_setprop(fdt, nodeoffset, prop, data, len);
			if (ret < 0) {
				printf ("libfdt %s\n", fdt_strerror(ret));
				return 1;
			}
		}

	/********************************************************************
	 * Print (recursive) / List (single level)
	 ********************************************************************/
	} else if ((op == 'p') || (op == 'l')) {
		/*
		 * Recursively print (a portion of) the fdt.
		 */
		static int offstack[MAX_LEVEL];
		static char tabs[MAX_LEVEL+1] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
		int depth = MAX_LEVEL;	/* how deep to print */
		char *pathp;		/* path */
		char *prop;			/* property */
		void *nodep;		/* property node pointer */
		int  nodeoffset;	/* node offset from libfdt */
		int  nextoffset;	/* next node offset from libfdt */
		uint32_t tag;		/* tag */
		int  len;			/* length of the property */
		int  level = 0;		/* keep track of nesting level */

		/*
		 * list is an alias for print, but limited to 1 level
		 */
		if (op == 'l') {
			depth = 1;
		}

		/*
		 * Get the starting path.  The root node is an oddball,
		 * the offset is zero and has no name.
		 */
		pathp = argv[2];
		if (argc > 3)
			prop = argv[3];
		else
			prop = NULL;

		if (strcmp(pathp, "/") == 0) {
			nodeoffset = 0;
			printf("/");
		} else {
			nodeoffset = fdt_path_offset (fdt, pathp);
			if (nodeoffset < 0) {
				/*
				 * Not found or something else bad happened.
				 */
				printf ("libfdt %s\n", fdt_strerror(nodeoffset));
				return 1;
			}
		}
		/*
		 * The user passed in a property as well as node path.  Print only
		 * the given property and then return.
		 */
		if (prop) {
			nodep = fdt_getprop (fdt, nodeoffset, prop, &len);
			if (len == 0) {
				printf("%s %s\n", pathp, prop);	/* no property value */
				return 0;
			} else if (len > 0) {
				printf("%s=", prop);
				print_data (nodep, len);
				printf("\n");
				return 0;
			} else {
				printf ("libfdt %s\n", fdt_strerror(len));
				return 1;
			}
		}

		/*
		 * The user passed in a node path and no property, print the node
		 * and all subnodes.
		 */
		offstack[0] = nodeoffset;

		while(level >= 0) {
			tag = fdt_next_tag(fdt, nodeoffset, &nextoffset, &pathp);
			switch(tag) {
			case FDT_BEGIN_NODE:
				if(level <= depth)
					printf("%s%s {\n", &tabs[MAX_LEVEL - level], pathp);
				level++;
				offstack[level] = nodeoffset;
				if (level >= MAX_LEVEL) {
					printf("Aaaiii <splat> nested too deep.\n");
					return 1;
				}
				break;
			case FDT_END_NODE:
				level--;
				if(level <= depth)
					printf("%s};\n", &tabs[MAX_LEVEL - level]);
				if (level == 0) {
					level = -1;		/* exit the loop */
				}
				break;
			case FDT_PROP:
				nodep = fdt_getprop (fdt, offstack[level], pathp, &len);
				if (len < 0) {
					printf ("libfdt %s\n", fdt_strerror(len));
					return 1;
				} else if (len == 0) {
					/* the property has no value */
					if(level <= depth)
						printf("%s%s;\n", &tabs[MAX_LEVEL - level], pathp);
				} else {
					if(level <= depth) {
						printf("%s%s=", &tabs[MAX_LEVEL - level], pathp);
						print_data (nodep, len);
						printf(";\n");
					}
				}
				break;
			case FDT_NOP:
				break;
			case FDT_END:
				return 1;
			default:
				if(level <= depth)
					printf("Unknown tag 0x%08X\n", tag);
				return 1;
			}
			nodeoffset = nextoffset;
		}

	/********************************************************************
	 * Remove a property/node
	 ********************************************************************/
	} else if (op == 'r') {
		int  nodeoffset;	/* node offset from libfdt */
		int  err;

		/*
		 * Get the path.  The root node is an oddball, the offset
		 * is zero and has no name.
		 */
		if (strcmp(argv[2], "/") == 0) {
			nodeoffset = 0;
		} else {
			nodeoffset = fdt_path_offset (fdt, argv[2]);
			if (nodeoffset < 0) {
				/*
				 * Not found or something else bad happened.
				 */
				printf ("libfdt %s\n", fdt_strerror(nodeoffset));
				return 1;
			}
		}
		/*
		 * Do the delete.  A fourth parameter means delete a property,
		 * otherwise delete the node.
		 */
		if (argc > 3) {
			err = fdt_delprop(fdt, nodeoffset, argv[3]);
			if (err < 0) {
				printf("fdt_delprop libfdt: %s\n", fdt_strerror(err));
				return err;
			}
		} else {
			err = fdt_del_node(fdt, nodeoffset);
			if (err < 0) {
				printf("fdt_del_node libfdt: %s\n", fdt_strerror(err));
				return err;
			}
		}

	/********************************************************************
	 * Create a chosen node
	 ********************************************************************/
	} else if (op == 'c') {
		fdt_chosen(fdt, 0, 0);

	/********************************************************************
	 * Create a u-boot-env node
	 ********************************************************************/
	} else if (op == 'e') {
		fdt_env(fdt);

	/********************************************************************
	 * Create a bd_t node
	 ********************************************************************/
	} else if (op == 'b') {
		fdt_bd_t(fdt);

	/********************************************************************
	 * Unrecognized command
	 ********************************************************************/
	} else {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	return 0;
}

/********************************************************************/

static int fdt_valid(void)
{
	if (fdt == NULL) {
		printf ("The address of the fdt is invalid.\n");
		return 0;
	}
	if (!fdt || (fdt_magic(fdt) != FDT_MAGIC)) {
		fdt = NULL;
		printf ("Unrecognized fdt: bad magic\n");
		return 0;
	}
	if (fdt_version(fdt) < FDT_FIRST_SUPPORTED_VERSION) {
		printf ("Unsupported fdt version: $d < %d\n",
			FDT_FIRST_SUPPORTED_VERSION, fdt_version(fdt));
		fdt = NULL;
		return 0;
	}
	if (fdt_last_comp_version(fdt) > FDT_LAST_SUPPORTED_VERSION) {
		printf ("Unsupported fdt version: $d > %d\n",
			fdt_version(fdt), FDT_LAST_SUPPORTED_VERSION);
		fdt = NULL;
		return 0;
	}
	return 1;
}

/********************************************************************/

/*
 * OF flat tree handling
 * Written by: Pantelis Antoniou <pantelis.antoniou@gmail.com>
 * Updated by: Matthew McClintock <msm@freescale.com>
 * Converted to libfdt by: Gerald Van Baren <vanbaren@cideas.com>
 */

static int is_printable_string(const void *data, int len)
{
	const char *s = data;

	/* zero length is not */
	if (len == 0)
		return 0;

	/* must terminate with zero */
	if (s[len - 1] != '\0')
		return 0;

	/* printable or a null byte (concatenated strings) */
	while (((*s == '\0') || isprint(*s)) && (len > 0)) {
		/*
		 * If we see a null, there are three possibilities:
		 * 1) If len == 1, it is the end of the string, printable
		 * 2) Next character also a null, not printable.
		 * 3) Next character not a null, continue to check.
		 */
		if (s[0] == '\0') {
			if (len == 1)
				return 1;
			if (s[1] == '\0')
				return 0;
		}
		s++;
		len--;
	}

	/* Not the null termination, or not done yet: not printable */
	if (*s != '\0' || (len != 0))
		return 0;

	return 1;
}

static void print_data(const void *data, int len)
{
	int j;
	const u8 *s;

	/* no data, don't print */
	if (len == 0)
		return;

	/*
	 * It is a string, but it may have multiple strings (embedded '\0's).
	 */
	if (is_printable_string(data, len)) {
		puts("\"");
		j = 0;
		while (j < len) {
			if (j > 0)
				puts("\", \"");
			puts(data);
			j    += strlen(data) + 1;
			data += strlen(data) + 1;
		}
		puts("\"");
		return;
	}

	switch (len) {
	case 1:	 /* byte */
		printf("<%02x>", (*(u8 *) data) & 0xff);
		break;
	case 2:	 /* half-word */
		printf("<%04x>", be16_to_cpu(*(u16 *) data) & 0xffff);
		break;
	case 4:	 /* word */
		printf("<%08x>", be32_to_cpu(*(u32 *) data) & 0xffffffffU);
		break;
	case 8:	 /* double-word */
#if __WORDSIZE == 64
		printf("<%016llx>", be64_to_cpu(*(uint64_t *) data));
#else
		printf("<%08x ", be32_to_cpu(*(u32 *) data) & 0xffffffffU);
		data += 4;
		printf("%08x>", be32_to_cpu(*(u32 *) data) & 0xffffffffU);
#endif
		break;
	default:		/* anything else... hexdump */
		printf("[");
		for (j = 0, s = data; j < len; j++)
			printf("%02x%s", s[j], j < len - 1 ? " " : "");
		printf("]");

		break;
	}
}

/********************************************************************/

static int fdt_chosen(void *fdt, ulong initrd_start, ulong initrd_end)
{
	bd_t *bd = gd->bd;
	int   nodeoffset;
	int   err;
	u32   tmp;			/* used to set 32 bit integer properties */
	char  *str;			/* used to set string properties */
	ulong clock;

	if (initrd_start && initrd_end) {
		err = fdt_add_reservemap_entry(fdt,
			initrd_start, initrd_end - initrd_start + 1);
		if (err < 0) {
			printf("libfdt: %s\n", fdt_strerror(err));
			return err;
		}
	}

	/*
	 * See if we already have a "chosen" node, create it if not.
	 */
	nodeoffset = fdt_path_offset (fdt, "/chosen");
	if (nodeoffset < 0) {
		/*
		 * Create a new node "/chosen" (offset 0 is root level)
		 */
		nodeoffset = fdt_add_subnode(fdt, 0, "chosen");
		if (nodeoffset < 0) {
			printf("libfdt: %s\n", fdt_strerror(nodeoffset));
			return nodeoffset;
		}
	}

	str = getenv("bootargs");
	if (str != NULL) {
		err = fdt_setprop(fdt, nodeoffset, "bootargs", str, strlen(str)+1);
		if (err < 0)
			printf("libfdt: %s\n", fdt_strerror(err));
	}
	if (initrd_start && initrd_end) {
		tmp = __cpu_to_be32(initrd_start);
		err = fdt_setprop(fdt, nodeoffset, "linux,initrd-start", &tmp, sizeof(tmp));
		if (err < 0)
			printf("libfdt: %s\n", fdt_strerror(err));
		tmp = __cpu_to_be32(initrd_end);
		err = fdt_setprop(fdt, nodeoffset, "linux,initrd-end", &tmp, sizeof(tmp));
		if (err < 0)
			printf("libfdt: %s\n", fdt_strerror(err));
	}
#ifdef OF_STDOUT_PATH
	err = fdt_setprop(fdt, nodeoffset, "linux,stdout-path", OF_STDOUT_PATH, strlen(OF_STDOUT_PATH)+1);
	if (err < 0)
		printf("libfdt: %s\n", fdt_strerror(err));
#endif

	nodeoffset = fdt_path_offset (fdt, "/cpus");
	if (nodeoffset >= 0) {
		clock = cpu_to_be32(bd->bi_intfreq);
		err = fdt_setprop(fdt, nodeoffset, "clock-frequency", &clock, 4);
		if (err < 0)
			printf("libfdt: %s\n", fdt_strerror(err));
	}
#ifdef OF_TBCLK
	nodeoffset = fdt_path_offset (fdt, "/cpus/" OF_CPU "/timebase-frequency");
	if (nodeoffset >= 0) {
		clock = cpu_to_be32(OF_TBCLK);
		err = fdt_setprop(fdt, nodeoffset, "clock-frequency", &clock, 4);
		if (err < 0)
			printf("libfdt: %s\n", fdt_strerror(err));
	}
#endif
}

/********************************************************************/

#ifdef CONFIG_OF_HAS_BD_T

/* Function that returns a character from the environment */
extern uchar(*env_get_char) (int);

#define BDM(x)	{	.name = #x, .offset = offsetof(bd_t, bi_ ##x ) }

static const struct {
	const char *name;
	int offset;
} bd_map[] = {
	BDM(memstart),
	BDM(memsize),
	BDM(flashstart),
	BDM(flashsize),
	BDM(flashoffset),
	BDM(sramstart),
	BDM(sramsize),
#if defined(CONFIG_5xx) || defined(CONFIG_8xx) || defined(CONFIG_8260) \
	|| defined(CONFIG_E500)
	BDM(immr_base),
#endif
#if defined(CONFIG_MPC5xxx)
	BDM(mbar_base),
#endif
#if defined(CONFIG_MPC83XX)
	BDM(immrbar),
#endif
#if defined(CONFIG_MPC8220)
	BDM(mbar_base),
	BDM(inpfreq),
	BDM(pcifreq),
	BDM(pevfreq),
	BDM(flbfreq),
	BDM(vcofreq),
#endif
	BDM(bootflags),
	BDM(ip_addr),
	BDM(intfreq),
	BDM(busfreq),
#ifdef CONFIG_CPM2
	BDM(cpmfreq),
	BDM(brgfreq),
	BDM(sccfreq),
	BDM(vco),
#endif
#if defined(CONFIG_MPC5xxx)
	BDM(ipbfreq),
	BDM(pcifreq),
#endif
	BDM(baudrate),
};

static int fdt_env(void *fdt)
{
	int   nodeoffset;
	int   err;
	int   k, nxt;
	int i;
	static char tmpenv[256];

	/*
	 * See if we already have a "u-boot-env" node, delete it if so.
	 * Then create a new empty node.
	 */
	nodeoffset = fdt_path_offset (fdt, "/u-boot-env");
	if (nodeoffset >= 0) {
		err = fdt_del_node(fdt, nodeoffset);
		if (err < 0) {
			printf("libfdt: %s\n", fdt_strerror(err));
			return err;
		}
	}
	/*
	 * Create a new node "/u-boot-env" (offset 0 is root level)
	 */
	nodeoffset = fdt_add_subnode(fdt, 0, "u-boot-env");
	if (nodeoffset < 0) {
		printf("libfdt: %s\n", fdt_strerror(nodeoffset));
		return nodeoffset;
	}

	for (i = 0; env_get_char(i) != '\0'; i = nxt + 1) {
		char *s, *lval, *rval;

		/*
		 * Find the end of the name=definition
		 */
		for (nxt = i; env_get_char(nxt) != '\0'; ++nxt)
			;
		s = tmpenv;
		for (k = i; k < nxt && s < &tmpenv[sizeof(tmpenv) - 1]; ++k)
			*s++ = env_get_char(k);
		*s++ = '\0';
		lval = tmpenv;
		/*
		 * Find the first '=': it separates the name from the value
		 */
		s = strchr(tmpenv, '=');
		if (s != NULL) {
			*s++ = '\0';
			rval = s;
		} else
			continue;
		err = fdt_setprop(fdt, nodeoffset, lval, rval, strlen(rval)+1);
		if (err < 0) {
			printf("\"%s\" - libfdt: %s\n", lval, fdt_strerror(err));
			return err;
		}
	}
	return 0;
}
#endif /* CONFIG_OF_HAS_UBOOT_ENV */

/********************************************************************/

#ifdef CONFIG_OF_HAS_BD_T
static int fdt_bd_t(void *fdt)
{
	bd_t *bd = gd->bd;
	int   nodeoffset;
	int   err;
	u32   tmp;			/* used to set 32 bit integer properties */
	int i;

	/*
	 * See if we already have a "bd_t" node, delete it if so.
	 * Then create a new empty node.
	 */
	nodeoffset = fdt_path_offset (fdt, "/bd_t");
	if (nodeoffset >= 0) {
		err = fdt_del_node(fdt, nodeoffset);
		if (err < 0) {
			printf("libfdt: %s\n", fdt_strerror(err));
			return err;
		}
	}
	/*
	 * Create a new node "/bd_t" (offset 0 is root level)
	 */
	nodeoffset = fdt_add_subnode(fdt, 0, "bd_t");
	if (nodeoffset < 0) {
		printf("libfdt: %s\n", fdt_strerror(nodeoffset));
		return nodeoffset;
	}
	/*
	 * Use the string/pointer structure to create the entries...
	 */
	for (i = 0; i < sizeof(bd_map)/sizeof(bd_map[0]); i++) {
		tmp = cpu_to_be32(getenv("bootargs"));
		err = fdt_setprop(fdt, nodeoffset, bd_map[i].name, &tmp, sizeof(tmp));
		if (err < 0)
			printf("libfdt: %s\n", fdt_strerror(err));
	}
	/*
	 * Add a couple of oddball entries...
	 */
	err = fdt_setprop(fdt, nodeoffset, "enetaddr", &bd->bi_enetaddr, 6);
	if (err < 0)
		printf("libfdt: %s\n", fdt_strerror(err));
	err = fdt_setprop(fdt, nodeoffset, "ethspeed", &bd->bi_ethspeed, 4);
	if (err < 0)
		printf("libfdt: %s\n", fdt_strerror(err));

#ifdef CONFIG_OF_BOARD_SETUP
	ft_board_setup(fdt, bd);
#endif

	return 0;
}
#endif /* CONFIG_OF_HAS_BD_T */

/********************************************************************/

U_BOOT_CMD(
	fdt,	5,	0,	do_fdt,
	"fdt     - flattened device tree utility commands\n",
	    "addr   <addr> [<length>]        - Set the fdt location to <addr>\n"
	"fdt move   <fdt> <newaddr> <length> - Copy the fdt to <addr>\n"
	"fdt print  <path> [<prop>]          - Recursive print starting at <path>\n"
	"fdt list   <path> [<prop>]          - Print one level starting at <path>\n"
	"fdt set    <path> <prop> [<val>]    - Set <property> [to <val>]\n"
	"fdt mknode <path> <node>            - Create a new node after <path>\n"
	"fdt rm     <path> [<prop>]          - Delete the node or <property>\n"
	"fdt chosen - Add/update the \"/chosen\" branch in the tree\n"
#ifdef CONFIG_OF_HAS_UBOOT_ENV
	"fdt env    - Add/replace the \"/u-boot-env\" branch in the tree\n"
#endif
#ifdef CONFIG_OF_HAS_BD_T
	"fdt bd_t   - Add/replace the \"/bd_t\" branch in the tree\n"
#endif
	"Hints:\n"
	" * Set a larger length with the fdt addr command to add to the blob.\n"
	" * If the property you are setting/printing has a '#' character,\n"
	"     you MUST escape it with a \\ character or quote it with \" or\n"
	"     it will be ignored as a comment.\n"
	" * If the value has spaces in it, you MUST escape the spaces with\n"
	"     \\ characters or quote it with \"\"\n"
	"Examples: fdt print /               # print the whole tree\n"
	"          fdt print /cpus \"#address-cells\"\n"
	"          fdt set   /cpus \"#address-cells\" \"[00 00 00 01]\"\n"
);

#endif /* CONFIG_OF_FLAT_TREE */
