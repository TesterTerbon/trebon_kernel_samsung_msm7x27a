/*
 * Device access for Dialog DA9052 PMICs.
 *
 * Copyright(c) 2011 Dialog Semiconductor Ltd.
 *
 * Author: David Dajun Chen <dchen@diasemi.com>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 */

#include <linux/device.h>
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/interrupt.h>
<<<<<<< HEAD
#include <linux/irq.h>
=======
>>>>>>> refs/remotes/origin/master
#include <linux/mfd/core.h>
#include <linux/slab.h>
#include <linux/module.h>

#include <linux/mfd/da9052/da9052.h>
#include <linux/mfd/da9052/pdata.h>
#include <linux/mfd/da9052/reg.h>

<<<<<<< HEAD
#define DA9052_NUM_IRQ_REGS		4
#define DA9052_IRQ_MASK_POS_1		0x01
#define DA9052_IRQ_MASK_POS_2		0x02
#define DA9052_IRQ_MASK_POS_3		0x04
#define DA9052_IRQ_MASK_POS_4		0x08
#define DA9052_IRQ_MASK_POS_5		0x10
#define DA9052_IRQ_MASK_POS_6		0x20
#define DA9052_IRQ_MASK_POS_7		0x40
#define DA9052_IRQ_MASK_POS_8		0x80

=======
>>>>>>> refs/remotes/origin/master
static bool da9052_reg_readable(struct device *dev, unsigned int reg)
{
	switch (reg) {
	case DA9052_PAGE0_CON_REG:
	case DA9052_STATUS_A_REG:
	case DA9052_STATUS_B_REG:
	case DA9052_STATUS_C_REG:
	case DA9052_STATUS_D_REG:
	case DA9052_EVENT_A_REG:
	case DA9052_EVENT_B_REG:
	case DA9052_EVENT_C_REG:
	case DA9052_EVENT_D_REG:
	case DA9052_FAULTLOG_REG:
	case DA9052_IRQ_MASK_A_REG:
	case DA9052_IRQ_MASK_B_REG:
	case DA9052_IRQ_MASK_C_REG:
	case DA9052_IRQ_MASK_D_REG:
	case DA9052_CONTROL_A_REG:
	case DA9052_CONTROL_B_REG:
	case DA9052_CONTROL_C_REG:
	case DA9052_CONTROL_D_REG:
	case DA9052_PDDIS_REG:
	case DA9052_INTERFACE_REG:
	case DA9052_RESET_REG:
	case DA9052_GPIO_0_1_REG:
	case DA9052_GPIO_2_3_REG:
	case DA9052_GPIO_4_5_REG:
	case DA9052_GPIO_6_7_REG:
	case DA9052_GPIO_14_15_REG:
	case DA9052_ID_0_1_REG:
	case DA9052_ID_2_3_REG:
	case DA9052_ID_4_5_REG:
	case DA9052_ID_6_7_REG:
	case DA9052_ID_8_9_REG:
	case DA9052_ID_10_11_REG:
	case DA9052_ID_12_13_REG:
	case DA9052_ID_14_15_REG:
	case DA9052_ID_16_17_REG:
	case DA9052_ID_18_19_REG:
	case DA9052_ID_20_21_REG:
	case DA9052_SEQ_STATUS_REG:
	case DA9052_SEQ_A_REG:
	case DA9052_SEQ_B_REG:
	case DA9052_SEQ_TIMER_REG:
	case DA9052_BUCKA_REG:
	case DA9052_BUCKB_REG:
	case DA9052_BUCKCORE_REG:
	case DA9052_BUCKPRO_REG:
	case DA9052_BUCKMEM_REG:
	case DA9052_BUCKPERI_REG:
	case DA9052_LDO1_REG:
	case DA9052_LDO2_REG:
	case DA9052_LDO3_REG:
	case DA9052_LDO4_REG:
	case DA9052_LDO5_REG:
	case DA9052_LDO6_REG:
	case DA9052_LDO7_REG:
	case DA9052_LDO8_REG:
	case DA9052_LDO9_REG:
	case DA9052_LDO10_REG:
	case DA9052_SUPPLY_REG:
	case DA9052_PULLDOWN_REG:
	case DA9052_CHGBUCK_REG:
	case DA9052_WAITCONT_REG:
	case DA9052_ISET_REG:
	case DA9052_BATCHG_REG:
	case DA9052_CHG_CONT_REG:
	case DA9052_INPUT_CONT_REG:
	case DA9052_CHG_TIME_REG:
	case DA9052_BBAT_CONT_REG:
	case DA9052_BOOST_REG:
	case DA9052_LED_CONT_REG:
	case DA9052_LEDMIN123_REG:
	case DA9052_LED1_CONF_REG:
	case DA9052_LED2_CONF_REG:
	case DA9052_LED3_CONF_REG:
	case DA9052_LED1CONT_REG:
	case DA9052_LED2CONT_REG:
	case DA9052_LED3CONT_REG:
	case DA9052_LED_CONT_4_REG:
	case DA9052_LED_CONT_5_REG:
	case DA9052_ADC_MAN_REG:
	case DA9052_ADC_CONT_REG:
	case DA9052_ADC_RES_L_REG:
	case DA9052_ADC_RES_H_REG:
	case DA9052_VDD_RES_REG:
	case DA9052_VDD_MON_REG:
	case DA9052_ICHG_AV_REG:
	case DA9052_ICHG_THD_REG:
	case DA9052_ICHG_END_REG:
	case DA9052_TBAT_RES_REG:
	case DA9052_TBAT_HIGHP_REG:
	case DA9052_TBAT_HIGHN_REG:
	case DA9052_TBAT_LOW_REG:
	case DA9052_T_OFFSET_REG:
	case DA9052_ADCIN4_RES_REG:
	case DA9052_AUTO4_HIGH_REG:
	case DA9052_AUTO4_LOW_REG:
	case DA9052_ADCIN5_RES_REG:
	case DA9052_AUTO5_HIGH_REG:
	case DA9052_AUTO5_LOW_REG:
	case DA9052_ADCIN6_RES_REG:
	case DA9052_AUTO6_HIGH_REG:
	case DA9052_AUTO6_LOW_REG:
	case DA9052_TJUNC_RES_REG:
	case DA9052_TSI_CONT_A_REG:
	case DA9052_TSI_CONT_B_REG:
	case DA9052_TSI_X_MSB_REG:
	case DA9052_TSI_Y_MSB_REG:
	case DA9052_TSI_LSB_REG:
	case DA9052_TSI_Z_MSB_REG:
	case DA9052_COUNT_S_REG:
	case DA9052_COUNT_MI_REG:
	case DA9052_COUNT_H_REG:
	case DA9052_COUNT_D_REG:
	case DA9052_COUNT_MO_REG:
	case DA9052_COUNT_Y_REG:
	case DA9052_ALARM_MI_REG:
	case DA9052_ALARM_H_REG:
	case DA9052_ALARM_D_REG:
	case DA9052_ALARM_MO_REG:
	case DA9052_ALARM_Y_REG:
	case DA9052_SECOND_A_REG:
	case DA9052_SECOND_B_REG:
	case DA9052_SECOND_C_REG:
	case DA9052_SECOND_D_REG:
	case DA9052_PAGE1_CON_REG:
		return true;
	default:
		return false;
	}
}

static bool da9052_reg_writeable(struct device *dev, unsigned int reg)
{
	switch (reg) {
	case DA9052_PAGE0_CON_REG:
	case DA9052_EVENT_A_REG:
	case DA9052_EVENT_B_REG:
	case DA9052_EVENT_C_REG:
	case DA9052_EVENT_D_REG:
	case DA9052_IRQ_MASK_A_REG:
	case DA9052_IRQ_MASK_B_REG:
	case DA9052_IRQ_MASK_C_REG:
	case DA9052_IRQ_MASK_D_REG:
	case DA9052_CONTROL_A_REG:
	case DA9052_CONTROL_B_REG:
	case DA9052_CONTROL_C_REG:
	case DA9052_CONTROL_D_REG:
	case DA9052_PDDIS_REG:
	case DA9052_RESET_REG:
	case DA9052_GPIO_0_1_REG:
	case DA9052_GPIO_2_3_REG:
	case DA9052_GPIO_4_5_REG:
	case DA9052_GPIO_6_7_REG:
	case DA9052_GPIO_14_15_REG:
	case DA9052_ID_0_1_REG:
	case DA9052_ID_2_3_REG:
	case DA9052_ID_4_5_REG:
	case DA9052_ID_6_7_REG:
	case DA9052_ID_8_9_REG:
	case DA9052_ID_10_11_REG:
	case DA9052_ID_12_13_REG:
	case DA9052_ID_14_15_REG:
	case DA9052_ID_16_17_REG:
	case DA9052_ID_18_19_REG:
	case DA9052_ID_20_21_REG:
	case DA9052_SEQ_STATUS_REG:
	case DA9052_SEQ_A_REG:
	case DA9052_SEQ_B_REG:
	case DA9052_SEQ_TIMER_REG:
	case DA9052_BUCKA_REG:
	case DA9052_BUCKB_REG:
	case DA9052_BUCKCORE_REG:
	case DA9052_BUCKPRO_REG:
	case DA9052_BUCKMEM_REG:
	case DA9052_BUCKPERI_REG:
	case DA9052_LDO1_REG:
	case DA9052_LDO2_REG:
	case DA9052_LDO3_REG:
	case DA9052_LDO4_REG:
	case DA9052_LDO5_REG:
	case DA9052_LDO6_REG:
	case DA9052_LDO7_REG:
	case DA9052_LDO8_REG:
	case DA9052_LDO9_REG:
	case DA9052_LDO10_REG:
	case DA9052_SUPPLY_REG:
	case DA9052_PULLDOWN_REG:
	case DA9052_CHGBUCK_REG:
	case DA9052_WAITCONT_REG:
	case DA9052_ISET_REG:
	case DA9052_BATCHG_REG:
	case DA9052_CHG_CONT_REG:
	case DA9052_INPUT_CONT_REG:
	case DA9052_BBAT_CONT_REG:
	case DA9052_BOOST_REG:
	case DA9052_LED_CONT_REG:
	case DA9052_LEDMIN123_REG:
	case DA9052_LED1_CONF_REG:
	case DA9052_LED2_CONF_REG:
	case DA9052_LED3_CONF_REG:
	case DA9052_LED1CONT_REG:
	case DA9052_LED2CONT_REG:
	case DA9052_LED3CONT_REG:
	case DA9052_LED_CONT_4_REG:
	case DA9052_LED_CONT_5_REG:
	case DA9052_ADC_MAN_REG:
	case DA9052_ADC_CONT_REG:
	case DA9052_ADC_RES_L_REG:
	case DA9052_ADC_RES_H_REG:
	case DA9052_VDD_RES_REG:
	case DA9052_VDD_MON_REG:
	case DA9052_ICHG_THD_REG:
	case DA9052_ICHG_END_REG:
	case DA9052_TBAT_HIGHP_REG:
	case DA9052_TBAT_HIGHN_REG:
	case DA9052_TBAT_LOW_REG:
	case DA9052_T_OFFSET_REG:
	case DA9052_AUTO4_HIGH_REG:
	case DA9052_AUTO4_LOW_REG:
	case DA9052_AUTO5_HIGH_REG:
	case DA9052_AUTO5_LOW_REG:
	case DA9052_AUTO6_HIGH_REG:
	case DA9052_AUTO6_LOW_REG:
	case DA9052_TSI_CONT_A_REG:
	case DA9052_TSI_CONT_B_REG:
	case DA9052_COUNT_S_REG:
	case DA9052_COUNT_MI_REG:
	case DA9052_COUNT_H_REG:
	case DA9052_COUNT_D_REG:
	case DA9052_COUNT_MO_REG:
	case DA9052_COUNT_Y_REG:
	case DA9052_ALARM_MI_REG:
	case DA9052_ALARM_H_REG:
	case DA9052_ALARM_D_REG:
	case DA9052_ALARM_MO_REG:
	case DA9052_ALARM_Y_REG:
	case DA9052_PAGE1_CON_REG:
		return true;
	default:
		return false;
	}
}

static bool da9052_reg_volatile(struct device *dev, unsigned int reg)
{
	switch (reg) {
	case DA9052_STATUS_A_REG:
	case DA9052_STATUS_B_REG:
	case DA9052_STATUS_C_REG:
	case DA9052_STATUS_D_REG:
	case DA9052_EVENT_A_REG:
	case DA9052_EVENT_B_REG:
	case DA9052_EVENT_C_REG:
	case DA9052_EVENT_D_REG:
	case DA9052_FAULTLOG_REG:
	case DA9052_CHG_TIME_REG:
	case DA9052_ADC_RES_L_REG:
	case DA9052_ADC_RES_H_REG:
	case DA9052_VDD_RES_REG:
	case DA9052_ICHG_AV_REG:
	case DA9052_TBAT_RES_REG:
	case DA9052_ADCIN4_RES_REG:
	case DA9052_ADCIN5_RES_REG:
	case DA9052_ADCIN6_RES_REG:
	case DA9052_TJUNC_RES_REG:
	case DA9052_TSI_X_MSB_REG:
	case DA9052_TSI_Y_MSB_REG:
	case DA9052_TSI_LSB_REG:
	case DA9052_TSI_Z_MSB_REG:
	case DA9052_COUNT_S_REG:
	case DA9052_COUNT_MI_REG:
	case DA9052_COUNT_H_REG:
	case DA9052_COUNT_D_REG:
	case DA9052_COUNT_MO_REG:
	case DA9052_COUNT_Y_REG:
	case DA9052_ALARM_MI_REG:
		return true;
	default:
		return false;
	}
}

<<<<<<< HEAD
static struct resource da9052_rtc_resource = {
	.name = "ALM",
	.start = DA9052_IRQ_ALARM,
	.end   = DA9052_IRQ_ALARM,
	.flags = IORESOURCE_IRQ,
};

static struct resource da9052_onkey_resource = {
	.name = "ONKEY",
	.start = DA9052_IRQ_NONKEY,
	.end   = DA9052_IRQ_NONKEY,
	.flags = IORESOURCE_IRQ,
};

static struct resource da9052_bat_resources[] = {
	{
		.name = "BATT TEMP",
		.start = DA9052_IRQ_TBAT,
		.end   = DA9052_IRQ_TBAT,
		.flags = IORESOURCE_IRQ,
	},
	{
		.name = "DCIN DET",
		.start = DA9052_IRQ_DCIN,
		.end   = DA9052_IRQ_DCIN,
		.flags = IORESOURCE_IRQ,
	},
	{
		.name = "DCIN REM",
		.start = DA9052_IRQ_DCINREM,
		.end   = DA9052_IRQ_DCINREM,
		.flags = IORESOURCE_IRQ,
	},
	{
		.name = "VBUS DET",
		.start = DA9052_IRQ_VBUS,
		.end   = DA9052_IRQ_VBUS,
		.flags = IORESOURCE_IRQ,
	},
	{
		.name = "VBUS REM",
		.start = DA9052_IRQ_VBUSREM,
		.end   = DA9052_IRQ_VBUSREM,
		.flags = IORESOURCE_IRQ,
	},
	{
		.name = "CHG END",
		.start = DA9052_IRQ_CHGEND,
		.end   = DA9052_IRQ_CHGEND,
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource da9052_tsi_resources[] = {
	{
		.name = "PENDWN",
		.start = DA9052_IRQ_PENDOWN,
		.end   = DA9052_IRQ_PENDOWN,
		.flags = IORESOURCE_IRQ,
	},
	{
		.name = "TSIRDY",
		.start = DA9052_IRQ_TSIREADY,
		.end   = DA9052_IRQ_TSIREADY,
		.flags = IORESOURCE_IRQ,
	},
};

static struct mfd_cell __devinitdata da9052_subdev_info[] = {
=======
/*
 * TBAT look-up table is computed from the R90 reg (8 bit register)
 * reading as below. The battery temperature is in milliCentigrade
 * TBAT = (1/(t1+1/298) - 273) * 1000 mC
 * where t1 = (1/B)* ln(( ADCval * 2.5)/(R25*ITBAT*255))
 * Default values are R25 = 10e3, B = 3380, ITBAT = 50e-6
 * Example:
 * R25=10E3, B=3380, ITBAT=50e-6, ADCVAL=62d calculates
 * TBAT = 20015 mili degrees Centrigrade
 *
*/
static const int32_t tbat_lookup[255] = {
	183258, 144221, 124334, 111336, 101826, 94397, 88343, 83257,
	78889, 75071, 71688, 68656, 65914, 63414, 61120, 59001,
	570366, 55204, 53490, 51881, 50364, 48931, 47574, 46285,
	45059, 43889, 42772, 41703, 40678, 39694, 38748, 37838,
	36961, 36115, 35297, 34507, 33743, 33002, 32284, 31588,
	30911, 30254, 29615, 28994, 28389, 27799, 27225, 26664,
	26117, 25584, 25062, 24553, 24054, 23567, 23091, 22624,
	22167, 21719, 21281, 20851, 20429, 20015, 19610, 19211,
	18820, 18436, 18058, 17688, 17323, 16965, 16612, 16266,
	15925, 15589, 15259, 14933, 14613, 14298, 13987, 13681,
	13379, 13082, 12788, 12499, 12214, 11933, 11655, 11382,
	11112, 10845, 10582, 10322, 10066, 9812, 9562, 9315,
	9071, 8830, 8591, 8356, 8123, 7893, 7665, 7440,
	7218, 6998, 6780, 6565, 6352, 6141, 5933, 5726,
	5522, 5320, 5120, 4922, 4726, 4532, 4340, 4149,
	3961, 3774, 3589, 3406, 3225, 3045, 2867, 2690,
	2516, 2342, 2170, 2000, 1831, 1664, 1498, 1334,
	1171, 1009, 849, 690, 532, 376, 221, 67,
	-84, -236, -386, -535, -683, -830, -975, -1119,
	-1263, -1405, -1546, -1686, -1825, -1964, -2101, -2237,
	-2372, -2506, -2639, -2771, -2902, -3033, -3162, -3291,
	-3418, -3545, -3671, -3796, -3920, -4044, -4166, -4288,
	-4409, -4529, -4649, -4767, -4885, -5002, -5119, -5235,
	-5349, -5464, -5577, -5690, -5802, -5913, -6024, -6134,
	-6244, -6352, -6461, -6568, -6675, -6781, -6887, -6992,
	-7096, -7200, -7303, -7406, -7508, -7609, -7710, -7810,
	-7910, -8009, -8108, -8206, -8304, -8401, -8497, -8593,
	-8689, -8784, -8878, -8972, -9066, -9159, -9251, -9343,
	-9435, -9526, -9617, -9707, -9796, -9886, -9975, -10063,
	-10151, -10238, -10325, -10412, -10839, -10923, -11007, -11090,
	-11173, -11256, -11338, -11420, -11501, -11583, -11663, -11744,
	-11823, -11903, -11982
};

static const u8 chan_mux[DA9052_ADC_VBBAT + 1] = {
	[DA9052_ADC_VDDOUT]	= DA9052_ADC_MAN_MUXSEL_VDDOUT,
	[DA9052_ADC_ICH]	= DA9052_ADC_MAN_MUXSEL_ICH,
	[DA9052_ADC_TBAT]	= DA9052_ADC_MAN_MUXSEL_TBAT,
	[DA9052_ADC_VBAT]	= DA9052_ADC_MAN_MUXSEL_VBAT,
	[DA9052_ADC_IN4]	= DA9052_ADC_MAN_MUXSEL_AD4,
	[DA9052_ADC_IN5]	= DA9052_ADC_MAN_MUXSEL_AD5,
	[DA9052_ADC_IN6]	= DA9052_ADC_MAN_MUXSEL_AD6,
	[DA9052_ADC_VBBAT]	= DA9052_ADC_MAN_MUXSEL_VBBAT
};

int da9052_adc_manual_read(struct da9052 *da9052, unsigned char channel)
{
	int ret;
	unsigned short calc_data;
	unsigned short data;
	unsigned char mux_sel;

	if (channel > DA9052_ADC_VBBAT)
		return -EINVAL;

	mutex_lock(&da9052->auxadc_lock);

	/* Channel gets activated on enabling the Conversion bit */
	mux_sel = chan_mux[channel] | DA9052_ADC_MAN_MAN_CONV;

	ret = da9052_reg_write(da9052, DA9052_ADC_MAN_REG, mux_sel);
	if (ret < 0)
		goto err;

	/* Wait for an interrupt */
	if (!wait_for_completion_timeout(&da9052->done,
					 msecs_to_jiffies(500))) {
		dev_err(da9052->dev,
			"timeout waiting for ADC conversion interrupt\n");
		ret = -ETIMEDOUT;
		goto err;
	}

	ret = da9052_reg_read(da9052, DA9052_ADC_RES_H_REG);
	if (ret < 0)
		goto err;

	calc_data = (unsigned short)ret;
	data = calc_data << 2;

	ret = da9052_reg_read(da9052, DA9052_ADC_RES_L_REG);
	if (ret < 0)
		goto err;

	calc_data = (unsigned short)(ret & DA9052_ADC_RES_LSB);
	data |= calc_data;

	ret = data;

err:
	mutex_unlock(&da9052->auxadc_lock);
	return ret;
}
EXPORT_SYMBOL_GPL(da9052_adc_manual_read);

int da9052_adc_read_temp(struct da9052 *da9052)
{
	int tbat;

	tbat = da9052_reg_read(da9052, DA9052_TBAT_RES_REG);
	if (tbat <= 0)
		return tbat;

	/* ARRAY_SIZE check is not needed since TBAT is a 8-bit register */
	return tbat_lookup[tbat - 1];
}
EXPORT_SYMBOL_GPL(da9052_adc_read_temp);

static const struct mfd_cell da9052_subdev_info[] = {
>>>>>>> refs/remotes/origin/master
	{
		.name = "da9052-regulator",
		.id = 1,
	},
	{
		.name = "da9052-regulator",
		.id = 2,
	},
	{
		.name = "da9052-regulator",
		.id = 3,
	},
	{
		.name = "da9052-regulator",
		.id = 4,
	},
	{
		.name = "da9052-regulator",
		.id = 5,
	},
	{
		.name = "da9052-regulator",
		.id = 6,
	},
	{
		.name = "da9052-regulator",
		.id = 7,
	},
	{
		.name = "da9052-regulator",
		.id = 8,
	},
	{
		.name = "da9052-regulator",
		.id = 9,
	},
	{
		.name = "da9052-regulator",
		.id = 10,
	},
	{
		.name = "da9052-regulator",
		.id = 11,
	},
	{
		.name = "da9052-regulator",
		.id = 12,
	},
	{
		.name = "da9052-regulator",
		.id = 13,
	},
	{
		.name = "da9052-regulator",
		.id = 14,
	},
	{
		.name = "da9052-onkey",
<<<<<<< HEAD
		.resources = &da9052_onkey_resource,
		.num_resources = 1,
	},
	{
		.name = "da9052-rtc",
		.resources = &da9052_rtc_resource,
		.num_resources = 1,
=======
	},
	{
		.name = "da9052-rtc",
>>>>>>> refs/remotes/origin/master
	},
	{
		.name = "da9052-gpio",
	},
	{
		.name = "da9052-hwmon",
	},
	{
		.name = "da9052-leds",
	},
	{
		.name = "da9052-wled1",
	},
	{
		.name = "da9052-wled2",
	},
	{
		.name = "da9052-wled3",
	},
	{
		.name = "da9052-tsi",
<<<<<<< HEAD
		.resources = da9052_tsi_resources,
		.num_resources = ARRAY_SIZE(da9052_tsi_resources),
	},
	{
		.name = "da9052-bat",
		.resources = da9052_bat_resources,
		.num_resources = ARRAY_SIZE(da9052_bat_resources),
=======
	},
	{
		.name = "da9052-bat",
>>>>>>> refs/remotes/origin/master
	},
	{
		.name = "da9052-watchdog",
	},
};

<<<<<<< HEAD
static struct regmap_irq da9052_irqs[] = {
	[DA9052_IRQ_DCIN] = {
		.reg_offset = 0,
		.mask = DA9052_IRQ_MASK_POS_1,
	},
	[DA9052_IRQ_VBUS] = {
		.reg_offset = 0,
		.mask = DA9052_IRQ_MASK_POS_2,
	},
	[DA9052_IRQ_DCINREM] = {
		.reg_offset = 0,
		.mask = DA9052_IRQ_MASK_POS_3,
	},
	[DA9052_IRQ_VBUSREM] = {
		.reg_offset = 0,
		.mask = DA9052_IRQ_MASK_POS_4,
	},
	[DA9052_IRQ_VDDLOW] = {
		.reg_offset = 0,
		.mask = DA9052_IRQ_MASK_POS_5,
	},
	[DA9052_IRQ_ALARM] = {
		.reg_offset = 0,
		.mask = DA9052_IRQ_MASK_POS_6,
	},
	[DA9052_IRQ_SEQRDY] = {
		.reg_offset = 0,
		.mask = DA9052_IRQ_MASK_POS_7,
	},
	[DA9052_IRQ_COMP1V2] = {
		.reg_offset = 0,
		.mask = DA9052_IRQ_MASK_POS_8,
	},
	[DA9052_IRQ_NONKEY] = {
		.reg_offset = 1,
		.mask = DA9052_IRQ_MASK_POS_1,
	},
	[DA9052_IRQ_IDFLOAT] = {
		.reg_offset = 1,
		.mask = DA9052_IRQ_MASK_POS_2,
	},
	[DA9052_IRQ_IDGND] = {
		.reg_offset = 1,
		.mask = DA9052_IRQ_MASK_POS_3,
	},
	[DA9052_IRQ_CHGEND] = {
		.reg_offset = 1,
		.mask = DA9052_IRQ_MASK_POS_4,
	},
	[DA9052_IRQ_TBAT] = {
		.reg_offset = 1,
		.mask = DA9052_IRQ_MASK_POS_5,
	},
	[DA9052_IRQ_ADC_EOM] = {
		.reg_offset = 1,
		.mask = DA9052_IRQ_MASK_POS_6,
	},
	[DA9052_IRQ_PENDOWN] = {
		.reg_offset = 1,
		.mask = DA9052_IRQ_MASK_POS_7,
	},
	[DA9052_IRQ_TSIREADY] = {
		.reg_offset = 1,
		.mask = DA9052_IRQ_MASK_POS_8,
	},
	[DA9052_IRQ_GPI0] = {
		.reg_offset = 2,
		.mask = DA9052_IRQ_MASK_POS_1,
	},
	[DA9052_IRQ_GPI1] = {
		.reg_offset = 2,
		.mask = DA9052_IRQ_MASK_POS_2,
	},
	[DA9052_IRQ_GPI2] = {
		.reg_offset = 2,
		.mask = DA9052_IRQ_MASK_POS_3,
	},
	[DA9052_IRQ_GPI3] = {
		.reg_offset = 2,
		.mask = DA9052_IRQ_MASK_POS_4,
	},
	[DA9052_IRQ_GPI4] = {
		.reg_offset = 2,
		.mask = DA9052_IRQ_MASK_POS_5,
	},
	[DA9052_IRQ_GPI5] = {
		.reg_offset = 2,
		.mask = DA9052_IRQ_MASK_POS_6,
	},
	[DA9052_IRQ_GPI6] = {
		.reg_offset = 2,
		.mask = DA9052_IRQ_MASK_POS_7,
	},
	[DA9052_IRQ_GPI7] = {
		.reg_offset = 2,
		.mask = DA9052_IRQ_MASK_POS_8,
	},
	[DA9052_IRQ_GPI8] = {
		.reg_offset = 3,
		.mask = DA9052_IRQ_MASK_POS_1,
	},
	[DA9052_IRQ_GPI9] = {
		.reg_offset = 3,
		.mask = DA9052_IRQ_MASK_POS_2,
	},
	[DA9052_IRQ_GPI10] = {
		.reg_offset = 3,
		.mask = DA9052_IRQ_MASK_POS_3,
	},
	[DA9052_IRQ_GPI11] = {
		.reg_offset = 3,
		.mask = DA9052_IRQ_MASK_POS_4,
	},
	[DA9052_IRQ_GPI12] = {
		.reg_offset = 3,
		.mask = DA9052_IRQ_MASK_POS_5,
	},
	[DA9052_IRQ_GPI13] = {
		.reg_offset = 3,
		.mask = DA9052_IRQ_MASK_POS_6,
	},
	[DA9052_IRQ_GPI14] = {
		.reg_offset = 3,
		.mask = DA9052_IRQ_MASK_POS_7,
	},
	[DA9052_IRQ_GPI15] = {
		.reg_offset = 3,
		.mask = DA9052_IRQ_MASK_POS_8,
	},
};

static struct regmap_irq_chip da9052_regmap_irq_chip = {
	.name = "da9052_irq",
	.status_base = DA9052_EVENT_A_REG,
	.mask_base = DA9052_IRQ_MASK_A_REG,
	.ack_base = DA9052_EVENT_A_REG,
	.num_regs = DA9052_NUM_IRQ_REGS,
	.irqs = da9052_irqs,
	.num_irqs = ARRAY_SIZE(da9052_irqs),
};

=======
>>>>>>> refs/remotes/origin/master
struct regmap_config da9052_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,

	.cache_type = REGCACHE_RBTREE,

	.max_register = DA9052_PAGE1_CON_REG,
	.readable_reg = da9052_reg_readable,
	.writeable_reg = da9052_reg_writeable,
	.volatile_reg = da9052_reg_volatile,
};
EXPORT_SYMBOL_GPL(da9052_regmap_config);

<<<<<<< HEAD
int __devinit da9052_device_init(struct da9052 *da9052, u8 chip_id)
{
	struct da9052_pdata *pdata = da9052->dev->platform_data;
	struct irq_desc *desc;
	int ret;

=======
int da9052_device_init(struct da9052 *da9052, u8 chip_id)
{
	struct da9052_pdata *pdata = dev_get_platdata(da9052->dev);
	int ret;

	mutex_init(&da9052->auxadc_lock);
	init_completion(&da9052->done);

>>>>>>> refs/remotes/origin/master
	if (pdata && pdata->init != NULL)
		pdata->init(da9052);

	da9052->chip_id = chip_id;

<<<<<<< HEAD
	if (!pdata || !pdata->irq_base)
		da9052->irq_base = -1;
	else
		da9052->irq_base = pdata->irq_base;

	ret = regmap_add_irq_chip(da9052->regmap, da9052->chip_irq,
				  IRQF_TRIGGER_LOW | IRQF_ONESHOT,
				  da9052->irq_base, &da9052_regmap_irq_chip,
				  NULL);
	if (ret < 0)
		goto regmap_err;

	desc = irq_to_desc(da9052->chip_irq);
	da9052->irq_base = regmap_irq_chip_get_base(desc->action->dev_id);

	ret = mfd_add_devices(da9052->dev, -1, da9052_subdev_info,
			      ARRAY_SIZE(da9052_subdev_info), NULL, 0);
	if (ret)
		goto err;
=======
	ret = da9052_irq_init(da9052);
	if (ret != 0) {
		dev_err(da9052->dev, "da9052_irq_init failed: %d\n", ret);
		return ret;
	}

	ret = mfd_add_devices(da9052->dev, -1, da9052_subdev_info,
			      ARRAY_SIZE(da9052_subdev_info), NULL, 0, NULL);
	if (ret) {
		dev_err(da9052->dev, "mfd_add_devices failed: %d\n", ret);
		goto err;
	}
>>>>>>> refs/remotes/origin/master

	return 0;

err:
<<<<<<< HEAD
	mfd_remove_devices(da9052->dev);
regmap_err:
=======
	da9052_irq_exit(da9052);

>>>>>>> refs/remotes/origin/master
	return ret;
}

void da9052_device_exit(struct da9052 *da9052)
{
<<<<<<< HEAD
	regmap_del_irq_chip(da9052->chip_irq,
			    irq_get_irq_data(da9052->irq_base)->chip_data);
	mfd_remove_devices(da9052->dev);
=======
	mfd_remove_devices(da9052->dev);
	da9052_irq_exit(da9052);
>>>>>>> refs/remotes/origin/master
}

MODULE_AUTHOR("David Dajun Chen <dchen@diasemi.com>");
MODULE_DESCRIPTION("DA9052 MFD Core");
MODULE_LICENSE("GPL");
