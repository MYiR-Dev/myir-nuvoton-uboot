/* SPDX-License-Identifier: GPL-2.0 */
/*
 * tegra_ahub.h - Definitions for Tegra124 audio hub driver
 * Taken from dc tegra_ahub.h
 *
 * Copyright 2018 Google LLC
 * Copyright (c) 2013, NVIDIA CORPORATION.  All rights reserved.
 */

#ifndef _TEGRA_AHUB_H_
#define _TEGRA_AHUB_H_

/*
 * Each TX CIF transmits data into the XBAR. Each RX CIF can receive audio
 * transmitted by a particular TX CIF.
 */
struct xbar_regs {
	u32 apbif_rx0;		/* AUDIO_APBIF_RX0, offset 0x00 */
	u32 apbif_rx1;		/* AUDIO_APBIF_RX1, offset 0x04 */
	u32 apbif_rx2;		/* AUDIO_APBIF_RX2, offset 0x08 */
	u32 apbif_rx3;		/* AUDIO_APBIF_RX3, offset 0x0C */

	u32 i2s0_rx0;		/* AUDIO_I2S0_RX0,  offset 0x10 */
	u32 i2s1_rx0;		/* AUDIO_I2S1_RX0,  offset 0x14 */
	u32 i2s2_rx0;		/* AUDIO_I2S2_RX0,  offset 0x18 */
	u32 i2s3_rx0;		/* AUDIO_I2S3_RX0,  offset 0x1C */
	u32 i2s4_rx0;		/* AUDIO_I2S4_RX0,  offset 0x20 */

	u32 dam0_rx0;		/* AUDIO_DAM0_RX0,  offset 0x24 */
	u32 dam0_rx1;		/* AUDIO_DAM0_RX1,  offset 0x28 */
	u32 dam1_rx0;		/* AUDIO_DAM1_RX0,  offset 0x2C */
	u32 dam1_rx1;		/* AUDIO_DAM1_RX1,  offset 0x30 */
	u32 dam2_rx0;		/* AUDIO_DAM2_RX0,  offset 0x34 */
	u32 dam2_rx1;		/* AUDIO_DAM2_RX1,  offset 0x38 */

	u32 spdif_rx0;		/* AUDIO_SPDIF_RX0, offset 0x3C */
	u32 spdif_rx1;		/* AUDIO_SPDIF_RX1, offset 0x40 */

	u32 apbif_rx4;		/* AUDIO_APBIF_RX4, offset 0x44 */
	u32 apbif_rx5;		/* AUDIO_APBIF_RX4, offset 0x48 */
	u32 apbif_rx6;		/* AUDIO_APBIF_RX4, offset 0x4C */
	u32 apbif_rx7;		/* AUDIO_APBIF_RX4, offset 0x50 */
	u32 apbif_rx8;		/* AUDIO_APBIF_RX4, offset 0x54 */
	u32 apbif_rx9;		/* AUDIO_APBIF_RX4, offset 0x58 */

	u32 amx0_rx0;		/* AUDIO_AMX0_RX0,  offset 0x5C */
	u32 amx0_rx1;		/* AUDIO_AMX0_RX1,  offset 0x60 */
	u32 amx0_rx2;		/* AUDIO_AMX0_RX2,  offset 0x64 */
	u32 amx0_rx3;		/* AUDIO_AMX0_RX3,  offset 0x68 */

	u32 adx0_rx0;		/* AUDIO_ADX0_RX0,  offset 0x6C */
};

struct apbif_regs {
	u32 channel0_ctrl;		/* APBIF_CHANNEL0_CTRL */
	u32 channel0_clr;		/* APBIF_CHANNEL0_CLEAR */
	u32 channel0_stat;		/* APBIF_CHANNEL0_STATUS */
	u32 channel0_txfifo;		/* APBIF_CHANNEL0_TXFIFO */
	u32 channel0_rxfifo;		/* APBIF_CHANNEL0_RXFIFO */
	u32 channel0_cif_tx0_ctrl;	/* APBIF_AUDIOCIF_TX0_CTRL */
	u32 channel0_cif_rx0_ctrl;	/* APBIF_AUDIOCIF_RX0_CTRL */
	u32 channel0_reserved0;		/* RESERVED, offset 0x1C */
	/* ahub_channel1_ctrl/clr/stat/txfifo/rxfifl/ciftx/cifrx ... here */
	/* ahub_channel2_ctrl/clr/stat/txfifo/rxfifl/ciftx/cifrx ... here */
	/* ahub_channel3_ctrl/clr/stat/txfifo/rxfifl/ciftx/cifrx ... here */
	u32 reserved123[3 * 8];
	u32 config_link_ctrl;		/* APBIF_CONFIG_LINK_CTRL_0, off 0x80 */
	u32 misc_ctrl;			/* APBIF_MISC_CTRL_0, offset 0x84 */
	u32 apbdma_live_stat;		/* APBIF_APBDMA_LIVE_STATUS_0 */
	u32 i2s_live_stat;		/* APBIF_I2S_LIVE_STATUS_0 */
	u32 dam0_live_stat;		/* APBIF_DAM0_LIVE_STATUS_0 */
	u32 dam1_live_stat;		/* APBIF_DAM0_LIVE_STATUS_0 */
	u32 dam2_live_stat;		/* APBIF_DAM0_LIVE_STATUS_0 */
	u32 spdif_live_stat;		/* APBIF_SPDIF_LIVE_STATUS_0 */
	u32 i2s_int_mask;		/* APBIF_I2S_INT_MASK_0, offset B0 */
	u32 dam_int_mask;		/* APBIF_DAM_INT_MASK_0 */
	u32 reserved_int_mask;		/* RESERVED, offset 0xB8 */
	u32 spdif_int_mask;		/* APBIF_SPDIF_INT_MASK_0 */
	u32 apbif_int_mask;		/* APBIF_APBIF_INT_MASK_0, off C0 */
	u32 reserved2_int_mask;		/* RESERVED, offset 0xC4 */
	u32 i2s_int_stat;		/* APBIF_I2S_INT_STATUS_0, offset C8 */
	u32 dam_int_stat;		/* APBIF_DAM_INT_STATUS_0 */
	u32 reserved_int_stat;		/* RESERVED, offset 0xD0 */
	u32 spdif_int_stat;		/* APBIF_SPDIF_INT_STATUS_0 */
	u32 apbif_int_stat;		/* APBIF_APBIF_INT_STATUS_0 */
	u32 reserved2_int_stat;		/* RESERVED, offset 0xDC */
	u32 i2s_int_src;		/* APBIF_I2S_INT_SOURCE_0, offset E0 */
	u32 dam_int_src;		/* APBIF_DAM_INT_SOURCE_0 */
	u32 reserved_int_src;		/* RESERVED, offset 0xE8 */
	u32 spdif_int_src;		/* APBIF_SPDIF_INT_SOURCE_0 */
	u32 apbif_int_src;		/* APBIF_APBIF_INT_SOURCE_0, off F0 */
	u32 reserved2_int_src;		/* RESERVED, offset 0xF4 */
	u32 i2s_int_set;		/* APBIF_I2S_INT_SET_0, offset 0xF8 */
	u32 dam_int_set;		/* APBIF_DAM_INT_SET_0, offset 0xFC */
	u32 spdif_int_set;		/* APBIF_SPDIF_INT_SET_0, off 0x100 */
	u32 apbif_int_set;		/* APBIF_APBIF_INT_SET_0, off 0x104 */
};

/*
 * Tegra AHUB Registers Definition
 */
enum {
	TEGRA_AUDIOCIF_CTRL_FIFO_THRESHOLD_SHIFT = 24,
	TEGRA_AUDIOCIF_CTRL_FIFO_THRESHOLD_MASK_US = 0x3f,
	TEGRA_AUDIOCIF_CTRL_FIFO_THRESHOLD_MASK =
			TEGRA_AUDIOCIF_CTRL_FIFO_THRESHOLD_MASK_US <<
			TEGRA_AUDIOCIF_CTRL_FIFO_THRESHOLD_SHIFT,

	/* Channel count minus 1 */
	TEGRA_AUDIOCIF_CTRL_AUDIO_CHANNELS_SHIFT = 20,
	TEGRA_AUDIOCIF_CTRL_AUDIO_CHANNELS_MASK_US = 0xf,
	TEGRA_AUDIOCIF_CTRL_AUDIO_CHANNELS_MASK =
			TEGRA_AUDIOCIF_CTRL_AUDIO_CHANNELS_MASK_US <<
			TEGRA_AUDIOCIF_CTRL_AUDIO_CHANNELS_SHIFT,

	/* Channel count minus 1 */
	TEGRA_AUDIOCIF_CTRL_CLIENT_CHANNELS_SHIFT = 16,
	TEGRA_AUDIOCIF_CTRL_CLIENT_CHANNELS_MASK_US = 0xf,
	TEGRA_AUDIOCIF_CTRL_CLIENT_CHANNELS_MASK =
			TEGRA_AUDIOCIF_CTRL_CLIENT_CHANNELS_MASK_US <<
			TEGRA_AUDIOCIF_CTRL_CLIENT_CHANNELS_SHIFT,

	TEGRA_AUDIOCIF_BITS_4 = 0,
	TEGRA_AUDIOCIF_BITS_8 = 1,
	TEGRA_AUDIOCIF_BITS_12 = 2,
	TEGRA_AUDIOCIF_BITS_16 = 3,
	TEGRA_AUDIOCIF_BITS_20 = 4,
	TEGRA_AUDIOCIF_BITS_24 = 5,
	TEGRA_AUDIOCIF_BITS_28 = 6,
	TEGRA_AUDIOCIF_BITS_32 = 7,

	TEGRA_AUDIOCIF_CTRL_AUDIO_BITS_SHIFT = 12,
	TEGRA_AUDIOCIF_CTRL_AUDIO_BITS_MASK =
		7 << TEGRA_AUDIOCIF_CTRL_AUDIO_BITS_SHIFT,
	TEGRA_AUDIOCIF_CTRL_AUDIO_BITS_4 =
		TEGRA_AUDIOCIF_BITS_4  << TEGRA_AUDIOCIF_CTRL_AUDIO_BITS_SHIFT,
	TEGRA_AUDIOCIF_CTRL_AUDIO_BITS_8 =
		TEGRA_AUDIOCIF_BITS_8  << TEGRA_AUDIOCIF_CTRL_AUDIO_BITS_SHIFT,
	TEGRA_AUDIOCIF_CTRL_AUDIO_BITS_12 =
		TEGRA_AUDIOCIF_BITS_12 << TEGRA_AUDIOCIF_CTRL_AUDIO_BITS_SHIFT,
	TEGRA_AUDIOCIF_CTRL_AUDIO_BITS_16 =
		TEGRA_AUDIOCIF_BITS_16 << TEGRA_AUDIOCIF_CTRL_AUDIO_BITS_SHIFT,
	TEGRA_AUDIOCIF_CTRL_AUDIO_BITS_20 =
		TEGRA_AUDIOCIF_BITS_20 << TEGRA_AUDIOCIF_CTRL_AUDIO_BITS_SHIFT,
	TEGRA_AUDIOCIF_CTRL_AUDIO_BITS_24 =
		TEGRA_AUDIOCIF_BITS_24 << TEGRA_AUDIOCIF_CTRL_AUDIO_BITS_SHIFT,
	TEGRA_AUDIOCIF_CTRL_AUDIO_BITS_28 =
		TEGRA_AUDIOCIF_BITS_28 << TEGRA_AUDIOCIF_CTRL_AUDIO_BITS_SHIFT,
	TEGRA_AUDIOCIF_CTRL_AUDIO_BITS_32 =
		TEGRA_AUDIOCIF_BITS_32 << TEGRA_AUDIOCIF_CTRL_AUDIO_BITS_SHIFT,

	TEGRA_AUDIOCIF_CTRL_CLIENT_BITS_SHIFT = 8,
	TEGRA_AUDIOCIF_CTRL_CLIENT_BITS_MASK =
		7 << TEGRA_AUDIOCIF_CTRL_CLIENT_BITS_SHIFT,
	TEGRA_AUDIOCIF_CTRL_CLIENT_BITS_4 =
		TEGRA_AUDIOCIF_BITS_4  << TEGRA_AUDIOCIF_CTRL_CLIENT_BITS_SHIFT,
	TEGRA_AUDIOCIF_CTRL_CLIENT_BITS_8 =
		TEGRA_AUDIOCIF_BITS_8  << TEGRA_AUDIOCIF_CTRL_CLIENT_BITS_SHIFT,
	TEGRA_AUDIOCIF_CTRL_CLIENT_BITS_12 =
		TEGRA_AUDIOCIF_BITS_12 << TEGRA_AUDIOCIF_CTRL_CLIENT_BITS_SHIFT,
	TEGRA_AUDIOCIF_CTRL_CLIENT_BITS_16 =
		TEGRA_AUDIOCIF_BITS_16 << TEGRA_AUDIOCIF_CTRL_CLIENT_BITS_SHIFT,
	TEGRA_AUDIOCIF_CTRL_CLIENT_BITS_20 =
		TEGRA_AUDIOCIF_BITS_20 << TEGRA_AUDIOCIF_CTRL_CLIENT_BITS_SHIFT,
	TEGRA_AUDIOCIF_CTRL_CLIENT_BITS_24 =
		TEGRA_AUDIOCIF_BITS_24 << TEGRA_AUDIOCIF_CTRL_CLIENT_BITS_SHIFT,
	TEGRA_AUDIOCIF_CTRL_CLIENT_BITS_28 =
		TEGRA_AUDIOCIF_BITS_28 << TEGRA_AUDIOCIF_CTRL_CLIENT_BITS_SHIFT,
	TEGRA_AUDIOCIF_CTRL_CLIENT_BITS_32 =
		TEGRA_AUDIOCIF_BITS_32 << TEGRA_AUDIOCIF_CTRL_CLIENT_BITS_SHIFT,

	TEGRA_AUDIOCIF_EXPAND_ZERO = 0,
	TEGRA_AUDIOCIF_EXPAND_ONE = 1,
	TEGRA_AUDIOCIF_EXPAND_LFSR = 2,

	TEGRA_AUDIOCIF_CTRL_EXPAND_SHIFT = 6,
	TEGRA_AUDIOCIF_CTRL_EXPAND_MASK = 3 << TEGRA_AUDIOCIF_CTRL_EXPAND_SHIFT,
	TEGRA_AUDIOCIF_CTRL_EXPAND_ZERO =
		TEGRA_AUDIOCIF_EXPAND_ZERO << TEGRA_AUDIOCIF_CTRL_EXPAND_SHIFT,
	TEGRA_AUDIOCIF_CTRL_EXPAND_ONE =
		TEGRA_AUDIOCIF_EXPAND_ONE  << TEGRA_AUDIOCIF_CTRL_EXPAND_SHIFT,
	TEGRA_AUDIOCIF_CTRL_EXPAND_LFSR =
		TEGRA_AUDIOCIF_EXPAND_LFSR << TEGRA_AUDIOCIF_CTRL_EXPAND_SHIFT,

	TEGRA_AUDIOCIF_STEREO_CONV_CH0 = 0,
	TEGRA_AUDIOCIF_STEREO_CONV_CH1 = 1,
	TEGRA_AUDIOCIF_STEREO_CONV_AVG = 2,

	TEGRA_AUDIOCIF_CTRL_STEREO_CONV_SHIFT = 4,
	TEGRA_AUDIOCIF_CTRL_STEREO_CONV_MASK =
			3 << TEGRA_AUDIOCIF_CTRL_STEREO_CONV_SHIFT,
	TEGRA_AUDIOCIF_CTRL_STEREO_CONV_CH0 =
			TEGRA_AUDIOCIF_STEREO_CONV_CH0 <<
			TEGRA_AUDIOCIF_CTRL_STEREO_CONV_SHIFT,
	TEGRA_AUDIOCIF_CTRL_STEREO_CONV_CH1 =
			TEGRA_AUDIOCIF_STEREO_CONV_CH1 <<
			TEGRA_AUDIOCIF_CTRL_STEREO_CONV_SHIFT,
	TEGRA_AUDIOCIF_CTRL_STEREO_CONV_AVG =
			TEGRA_AUDIOCIF_STEREO_CONV_AVG <<
			TEGRA_AUDIOCIF_CTRL_STEREO_CONV_SHIFT,

	TEGRA_AUDIOCIF_CTRL_REPLICATE = 3,

	TEGRA_AUDIOCIF_DIRECTION_TX = 0,
	TEGRA_AUDIOCIF_DIRECTION_RX = 1,

	TEGRA_AUDIOCIF_CTRL_DIRECTION_SHIFT = 2,
	TEGRA_AUDIOCIF_CTRL_DIRECTION_MASK =
			1 << TEGRA_AUDIOCIF_CTRL_DIRECTION_SHIFT,
	TEGRA_AUDIOCIF_CTRL_DIRECTION_TX =
			TEGRA_AUDIOCIF_DIRECTION_TX <<
			TEGRA_AUDIOCIF_CTRL_DIRECTION_SHIFT,
	TEGRA_AUDIOCIF_CTRL_DIRECTION_RX =
			TEGRA_AUDIOCIF_DIRECTION_RX <<
			TEGRA_AUDIOCIF_CTRL_DIRECTION_SHIFT,

	TEGRA_AUDIOCIF_TRUNCATE_ROUND = 0,
	TEGRA_AUDIOCIF_TRUNCATE_CHOP = 1,

	TEGRA_AUDIOCIF_CTRL_TRUNCATE_SHIFT = 1,
	TEGRA_AUDIOCIF_CTRL_TRUNCATE_MASK =
			1 << TEGRA_AUDIOCIF_CTRL_TRUNCATE_SHIFT,
	TEGRA_AUDIOCIF_CTRL_TRUNCATE_ROUND =
			TEGRA_AUDIOCIF_TRUNCATE_ROUND <<
			TEGRA_AUDIOCIF_CTRL_TRUNCATE_SHIFT,
	TEGRA_AUDIOCIF_CTRL_TRUNCATE_CHOP =
			TEGRA_AUDIOCIF_TRUNCATE_CHOP  <<
			TEGRA_AUDIOCIF_CTRL_TRUNCATE_SHIFT,

	TEGRA_AUDIOCIF_MONO_CONV_ZERO = 0,
	TEGRA_AUDIOCIF_MONO_CONV_COPY = 1,

	TEGRA_AUDIOCIF_CTRL_MONO_CONV_SHIFT = 0,
	TEGRA_AUDIOCIF_CTRL_MONO_CONV_MASK =
			1 << TEGRA_AUDIOCIF_CTRL_MONO_CONV_SHIFT,
	TEGRA_AUDIOCIF_CTRL_MONO_CONV_ZERO =
			TEGRA_AUDIOCIF_MONO_CONV_ZERO <<
			TEGRA_AUDIOCIF_CTRL_MONO_CONV_SHIFT,
	TEGRA_AUDIOCIF_CTRL_MONO_CONV_COPY =
			TEGRA_AUDIOCIF_MONO_CONV_COPY <<
			TEGRA_AUDIOCIF_CTRL_MONO_CONV_SHIFT,

	/* Registers within TEGRA_AUDIO_CLUSTER_BASE */

	TEGRA_AHUB_CHANNEL_CTRL = 0x0,
	TEGRA_AHUB_CHANNEL_CTRL_STRIDE = 0x20,
	TEGRA_AHUB_CHANNEL_CTRL_COUNT = 4,
	TEGRA_AHUB_CHANNEL_CTRL_TX_EN = 1 << 31,
	TEGRA_AHUB_CHANNEL_CTRL_RX_EN = 1 << 30,
	TEGRA_AHUB_CHANNEL_CTRL_LOOPBACK = 1 << 29,

	TEGRA_AHUB_CHANNEL_CTRL_TX_THRESHOLD_SHIFT = 16,
	TEGRA_AHUB_CHANNEL_CTRL_TX_THRESHOLD_MASK_US = 0xff,
	TEGRA_AHUB_CHANNEL_CTRL_TX_THRESHOLD_MASK =
			TEGRA_AHUB_CHANNEL_CTRL_TX_THRESHOLD_MASK_US <<
			TEGRA_AHUB_CHANNEL_CTRL_TX_THRESHOLD_SHIFT,

	TEGRA_AHUB_CHANNEL_CTRL_RX_THRESHOLD_SHIFT = 8,
	TEGRA_AHUB_CHANNEL_CTRL_RX_THRESHOLD_MASK_US = 0xff,
	TEGRA_AHUB_CHANNEL_CTRL_RX_THRESHOLD_MASK =
			TEGRA_AHUB_CHANNEL_CTRL_RX_THRESHOLD_MASK_US <<
			TEGRA_AHUB_CHANNEL_CTRL_RX_THRESHOLD_SHIFT,

	TEGRA_AHUB_CHANNEL_CTRL_TX_PACK_EN = 1 << 6,

	TEGRA_PACK_8_4 = 2,
	TEGRA_PACK_16 = 3,

	TEGRA_AHUB_CHANNEL_CTRL_TX_PACK_SHIFT = 4,
	TEGRA_AHUB_CHANNEL_CTRL_TX_PACK_MASK_US = 3,
	TEGRA_AHUB_CHANNEL_CTRL_TX_PACK_MASK =
		TEGRA_AHUB_CHANNEL_CTRL_TX_PACK_MASK_US <<
		TEGRA_AHUB_CHANNEL_CTRL_TX_PACK_SHIFT,
	TEGRA_AHUB_CHANNEL_CTRL_TX_PACK_8_4 =
		TEGRA_PACK_8_4 << TEGRA_AHUB_CHANNEL_CTRL_TX_PACK_SHIFT,
	TEGRA_AHUB_CHANNEL_CTRL_TX_PACK_16 =
		TEGRA_PACK_16 << TEGRA_AHUB_CHANNEL_CTRL_TX_PACK_SHIFT,

	TEGRA_AHUB_CHANNEL_CTRL_RX_PACK_EN = 1 << 2,

	TEGRA_AHUB_CHANNEL_CTRL_RX_PACK_SHIFT = 0,
	TEGRA_AHUB_CHANNEL_CTRL_RX_PACK_MASK_US = 3,
	TEGRA_AHUB_CHANNEL_CTRL_RX_PACK_MASK =
		TEGRA_AHUB_CHANNEL_CTRL_RX_PACK_MASK_US <<
		TEGRA_AHUB_CHANNEL_CTRL_RX_PACK_SHIFT,
	TEGRA_AHUB_CHANNEL_CTRL_RX_PACK_8_4 =
		TEGRA_PACK_8_4 << TEGRA_AHUB_CHANNEL_CTRL_RX_PACK_SHIFT,
	TEGRA_AHUB_CHANNEL_CTRL_RX_PACK_16 =
		TEGRA_PACK_16 << TEGRA_AHUB_CHANNEL_CTRL_RX_PACK_SHIFT,

	/* TEGRA_AHUB_CHANNEL_CLEAR */

	TEGRA_AHUB_CHANNEL_CLEAR = 0x4,
	TEGRA_AHUB_CHANNEL_CLEAR_STRIDE = 0x20,
	TEGRA_AHUB_CHANNEL_CLEAR_COUNT = 4,
	TEGRA_AHUB_CHANNEL_CLEAR_TX_SOFT_RESET = 1 << 31,
	TEGRA_AHUB_CHANNEL_CLEAR_RX_SOFT_RESET = 1 << 30,

	TEGRA_AHUB_CHANNEL_STATUS = 0x8,
	TEGRA_AHUB_CHANNEL_STATUS_STRIDE = 0x20,
	TEGRA_AHUB_CHANNEL_STATUS_COUNT = 4,
	TEGRA_AHUB_CHANNEL_STATUS_TX_FREE_SHIFT = 24,
	TEGRA_AHUB_CHANNEL_STATUS_TX_FREE_MASK_US = 0xff,
	TEGRA_AHUB_CHANNEL_STATUS_TX_FREE_MASK =
			TEGRA_AHUB_CHANNEL_STATUS_TX_FREE_MASK_US <<
			TEGRA_AHUB_CHANNEL_STATUS_TX_FREE_SHIFT,
	TEGRA_AHUB_CHANNEL_STATUS_RX_FREE_SHIFT = 16,
	TEGRA_AHUB_CHANNEL_STATUS_RX_FREE_MASK_US = 0xff,
	TEGRA_AHUB_CHANNEL_STATUS_RX_FREE_MASK =
			TEGRA_AHUB_CHANNEL_STATUS_RX_FREE_MASK_US <<
			TEGRA_AHUB_CHANNEL_STATUS_RX_FREE_SHIFT,
	TEGRA_AHUB_CHANNEL_STATUS_TX_TRIG = 1 << 1,
	TEGRA_AHUB_CHANNEL_STATUS_RX_TRIG = 1 << 0,

	TEGRA_AHUB_CHANNEL_TXFIFO = 0xc,
	TEGRA_AHUB_CHANNEL_TXFIFO_STRIDE = 0x20,
	TEGRA_AHUB_CHANNEL_TXFIFO_COUNT = 4,

	TEGRA_AHUB_CHANNEL_RXFIFO = 0x10,
	TEGRA_AHUB_CHANNEL_RXFIFO_STRIDE = 0x20,
	TEGRA_AHUB_CHANNEL_RXFIFO_COUNT = 4,

	TEGRA_AHUB_CIF_TX_CTRL = 0x14,
	TEGRA_AHUB_CIF_TX_CTRL_STRIDE = 0x20,
	TEGRA_AHUB_CIF_TX_CTRL_COUNT = 4,
	/* Uses field from TEGRA_AUDIOCIF_CTRL_* */

	TEGRA_AHUB_CIF_RX_CTRL = 0x18,
	TEGRA_AHUB_CIF_RX_CTRL_STRIDE = 0x20,
	TEGRA_AHUB_CIF_RX_CTRL_COUNT = 4,
	/* Uses field from TEGRA_AUDIOCIF_CTRL_* */

	TEGRA_AHUB_CONFIG_LINK_CTRL = 0x80,
	TEGRA_AHUB_CONFIG_LINK_CTRL_MASTER_FIFO_FULL_CNT_SHIFT = 28,
	TEGRA_AHUB_CONFIG_LINK_CTRL_MASTER_FIFO_FULL_CNT_MASK_US = 0xf,
	TEGRA_AHUB_CONFIG_LINK_CTRL_MASTER_FIFO_FULL_CNT_MASK =
		TEGRA_AHUB_CONFIG_LINK_CTRL_MASTER_FIFO_FULL_CNT_MASK_US <<
		TEGRA_AHUB_CONFIG_LINK_CTRL_MASTER_FIFO_FULL_CNT_SHIFT,
	TEGRA_AHUB_CONFIG_LINK_CTRL_TIMEOUT_CNT_SHIFT = 16,
	TEGRA_AHUB_CONFIG_LINK_CTRL_TIMEOUT_CNT_MASK_US = 0xfff,
	TEGRA_AHUB_CONFIG_LINK_CTRL_TIMEOUT_CNT_MASK =
		TEGRA_AHUB_CONFIG_LINK_CTRL_TIMEOUT_CNT_MASK_US <<
		TEGRA_AHUB_CONFIG_LINK_CTRL_TIMEOUT_CNT_SHIFT,
	TEGRA_AHUB_CONFIG_LINK_CTRL_IDLE_CNT_SHIFT = 4,
	TEGRA_AHUB_CONFIG_LINK_CTRL_IDLE_CNT_MASK_US = 0xfff,
	TEGRA_AHUB_CONFIG_LINK_CTRL_IDLE_CNT_MASK =
		TEGRA_AHUB_CONFIG_LINK_CTRL_IDLE_CNT_MASK_US <<
		TEGRA_AHUB_CONFIG_LINK_CTRL_IDLE_CNT_SHIFT,
	TEGRA_AHUB_CONFIG_LINK_CTRL_CG_EN = 1 << 2,
	TEGRA_AHUB_CONFIG_LINK_CTRL_CLEAR_TIMEOUT_CNTR = 1 << 1,
	TEGRA_AHUB_CONFIG_LINK_CTRL_SOFT_RESET = 1 << 0,

	TEGRA_AHUB_MISC_CTRL = 0x84,
	TEGRA_AHUB_MISC_CTRL_AUDIO_ACTIVE = 1 << 31,
	TEGRA_AHUB_MISC_CTRL_AUDIO_CG_EN = 1 << 8,
	TEGRA_AHUB_MISC_CTRL_AUDIO_OBS_SEL_SHIFT = 0,
	TEGRA_AHUB_MISC_CTRL_AUDIO_OBS_SEL_MASK =
			0x1f << TEGRA_AHUB_MISC_CTRL_AUDIO_OBS_SEL_SHIFT,

	TEGRA_AHUB_APBDMA_LIVE_STATUS = 0x88,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH3_RX_CIF_FIFO_FULL = 1 << 31,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH3_TX_CIF_FIFO_FULL = 1 << 30,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH2_RX_CIF_FIFO_FULL = 1 << 29,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH2_TX_CIF_FIFO_FULL = 1 << 28,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH1_RX_CIF_FIFO_FULL = 1 << 27,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH1_TX_CIF_FIFO_FULL = 1 << 26,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH0_RX_CIF_FIFO_FULL = 1 << 25,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH0_TX_CIF_FIFO_FULL = 1 << 24,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH3_RX_CIF_FIFO_EMPTY = 1 << 23,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH3_TX_CIF_FIFO_EMPTY = 1 << 22,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH2_RX_CIF_FIFO_EMPTY = 1 << 21,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH2_TX_CIF_FIFO_EMPTY = 1 << 20,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH1_RX_CIF_FIFO_EMPTY = 1 << 19,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH1_TX_CIF_FIFO_EMPTY = 1 << 18,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH0_RX_CIF_FIFO_EMPTY = 1 << 17,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH0_TX_CIF_FIFO_EMPTY = 1 << 16,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH3_RX_DMA_FIFO_FULL = 1 << 15,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH3_TX_DMA_FIFO_FULL = 1 << 14,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH2_RX_DMA_FIFO_FULL = 1 << 13,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH2_TX_DMA_FIFO_FULL = 1 << 12,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH1_RX_DMA_FIFO_FULL = 1 << 11,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH1_TX_DMA_FIFO_FULL = 1 << 10,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH0_RX_DMA_FIFO_FULL = 1 << 9,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH0_TX_DMA_FIFO_FULL = 1 << 8,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH3_RX_DMA_FIFO_EMPTY = 1 << 7,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH3_TX_DMA_FIFO_EMPTY = 1 << 6,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH2_RX_DMA_FIFO_EMPTY = 1 << 5,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH2_TX_DMA_FIFO_EMPTY = 1 << 4,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH1_RX_DMA_FIFO_EMPTY = 1 << 3,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH1_TX_DMA_FIFO_EMPTY = 1 << 2,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH0_RX_DMA_FIFO_EMPTY = 1 << 1,
	TEGRA_AHUB_APBDMA_LIVE_STATUS_CH0_TX_DMA_FIFO_EMPTY = 1 << 0,

	TEGRA_AHUB_I2S_LIVE_STATUS = 0x8c,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S4_RX_FIFO_FULL = 1 << 29,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S4_TX_FIFO_FULL = 1 << 28,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S3_RX_FIFO_FULL = 1 << 27,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S3_TX_FIFO_FULL = 1 << 26,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S2_RX_FIFO_FULL = 1 << 25,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S2_TX_FIFO_FULL = 1 << 24,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S1_RX_FIFO_FULL = 1 << 23,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S1_TX_FIFO_FULL = 1 << 22,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S0_RX_FIFO_FULL = 1 << 21,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S0_TX_FIFO_FULL = 1 << 20,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S4_RX_FIFO_ENABLED = 1 << 19,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S4_TX_FIFO_ENABLED = 1 << 18,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S3_RX_FIFO_ENABLED = 1 << 17,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S3_TX_FIFO_ENABLED = 1 << 16,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S2_RX_FIFO_ENABLED = 1 << 15,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S2_TX_FIFO_ENABLED = 1 << 14,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S1_RX_FIFO_ENABLED = 1 << 13,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S1_TX_FIFO_ENABLED = 1 << 12,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S0_RX_FIFO_ENABLED = 1 << 11,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S0_TX_FIFO_ENABLED = 1 << 10,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S4_RX_FIFO_EMPTY = 1 << 9,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S4_TX_FIFO_EMPTY = 1 << 8,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S3_RX_FIFO_EMPTY = 1 << 7,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S3_TX_FIFO_EMPTY = 1 << 6,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S2_RX_FIFO_EMPTY = 1 << 5,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S2_TX_FIFO_EMPTY = 1 << 4,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S1_RX_FIFO_EMPTY = 1 << 3,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S1_TX_FIFO_EMPTY = 1 << 2,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S0_RX_FIFO_EMPTY = 1 << 1,
	TEGRA_AHUB_I2S_LIVE_STATUS_I2S0_TX_FIFO_EMPTY = 1 << 0,

	TEGRA_AHUB_DAM_LIVE_STATUS = 0x90,
	TEGRA_AHUB_DAM_LIVE_STATUS_STRIDE = 0x8,
	TEGRA_AHUB_DAM_LIVE_STATUS_COUNT = 3,
	TEGRA_AHUB_DAM_LIVE_STATUS_TX_ENABLED = 1 << 26,
	TEGRA_AHUB_DAM_LIVE_STATUS_RX1_ENABLED = 1 << 25,
	TEGRA_AHUB_DAM_LIVE_STATUS_RX0_ENABLED = 1 << 24,
	TEGRA_AHUB_DAM_LIVE_STATUS_TXFIFO_FULL = 1 << 15,
	TEGRA_AHUB_DAM_LIVE_STATUS_RX1FIFO_FULL = 1 << 9,
	TEGRA_AHUB_DAM_LIVE_STATUS_RX0FIFO_FULL = 1 << 8,
	TEGRA_AHUB_DAM_LIVE_STATUS_TXFIFO_EMPTY = 1 << 7,
	TEGRA_AHUB_DAM_LIVE_STATUS_RX1FIFO_EMPTY = 1 << 1,
	TEGRA_AHUB_DAM_LIVE_STATUS_RX0FIFO_EMPTY = 1 << 0,

	TEGRA_AHUB_SPDIF_LIVE_STATUS = 0xa8,
	TEGRA_AHUB_SPDIF_LIVE_STATUS_USER_TX_ENABLED = 1 << 11,
	TEGRA_AHUB_SPDIF_LIVE_STATUS_USER_RX_ENABLED = 1 << 10,
	TEGRA_AHUB_SPDIF_LIVE_STATUS_DATA_TX_ENABLED = 1 << 9,
	TEGRA_AHUB_SPDIF_LIVE_STATUS_DATA_RX_ENABLED = 1 << 8,
	TEGRA_AHUB_SPDIF_LIVE_STATUS_USER_TXFIFO_FULL = 1 << 7,
	TEGRA_AHUB_SPDIF_LIVE_STATUS_USER_RXFIFO_FULL = 1 << 6,
	TEGRA_AHUB_SPDIF_LIVE_STATUS_DATA_TXFIFO_FULL = 1 << 5,
	TEGRA_AHUB_SPDIF_LIVE_STATUS_DATA_RXFIFO_FULL = 1 << 4,
	TEGRA_AHUB_SPDIF_LIVE_STATUS_USER_TXFIFO_EMPTY = 1 << 3,
	TEGRA_AHUB_SPDIF_LIVE_STATUS_USER_RXFIFO_EMPTY = 1 << 2,
	TEGRA_AHUB_SPDIF_LIVE_STATUS_DATA_TXFIFO_EMPTY = 1 << 1,
	TEGRA_AHUB_SPDIF_LIVE_STATUS_DATA_RXFIFO_EMPTY = 1 << 0,

	TEGRA_AHUB_I2S_INT_MASK = 0xb0,
	TEGRA_AHUB_DAM_INT_MASK = 0xb4,
	TEGRA_AHUB_SPDIF_INT_MASK = 0xbc,
	TEGRA_AHUB_APBIF_INT_MASK = 0xc0,
	TEGRA_AHUB_I2S_INT_STATUS = 0xc8,
	TEGRA_AHUB_DAM_INT_STATUS = 0xcc,
	TEGRA_AHUB_SPDIF_INT_STATUS = 0xd4,
	TEGRA_AHUB_APBIF_INT_STATUS = 0xd8,
	TEGRA_AHUB_I2S_INT_SOURCE = 0xe0,
	TEGRA_AHUB_DAM_INT_SOURCE = 0xe4,
	TEGRA_AHUB_SPDIF_INT_SOURCE = 0xec,
	TEGRA_AHUB_APBIF_INT_SOURCE = 0xf0,
	TEGRA_AHUB_I2S_INT_SET = 0xf8,
	TEGRA_AHUB_DAM_INT_SET = 0xfc,
	TEGRA_AHUB_SPDIF_INT_SET = 0x100,
	TEGRA_AHUB_APBIF_INT_SET = 0x104,

	TEGRA_AHUB_AUDIO_RX = 0x0,
	TEGRA_AHUB_AUDIO_RX_STRIDE = 0x4,
	TEGRA_AHUB_AUDIO_RX_COUNT = 17,
};

#endif	/* _TEGRA_I2C_H_ */
