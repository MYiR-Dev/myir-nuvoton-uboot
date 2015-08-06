/*
 * Copyright (c) 2015 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CONFIG_SPRING_H
#define __CONFIG_SPRING_H

#include <configs/exynos5250-common.h>
#include <configs/exynos5-dt-common.h>
#include <configs/exynos5-common.h>

#define CONFIG_BOARD_COMMON

#define CONFIG_SYS_PROMPT		"spring # "
#define CONFIG_IDENT_STRING		" for spring"
#define CONFIG_DEFAULT_CONSOLE		"console=ttySAC1,115200n8\0"

#endif	/* __CONFIG_SPRING_H */
