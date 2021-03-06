/*
 * Driver for EETI eGalax Multiple Touch Controller
 *
 * Copyright (C) 2011 Freescale Semiconductor, Inc.
 *
 * based on max11801_ts.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/* EETI eGalax serial touch screen controller is a I2C based multiple
 * touch screen controller, it supports 5 point multiple touch. */

/* TODO:
  - auto idle mode support
*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/bitops.h>
#include <linux/input/mt.h>
<<<<<<< HEAD
=======
#include <linux/of_gpio.h>
>>>>>>> refs/remotes/origin/master

/*
 * Mouse Mode: some panel may configure the controller to mouse mode,
 * which can only report one point at a given time.
 * This driver will ignore events in this mode.
 */
#define REPORT_MODE_MOUSE		0x1
/*
 * Vendor Mode: this mode is used to transfer some vendor specific
 * messages.
 * This driver will ignore events in this mode.
 */
#define REPORT_MODE_VENDOR		0x3
/* Multiple Touch Mode */
#define REPORT_MODE_MTTOUCH		0x4

#define MAX_SUPPORT_POINTS		5

#define EVENT_VALID_OFFSET	7
#define EVENT_VALID_MASK	(0x1 << EVENT_VALID_OFFSET)
#define EVENT_ID_OFFSET		2
#define EVENT_ID_MASK		(0xf << EVENT_ID_OFFSET)
#define EVENT_IN_RANGE		(0x1 << 1)
#define EVENT_DOWN_UP		(0X1 << 0)

#define MAX_I2C_DATA_LEN	10

#define EGALAX_MAX_X	32760
#define EGALAX_MAX_Y	32760
#define EGALAX_MAX_TRIES 100

struct egalax_ts {
	struct i2c_client		*client;
	struct input_dev		*input_dev;
};

static irqreturn_t egalax_ts_interrupt(int irq, void *dev_id)
{
	struct egalax_ts *ts = dev_id;
	struct input_dev *input_dev = ts->input_dev;
	struct i2c_client *client = ts->client;
	u8 buf[MAX_I2C_DATA_LEN];
	int id, ret, x, y, z;
	int tries = 0;
	bool down, valid;
	u8 state;

	do {
		ret = i2c_master_recv(client, buf, MAX_I2C_DATA_LEN);
	} while (ret == -EAGAIN && tries++ < EGALAX_MAX_TRIES);

	if (ret < 0)
		return IRQ_HANDLED;

	if (buf[0] != REPORT_MODE_MTTOUCH) {
		/* ignore mouse events and vendor events */
		return IRQ_HANDLED;
	}

	state = buf[1];
	x = (buf[3] << 8) | buf[2];
	y = (buf[5] << 8) | buf[4];
	z = (buf[7] << 8) | buf[6];

	valid = state & EVENT_VALID_MASK;
	id = (state & EVENT_ID_MASK) >> EVENT_ID_OFFSET;
	down = state & EVENT_DOWN_UP;

	if (!valid || id > MAX_SUPPORT_POINTS) {
		dev_dbg(&client->dev, "point invalid\n");
		return IRQ_HANDLED;
	}

	input_mt_slot(input_dev, id);
	input_mt_report_slot_state(input_dev, MT_TOOL_FINGER, down);

	dev_dbg(&client->dev, "%s id:%d x:%d y:%d z:%d",
		down ? "down" : "up", id, x, y, z);

	if (down) {
		input_report_abs(input_dev, ABS_MT_POSITION_X, x);
		input_report_abs(input_dev, ABS_MT_POSITION_Y, y);
		input_report_abs(input_dev, ABS_MT_PRESSURE, z);
	}

	input_mt_report_pointer_emulation(input_dev, true);
	input_sync(input_dev);

	return IRQ_HANDLED;
}

/* wake up controller by an falling edge of interrupt gpio.  */
static int egalax_wake_up_device(struct i2c_client *client)
{
<<<<<<< HEAD
	int gpio = irq_to_gpio(client->irq);
	int ret;

=======
	struct device_node *np = client->dev.of_node;
	int gpio;
	int ret;

	if (!np)
		return -ENODEV;

	gpio = of_get_named_gpio(np, "wakeup-gpios", 0);
	if (!gpio_is_valid(gpio))
		return -ENODEV;

>>>>>>> refs/remotes/origin/master
	ret = gpio_request(gpio, "egalax_irq");
	if (ret < 0) {
		dev_err(&client->dev,
			"request gpio failed, cannot wake up controller: %d\n",
			ret);
		return ret;
	}

	/* wake up controller via an falling edge on IRQ gpio. */
	gpio_direction_output(gpio, 0);
	gpio_set_value(gpio, 1);

	/* controller should be waken up, return irq.  */
	gpio_direction_input(gpio);
	gpio_free(gpio);

	return 0;
}

<<<<<<< HEAD
static int __devinit egalax_firmware_version(struct i2c_client *client)
=======
static int egalax_firmware_version(struct i2c_client *client)
>>>>>>> refs/remotes/origin/master
{
	static const u8 cmd[MAX_I2C_DATA_LEN] = { 0x03, 0x03, 0xa, 0x01, 0x41 };
	int ret;

	ret = i2c_master_send(client, cmd, MAX_I2C_DATA_LEN);
	if (ret < 0)
		return ret;

	return 0;
}

<<<<<<< HEAD
static int __devinit egalax_ts_probe(struct i2c_client *client,
				       const struct i2c_device_id *id)
{
	struct egalax_ts *ts;
	struct input_dev *input_dev;
	int ret;
	int error;

	ts = kzalloc(sizeof(struct egalax_ts), GFP_KERNEL);
=======
static int egalax_ts_probe(struct i2c_client *client,
			   const struct i2c_device_id *id)
{
	struct egalax_ts *ts;
	struct input_dev *input_dev;
	int error;

	ts = devm_kzalloc(&client->dev, sizeof(struct egalax_ts), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!ts) {
		dev_err(&client->dev, "Failed to allocate memory\n");
		return -ENOMEM;
	}

<<<<<<< HEAD
	input_dev = input_allocate_device();
	if (!input_dev) {
		dev_err(&client->dev, "Failed to allocate memory\n");
		error = -ENOMEM;
		goto err_free_ts;
=======
	input_dev = devm_input_allocate_device(&client->dev);
	if (!input_dev) {
		dev_err(&client->dev, "Failed to allocate memory\n");
		return -ENOMEM;
>>>>>>> refs/remotes/origin/master
	}

	ts->client = client;
	ts->input_dev = input_dev;

	/* controller may be in sleep, wake it up. */
<<<<<<< HEAD
	egalax_wake_up_device(client);

	ret = egalax_firmware_version(client);
	if (ret < 0) {
		dev_err(&client->dev, "Failed to read firmware version\n");
		error = -EIO;
		goto err_free_dev;
=======
	error = egalax_wake_up_device(client);
	if (error) {
		dev_err(&client->dev, "Failed to wake up the controller\n");
		return error;
	}

	error = egalax_firmware_version(client);
	if (error < 0) {
		dev_err(&client->dev, "Failed to read firmware version\n");
		return error;
>>>>>>> refs/remotes/origin/master
	}

	input_dev->name = "EETI eGalax Touch Screen";
	input_dev->id.bustype = BUS_I2C;
<<<<<<< HEAD
	input_dev->dev.parent = &client->dev;
=======
>>>>>>> refs/remotes/origin/master

	__set_bit(EV_ABS, input_dev->evbit);
	__set_bit(EV_KEY, input_dev->evbit);
	__set_bit(BTN_TOUCH, input_dev->keybit);

	input_set_abs_params(input_dev, ABS_X, 0, EGALAX_MAX_X, 0, 0);
	input_set_abs_params(input_dev, ABS_Y, 0, EGALAX_MAX_Y, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_MT_POSITION_X, 0, EGALAX_MAX_X, 0, 0);
	input_set_abs_params(input_dev,
<<<<<<< HEAD
			     ABS_MT_POSITION_X, 0, EGALAX_MAX_Y, 0, 0);
	input_mt_init_slots(input_dev, MAX_SUPPORT_POINTS);

	input_set_drvdata(input_dev, ts);

	error = request_threaded_irq(client->irq, NULL, egalax_ts_interrupt,
				     IRQF_TRIGGER_LOW | IRQF_ONESHOT,
				     "egalax_ts", ts);
	if (error < 0) {
		dev_err(&client->dev, "Failed to register interrupt\n");
		goto err_free_dev;
=======
			     ABS_MT_POSITION_Y, 0, EGALAX_MAX_Y, 0, 0);
	input_mt_init_slots(input_dev, MAX_SUPPORT_POINTS, 0);

	input_set_drvdata(input_dev, ts);

	error = devm_request_threaded_irq(&client->dev, client->irq, NULL,
					  egalax_ts_interrupt,
					  IRQF_TRIGGER_LOW | IRQF_ONESHOT,
					  "egalax_ts", ts);
	if (error < 0) {
		dev_err(&client->dev, "Failed to register interrupt\n");
		return error;
>>>>>>> refs/remotes/origin/master
	}

	error = input_register_device(ts->input_dev);
	if (error)
<<<<<<< HEAD
		goto err_free_irq;

	i2c_set_clientdata(client, ts);
	return 0;

err_free_irq:
	free_irq(client->irq, ts);
err_free_dev:
	input_free_device(input_dev);
err_free_ts:
	kfree(ts);

	return error;
}

static __devexit int egalax_ts_remove(struct i2c_client *client)
{
	struct egalax_ts *ts = i2c_get_clientdata(client);

	free_irq(client->irq, ts);

	input_unregister_device(ts->input_dev);
	kfree(ts);

	return 0;
=======
		return error;

	i2c_set_clientdata(client, ts);
	return 0;
>>>>>>> refs/remotes/origin/master
}

static const struct i2c_device_id egalax_ts_id[] = {
	{ "egalax_ts", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, egalax_ts_id);

#ifdef CONFIG_PM_SLEEP
static int egalax_ts_suspend(struct device *dev)
{
	static const u8 suspend_cmd[MAX_I2C_DATA_LEN] = {
		0x3, 0x6, 0xa, 0x3, 0x36, 0x3f, 0x2, 0, 0, 0
	};
	struct i2c_client *client = to_i2c_client(dev);
	int ret;

	ret = i2c_master_send(client, suspend_cmd, MAX_I2C_DATA_LEN);
	return ret > 0 ? 0 : ret;
}

static int egalax_ts_resume(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);

	return egalax_wake_up_device(client);
}
#endif

static SIMPLE_DEV_PM_OPS(egalax_ts_pm_ops, egalax_ts_suspend, egalax_ts_resume);

<<<<<<< HEAD
=======
static struct of_device_id egalax_ts_dt_ids[] = {
	{ .compatible = "eeti,egalax_ts" },
	{ /* sentinel */ }
};

>>>>>>> refs/remotes/origin/master
static struct i2c_driver egalax_ts_driver = {
	.driver = {
		.name	= "egalax_ts",
		.owner	= THIS_MODULE,
		.pm	= &egalax_ts_pm_ops,
<<<<<<< HEAD
	},
	.id_table	= egalax_ts_id,
	.probe		= egalax_ts_probe,
	.remove		= __devexit_p(egalax_ts_remove),
=======
		.of_match_table	= egalax_ts_dt_ids,
	},
	.id_table	= egalax_ts_id,
	.probe		= egalax_ts_probe,
>>>>>>> refs/remotes/origin/master
};

module_i2c_driver(egalax_ts_driver);

MODULE_AUTHOR("Freescale Semiconductor, Inc.");
MODULE_DESCRIPTION("Touchscreen driver for EETI eGalax touch controller");
MODULE_LICENSE("GPL");
