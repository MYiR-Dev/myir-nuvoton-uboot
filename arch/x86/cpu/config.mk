# SPDX-License-Identifier: GPL-2.0+
#
# (C) Copyright 2002
# Daniel Engström, Omicron Ceti AB, daniel@omicron.se.

CROSS_COMPILE ?= i386-linux-

# DO NOT MODIFY THE FOLLOWING UNLESS YOU REALLY KNOW WHAT YOU ARE DOING!
LDPPFLAGS += -DRESET_SEG_START=$(CONFIG_RESET_SEG_START)
LDPPFLAGS += -DRESET_VEC_LOC=$(CONFIG_RESET_VEC_LOC)
LDPPFLAGS += -DSTART_16=$(CONFIG_SYS_X86_START16)

ifdef CONFIG_X86_64
ifndef CONFIG_SPL_BUILD
LDSCRIPT = $(srctree)/arch/x86/cpu/u-boot-64.lds
endif
endif
