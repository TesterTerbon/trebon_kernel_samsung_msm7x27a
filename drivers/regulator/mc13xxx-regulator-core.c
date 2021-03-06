/*
 * Regulator Driver for Freescale MC13xxx PMIC
 *
 * Copyright 2010 Yong Shen <yong.shen@linaro.org>
 *
 * Based on mc13783 regulator driver :
 * Copyright (C) 2008 Sascha Hauer, Pengutronix <s.hauer@pengutronix.de>
 * Copyright 2009 Alberto Panizzo <maramaopercheseimorto@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Regs infos taken from mc13xxx drivers from freescale and mc13xxx.pdf file
 * from freescale
 */

#include <linux/mfd/mc13xxx.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/driver.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/regulator/of_regulator.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/regulator/of_regulator.h>
>>>>>>> refs/remotes/origin/master
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/err.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/module.h>
#include <linux/of.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/module.h>
#include <linux/of.h>
>>>>>>> refs/remotes/origin/master
#include "mc13xxx.h"

static int mc13xxx_regulator_enable(struct regulator_dev *rdev)
{
	struct mc13xxx_regulator_priv *priv = rdev_get_drvdata(rdev);
	struct mc13xxx_regulator *mc13xxx_regulators = priv->mc13xxx_regulators;
	int id = rdev_get_id(rdev);
	int ret;

	dev_dbg(rdev_get_dev(rdev), "%s id: %d\n", __func__, id);

	mc13xxx_lock(priv->mc13xxx);
	ret = mc13xxx_reg_rmw(priv->mc13xxx, mc13xxx_regulators[id].reg,
			mc13xxx_regulators[id].enable_bit,
			mc13xxx_regulators[id].enable_bit);
	mc13xxx_unlock(priv->mc13xxx);

	return ret;
}

static int mc13xxx_regulator_disable(struct regulator_dev *rdev)
{
	struct mc13xxx_regulator_priv *priv = rdev_get_drvdata(rdev);
	struct mc13xxx_regulator *mc13xxx_regulators = priv->mc13xxx_regulators;
	int id = rdev_get_id(rdev);
	int ret;

	dev_dbg(rdev_get_dev(rdev), "%s id: %d\n", __func__, id);

	mc13xxx_lock(priv->mc13xxx);
	ret = mc13xxx_reg_rmw(priv->mc13xxx, mc13xxx_regulators[id].reg,
			mc13xxx_regulators[id].enable_bit, 0);
	mc13xxx_unlock(priv->mc13xxx);

	return ret;
}

static int mc13xxx_regulator_is_enabled(struct regulator_dev *rdev)
{
	struct mc13xxx_regulator_priv *priv = rdev_get_drvdata(rdev);
	struct mc13xxx_regulator *mc13xxx_regulators = priv->mc13xxx_regulators;
	int ret, id = rdev_get_id(rdev);
	unsigned int val;

	mc13xxx_lock(priv->mc13xxx);
	ret = mc13xxx_reg_read(priv->mc13xxx, mc13xxx_regulators[id].reg, &val);
	mc13xxx_unlock(priv->mc13xxx);

	if (ret)
		return ret;

	return (val & mc13xxx_regulators[id].enable_bit) != 0;
}

<<<<<<< HEAD
int mc13xxx_regulator_list_voltage(struct regulator_dev *rdev,
						unsigned selector)
{
	int id = rdev_get_id(rdev);
	struct mc13xxx_regulator_priv *priv = rdev_get_drvdata(rdev);
	struct mc13xxx_regulator *mc13xxx_regulators = priv->mc13xxx_regulators;

	if (selector >= mc13xxx_regulators[id].desc.n_voltages)
		return -EINVAL;

	return mc13xxx_regulators[id].voltages[selector];
}
EXPORT_SYMBOL_GPL(mc13xxx_regulator_list_voltage);

int mc13xxx_get_best_voltage_index(struct regulator_dev *rdev,
						int min_uV, int max_uV)
{
	struct mc13xxx_regulator_priv *priv = rdev_get_drvdata(rdev);
	struct mc13xxx_regulator *mc13xxx_regulators = priv->mc13xxx_regulators;
	int reg_id = rdev_get_id(rdev);
	int i;
	int bestmatch;
	int bestindex;

	/*
	 * Locate the minimum voltage fitting the criteria on
	 * this regulator. The switchable voltages are not
	 * in strict falling order so we need to check them
	 * all for the best match.
	 */
	bestmatch = INT_MAX;
	bestindex = -1;
	for (i = 0; i < mc13xxx_regulators[reg_id].desc.n_voltages; i++) {
		if (mc13xxx_regulators[reg_id].voltages[i] >= min_uV &&
		    mc13xxx_regulators[reg_id].voltages[i] < bestmatch) {
			bestmatch = mc13xxx_regulators[reg_id].voltages[i];
			bestindex = i;
		}
	}

	if (bestindex < 0 || bestmatch > max_uV) {
		dev_warn(&rdev->dev, "no possible value for %d<=x<=%d uV\n",
				min_uV, max_uV);
		return -EINVAL;
	}
	return bestindex;
}
EXPORT_SYMBOL_GPL(mc13xxx_get_best_voltage_index);

static int mc13xxx_regulator_set_voltage(struct regulator_dev *rdev, int min_uV,
		int max_uV, unsigned *selector)
{
	struct mc13xxx_regulator_priv *priv = rdev_get_drvdata(rdev);
	struct mc13xxx_regulator *mc13xxx_regulators = priv->mc13xxx_regulators;
	int value, id = rdev_get_id(rdev);
	int ret;

	dev_dbg(rdev_get_dev(rdev), "%s id: %d min_uV: %d max_uV: %d\n",
		__func__, id, min_uV, max_uV);

	/* Find the best index */
	value = mc13xxx_get_best_voltage_index(rdev, min_uV, max_uV);
	dev_dbg(rdev_get_dev(rdev), "%s best value: %d\n", __func__, value);
	if (value < 0)
		return value;

	mc13xxx_lock(priv->mc13xxx);
	ret = mc13xxx_reg_rmw(priv->mc13xxx, mc13xxx_regulators[id].vsel_reg,
			mc13xxx_regulators[id].vsel_mask,
			value << mc13xxx_regulators[id].vsel_shift);
=======
static int mc13xxx_regulator_set_voltage_sel(struct regulator_dev *rdev,
					     unsigned selector)
{
	struct mc13xxx_regulator_priv *priv = rdev_get_drvdata(rdev);
	struct mc13xxx_regulator *mc13xxx_regulators = priv->mc13xxx_regulators;
	int id = rdev_get_id(rdev);
	int ret;

	mc13xxx_lock(priv->mc13xxx);
	ret = mc13xxx_reg_rmw(priv->mc13xxx, mc13xxx_regulators[id].vsel_reg,
			mc13xxx_regulators[id].vsel_mask,
			selector << mc13xxx_regulators[id].vsel_shift);
>>>>>>> refs/remotes/origin/master
	mc13xxx_unlock(priv->mc13xxx);

	return ret;
}

static int mc13xxx_regulator_get_voltage(struct regulator_dev *rdev)
{
	struct mc13xxx_regulator_priv *priv = rdev_get_drvdata(rdev);
	struct mc13xxx_regulator *mc13xxx_regulators = priv->mc13xxx_regulators;
	int ret, id = rdev_get_id(rdev);
	unsigned int val;

	dev_dbg(rdev_get_dev(rdev), "%s id: %d\n", __func__, id);

	mc13xxx_lock(priv->mc13xxx);
	ret = mc13xxx_reg_read(priv->mc13xxx,
				mc13xxx_regulators[id].vsel_reg, &val);
	mc13xxx_unlock(priv->mc13xxx);

	if (ret)
		return ret;

	val = (val & mc13xxx_regulators[id].vsel_mask)
		>> mc13xxx_regulators[id].vsel_shift;

	dev_dbg(rdev_get_dev(rdev), "%s id: %d val: %d\n", __func__, id, val);

	BUG_ON(val >= mc13xxx_regulators[id].desc.n_voltages);

<<<<<<< HEAD
	return mc13xxx_regulators[id].voltages[val];
=======
	return rdev->desc->volt_table[val];
>>>>>>> refs/remotes/origin/master
}

struct regulator_ops mc13xxx_regulator_ops = {
	.enable = mc13xxx_regulator_enable,
	.disable = mc13xxx_regulator_disable,
	.is_enabled = mc13xxx_regulator_is_enabled,
<<<<<<< HEAD
	.list_voltage = mc13xxx_regulator_list_voltage,
	.set_voltage = mc13xxx_regulator_set_voltage,
=======
	.list_voltage = regulator_list_voltage_table,
	.set_voltage_sel = mc13xxx_regulator_set_voltage_sel,
>>>>>>> refs/remotes/origin/master
	.get_voltage = mc13xxx_regulator_get_voltage,
};
EXPORT_SYMBOL_GPL(mc13xxx_regulator_ops);

int mc13xxx_fixed_regulator_set_voltage(struct regulator_dev *rdev, int min_uV,
	       int max_uV, unsigned *selector)
{
<<<<<<< HEAD
	struct mc13xxx_regulator_priv *priv = rdev_get_drvdata(rdev);
	struct mc13xxx_regulator *mc13xxx_regulators = priv->mc13xxx_regulators;
=======
>>>>>>> refs/remotes/origin/master
	int id = rdev_get_id(rdev);

	dev_dbg(rdev_get_dev(rdev), "%s id: %d min_uV: %d max_uV: %d\n",
		__func__, id, min_uV, max_uV);

<<<<<<< HEAD
	if (min_uV >= mc13xxx_regulators[id].voltages[0] &&
	    max_uV <= mc13xxx_regulators[id].voltages[0])
		return 0;
	else
		return -EINVAL;
}
EXPORT_SYMBOL_GPL(mc13xxx_fixed_regulator_set_voltage);

int mc13xxx_fixed_regulator_get_voltage(struct regulator_dev *rdev)
{
	struct mc13xxx_regulator_priv *priv = rdev_get_drvdata(rdev);
	struct mc13xxx_regulator *mc13xxx_regulators = priv->mc13xxx_regulators;
	int id = rdev_get_id(rdev);

	dev_dbg(rdev_get_dev(rdev), "%s id: %d\n", __func__, id);

	return mc13xxx_regulators[id].voltages[0];
}
EXPORT_SYMBOL_GPL(mc13xxx_fixed_regulator_get_voltage);

=======
	if (min_uV <= rdev->desc->volt_table[0] &&
	    rdev->desc->volt_table[0] <= max_uV) {
		*selector = 0;
		return 0;
	} else {
		return -EINVAL;
	}
}
EXPORT_SYMBOL_GPL(mc13xxx_fixed_regulator_set_voltage);

>>>>>>> refs/remotes/origin/master
struct regulator_ops mc13xxx_fixed_regulator_ops = {
	.enable = mc13xxx_regulator_enable,
	.disable = mc13xxx_regulator_disable,
	.is_enabled = mc13xxx_regulator_is_enabled,
<<<<<<< HEAD
	.list_voltage = mc13xxx_regulator_list_voltage,
	.set_voltage = mc13xxx_fixed_regulator_set_voltage,
	.get_voltage = mc13xxx_fixed_regulator_get_voltage,
};
EXPORT_SYMBOL_GPL(mc13xxx_fixed_regulator_ops);

int mc13xxx_sw_regulator_is_enabled(struct regulator_dev *rdev)
{
	return 1;
}
EXPORT_SYMBOL_GPL(mc13xxx_sw_regulator_is_enabled);

<<<<<<< HEAD
=======
#ifdef CONFIG_OF
int __devinit mc13xxx_get_num_regulators_dt(struct platform_device *pdev)
{
	struct device_node *parent, *child;
	int num = 0;
=======
	.list_voltage = regulator_list_voltage_table,
	.set_voltage = mc13xxx_fixed_regulator_set_voltage,
};
EXPORT_SYMBOL_GPL(mc13xxx_fixed_regulator_ops);

#ifdef CONFIG_OF
int mc13xxx_get_num_regulators_dt(struct platform_device *pdev)
{
	struct device_node *parent;
	int num;
>>>>>>> refs/remotes/origin/master

	of_node_get(pdev->dev.parent->of_node);
	parent = of_find_node_by_name(pdev->dev.parent->of_node, "regulators");
	if (!parent)
		return -ENODEV;

<<<<<<< HEAD
	for_each_child_of_node(parent, child)
		num++;

=======
	num = of_get_child_count(parent);
	of_node_put(parent);
>>>>>>> refs/remotes/origin/master
	return num;
}
EXPORT_SYMBOL_GPL(mc13xxx_get_num_regulators_dt);

<<<<<<< HEAD
struct mc13xxx_regulator_init_data * __devinit mc13xxx_parse_regulators_dt(
=======
struct mc13xxx_regulator_init_data *mc13xxx_parse_regulators_dt(
>>>>>>> refs/remotes/origin/master
	struct platform_device *pdev, struct mc13xxx_regulator *regulators,
	int num_regulators)
{
	struct mc13xxx_regulator_priv *priv = platform_get_drvdata(pdev);
	struct mc13xxx_regulator_init_data *data, *p;
	struct device_node *parent, *child;
<<<<<<< HEAD
	int i;
=======
	int i, parsed = 0;
>>>>>>> refs/remotes/origin/master

	of_node_get(pdev->dev.parent->of_node);
	parent = of_find_node_by_name(pdev->dev.parent->of_node, "regulators");
	if (!parent)
		return NULL;

	data = devm_kzalloc(&pdev->dev, sizeof(*data) * priv->num_regulators,
			    GFP_KERNEL);
<<<<<<< HEAD
	if (!data)
		return NULL;
	p = data;

	for_each_child_of_node(parent, child) {
		for (i = 0; i < num_regulators; i++) {
=======
	if (!data) {
		of_node_put(parent);
		return NULL;
	}

	p = data;

	for_each_child_of_node(parent, child) {
		int found = 0;

		for (i = 0; i < num_regulators; i++) {
			if (!regulators[i].desc.name)
				continue;
>>>>>>> refs/remotes/origin/master
			if (!of_node_cmp(child->name,
					 regulators[i].desc.name)) {
				p->id = i;
				p->init_data = of_get_regulator_init_data(
							&pdev->dev, child);
				p->node = child;
				p++;
<<<<<<< HEAD
				break;
			}
		}
	}
=======

				parsed++;
				found = 1;
				break;
			}
		}

		if (!found)
			dev_warn(&pdev->dev,
				 "Unknown regulator: %s\n", child->name);
	}
	of_node_put(parent);

	priv->num_regulators = parsed;
>>>>>>> refs/remotes/origin/master

	return data;
}
EXPORT_SYMBOL_GPL(mc13xxx_parse_regulators_dt);
#endif

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Yong Shen <yong.shen@linaro.org>");
MODULE_DESCRIPTION("Regulator Driver for Freescale MC13xxx PMIC");
MODULE_ALIAS("mc13xxx-regulator-core");
