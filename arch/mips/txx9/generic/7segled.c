/*
 * 7 Segment LED routines
 * Based on RBTX49xx patch from CELF patch archive.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * (C) Copyright TOSHIBA CORPORATION 2005-2007
 * All Rights Reserved.
 */
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/sysdev.h>
=======
#include <linux/device.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/device.h>
>>>>>>> refs/remotes/origin/master
#include <linux/slab.h>
#include <linux/map_to_7segment.h>
#include <asm/txx9/generic.h>

static unsigned int tx_7segled_num;
static void (*tx_7segled_putc)(unsigned int pos, unsigned char val);

void __init txx9_7segled_init(unsigned int num,
			      void (*putc)(unsigned int pos, unsigned char val))
{
	tx_7segled_num = num;
	tx_7segled_putc = putc;
}

static SEG7_CONVERSION_MAP(txx9_seg7map, MAP_ASCII7SEG_ALPHANUM_LC);

int txx9_7segled_putc(unsigned int pos, char c)
{
	if (pos >= tx_7segled_num)
		return -EINVAL;
	c = map_to_seg7(&txx9_seg7map, c);
	if (c < 0)
		return c;
	tx_7segled_putc(pos, c);
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static ssize_t ascii_store(struct sys_device *dev,
			   struct sysdev_attribute *attr,
=======
static ssize_t ascii_store(struct device *dev,
			   struct device_attribute *attr,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static ssize_t ascii_store(struct device *dev,
			   struct device_attribute *attr,
>>>>>>> refs/remotes/origin/master
			   const char *buf, size_t size)
{
	unsigned int ch = dev->id;
	txx9_7segled_putc(ch, buf[0]);
	return size;
}

<<<<<<< HEAD
<<<<<<< HEAD
static ssize_t raw_store(struct sys_device *dev,
			 struct sysdev_attribute *attr,
=======
static ssize_t raw_store(struct device *dev,
			 struct device_attribute *attr,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static ssize_t raw_store(struct device *dev,
			 struct device_attribute *attr,
>>>>>>> refs/remotes/origin/master
			 const char *buf, size_t size)
{
	unsigned int ch = dev->id;
	tx_7segled_putc(ch, buf[0]);
	return size;
}

<<<<<<< HEAD
<<<<<<< HEAD
static SYSDEV_ATTR(ascii, 0200, NULL, ascii_store);
static SYSDEV_ATTR(raw, 0200, NULL, raw_store);

static ssize_t map_seg7_show(struct sysdev_class *class,
			     struct sysdev_class_attribute *attr,
=======
=======
>>>>>>> refs/remotes/origin/master
static DEVICE_ATTR(ascii, 0200, NULL, ascii_store);
static DEVICE_ATTR(raw, 0200, NULL, raw_store);

static ssize_t map_seg7_show(struct device *dev,
			     struct device_attribute *attr,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			     char *buf)
{
	memcpy(buf, &txx9_seg7map, sizeof(txx9_seg7map));
	return sizeof(txx9_seg7map);
}

<<<<<<< HEAD
<<<<<<< HEAD
static ssize_t map_seg7_store(struct sysdev_class *class,
			      struct sysdev_class_attribute *attr,
=======
static ssize_t map_seg7_store(struct device *dev,
			      struct device_attribute *attr,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static ssize_t map_seg7_store(struct device *dev,
			      struct device_attribute *attr,
>>>>>>> refs/remotes/origin/master
			      const char *buf, size_t size)
{
	if (size != sizeof(txx9_seg7map))
		return -EINVAL;
	memcpy(&txx9_seg7map, buf, size);
	return size;
}

<<<<<<< HEAD
<<<<<<< HEAD
static SYSDEV_CLASS_ATTR(map_seg7, 0600, map_seg7_show, map_seg7_store);

static struct sysdev_class tx_7segled_sysdev_class = {
	.name	= "7segled",
=======
=======
>>>>>>> refs/remotes/origin/master
static DEVICE_ATTR(map_seg7, 0600, map_seg7_show, map_seg7_store);

static struct bus_type tx_7segled_subsys = {
	.name		= "7segled",
	.dev_name	= "7segled",
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

static int __init tx_7segled_init_sysfs(void)
{
	int error, i;
	if (!tx_7segled_num)
		return -ENODEV;
<<<<<<< HEAD
<<<<<<< HEAD
	error = sysdev_class_register(&tx_7segled_sysdev_class);
	if (error)
		return error;
	error = sysdev_class_create_file(&tx_7segled_sysdev_class,
					 &attr_map_seg7);
	if (error)
		return error;
	for (i = 0; i < tx_7segled_num; i++) {
		struct sys_device *dev;
=======
=======
>>>>>>> refs/remotes/origin/master
	error = subsys_system_register(&tx_7segled_subsys, NULL);
	if (error)
		return error;
	error = device_create_file(tx_7segled_subsys.dev_root, &dev_attr_map_seg7);
	if (error)
		return error;
	for (i = 0; i < tx_7segled_num; i++) {
		struct device *dev;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		dev = kzalloc(sizeof(*dev), GFP_KERNEL);
		if (!dev) {
			error = -ENODEV;
			break;
		}
		dev->id = i;
<<<<<<< HEAD
<<<<<<< HEAD
		dev->cls = &tx_7segled_sysdev_class;
		error = sysdev_register(dev);
		if (!error) {
			sysdev_create_file(dev, &attr_ascii);
			sysdev_create_file(dev, &attr_raw);
=======
=======
>>>>>>> refs/remotes/origin/master
		dev->bus = &tx_7segled_subsys;
		error = device_register(dev);
		if (!error) {
			device_create_file(dev, &dev_attr_ascii);
			device_create_file(dev, &dev_attr_raw);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}
	}
	return error;
}

device_initcall(tx_7segled_init_sysfs);
