/*
 * Copyright (c) 2006 Dave Airlie <airlied@linux.ie>
 * Copyright © 2006-2008,2010 Intel Corporation
 *   Jesse Barnes <jesse.barnes@intel.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *	Eric Anholt <eric@anholt.net>
 *	Chris Wilson <chris@chris-wilson.co.uk>
 */
#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include "drmP.h"
#include "drm.h"
#include "intel_drv.h"
#include "i915_drm.h"
#include "i915_drv.h"

/* Intel GPIO access functions */

<<<<<<< HEAD
#define I2C_RISEFALL_TIME 20
=======
#define I2C_RISEFALL_TIME 10
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
#include <drm/drmP.h>
#include "intel_drv.h"
#include <drm/i915_drm.h>
#include "i915_drv.h"

enum disp_clk {
	CDCLK,
	CZCLK
};

struct gmbus_port {
	const char *name;
	int reg;
};

static const struct gmbus_port gmbus_ports[] = {
	{ "ssc", GPIOB },
	{ "vga", GPIOA },
	{ "panel", GPIOC },
	{ "dpc", GPIOD },
	{ "dpb", GPIOE },
	{ "dpd", GPIOF },
};

/* Intel GPIO access functions */

#define I2C_RISEFALL_TIME 10
>>>>>>> refs/remotes/origin/master

static inline struct intel_gmbus *
to_intel_gmbus(struct i2c_adapter *i2c)
{
	return container_of(i2c, struct intel_gmbus, adapter);
}

<<<<<<< HEAD
<<<<<<< HEAD
struct intel_gpio {
	struct i2c_adapter adapter;
	struct i2c_algo_bit_data algo;
	struct drm_i915_private *dev_priv;
	u32 reg;
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int get_disp_clk_div(struct drm_i915_private *dev_priv,
			    enum disp_clk clk)
{
	u32 reg_val;
	int clk_ratio;

	reg_val = I915_READ(CZCLK_CDCLK_FREQ_RATIO);

	if (clk == CDCLK)
		clk_ratio =
			((reg_val & CDCLK_FREQ_MASK) >> CDCLK_FREQ_SHIFT) + 1;
	else
		clk_ratio = (reg_val & CZCLK_FREQ_MASK) + 1;

	return clk_ratio;
}

static void gmbus_set_freq(struct drm_i915_private *dev_priv)
{
	int vco_freq[] = { 800, 1600, 2000, 2400 };
	int gmbus_freq = 0, cdclk_div, hpll_freq;

	BUG_ON(!IS_VALLEYVIEW(dev_priv->dev));

	/* Skip setting the gmbus freq if BIOS has already programmed it */
	if (I915_READ(GMBUSFREQ_VLV) != 0xA0)
		return;

	/* Obtain SKU information */
	mutex_lock(&dev_priv->dpio_lock);
	hpll_freq =
		vlv_cck_read(dev_priv, CCK_FUSE_REG) & CCK_FUSE_HPLL_FREQ_MASK;
	mutex_unlock(&dev_priv->dpio_lock);

	/* Get the CDCLK divide ratio */
	cdclk_div = get_disp_clk_div(dev_priv, CDCLK);

	/*
	 * Program the gmbus_freq based on the cdclk frequency.
	 * BSpec erroneously claims we should aim for 4MHz, but
	 * in fact 1MHz is the correct frequency.
	 */
	if (cdclk_div)
		gmbus_freq = (vco_freq[hpll_freq] << 1) / cdclk_div;

	if (WARN_ON(gmbus_freq == 0))
		return;

	I915_WRITE(GMBUSFREQ_VLV, gmbus_freq);
}

>>>>>>> refs/remotes/origin/master
void
intel_i2c_reset(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
<<<<<<< HEAD
	if (HAS_PCH_SPLIT(dev))
		I915_WRITE(PCH_GMBUS0, 0);
	else
		I915_WRITE(GMBUS0, 0);
=======

	/*
	 * In BIOS-less system, program the correct gmbus frequency
	 * before reading edid.
	 */
	if (IS_VALLEYVIEW(dev))
		gmbus_set_freq(dev_priv);

	I915_WRITE(dev_priv->gpio_mmio_base + GMBUS0, 0);
	I915_WRITE(dev_priv->gpio_mmio_base + GMBUS4, 0);
>>>>>>> refs/remotes/origin/master
}

static void intel_i2c_quirk_set(struct drm_i915_private *dev_priv, bool enable)
{
	u32 val;

	/* When using bit bashing for I2C, this bit needs to be set to 1 */
	if (!IS_PINEVIEW(dev_priv->dev))
		return;

	val = I915_READ(DSPCLK_GATE_D);
	if (enable)
		val |= DPCUNIT_CLOCK_GATE_DISABLE;
	else
		val &= ~DPCUNIT_CLOCK_GATE_DISABLE;
	I915_WRITE(DSPCLK_GATE_D, val);
}

<<<<<<< HEAD
<<<<<<< HEAD
static u32 get_reserved(struct intel_gpio *gpio)
{
	struct drm_i915_private *dev_priv = gpio->dev_priv;
=======
static u32 get_reserved(struct intel_gmbus *bus)
{
	struct drm_i915_private *dev_priv = bus->dev_priv;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static u32 get_reserved(struct intel_gmbus *bus)
{
	struct drm_i915_private *dev_priv = bus->dev_priv;
>>>>>>> refs/remotes/origin/master
	struct drm_device *dev = dev_priv->dev;
	u32 reserved = 0;

	/* On most chips, these bits must be preserved in software. */
	if (!IS_I830(dev) && !IS_845G(dev))
<<<<<<< HEAD
<<<<<<< HEAD
		reserved = I915_READ_NOTRACE(gpio->reg) &
=======
		reserved = I915_READ_NOTRACE(bus->gpio_reg) &
>>>>>>> refs/remotes/origin/cm-10.0
=======
		reserved = I915_READ_NOTRACE(bus->gpio_reg) &
>>>>>>> refs/remotes/origin/master
					     (GPIO_DATA_PULLUP_DISABLE |
					      GPIO_CLOCK_PULLUP_DISABLE);

	return reserved;
}

static int get_clock(void *data)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct intel_gpio *gpio = data;
	struct drm_i915_private *dev_priv = gpio->dev_priv;
	u32 reserved = get_reserved(gpio);
	I915_WRITE_NOTRACE(gpio->reg, reserved | GPIO_CLOCK_DIR_MASK);
	I915_WRITE_NOTRACE(gpio->reg, reserved);
	return (I915_READ_NOTRACE(gpio->reg) & GPIO_CLOCK_VAL_IN) != 0;
=======
=======
>>>>>>> refs/remotes/origin/master
	struct intel_gmbus *bus = data;
	struct drm_i915_private *dev_priv = bus->dev_priv;
	u32 reserved = get_reserved(bus);
	I915_WRITE_NOTRACE(bus->gpio_reg, reserved | GPIO_CLOCK_DIR_MASK);
	I915_WRITE_NOTRACE(bus->gpio_reg, reserved);
	return (I915_READ_NOTRACE(bus->gpio_reg) & GPIO_CLOCK_VAL_IN) != 0;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static int get_data(void *data)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct intel_gpio *gpio = data;
	struct drm_i915_private *dev_priv = gpio->dev_priv;
	u32 reserved = get_reserved(gpio);
	I915_WRITE_NOTRACE(gpio->reg, reserved | GPIO_DATA_DIR_MASK);
	I915_WRITE_NOTRACE(gpio->reg, reserved);
	return (I915_READ_NOTRACE(gpio->reg) & GPIO_DATA_VAL_IN) != 0;
=======
=======
>>>>>>> refs/remotes/origin/master
	struct intel_gmbus *bus = data;
	struct drm_i915_private *dev_priv = bus->dev_priv;
	u32 reserved = get_reserved(bus);
	I915_WRITE_NOTRACE(bus->gpio_reg, reserved | GPIO_DATA_DIR_MASK);
	I915_WRITE_NOTRACE(bus->gpio_reg, reserved);
	return (I915_READ_NOTRACE(bus->gpio_reg) & GPIO_DATA_VAL_IN) != 0;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static void set_clock(void *data, int state_high)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct intel_gpio *gpio = data;
	struct drm_i915_private *dev_priv = gpio->dev_priv;
	u32 reserved = get_reserved(gpio);
=======
	struct intel_gmbus *bus = data;
	struct drm_i915_private *dev_priv = bus->dev_priv;
	u32 reserved = get_reserved(bus);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct intel_gmbus *bus = data;
	struct drm_i915_private *dev_priv = bus->dev_priv;
	u32 reserved = get_reserved(bus);
>>>>>>> refs/remotes/origin/master
	u32 clock_bits;

	if (state_high)
		clock_bits = GPIO_CLOCK_DIR_IN | GPIO_CLOCK_DIR_MASK;
	else
		clock_bits = GPIO_CLOCK_DIR_OUT | GPIO_CLOCK_DIR_MASK |
			GPIO_CLOCK_VAL_MASK;

<<<<<<< HEAD
<<<<<<< HEAD
	I915_WRITE_NOTRACE(gpio->reg, reserved | clock_bits);
	POSTING_READ(gpio->reg);
=======
	I915_WRITE_NOTRACE(bus->gpio_reg, reserved | clock_bits);
	POSTING_READ(bus->gpio_reg);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	I915_WRITE_NOTRACE(bus->gpio_reg, reserved | clock_bits);
	POSTING_READ(bus->gpio_reg);
>>>>>>> refs/remotes/origin/master
}

static void set_data(void *data, int state_high)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct intel_gpio *gpio = data;
	struct drm_i915_private *dev_priv = gpio->dev_priv;
	u32 reserved = get_reserved(gpio);
=======
	struct intel_gmbus *bus = data;
	struct drm_i915_private *dev_priv = bus->dev_priv;
	u32 reserved = get_reserved(bus);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct intel_gmbus *bus = data;
	struct drm_i915_private *dev_priv = bus->dev_priv;
	u32 reserved = get_reserved(bus);
>>>>>>> refs/remotes/origin/master
	u32 data_bits;

	if (state_high)
		data_bits = GPIO_DATA_DIR_IN | GPIO_DATA_DIR_MASK;
	else
		data_bits = GPIO_DATA_DIR_OUT | GPIO_DATA_DIR_MASK |
			GPIO_DATA_VAL_MASK;

<<<<<<< HEAD
<<<<<<< HEAD
	I915_WRITE_NOTRACE(gpio->reg, reserved | data_bits);
	POSTING_READ(gpio->reg);
}

static struct i2c_adapter *
intel_gpio_create(struct drm_i915_private *dev_priv, u32 pin)
{
=======
=======
>>>>>>> refs/remotes/origin/master
	I915_WRITE_NOTRACE(bus->gpio_reg, reserved | data_bits);
	POSTING_READ(bus->gpio_reg);
}

<<<<<<< HEAD
static bool
intel_gpio_setup(struct intel_gmbus *bus, u32 pin)
{
	struct drm_i915_private *dev_priv = bus->dev_priv;
>>>>>>> refs/remotes/origin/cm-10.0
	static const int map_pin_to_reg[] = {
		0,
		GPIOB,
		GPIOA,
		GPIOC,
		GPIOD,
		GPIOE,
		0,
		GPIOF,
	};
<<<<<<< HEAD
	struct intel_gpio *gpio;

	if (pin >= ARRAY_SIZE(map_pin_to_reg) || !map_pin_to_reg[pin])
		return NULL;

	gpio = kzalloc(sizeof(struct intel_gpio), GFP_KERNEL);
	if (gpio == NULL)
		return NULL;

	gpio->reg = map_pin_to_reg[pin];
	if (HAS_PCH_SPLIT(dev_priv->dev))
		gpio->reg += PCH_GPIOA - GPIOA;
	gpio->dev_priv = dev_priv;

	snprintf(gpio->adapter.name, sizeof(gpio->adapter.name),
		 "i915 GPIO%c", "?BACDE?F"[pin]);
	gpio->adapter.owner = THIS_MODULE;
	gpio->adapter.algo_data	= &gpio->algo;
	gpio->adapter.dev.parent = &dev_priv->dev->pdev->dev;
	gpio->algo.setsda = set_data;
	gpio->algo.setscl = set_clock;
	gpio->algo.getsda = get_data;
	gpio->algo.getscl = get_clock;
	gpio->algo.udelay = I2C_RISEFALL_TIME;
	gpio->algo.timeout = usecs_to_jiffies(2200);
	gpio->algo.data = gpio;

	if (i2c_bit_add_bus(&gpio->adapter))
		goto out_free;

	return &gpio->adapter;

out_free:
	kfree(gpio);
	return NULL;
}

static int
intel_i2c_quirk_xfer(struct drm_i915_private *dev_priv,
		     struct i2c_adapter *adapter,
		     struct i2c_msg *msgs,
		     int num)
{
	struct intel_gpio *gpio = container_of(adapter,
					       struct intel_gpio,
					       adapter);
=======
	struct i2c_algo_bit_data *algo;

	if (pin >= ARRAY_SIZE(map_pin_to_reg) || !map_pin_to_reg[pin])
		return false;

	algo = &bus->bit_algo;

	bus->gpio_reg = map_pin_to_reg[pin];
	if (HAS_PCH_SPLIT(dev_priv->dev))
		bus->gpio_reg += PCH_GPIOA - GPIOA;
=======
static int
intel_gpio_pre_xfer(struct i2c_adapter *adapter)
{
	struct intel_gmbus *bus = container_of(adapter,
					       struct intel_gmbus,
					       adapter);
	struct drm_i915_private *dev_priv = bus->dev_priv;

	intel_i2c_reset(dev_priv->dev);
	intel_i2c_quirk_set(dev_priv, true);
	set_data(bus, 1);
	set_clock(bus, 1);
	udelay(I2C_RISEFALL_TIME);
	return 0;
}

static void
intel_gpio_post_xfer(struct i2c_adapter *adapter)
{
	struct intel_gmbus *bus = container_of(adapter,
					       struct intel_gmbus,
					       adapter);
	struct drm_i915_private *dev_priv = bus->dev_priv;

	set_data(bus, 1);
	set_clock(bus, 1);
	intel_i2c_quirk_set(dev_priv, false);
}

static void
intel_gpio_setup(struct intel_gmbus *bus, u32 pin)
{
	struct drm_i915_private *dev_priv = bus->dev_priv;
	struct i2c_algo_bit_data *algo;

	algo = &bus->bit_algo;

	/* -1 to map pin pair to gmbus index */
	bus->gpio_reg = dev_priv->gpio_mmio_base + gmbus_ports[pin - 1].reg;
>>>>>>> refs/remotes/origin/master

	bus->adapter.algo_data = algo;
	algo->setsda = set_data;
	algo->setscl = set_clock;
	algo->getsda = get_data;
	algo->getscl = get_clock;
<<<<<<< HEAD
	algo->udelay = I2C_RISEFALL_TIME;
	algo->timeout = usecs_to_jiffies(2200);
	algo->data = bus;

	return true;
}

static int
intel_i2c_quirk_xfer(struct intel_gmbus *bus,
		     struct i2c_msg *msgs,
		     int num)
{
	struct drm_i915_private *dev_priv = bus->dev_priv;
>>>>>>> refs/remotes/origin/cm-10.0
	int ret;

	intel_i2c_reset(dev_priv->dev);

	intel_i2c_quirk_set(dev_priv, true);
<<<<<<< HEAD
	set_data(gpio, 1);
	set_clock(gpio, 1);
	udelay(I2C_RISEFALL_TIME);

	ret = adapter->algo->master_xfer(adapter, msgs, num);

	set_data(gpio, 1);
	set_clock(gpio, 1);
=======
	set_data(bus, 1);
	set_clock(bus, 1);
	udelay(I2C_RISEFALL_TIME);

	ret = i2c_bit_algo.master_xfer(&bus->adapter, msgs, num);

	set_data(bus, 1);
	set_clock(bus, 1);
>>>>>>> refs/remotes/origin/cm-10.0
	intel_i2c_quirk_set(dev_priv, false);
=======
	algo->pre_xfer = intel_gpio_pre_xfer;
	algo->post_xfer = intel_gpio_post_xfer;
	algo->udelay = I2C_RISEFALL_TIME;
	algo->timeout = usecs_to_jiffies(2200);
	algo->data = bus;
}

/*
 * gmbus on gen4 seems to be able to generate legacy interrupts even when in MSI
 * mode. This results in spurious interrupt warnings if the legacy irq no. is
 * shared with another device. The kernel then disables that interrupt source
 * and so prevents the other device from working properly.
 */
#define HAS_GMBUS_IRQ(dev) (INTEL_INFO(dev)->gen >= 5)
static int
gmbus_wait_hw_status(struct drm_i915_private *dev_priv,
		     u32 gmbus2_status,
		     u32 gmbus4_irq_en)
{
	int i;
	int reg_offset = dev_priv->gpio_mmio_base;
	u32 gmbus2 = 0;
	DEFINE_WAIT(wait);

	if (!HAS_GMBUS_IRQ(dev_priv->dev))
		gmbus4_irq_en = 0;

	/* Important: The hw handles only the first bit, so set only one! Since
	 * we also need to check for NAKs besides the hw ready/idle signal, we
	 * need to wake up periodically and check that ourselves. */
	I915_WRITE(GMBUS4 + reg_offset, gmbus4_irq_en);

	for (i = 0; i < msecs_to_jiffies_timeout(50); i++) {
		prepare_to_wait(&dev_priv->gmbus_wait_queue, &wait,
				TASK_UNINTERRUPTIBLE);

		gmbus2 = I915_READ_NOTRACE(GMBUS2 + reg_offset);
		if (gmbus2 & (GMBUS_SATOER | gmbus2_status))
			break;

		schedule_timeout(1);
	}
	finish_wait(&dev_priv->gmbus_wait_queue, &wait);

	I915_WRITE(GMBUS4 + reg_offset, 0);

	if (gmbus2 & GMBUS_SATOER)
		return -ENXIO;
	if (gmbus2 & gmbus2_status)
		return 0;
	return -ETIMEDOUT;
}

static int
gmbus_wait_idle(struct drm_i915_private *dev_priv)
{
	int ret;
	int reg_offset = dev_priv->gpio_mmio_base;

#define C ((I915_READ_NOTRACE(GMBUS2 + reg_offset) & GMBUS_ACTIVE) == 0)

	if (!HAS_GMBUS_IRQ(dev_priv->dev))
		return wait_for(C, 10);

	/* Important: The hw handles only the first bit, so set only one! */
	I915_WRITE(GMBUS4 + reg_offset, GMBUS_IDLE_EN);

	ret = wait_event_timeout(dev_priv->gmbus_wait_queue, C,
				 msecs_to_jiffies_timeout(10));

	I915_WRITE(GMBUS4 + reg_offset, 0);

	if (ret)
		return 0;
	else
		return -ETIMEDOUT;
#undef C
}

static int
gmbus_xfer_read(struct drm_i915_private *dev_priv, struct i2c_msg *msg,
		u32 gmbus1_index)
{
	int reg_offset = dev_priv->gpio_mmio_base;
	u16 len = msg->len;
	u8 *buf = msg->buf;

	I915_WRITE(GMBUS1 + reg_offset,
		   gmbus1_index |
		   GMBUS_CYCLE_WAIT |
		   (len << GMBUS_BYTE_COUNT_SHIFT) |
		   (msg->addr << GMBUS_SLAVE_ADDR_SHIFT) |
		   GMBUS_SLAVE_READ | GMBUS_SW_RDY);
	while (len) {
		int ret;
		u32 val, loop = 0;

		ret = gmbus_wait_hw_status(dev_priv, GMBUS_HW_RDY,
					   GMBUS_HW_RDY_EN);
		if (ret)
			return ret;

		val = I915_READ(GMBUS3 + reg_offset);
		do {
			*buf++ = val & 0xff;
			val >>= 8;
		} while (--len && ++loop < 4);
	}

	return 0;
}

static int
gmbus_xfer_write(struct drm_i915_private *dev_priv, struct i2c_msg *msg)
{
	int reg_offset = dev_priv->gpio_mmio_base;
	u16 len = msg->len;
	u8 *buf = msg->buf;
	u32 val, loop;

	val = loop = 0;
	while (len && loop < 4) {
		val |= *buf++ << (8 * loop++);
		len -= 1;
	}

	I915_WRITE(GMBUS3 + reg_offset, val);
	I915_WRITE(GMBUS1 + reg_offset,
		   GMBUS_CYCLE_WAIT |
		   (msg->len << GMBUS_BYTE_COUNT_SHIFT) |
		   (msg->addr << GMBUS_SLAVE_ADDR_SHIFT) |
		   GMBUS_SLAVE_WRITE | GMBUS_SW_RDY);
	while (len) {
		int ret;

		val = loop = 0;
		do {
			val |= *buf++ << (8 * loop);
		} while (--len && ++loop < 4);

		I915_WRITE(GMBUS3 + reg_offset, val);

		ret = gmbus_wait_hw_status(dev_priv, GMBUS_HW_RDY,
					   GMBUS_HW_RDY_EN);
		if (ret)
			return ret;
	}
	return 0;
}

/*
 * The gmbus controller can combine a 1 or 2 byte write with a read that
 * immediately follows it by using an "INDEX" cycle.
 */
static bool
gmbus_is_index_read(struct i2c_msg *msgs, int i, int num)
{
	return (i + 1 < num &&
		!(msgs[i].flags & I2C_M_RD) && msgs[i].len <= 2 &&
		(msgs[i + 1].flags & I2C_M_RD));
}

static int
gmbus_xfer_index_read(struct drm_i915_private *dev_priv, struct i2c_msg *msgs)
{
	int reg_offset = dev_priv->gpio_mmio_base;
	u32 gmbus1_index = 0;
	u32 gmbus5 = 0;
	int ret;

	if (msgs[0].len == 2)
		gmbus5 = GMBUS_2BYTE_INDEX_EN |
			 msgs[0].buf[1] | (msgs[0].buf[0] << 8);
	if (msgs[0].len == 1)
		gmbus1_index = GMBUS_CYCLE_INDEX |
			       (msgs[0].buf[0] << GMBUS_SLAVE_INDEX_SHIFT);

	/* GMBUS5 holds 16-bit index */
	if (gmbus5)
		I915_WRITE(GMBUS5 + reg_offset, gmbus5);

	ret = gmbus_xfer_read(dev_priv, &msgs[1], gmbus1_index);

	/* Clear GMBUS5 after each index transfer */
	if (gmbus5)
		I915_WRITE(GMBUS5 + reg_offset, 0);
>>>>>>> refs/remotes/origin/master

	return ret;
}

static int
gmbus_xfer(struct i2c_adapter *adapter,
	   struct i2c_msg *msgs,
	   int num)
{
	struct intel_gmbus *bus = container_of(adapter,
					       struct intel_gmbus,
					       adapter);
<<<<<<< HEAD
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = adapter->algo_data;
	int i, reg_offset;

	if (bus->force_bit)
		return intel_i2c_quirk_xfer(dev_priv,
					    bus->force_bit, msgs, num);
=======
	struct drm_i915_private *dev_priv = bus->dev_priv;
	int i, reg_offset, ret;

	mutex_lock(&dev_priv->gmbus_mutex);

	if (bus->force_bit) {
		ret = intel_i2c_quirk_xfer(bus, msgs, num);
		goto out;
	}
>>>>>>> refs/remotes/origin/cm-10.0

	reg_offset = HAS_PCH_SPLIT(dev_priv->dev) ? PCH_GMBUS0 - GMBUS0 : 0;
=======
	struct drm_i915_private *dev_priv = bus->dev_priv;
	int i, reg_offset;
	int ret = 0;

	intel_aux_display_runtime_get(dev_priv);
	mutex_lock(&dev_priv->gmbus_mutex);

	if (bus->force_bit) {
		ret = i2c_bit_algo.master_xfer(adapter, msgs, num);
		goto out;
	}

	reg_offset = dev_priv->gpio_mmio_base;
>>>>>>> refs/remotes/origin/master

	I915_WRITE(GMBUS0 + reg_offset, bus->reg0);

	for (i = 0; i < num; i++) {
<<<<<<< HEAD
		u16 len = msgs[i].len;
		u8 *buf = msgs[i].buf;

		if (msgs[i].flags & I2C_M_RD) {
			I915_WRITE(GMBUS1 + reg_offset,
<<<<<<< HEAD
				   GMBUS_CYCLE_WAIT | (i + 1 == num ? GMBUS_CYCLE_STOP : 0) |
=======
				   GMBUS_CYCLE_WAIT |
				   (i + 1 == num ? GMBUS_CYCLE_STOP : 0) |
>>>>>>> refs/remotes/origin/cm-10.0
				   (len << GMBUS_BYTE_COUNT_SHIFT) |
				   (msgs[i].addr << GMBUS_SLAVE_ADDR_SHIFT) |
				   GMBUS_SLAVE_READ | GMBUS_SW_RDY);
			POSTING_READ(GMBUS2+reg_offset);
			do {
				u32 val, loop = 0;

				if (wait_for(I915_READ(GMBUS2 + reg_offset) & (GMBUS_SATOER | GMBUS_HW_RDY), 50))
					goto timeout;
				if (I915_READ(GMBUS2 + reg_offset) & GMBUS_SATOER)
					goto clear_err;

				val = I915_READ(GMBUS3 + reg_offset);
				do {
					*buf++ = val & 0xff;
					val >>= 8;
				} while (--len && ++loop < 4);
			} while (len);
		} else {
			u32 val, loop;

			val = loop = 0;
			do {
				val |= *buf++ << (8 * loop);
			} while (--len && ++loop < 4);

			I915_WRITE(GMBUS3 + reg_offset, val);
			I915_WRITE(GMBUS1 + reg_offset,
<<<<<<< HEAD
				   (i + 1 == num ? GMBUS_CYCLE_STOP : GMBUS_CYCLE_WAIT) |
=======
				   GMBUS_CYCLE_WAIT |
				   (i + 1 == num ? GMBUS_CYCLE_STOP : 0) |
>>>>>>> refs/remotes/origin/cm-10.0
				   (msgs[i].len << GMBUS_BYTE_COUNT_SHIFT) |
				   (msgs[i].addr << GMBUS_SLAVE_ADDR_SHIFT) |
				   GMBUS_SLAVE_WRITE | GMBUS_SW_RDY);
			POSTING_READ(GMBUS2+reg_offset);

			while (len) {
				if (wait_for(I915_READ(GMBUS2 + reg_offset) & (GMBUS_SATOER | GMBUS_HW_RDY), 50))
					goto timeout;
				if (I915_READ(GMBUS2 + reg_offset) & GMBUS_SATOER)
					goto clear_err;

				val = loop = 0;
				do {
					val |= *buf++ << (8 * loop);
				} while (--len && ++loop < 4);

				I915_WRITE(GMBUS3 + reg_offset, val);
				POSTING_READ(GMBUS2+reg_offset);
			}
		}

		if (i + 1 < num && wait_for(I915_READ(GMBUS2 + reg_offset) & (GMBUS_SATOER | GMBUS_HW_WAIT_PHASE), 50))
			goto timeout;
		if (I915_READ(GMBUS2 + reg_offset) & GMBUS_SATOER)
			goto clear_err;
	}

	goto done;

clear_err:
=======
		if (gmbus_is_index_read(msgs, i, num)) {
			ret = gmbus_xfer_index_read(dev_priv, &msgs[i]);
			i += 1;  /* set i to the index of the read xfer */
		} else if (msgs[i].flags & I2C_M_RD) {
			ret = gmbus_xfer_read(dev_priv, &msgs[i], 0);
		} else {
			ret = gmbus_xfer_write(dev_priv, &msgs[i]);
		}

		if (ret == -ETIMEDOUT)
			goto timeout;
		if (ret == -ENXIO)
			goto clear_err;

		ret = gmbus_wait_hw_status(dev_priv, GMBUS_HW_WAIT_PHASE,
					   GMBUS_HW_WAIT_EN);
		if (ret == -ENXIO)
			goto clear_err;
		if (ret)
			goto timeout;
	}

	/* Generate a STOP condition on the bus. Note that gmbus can't generata
	 * a STOP on the very first cycle. To simplify the code we
	 * unconditionally generate the STOP condition with an additional gmbus
	 * cycle. */
	I915_WRITE(GMBUS1 + reg_offset, GMBUS_CYCLE_STOP | GMBUS_SW_RDY);

	/* Mark the GMBUS interface as disabled after waiting for idle.
	 * We will re-enable it at the start of the next xfer,
	 * till then let it sleep.
	 */
	if (gmbus_wait_idle(dev_priv)) {
		DRM_DEBUG_KMS("GMBUS [%s] timed out waiting for idle\n",
			 adapter->name);
		ret = -ETIMEDOUT;
	}
	I915_WRITE(GMBUS0 + reg_offset, 0);
	ret = ret ?: i;
	goto out;

clear_err:
	/*
	 * Wait for bus to IDLE before clearing NAK.
	 * If we clear the NAK while bus is still active, then it will stay
	 * active and the next transaction may fail.
	 *
	 * If no ACK is received during the address phase of a transaction, the
	 * adapter must report -ENXIO. It is not clear what to return if no ACK
	 * is received at other times. But we have to be careful to not return
	 * spurious -ENXIO because that will prevent i2c and drm edid functions
	 * from retrying. So return -ENXIO only when gmbus properly quiescents -
	 * timing out seems to happen when there _is_ a ddc chip present, but
	 * it's slow responding and only answers on the 2nd retry.
	 */
	ret = -ENXIO;
	if (gmbus_wait_idle(dev_priv)) {
		DRM_DEBUG_KMS("GMBUS [%s] timed out after NAK\n",
			      adapter->name);
		ret = -ETIMEDOUT;
	}

>>>>>>> refs/remotes/origin/master
	/* Toggle the Software Clear Interrupt bit. This has the effect
	 * of resetting the GMBUS controller and so clearing the
	 * BUS_ERROR raised by the slave's NAK.
	 */
	I915_WRITE(GMBUS1 + reg_offset, GMBUS_SW_CLR_INT);
	I915_WRITE(GMBUS1 + reg_offset, 0);
<<<<<<< HEAD

done:
<<<<<<< HEAD
	/* Mark the GMBUS interface as disabled. We will re-enable it at the
	 * start of the next xfer, till then let it sleep.
	 */
	I915_WRITE(GMBUS0 + reg_offset, 0);
	return i;
=======
	/* Mark the GMBUS interface as disabled after waiting for idle.
	 * We will re-enable it at the start of the next xfer,
	 * till then let it sleep.
	 */
	if (wait_for((I915_READ(GMBUS2 + reg_offset) & GMBUS_ACTIVE) == 0, 10))
		DRM_INFO("GMBUS timed out waiting for idle\n");
	I915_WRITE(GMBUS0 + reg_offset, 0);
	ret = i;
	goto out;
>>>>>>> refs/remotes/origin/cm-10.0

timeout:
	DRM_INFO("GMBUS timed out, falling back to bit banging on pin %d [%s]\n",
		 bus->reg0 & 0xff, bus->adapter.name);
	I915_WRITE(GMBUS0 + reg_offset, 0);

	/* Hardware may not support GMBUS over these pins? Try GPIO bitbanging instead. */
<<<<<<< HEAD
	bus->force_bit = intel_gpio_create(dev_priv, bus->reg0 & 0xff);
	if (!bus->force_bit)
		return -ENOMEM;

	return intel_i2c_quirk_xfer(dev_priv, bus->force_bit, msgs, num);
=======
	if (!bus->has_gpio) {
		ret = -EIO;
	} else {
		bus->force_bit = true;
		ret = intel_i2c_quirk_xfer(bus, msgs, num);
	}
out:
	mutex_unlock(&dev_priv->gmbus_mutex);
	return ret;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	I915_WRITE(GMBUS0 + reg_offset, 0);

	DRM_DEBUG_KMS("GMBUS [%s] NAK for addr: %04x %c(%d)\n",
			 adapter->name, msgs[i].addr,
			 (msgs[i].flags & I2C_M_RD) ? 'r' : 'w', msgs[i].len);

	goto out;

timeout:
	DRM_INFO("GMBUS [%s] timed out, falling back to bit banging on pin %d\n",
		 bus->adapter.name, bus->reg0 & 0xff);
	I915_WRITE(GMBUS0 + reg_offset, 0);

	/* Hardware may not support GMBUS over these pins? Try GPIO bitbanging instead. */
	bus->force_bit = 1;
	ret = i2c_bit_algo.master_xfer(adapter, msgs, num);

out:
	mutex_unlock(&dev_priv->gmbus_mutex);
	intel_aux_display_runtime_put(dev_priv);
	return ret;
>>>>>>> refs/remotes/origin/master
}

static u32 gmbus_func(struct i2c_adapter *adapter)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct intel_gmbus *bus = container_of(adapter,
					       struct intel_gmbus,
					       adapter);

	if (bus->force_bit)
		bus->force_bit->algo->functionality(bus->force_bit);

	return (I2C_FUNC_I2C | I2C_FUNC_SMBUS_EMUL |
=======
	return i2c_bit_algo.functionality(adapter) &
		(I2C_FUNC_I2C | I2C_FUNC_SMBUS_EMUL |
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return i2c_bit_algo.functionality(adapter) &
		(I2C_FUNC_I2C | I2C_FUNC_SMBUS_EMUL |
>>>>>>> refs/remotes/origin/master
		/* I2C_FUNC_10BIT_ADDR | */
		I2C_FUNC_SMBUS_READ_BLOCK_DATA |
		I2C_FUNC_SMBUS_BLOCK_PROC_CALL);
}

static const struct i2c_algorithm gmbus_algorithm = {
	.master_xfer	= gmbus_xfer,
	.functionality	= gmbus_func
};

/**
 * intel_gmbus_setup - instantiate all Intel i2c GMBuses
 * @dev: DRM device
 */
int intel_setup_gmbus(struct drm_device *dev)
{
<<<<<<< HEAD
	static const char *names[GMBUS_NUM_PORTS] = {
		"disabled",
		"ssc",
		"vga",
		"panel",
		"dpc",
		"dpb",
		"reserved",
		"dpd",
	};
	struct drm_i915_private *dev_priv = dev->dev_private;
	int ret, i;

<<<<<<< HEAD
	dev_priv->gmbus = kcalloc(sizeof(struct intel_gmbus), GMBUS_NUM_PORTS,
=======
	dev_priv->gmbus = kcalloc(GMBUS_NUM_PORTS, sizeof(struct intel_gmbus),
>>>>>>> refs/remotes/origin/cm-10.0
				  GFP_KERNEL);
	if (dev_priv->gmbus == NULL)
		return -ENOMEM;

<<<<<<< HEAD
=======
	mutex_init(&dev_priv->gmbus_mutex);

>>>>>>> refs/remotes/origin/cm-10.0
	for (i = 0; i < GMBUS_NUM_PORTS; i++) {
		struct intel_gmbus *bus = &dev_priv->gmbus[i];
=======
	struct drm_i915_private *dev_priv = dev->dev_private;
	int ret, i;

	if (HAS_PCH_NOP(dev))
		return 0;
	else if (HAS_PCH_SPLIT(dev))
		dev_priv->gpio_mmio_base = PCH_GPIOA - GPIOA;
	else if (IS_VALLEYVIEW(dev))
		dev_priv->gpio_mmio_base = VLV_DISPLAY_BASE;
	else
		dev_priv->gpio_mmio_base = 0;

	mutex_init(&dev_priv->gmbus_mutex);
	init_waitqueue_head(&dev_priv->gmbus_wait_queue);

	for (i = 0; i < GMBUS_NUM_PORTS; i++) {
		struct intel_gmbus *bus = &dev_priv->gmbus[i];
		u32 port = i + 1; /* +1 to map gmbus index to pin pair */
>>>>>>> refs/remotes/origin/master

		bus->adapter.owner = THIS_MODULE;
		bus->adapter.class = I2C_CLASS_DDC;
		snprintf(bus->adapter.name,
			 sizeof(bus->adapter.name),
			 "i915 gmbus %s",
<<<<<<< HEAD
			 names[i]);

		bus->adapter.dev.parent = &dev->pdev->dev;
<<<<<<< HEAD
		bus->adapter.algo_data	= dev_priv;
=======
		bus->dev_priv = dev_priv;
>>>>>>> refs/remotes/origin/cm-10.0

		bus->adapter.algo = &gmbus_algorithm;
		ret = i2c_add_adapter(&bus->adapter);
		if (ret)
			goto err;

		/* By default use a conservative clock rate */
		bus->reg0 = i | GMBUS_RATE_100KHZ;

<<<<<<< HEAD
		/* XXX force bit banging until GMBUS is fully debugged */
		bus->force_bit = intel_gpio_create(dev_priv, i);
=======
		bus->has_gpio = intel_gpio_setup(bus, i);

		/* XXX force bit banging until GMBUS is fully debugged */
		if (bus->has_gpio)
			bus->force_bit = true;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			 gmbus_ports[i].name);

		bus->adapter.dev.parent = &dev->pdev->dev;
		bus->dev_priv = dev_priv;

		bus->adapter.algo = &gmbus_algorithm;

		/* By default use a conservative clock rate */
		bus->reg0 = port | GMBUS_RATE_100KHZ;

		/* gmbus seems to be broken on i830 */
		if (IS_I830(dev))
			bus->force_bit = 1;

		intel_gpio_setup(bus, port);

		ret = i2c_add_adapter(&bus->adapter);
		if (ret)
			goto err;
>>>>>>> refs/remotes/origin/master
	}

	intel_i2c_reset(dev_priv->dev);

	return 0;

err:
	while (--i) {
		struct intel_gmbus *bus = &dev_priv->gmbus[i];
		i2c_del_adapter(&bus->adapter);
	}
<<<<<<< HEAD
	kfree(dev_priv->gmbus);
	dev_priv->gmbus = NULL;
	return ret;
}

=======
	return ret;
}

struct i2c_adapter *intel_gmbus_get_adapter(struct drm_i915_private *dev_priv,
					    unsigned port)
{
	WARN_ON(!intel_gmbus_is_port_valid(port));
	/* -1 to map pin pair to gmbus index */
	return (intel_gmbus_is_port_valid(port)) ?
		&dev_priv->gmbus[port - 1].adapter : NULL;
}

>>>>>>> refs/remotes/origin/master
void intel_gmbus_set_speed(struct i2c_adapter *adapter, int speed)
{
	struct intel_gmbus *bus = to_intel_gmbus(adapter);

<<<<<<< HEAD
<<<<<<< HEAD
	/* speed:
	 * 0x0 = 100 KHz
	 * 0x1 = 50 KHz
	 * 0x2 = 400 KHz
	 * 0x3 = 1000 Khz
	 */
	bus->reg0 = (bus->reg0 & ~(0x3 << 8)) | (speed << 8);
=======
	bus->reg0 = (bus->reg0 & ~(0x3 << 8)) | speed;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bus->reg0 = (bus->reg0 & ~(0x3 << 8)) | speed;
>>>>>>> refs/remotes/origin/master
}

void intel_gmbus_force_bit(struct i2c_adapter *adapter, bool force_bit)
{
	struct intel_gmbus *bus = to_intel_gmbus(adapter);

<<<<<<< HEAD
<<<<<<< HEAD
	if (force_bit) {
		if (bus->force_bit == NULL) {
			struct drm_i915_private *dev_priv = adapter->algo_data;
			bus->force_bit = intel_gpio_create(dev_priv,
							   bus->reg0 & 0xff);
		}
	} else {
		if (bus->force_bit) {
			i2c_del_adapter(bus->force_bit);
			kfree(bus->force_bit);
			bus->force_bit = NULL;
		}
	}
=======
	if (bus->has_gpio)
		bus->force_bit = force_bit;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bus->force_bit += force_bit ? 1 : -1;
	DRM_DEBUG_KMS("%sabling bit-banging on %s. force bit now %d\n",
		      force_bit ? "en" : "dis", adapter->name,
		      bus->force_bit);
>>>>>>> refs/remotes/origin/master
}

void intel_teardown_gmbus(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	int i;

<<<<<<< HEAD
	if (dev_priv->gmbus == NULL)
		return;

	for (i = 0; i < GMBUS_NUM_PORTS; i++) {
		struct intel_gmbus *bus = &dev_priv->gmbus[i];
<<<<<<< HEAD
		if (bus->force_bit) {
			i2c_del_adapter(bus->force_bit);
			kfree(bus->force_bit);
		}
=======
>>>>>>> refs/remotes/origin/cm-10.0
		i2c_del_adapter(&bus->adapter);
	}

	kfree(dev_priv->gmbus);
	dev_priv->gmbus = NULL;
=======
	for (i = 0; i < GMBUS_NUM_PORTS; i++) {
		struct intel_gmbus *bus = &dev_priv->gmbus[i];
		i2c_del_adapter(&bus->adapter);
	}
>>>>>>> refs/remotes/origin/master
}
