/*
 * Driver for TPS61050/61052 boost converters, typically used for white LEDs
 * or audio amplifiers.
 *
 * Copyright (C) 2011 ST-Ericsson SA
 * Written on behalf of Linaro for ST-Ericsson
 *
 * Author: Linus Walleij <linus.walleij@linaro.org>
 *
 * License terms: GNU General Public License (GPL) version 2
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/mfd/core.h>
#include <linux/mfd/tps6105x.h>

<<<<<<< HEAD
static const int tps6105x_voltages[] = {
=======
static const unsigned int tps6105x_voltages[] = {
>>>>>>> refs/remotes/origin/master
	4500000,
	5000000,
	5250000,
	5000000, /* There is an additional 5V */
};

static int tps6105x_regulator_enable(struct regulator_dev *rdev)
{
	struct tps6105x *tps6105x = rdev_get_drvdata(rdev);
	int ret;

	/* Activate voltage mode */
	ret = tps6105x_mask_and_set(tps6105x, TPS6105X_REG_0,
		TPS6105X_REG0_MODE_MASK,
		TPS6105X_REG0_MODE_VOLTAGE << TPS6105X_REG0_MODE_SHIFT);
	if (ret)
		return ret;

	return 0;
}

static int tps6105x_regulator_disable(struct regulator_dev *rdev)
{
	struct tps6105x *tps6105x = rdev_get_drvdata(rdev);
	int ret;

	/* Set into shutdown mode */
	ret = tps6105x_mask_and_set(tps6105x, TPS6105X_REG_0,
		TPS6105X_REG0_MODE_MASK,
		TPS6105X_REG0_MODE_SHUTDOWN << TPS6105X_REG0_MODE_SHIFT);
	if (ret)
		return ret;

	return 0;
}

static int tps6105x_regulator_is_enabled(struct regulator_dev *rdev)
{
	struct tps6105x *tps6105x = rdev_get_drvdata(rdev);
	u8 regval;
	int ret;

	ret = tps6105x_get(tps6105x, TPS6105X_REG_0, &regval);
	if (ret)
		return ret;
	regval &= TPS6105X_REG0_MODE_MASK;
	regval >>= TPS6105X_REG0_MODE_SHIFT;

	if (regval == TPS6105X_REG0_MODE_VOLTAGE)
		return 1;

	return 0;
}

static int tps6105x_regulator_get_voltage_sel(struct regulator_dev *rdev)
{
	struct tps6105x *tps6105x = rdev_get_drvdata(rdev);
	u8 regval;
	int ret;

	ret = tps6105x_get(tps6105x, TPS6105X_REG_0, &regval);
	if (ret)
		return ret;

	regval &= TPS6105X_REG0_VOLTAGE_MASK;
	regval >>= TPS6105X_REG0_VOLTAGE_SHIFT;
	return (int) regval;
}

static int tps6105x_regulator_set_voltage_sel(struct regulator_dev *rdev,
					      unsigned selector)
{
	struct tps6105x *tps6105x = rdev_get_drvdata(rdev);
	int ret;

	ret = tps6105x_mask_and_set(tps6105x, TPS6105X_REG_0,
				    TPS6105X_REG0_VOLTAGE_MASK,
				    selector << TPS6105X_REG0_VOLTAGE_SHIFT);
	if (ret)
		return ret;

	return 0;
}

<<<<<<< HEAD
static int tps6105x_regulator_list_voltage(struct regulator_dev *rdev,
					   unsigned selector)
{
	if (selector >= ARRAY_SIZE(tps6105x_voltages))
		return -EINVAL;

	return tps6105x_voltages[selector];
}

=======
>>>>>>> refs/remotes/origin/master
static struct regulator_ops tps6105x_regulator_ops = {
	.enable		= tps6105x_regulator_enable,
	.disable	= tps6105x_regulator_disable,
	.is_enabled	= tps6105x_regulator_is_enabled,
	.get_voltage_sel = tps6105x_regulator_get_voltage_sel,
	.set_voltage_sel = tps6105x_regulator_set_voltage_sel,
<<<<<<< HEAD
	.list_voltage	= tps6105x_regulator_list_voltage,
};

static struct regulator_desc tps6105x_regulator_desc = {
=======
	.list_voltage	= regulator_list_voltage_table,
};

static const struct regulator_desc tps6105x_regulator_desc = {
>>>>>>> refs/remotes/origin/master
	.name		= "tps6105x-boost",
	.ops		= &tps6105x_regulator_ops,
	.type		= REGULATOR_VOLTAGE,
	.id		= 0,
	.owner		= THIS_MODULE,
	.n_voltages	= ARRAY_SIZE(tps6105x_voltages),
<<<<<<< HEAD
=======
	.volt_table	= tps6105x_voltages,
>>>>>>> refs/remotes/origin/master
};

/*
 * Registers the chip as a voltage regulator
 */
<<<<<<< HEAD
static int __devinit tps6105x_regulator_probe(struct platform_device *pdev)
{
	struct tps6105x *tps6105x = dev_get_platdata(&pdev->dev);
	struct tps6105x_platform_data *pdata = tps6105x->pdata;
=======
static int tps6105x_regulator_probe(struct platform_device *pdev)
{
	struct tps6105x *tps6105x = dev_get_platdata(&pdev->dev);
	struct tps6105x_platform_data *pdata = tps6105x->pdata;
	struct regulator_config config = { };
>>>>>>> refs/remotes/origin/master
	int ret;

	/* This instance is not set for regulator mode so bail out */
	if (pdata->mode != TPS6105X_MODE_VOLTAGE) {
		dev_info(&pdev->dev,
<<<<<<< HEAD
			 "chip not in voltage mode mode, exit probe \n");
		return 0;
	}

	/* Register regulator with framework */
	tps6105x->regulator = regulator_register(&tps6105x_regulator_desc,
					     &tps6105x->client->dev,
<<<<<<< HEAD
					     pdata->regulator_data, tps6105x);
=======
					     pdata->regulator_data, tps6105x,
					     NULL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			"chip not in voltage mode mode, exit probe\n");
		return 0;
	}

	config.dev = &tps6105x->client->dev;
	config.init_data = pdata->regulator_data;
	config.driver_data = tps6105x;

	/* Register regulator with framework */
	tps6105x->regulator = devm_regulator_register(&pdev->dev,
						      &tps6105x_regulator_desc,
						      &config);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(tps6105x->regulator)) {
		ret = PTR_ERR(tps6105x->regulator);
		dev_err(&tps6105x->client->dev,
			"failed to register regulator\n");
		return ret;
	}
	platform_set_drvdata(pdev, tps6105x);

	return 0;
}

<<<<<<< HEAD
static int __devexit tps6105x_regulator_remove(struct platform_device *pdev)
{
	struct tps6105x *tps6105x = dev_get_platdata(&pdev->dev);
	regulator_unregister(tps6105x->regulator);
	return 0;
}

=======
>>>>>>> refs/remotes/origin/master
static struct platform_driver tps6105x_regulator_driver = {
	.driver = {
		.name  = "tps6105x-regulator",
		.owner = THIS_MODULE,
	},
	.probe = tps6105x_regulator_probe,
<<<<<<< HEAD
	.remove = __devexit_p(tps6105x_regulator_remove),
=======
>>>>>>> refs/remotes/origin/master
};

static __init int tps6105x_regulator_init(void)
{
	return platform_driver_register(&tps6105x_regulator_driver);
}
subsys_initcall(tps6105x_regulator_init);

static __exit void tps6105x_regulator_exit(void)
{
	platform_driver_unregister(&tps6105x_regulator_driver);
}
module_exit(tps6105x_regulator_exit);

MODULE_AUTHOR("Linus Walleij <linus.walleij@linaro.org>");
MODULE_DESCRIPTION("TPS6105x regulator driver");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:tps6105x-regulator");
