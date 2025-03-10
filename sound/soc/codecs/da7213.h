/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * da7213.h - DA7213 ASoC Codec Driver
 *
 * Copyright (c) 2013 Dialog Semiconductor
 *
 * Author: Adam Thomson <Adam.Thomson.Opensource@diasemi.com>
 * Author: David Rau <David.Rau.opensource@dm.renesas.com>
 */

#ifndef _DA7213_H
#define _DA7213_H

#include <linux/clk.h>
#include <linux/regmap.h>
#include <linux/regulator/consumer.h>
#include <sound/da7213.h>

/*
 * Registers
 */

/* Status Registers */
#define DA7213_STATUS1			0x02
#define DA7213_PLL_STATUS		0x03
#define DA7213_AUX_L_GAIN_STATUS	0x04
#define DA7213_AUX_R_GAIN_STATUS	0x05
#define DA7213_MIC_1_GAIN_STATUS	0x06
#define DA7213_MIC_2_GAIN_STATUS	0x07
#define DA7213_MIXIN_L_GAIN_STATUS	0x08
#define DA7213_MIXIN_R_GAIN_STATUS	0x09
#define DA7213_ADC_L_GAIN_STATUS	0x0A
#define DA7213_ADC_R_GAIN_STATUS	0x0B
#define DA7213_DAC_L_GAIN_STATUS	0x0C
#define DA7213_DAC_R_GAIN_STATUS	0x0D
#define DA7213_HP_L_GAIN_STATUS		0x0E
#define DA7213_HP_R_GAIN_STATUS		0x0F
#define DA7213_LINE_GAIN_STATUS		0x10

/* System Initialisation Registers */
#define DA7213_DIG_ROUTING_DAI		0x21
#define DA7213_SR			0x22
#define DA7213_REFERENCES		0x23
#define DA7213_PLL_FRAC_TOP		0x24
#define DA7213_PLL_FRAC_BOT		0x25
#define DA7213_PLL_INTEGER		0x26
#define DA7213_PLL_CTRL			0x27
#define DA7213_DAI_CLK_MODE		0x28
#define DA7213_DAI_CTRL			0x29
#define DA7213_DIG_ROUTING_DAC		0x2A
#define DA7213_ALC_CTRL1		0x2B

/* Input - Gain, Select and Filter Registers */
#define DA7213_AUX_L_GAIN		0x30
#define DA7213_AUX_R_GAIN		0x31
#define DA7213_MIXIN_L_SELECT		0x32
#define DA7213_MIXIN_R_SELECT		0x33
#define DA7213_MIXIN_L_GAIN		0x34
#define DA7213_MIXIN_R_GAIN		0x35
#define DA7213_ADC_L_GAIN		0x36
#define DA7213_ADC_R_GAIN		0x37
#define DA7213_ADC_FILTERS1		0x38
#define DA7213_MIC_1_GAIN		0x39
#define DA7213_MIC_2_GAIN		0x3A

/* Output - Gain, Select and Filter Registers */
#define DA7213_DAC_FILTERS5		0x40
#define DA7213_DAC_FILTERS2		0x41
#define DA7213_DAC_FILTERS3		0x42
#define DA7213_DAC_FILTERS4		0x43
#define DA7213_DAC_FILTERS1		0x44
#define DA7213_DAC_L_GAIN		0x45
#define DA7213_DAC_R_GAIN		0x46
#define DA7213_CP_CTRL			0x47
#define DA7213_HP_L_GAIN		0x48
#define DA7213_HP_R_GAIN		0x49
#define DA7213_LINE_GAIN		0x4A
#define DA7213_MIXOUT_L_SELECT		0x4B
#define DA7213_MIXOUT_R_SELECT		0x4C

/* System Controller Registers */
#define DA7213_SYSTEM_MODES_INPUT	0x50
#define DA7213_SYSTEM_MODES_OUTPUT	0x51

/* Control Registers */
#define DA7213_AUX_L_CTRL		0x60
#define DA7213_AUX_R_CTRL		0x61
#define DA7213_MICBIAS_CTRL		0x62
#define DA7213_MIC_1_CTRL		0x63
#define DA7213_MIC_2_CTRL		0x64
#define DA7213_MIXIN_L_CTRL		0x65
#define DA7213_MIXIN_R_CTRL		0x66
#define DA7213_ADC_L_CTRL		0x67
#define DA7213_ADC_R_CTRL		0x68
#define DA7213_DAC_L_CTRL		0x69
#define DA7213_DAC_R_CTRL		0x6A
#define DA7213_HP_L_CTRL		0x6B
#define DA7213_HP_R_CTRL		0x6C
#define DA7213_LINE_CTRL		0x6D
#define DA7213_MIXOUT_L_CTRL		0x6E
#define DA7213_MIXOUT_R_CTRL		0x6F

/* Configuration Registers */
#define DA7213_LDO_CTRL			0x90
#define DA7213_IO_CTRL			0x91
#define DA7213_GAIN_RAMP_CTRL		0x92
#define DA7213_MIC_CONFIG		0x93
#define DA7213_PC_COUNT			0x94
#define DA7213_CP_VOL_THRESHOLD1	0x95
#define DA7213_CP_DELAY			0x96
#define DA7213_CP_DETECTOR		0x97
#define DA7213_DAI_OFFSET		0x98
#define DA7213_DIG_CTRL			0x99
#define DA7213_ALC_CTRL2		0x9A
#define DA7213_ALC_CTRL3		0x9B
#define DA7213_ALC_NOISE		0x9C
#define DA7213_ALC_TARGET_MIN		0x9D
#define DA7213_ALC_TARGET_MAX		0x9E
#define DA7213_ALC_GAIN_LIMITS		0x9F
#define DA7213_ALC_ANA_GAIN_LIMITS	0xA0
#define DA7213_ALC_ANTICLIP_CTRL	0xA1
#define DA7213_ALC_ANTICLIP_LEVEL	0xA2

#define DA7213_ALC_OFFSET_AUTO_M_L	0xA3
#define DA7213_ALC_OFFSET_AUTO_U_L	0xA4
#define DA7213_ALC_OFFSET_MAN_M_L	0xA6
#define DA7213_ALC_OFFSET_MAN_U_L	0xA7
#define DA7213_ALC_OFFSET_AUTO_M_R	0xA8
#define DA7213_ALC_OFFSET_AUTO_U_R	0xA9
#define DA7213_ALC_OFFSET_MAN_M_R	0xAB
#define DA7213_ALC_OFFSET_MAN_U_R	0xAC
#define DA7213_ALC_CIC_OP_LVL_CTRL	0xAD
#define DA7213_ALC_CIC_OP_LVL_DATA	0xAE
#define DA7213_DAC_NG_SETUP_TIME	0xAF
#define DA7213_DAC_NG_OFF_THRESHOLD	0xB0
#define DA7213_DAC_NG_ON_THRESHOLD	0xB1
#define DA7213_DAC_NG_CTRL		0xB2

#define DA7213_TONE_GEN_CFG1		0xB4
#define DA7213_TONE_GEN_CFG2		0xB5
#define DA7213_TONE_GEN_CYCLES		0xB6
#define DA7213_TONE_GEN_FREQ1_L		0xB7
#define DA7213_TONE_GEN_FREQ1_U		0xB8
#define DA7213_TONE_GEN_FREQ2_L		0xB9
#define DA7213_TONE_GEN_FREQ2_U		0xBA
#define DA7213_TONE_GEN_ON_PER		0xBB
#define DA7213_TONE_GEN_OFF_PER		0xBC

/*
 * Bit fields
 */

#define DA7213_SWITCH_EN_MAX		0x1

/* DA7213_PLL_STATUS = 0x03 */
#define DA7213_PLL_SRM_LOCK					(0x1 << 1)

/* DA7213_SR = 0x22 */
#define DA7213_SR_8000						(0x1 << 0)
#define DA7213_SR_11025						(0x2 << 0)
#define DA7213_SR_12000						(0x3 << 0)
#define DA7213_SR_16000						(0x5 << 0)
#define DA7213_SR_22050						(0x6 << 0)
#define DA7213_SR_24000						(0x7 << 0)
#define DA7213_SR_32000						(0x9 << 0)
#define DA7213_SR_44100						(0xA << 0)
#define DA7213_SR_48000						(0xB << 0)
#define DA7213_SR_88200						(0xE << 0)
#define DA7213_SR_96000						(0xF << 0)

/* DA7213_REFERENCES = 0x23 */
#define DA7213_BIAS_EN						(0x1 << 3)
#define DA7213_VMID_EN						(0x1 << 7)

/* DA7213_PLL_CTRL = 0x27 */
#define DA7213_PLL_INDIV_5_TO_9_MHZ				(0x0 << 2)
#define DA7213_PLL_INDIV_9_TO_18_MHZ				(0x1 << 2)
#define DA7213_PLL_INDIV_18_TO_36_MHZ				(0x2 << 2)
#define DA7213_PLL_INDIV_36_TO_54_MHZ				(0x3 << 2)
#define DA7213_PLL_INDIV_MASK					(0x3 << 2)
#define DA7213_PLL_MCLK_SQR_EN					(0x1 << 4)
#define DA7213_PLL_32K_MODE					(0x1 << 5)
#define DA7213_PLL_SRM_EN					(0x1 << 6)
#define DA7213_PLL_EN						(0x1 << 7)
#define DA7213_PLL_MODE_MASK					(0x7 << 5)

/* DA7213_DAI_CLK_MODE = 0x28 */
#define DA7213_DAI_BCLKS_PER_WCLK_32				(0x0 << 0)
#define DA7213_DAI_BCLKS_PER_WCLK_64				(0x1 << 0)
#define DA7213_DAI_BCLKS_PER_WCLK_128				(0x2 << 0)
#define DA7213_DAI_BCLKS_PER_WCLK_256				(0x3 << 0)
#define DA7213_DAI_BCLKS_PER_WCLK_MASK				(0x3 << 0)
#define DA7213_DAI_CLK_POL_INV					(0x1 << 2)
#define DA7213_DAI_CLK_POL_MASK					(0x1 << 2)
#define DA7213_DAI_WCLK_POL_INV					(0x1 << 3)
#define DA7213_DAI_WCLK_POL_MASK				(0x1 << 3)
#define DA7213_DAI_CLK_EN_MASK					(0x1 << 7)

/* DA7213_DAI_CTRL = 0x29 */
#define DA7213_DAI_FORMAT_I2S_MODE				(0x0 << 0)
#define DA7213_DAI_FORMAT_LEFT_J				(0x1 << 0)
#define DA7213_DAI_FORMAT_RIGHT_J				(0x2 << 0)
#define DA7213_DAI_FORMAT_DSP					(0x3 << 0)
#define DA7213_DAI_FORMAT_MASK					(0x3 << 0)
#define DA7213_DAI_WORD_LENGTH_S16_LE				(0x0 << 2)
#define DA7213_DAI_WORD_LENGTH_S20_LE				(0x1 << 2)
#define DA7213_DAI_WORD_LENGTH_S24_LE				(0x2 << 2)
#define DA7213_DAI_WORD_LENGTH_S32_LE				(0x3 << 2)
#define DA7213_DAI_WORD_LENGTH_MASK				(0x3 << 2)
#define DA7213_DAI_MONO_MODE_EN					(0x1 << 4)
#define DA7213_DAI_MONO_MODE_MASK				(0x1 << 4)
#define DA7213_DAI_EN_SHIFT					7

/* DA7213_DIG_ROUTING_DAI = 0x21 */
#define DA7213_DAI_L_SRC_SHIFT					0
#define DA7213_DAI_R_SRC_SHIFT					4
#define DA7213_DAI_SRC_MAX					4

/* DA7213_DIG_ROUTING_DAC = 0x2A */
#define DA7213_DAC_L_SRC_SHIFT					0
#define DA7213_DAC_L_MONO_SHIFT					3
#define DA7213_DAC_R_SRC_SHIFT					4
#define DA7213_DAC_R_MONO_SHIFT					7
#define DA7213_DAC_SRC_MAX					4
#define DA7213_DAC_MONO_MAX					0x1

/* DA7213_ALC_CTRL1 = 0x2B */
#define DA7213_ALC_OFFSET_EN_SHIFT				0
#define DA7213_ALC_OFFSET_EN_MAX				0x1
#define DA7213_ALC_OFFSET_EN					(0x1 << 0)
#define DA7213_ALC_SYNC_MODE					(0x1 << 1)
#define DA7213_ALC_CALIB_MODE_MAN				(0x1 << 2)
#define DA7213_ALC_L_EN_SHIFT					3
#define DA7213_ALC_AUTO_CALIB_EN				(0x1 << 4)
#define DA7213_ALC_CALIB_OVERFLOW				(0x1 << 5)
#define DA7213_ALC_R_EN_SHIFT					7
#define DA7213_ALC_EN_MAX					0x1

/* DA7213_AUX_L/R_GAIN = 0x30/0x31 */
#define DA7213_AUX_AMP_GAIN_SHIFT				0
#define DA7213_AUX_AMP_GAIN_MAX					0x3F

/* DA7213_MIXIN_L/R_SELECT = 0x32/0x33 */
#define DA7213_DMIC_EN_SHIFT					7
#define DA7213_DMIC_EN_MAX					0x1

/* DA7213_MIXIN_L_SELECT = 0x32 */
#define DA7213_MIXIN_L_MIX_SELECT_AUX_L_SHIFT			0
#define DA7213_MIXIN_L_MIX_SELECT_MIC_1_SHIFT			1
#define DA7213_MIXIN_L_MIX_SELECT_MIC_1				(0x1 << 1)
#define DA7213_MIXIN_L_MIX_SELECT_MIC_2_SHIFT			2
#define DA7213_MIXIN_L_MIX_SELECT_MIC_2				(0x1 << 2)
#define DA7213_MIXIN_L_MIX_SELECT_MIXIN_R_SHIFT			3
#define DA7213_MIXIN_L_MIX_SELECT_MAX				0x1

/* DA7213_MIXIN_R_SELECT =  0x33 */
#define DA7213_MIXIN_R_MIX_SELECT_AUX_R_SHIFT			0
#define DA7213_MIXIN_R_MIX_SELECT_MIC_2_SHIFT			1
#define DA7213_MIXIN_R_MIX_SELECT_MIC_2				(0x1 << 1)
#define DA7213_MIXIN_R_MIX_SELECT_MIC_1_SHIFT			2
#define DA7213_MIXIN_R_MIX_SELECT_MIC_1				(0x1 << 2)
#define DA7213_MIXIN_R_MIX_SELECT_MIXIN_L_SHIFT			3
#define DA7213_MIXIN_R_MIX_SELECT_MAX				0x1
#define DA7213_MIC_BIAS_OUTPUT_SELECT_2				(0x1 << 6)

/* DA7213_MIXIN_L/R_GAIN = 0x34/0x35 */
#define DA7213_MIXIN_AMP_GAIN_SHIFT				0
#define DA7213_MIXIN_AMP_GAIN_MAX				0xF

/* DA7213_ADC_L/R_GAIN = 0x36/0x37 */
#define DA7213_ADC_AMP_GAIN_SHIFT				0
#define DA7213_ADC_AMP_GAIN_MAX					0x7F

/* DA7213_ADC/DAC_FILTERS1 = 0x38/0x44 */
#define DA7213_VOICE_HPF_CORNER_SHIFT				0
#define DA7213_VOICE_HPF_CORNER_MAX				8
#define DA7213_VOICE_EN_SHIFT					3
#define DA7213_VOICE_EN_MAX					0x1
#define DA7213_AUDIO_HPF_CORNER_SHIFT				4
#define DA7213_AUDIO_HPF_CORNER_MAX				4
#define DA7213_HPF_EN_SHIFT					7
#define DA7213_HPF_EN_MAX					0x1

/* DA7213_MIC_1/2_GAIN = 0x39/0x3A */
#define DA7213_MIC_AMP_GAIN_SHIFT				0
#define DA7213_MIC_AMP_GAIN_MAX					0x7

/* DA7213_DAC_FILTERS5 = 0x40 */
#define DA7213_DAC_SOFTMUTE_EN_SHIFT				7
#define DA7213_DAC_SOFTMUTE_EN_MAX				0x1
#define DA7213_DAC_SOFTMUTE_RATE_SHIFT				4
#define DA7213_DAC_SOFTMUTE_RATE_MAX				7

/* DA7213_DAC_FILTERS2/3/4 = 0x41/0x42/0x43 */
#define DA7213_DAC_EQ_BAND_MAX					0xF

/* DA7213_DAC_FILTERS2 = 0x41 */
#define DA7213_DAC_EQ_BAND1_SHIFT				0
#define DA7213_DAC_EQ_BAND2_SHIFT				4

/* DA7213_DAC_FILTERS2 = 0x42 */
#define DA7213_DAC_EQ_BAND3_SHIFT				0
#define DA7213_DAC_EQ_BAND4_SHIFT				4

/* DA7213_DAC_FILTERS4 = 0x43 */
#define DA7213_DAC_EQ_BAND5_SHIFT				0
#define DA7213_DAC_EQ_EN_SHIFT					7
#define DA7213_DAC_EQ_EN_MAX					0x1

/* DA7213_DAC_L/R_GAIN = 0x45/0x46 */
#define DA7213_DAC_AMP_GAIN_SHIFT				0
#define DA7213_DAC_AMP_GAIN_MAX					0x7F

/* DA7213_HP_L/R_GAIN = 0x45/0x46 */
#define DA7213_HP_AMP_GAIN_SHIFT				0
#define DA7213_HP_AMP_GAIN_MAX					0x3F

/* DA7213_CP_CTRL = 0x47 */
#define DA7213_CP_EN_SHIFT					7

/* DA7213_LINE_GAIN = 0x4A */
#define DA7213_LINE_AMP_GAIN_SHIFT				0
#define DA7213_LINE_AMP_GAIN_MAX				0x3F

/* DA7213_MIXOUT_L_SELECT = 0x4B */
#define DA7213_MIXOUT_L_MIX_SELECT_AUX_L_SHIFT			0
#define DA7213_MIXOUT_L_MIX_SELECT_MIXIN_L_SHIFT		1
#define DA7213_MIXOUT_L_MIX_SELECT_MIXIN_R_SHIFT		2
#define DA7213_MIXOUT_L_MIX_SELECT_DAC_L_SHIFT			3
#define DA7213_MIXOUT_L_MIX_SELECT_AUX_L_INVERTED_SHIFT		4
#define DA7213_MIXOUT_L_MIX_SELECT_MIXIN_L_INVERTED_SHIFT	5
#define DA7213_MIXOUT_L_MIX_SELECT_MIXIN_R_INVERTED_SHIFT	6
#define DA7213_MIXOUT_L_MIX_SELECT_MAX				0x1

/* DA7213_MIXOUT_R_SELECT = 0x4C */
#define DA7213_MIXOUT_R_MIX_SELECT_AUX_R_SHIFT			0
#define DA7213_MIXOUT_R_MIX_SELECT_MIXIN_R_SHIFT		1
#define DA7213_MIXOUT_R_MIX_SELECT_MIXIN_L_SHIFT		2
#define DA7213_MIXOUT_R_MIX_SELECT_DAC_R_SHIFT			3
#define DA7213_MIXOUT_R_MIX_SELECT_AUX_R_INVERTED_SHIFT		4
#define DA7213_MIXOUT_R_MIX_SELECT_MIXIN_R_INVERTED_SHIFT	5
#define DA7213_MIXOUT_R_MIX_SELECT_MIXIN_L_INVERTED_SHIFT	6
#define DA7213_MIXOUT_R_MIX_SELECT_MAX				0x1

/*
 * DA7213_AUX_L/R_CTRL = 0x60/0x61,
 * DA7213_MIC_1/2_CTRL = 0x63/0x64,
 * DA7213_MIXIN_L/R_CTRL = 0x65/0x66,
 * DA7213_ADC_L/R_CTRL = 0x65/0x66,
 * DA7213_DAC_L/R_CTRL = 0x69/0x6A,
 * DA7213_HP_L/R_CTRL = 0x6B/0x6C,
 * DA7213_LINE_CTRL = 0x6D
 */
#define DA7213_MUTE_EN_SHIFT					6
#define DA7213_MUTE_EN_MAX					0x1
#define DA7213_MUTE_EN						(0x1 << 6)

/*
 * DA7213_AUX_L/R_CTRL = 0x60/0x61,
 * DA7213_MIXIN_L/R_CTRL = 0x65/0x66,
 * DA7213_ADC_L/R_CTRL = 0x65/0x66,
 * DA7213_DAC_L/R_CTRL = 0x69/0x6A,
 * DA7213_HP_L/R_CTRL = 0x6B/0x6C,
 * DA7213_LINE_CTRL = 0x6D
 */
#define DA7213_GAIN_RAMP_EN_SHIFT				5
#define DA7213_GAIN_RAMP_EN_MAX					0x1
#define DA7213_GAIN_RAMP_EN					(0x1 << 5)

/*
 * DA7213_AUX_L/R_CTRL = 0x60/0x61,
 * DA7213_MIXIN_L/R_CTRL = 0x65/0x66,
 * DA7213_HP_L/R_CTRL = 0x6B/0x6C,
 * DA7213_LINE_CTRL = 0x6D
 */
#define DA7213_ZC_EN_SHIFT					4
#define DA7213_ZC_EN_MAX					0x1

/*
 * DA7213_AUX_L/R_CTRL = 0x60/0x61,
 * DA7213_MIC_1/2_CTRL = 0x63/0x64,
 * DA7213_MIXIN_L/R_CTRL = 0x65/0x66,
 * DA7213_HP_L/R_CTRL = 0x6B/0x6C,
 * DA7213_MIXOUT_L/R_CTRL = 0x6E/0x6F,
 * DA7213_LINE_CTRL = 0x6D
 */
#define DA7213_AMP_EN_SHIFT					7

/* DA7213_MIC_1/2_CTRL = 0x63/0x64 */
#define DA7213_MIC_AMP_IN_SEL_SHIFT				2
#define DA7213_MIC_AMP_IN_SEL_MAX				3

/* DA7213_MICBIAS_CTRL = 0x62 */
#define DA7213_MICBIAS1_LEVEL_SHIFT				0
#define DA7213_MICBIAS1_LEVEL_MASK				(0x3 << 0)
#define DA7213_MICBIAS1_EN_SHIFT				3
#define DA7213_MICBIAS2_LEVEL_SHIFT				4
#define DA7213_MICBIAS2_LEVEL_MASK				(0x3 << 4)
#define DA7213_MICBIAS2_EN_SHIFT				7

/* DA7213_MIXIN_L/R_CTRL = 0x65/0x66 */
#define DA7213_MIXIN_MIX_EN					(0x1 << 3)

/* DA7213_ADC_L/R_CTRL = 0x67/0x68 */
#define DA7213_ADC_EN_SHIFT					7
#define DA7213_ADC_EN						(0x1 << 7)

/* DA7213_DAC_L/R_CTRL =  0x69/0x6A*/
#define DA7213_DAC_EN_SHIFT					7

/* DA7213_HP_L/R_CTRL = 0x6B/0x6C */
#define DA7213_HP_AMP_OE					(0x1 << 3)

/* DA7213_LINE_CTRL = 0x6D */
#define DA7213_LINE_AMP_OE					(0x1 << 3)

/* DA7213_MIXOUT_L/R_CTRL = 0x6E/0x6F */
#define DA7213_MIXOUT_MIX_EN					(0x1 << 3)

/* DA7213_GAIN_RAMP_CTRL = 0x92 */
#define DA7213_GAIN_RAMP_RATE_SHIFT				0
#define DA7213_GAIN_RAMP_RATE_MAX				4

/* DA7213_MIC_CONFIG = 0x93 */
#define DA7213_DMIC_DATA_SEL_SHIFT				0
#define DA7213_DMIC_DATA_SEL_MASK				(0x1 << 0)
#define DA7213_DMIC_SAMPLEPHASE_SHIFT				1
#define DA7213_DMIC_SAMPLEPHASE_MASK				(0x1 << 1)
#define DA7213_DMIC_CLK_RATE_SHIFT				2
#define DA7213_DMIC_CLK_RATE_MASK				(0x1 << 2)

/* DA7213_PC_COUNT = 0x94 */
#define DA7213_PC_FREERUN_MASK					(0x1 << 0)

/* DA7213_DIG_CTRL = 0x99 */
#define DA7213_DAC_L_INV_SHIFT					3
#define DA7213_DAC_R_INV_SHIFT					7
#define DA7213_DAC_INV_MAX					0x1

/* DA7213_ALC_CTRL2 = 0x9A */
#define DA7213_ALC_ATTACK_SHIFT					0
#define DA7213_ALC_ATTACK_MAX					13
#define DA7213_ALC_RELEASE_SHIFT				4
#define DA7213_ALC_RELEASE_MAX					11

/* DA7213_ALC_CTRL3 = 0x9B */
#define DA7213_ALC_HOLD_SHIFT					0
#define DA7213_ALC_HOLD_MAX					16
#define DA7213_ALC_INTEG_ATTACK_SHIFT				4
#define DA7213_ALC_INTEG_RELEASE_SHIFT				6
#define DA7213_ALC_INTEG_MAX					4

/*
 * DA7213_ALC_NOISE = 0x9C,
 * DA7213_ALC_TARGET_MIN/MAX = 0x9D/0x9E
 */
#define DA7213_ALC_THRESHOLD_SHIFT				0
#define DA7213_ALC_THRESHOLD_MAX				0x3F

/* DA7213_ALC_GAIN_LIMITS = 0x9F */
#define DA7213_ALC_ATTEN_MAX_SHIFT				0
#define DA7213_ALC_GAIN_MAX_SHIFT				4
#define DA7213_ALC_ATTEN_GAIN_MAX_MAX				0xF

/* DA7213_ALC_ANA_GAIN_LIMITS = 0xA0 */
#define DA7213_ALC_ANA_GAIN_MIN_SHIFT				0
#define DA7213_ALC_ANA_GAIN_MAX_SHIFT				4
#define DA7213_ALC_ANA_GAIN_MAX					0x7

/* DA7213_ALC_ANTICLIP_CTRL = 0xA1 */
#define DA7213_ALC_ANTICLIP_EN_SHIFT				7
#define DA7213_ALC_ANTICLIP_EN_MAX				0x1

/* DA7213_ALC_ANTICLIP_LEVEL = 0xA2 */
#define DA7213_ALC_ANTICLIP_LEVEL_SHIFT				0
#define DA7213_ALC_ANTICLIP_LEVEL_MAX				0x7F

/* DA7213_ALC_CIC_OP_LVL_CTRL = 0xAD */
#define DA7213_ALC_DATA_MIDDLE					(0x2 << 0)
#define DA7213_ALC_DATA_TOP					(0x3 << 0)
#define DA7213_ALC_CIC_OP_CHANNEL_LEFT				(0x0 << 7)
#define DA7213_ALC_CIC_OP_CHANNEL_RIGHT				(0x1 << 7)

/* DA7213_DAC_NG_SETUP_TIME = 0xAF */
#define DA7213_DAC_NG_SETUP_TIME_SHIFT				0
#define DA7213_DAC_NG_SETUP_TIME_MAX				4
#define DA7213_DAC_NG_RAMPUP_RATE_SHIFT				2
#define DA7213_DAC_NG_RAMPDN_RATE_SHIFT				3
#define DA7213_DAC_NG_RAMP_RATE_MAX				2

/* DA7213_DAC_NG_OFF/ON_THRESH = 0xB0/0xB1 */
#define DA7213_DAC_NG_THRESHOLD_SHIFT				0
#define DA7213_DAC_NG_THRESHOLD_MAX				0x7

/* DA7213_DAC_NG_CTRL = 0xB2 */
#define DA7213_DAC_NG_EN_SHIFT					7
#define DA7213_DAC_NG_EN_MAX					0x1

/* DA7213_TONE_GEN_CFG1 = 0xB4 */
#define DA7213_DTMF_REG_SHIFT		0
#define DA7213_DTMF_REG_MASK		(0xF << 0)
#define DA7213_DTMF_REG_MAX		16
#define DA7213_DTMF_EN_SHIFT		4
#define DA7213_DTMF_EN_MASK		(0x1 << 4)
#define DA7213_START_STOPN_SHIFT	7
#define DA7213_START_STOPN_MASK		(0x1 << 7)

/* DA7213_TONE_GEN_CFG2 = 0xB5 */
#define DA7213_SWG_SEL_SHIFT		0
#define DA7213_SWG_SEL_MASK		(0x3 << 0)
#define DA7213_SWG_SEL_MAX		4
#define DA7213_SWG_SEL_SRAMP		(0x3 << 0)
#define DA7213_TONE_GEN_GAIN_SHIFT	4
#define DA7213_TONE_GEN_GAIN_MASK	(0xF << 4)
#define DA7213_TONE_GEN_GAIN_MAX	0xF
#define DA7213_TONE_GEN_GAIN_MINUS_9DB	(0x3 << 4)
#define DA7213_TONE_GEN_GAIN_MINUS_15DB	(0x5 << 4)

/* DA7213_TONE_GEN_CYCLES = 0xB6 */
#define DA7213_BEEP_CYCLES_SHIFT	0
#define DA7213_BEEP_CYCLES_MASK		(0x7 << 0)

/* DA7213_TONE_GEN_FREQ1_L = 0xB7 */
#define DA7213_FREQ1_L_SHIFT	0
#define DA7213_FREQ1_L_MASK	(0xFF << 0)
#define DA7213_FREQ_MAX		0xFFFF

/* DA7213_TONE_GEN_FREQ1_U = 0xB8 */
#define DA7213_FREQ1_U_SHIFT	0
#define DA7213_FREQ1_U_MASK	(0xFF << 0)

/* DA7213_TONE_GEN_FREQ2_L = 0xB9 */
#define DA7213_FREQ2_L_SHIFT	0
#define DA7213_FREQ2_L_MASK	(0xFF << 0)

/* DA7213_TONE_GEN_FREQ2_U = 0xBA */
#define DA7213_FREQ2_U_SHIFT	0
#define DA7213_FREQ2_U_MASK	(0xFF << 0)

/* DA7213_TONE_GEN_ON_PER = 0xBB */
#define DA7213_BEEP_ON_PER_SHIFT	0
#define DA7213_BEEP_ON_PER_MASK		(0x3F << 0)
#define DA7213_BEEP_ON_OFF_MAX		0x3F

/* DA7213_TONE_GEN_OFF_PER = 0xBC */
#define DA7213_BEEP_OFF_PER_SHIFT	0
#define DA7213_BEEP_OFF_PER_MASK	(0x3F << 0)

/*
 * General defines
 */

/* Register inversion */
#define DA7213_NO_INVERT		0
#define DA7213_INVERT			1

/* Byte related defines */
#define DA7213_BYTE_SHIFT		8
#define DA7213_BYTE_MASK		0xFF

/* ALC related */
#define DA7213_ALC_OFFSET_15_8		0x00FF00
#define DA7213_ALC_OFFSET_19_16		0x0F0000
#define DA7213_ALC_AVG_ITERATIONS	5

/* PLL related */
#define DA7213_PLL_FREQ_OUT_90316800		90316800
#define DA7213_PLL_FREQ_OUT_98304000		98304000
#define DA7213_PLL_FREQ_OUT_94310400		94310400
#define DA7213_PLL_INDIV_5_TO_9_MHZ_VAL		2
#define DA7213_PLL_INDIV_9_TO_18_MHZ_VAL	4
#define DA7213_PLL_INDIV_18_TO_36_MHZ_VAL	8
#define DA7213_PLL_INDIV_36_TO_54_MHZ_VAL	16
#define DA7213_SRM_CHECK_RETRIES		8

enum da7213_clk_src {
	DA7213_CLKSRC_MCLK = 0,
	DA7213_CLKSRC_MCLK_SQR,
};

enum da7213_sys_clk {
	DA7213_SYSCLK_MCLK = 0,
	DA7213_SYSCLK_PLL,
	DA7213_SYSCLK_PLL_SRM,
	DA7213_SYSCLK_PLL_32KHZ
};

/* Regulators */
enum da7213_supplies {
	DA7213_SUPPLY_VDDA = 0,
	DA7213_SUPPLY_VDDIO,
	DA7213_NUM_SUPPLIES,
};

/* Codec private data */
struct da7213_priv {
	struct regmap *regmap;
	struct mutex ctrl_lock;
	struct regulator_bulk_data supplies[DA7213_NUM_SUPPLIES];
	struct clk *mclk;
	unsigned int mclk_rate;
	unsigned int out_rate;
	unsigned int fin_min_rate;
	int clk_src;
	bool master;
	bool alc_calib_auto;
	bool alc_en;
	bool fixed_clk_auto_pll;
	struct da7213_platform_data *pdata;
	int fmt;
};

#endif /* _DA7213_H */
