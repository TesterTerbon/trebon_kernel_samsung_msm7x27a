/**
 * Copyright (c) 2011 Jonathan Cameron
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * Buffer handling elements of industrial I/O reference driver.
 * Uses the kfifo buffer.
 *
 * To test without hardware use the sysfs trigger.
 */

#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/bitmap.h>

<<<<<<< HEAD
#include "iio.h"
#include "trigger_consumer.h"
#include "kfifo_buf.h"
=======
#include <linux/iio/iio.h>
#include <linux/iio/trigger_consumer.h>
#include <linux/iio/kfifo_buf.h>
>>>>>>> refs/remotes/origin/master

#include "iio_simple_dummy.h"

/* Some fake data */

static const s16 fakedata[] = {
	[voltage0] = 7,
	[diffvoltage1m2] = -33,
	[diffvoltage3m4] = -2,
	[accelx] = 344,
};
/**
 * iio_simple_dummy_trigger_h() - the trigger handler function
 * @irq: the interrupt number
 * @p: private data - always a pointer to the poll func.
 *
<<<<<<< HEAD
 * This is the guts of buffered capture. On a trigger event occuring,
=======
 * This is the guts of buffered capture. On a trigger event occurring,
>>>>>>> refs/remotes/origin/master
 * if the pollfunc is attached then this handler is called as a threaded
 * interrupt (and hence may sleep). It is responsible for grabbing data
 * from the device and pushing it into the associated buffer.
 */
static irqreturn_t iio_simple_dummy_trigger_h(int irq, void *p)
{
	struct iio_poll_func *pf = p;
	struct iio_dev *indio_dev = pf->indio_dev;
<<<<<<< HEAD
	struct iio_buffer *buffer = indio_dev->buffer;
	int len = 0;
	/*
	 * The datasize is obtained from the buffer. It was stored when
	 * the preenable setup function was called.
	 */
	size_t datasize = buffer->access->get_bytes_per_datum(buffer);
	u16 *data = kmalloc(datasize, GFP_KERNEL);
	if (data == NULL)
		return -ENOMEM;
=======
	int len = 0;
	u16 *data;

	data = kmalloc(indio_dev->scan_bytes, GFP_KERNEL);
	if (data == NULL)
		goto done;
>>>>>>> refs/remotes/origin/master

	if (!bitmap_empty(indio_dev->active_scan_mask, indio_dev->masklength)) {
		/*
		 * Three common options here:
		 * hardware scans: certain combinations of channels make
		 *   up a fast read.  The capture will consist of all of them.
		 *   Hence we just call the grab data function and fill the
		 *   buffer without processing.
<<<<<<< HEAD
		 * sofware scans: can be considered to be random access
=======
		 * software scans: can be considered to be random access
>>>>>>> refs/remotes/origin/master
		 *   so efficient reading is just a case of minimal bus
		 *   transactions.
		 * software culled hardware scans:
		 *   occasionally a driver may process the nearest hardware
		 *   scan to avoid storing elements that are not desired. This
<<<<<<< HEAD
		 *   is the fidliest option by far.
		 * Here lets pretend we have random access. And the values are
=======
		 *   is the fiddliest option by far.
		 * Here let's pretend we have random access. And the values are
>>>>>>> refs/remotes/origin/master
		 * in the constant table fakedata.
		 */
		int i, j;
		for (i = 0, j = 0;
		     i < bitmap_weight(indio_dev->active_scan_mask,
				       indio_dev->masklength);
<<<<<<< HEAD
		     i++) {
			j = find_next_bit(buffer->scan_mask,
					  indio_dev->masklength, j + 1);
			/* random access read form the 'device' */
=======
		     i++, j++) {
			j = find_next_bit(indio_dev->active_scan_mask,
					  indio_dev->masklength, j);
			/* random access read from the 'device' */
>>>>>>> refs/remotes/origin/master
			data[i] = fakedata[j];
			len += 2;
		}
	}
<<<<<<< HEAD
	/* Store a timestampe at an 8 byte boundary */
	if (buffer->scan_timestamp)
		*(s64 *)(((phys_addr_t)data + len
				+ sizeof(s64) - 1) & ~(sizeof(s64) - 1))
			= iio_get_time_ns();
	buffer->access->store_to(buffer, (u8 *)data, pf->timestamp);

	kfree(data);

=======

	iio_push_to_buffers_with_timestamp(indio_dev, data, iio_get_time_ns());

	kfree(data);

done:
>>>>>>> refs/remotes/origin/master
	/*
	 * Tell the core we are done with this trigger and ready for the
	 * next one.
	 */
	iio_trigger_notify_done(indio_dev->trig);

	return IRQ_HANDLED;
}

static const struct iio_buffer_setup_ops iio_simple_dummy_buffer_setup_ops = {
	/*
<<<<<<< HEAD
	 * iio_sw_buffer_preenable:
	 * Generic function for equal sized ring elements + 64 bit timestamp
	 * Assumes that any combination of channels can be enabled.
	 * Typically replaced to implement restrictions on what combinations
	 * can be captured (hardware scan modes).
	 */
	.preenable = &iio_sw_buffer_preenable,
	/*
=======
>>>>>>> refs/remotes/origin/master
	 * iio_triggered_buffer_postenable:
	 * Generic function that simply attaches the pollfunc to the trigger.
	 * Replace this to mess with hardware state before we attach the
	 * trigger.
	 */
	.postenable = &iio_triggered_buffer_postenable,
	/*
	 * iio_triggered_buffer_predisable:
	 * Generic function that simple detaches the pollfunc from the trigger.
	 * Replace this to put hardware state back again after the trigger is
	 * detached but before userspace knows we have disabled the ring.
	 */
	.predisable = &iio_triggered_buffer_predisable,
};

<<<<<<< HEAD
int iio_simple_dummy_configure_buffer(struct iio_dev *indio_dev)
=======
int iio_simple_dummy_configure_buffer(struct iio_dev *indio_dev,
	const struct iio_chan_spec *channels, unsigned int num_channels)
>>>>>>> refs/remotes/origin/master
{
	int ret;
	struct iio_buffer *buffer;

	/* Allocate a buffer to use - here a kfifo */
	buffer = iio_kfifo_allocate(indio_dev);
	if (buffer == NULL) {
		ret = -ENOMEM;
		goto error_ret;
	}

<<<<<<< HEAD
	indio_dev->buffer = buffer;
=======
	iio_device_attach_buffer(indio_dev, buffer);
>>>>>>> refs/remotes/origin/master

	/* Enable timestamps by default */
	buffer->scan_timestamp = true;

	/*
	 * Tell the core what device type specific functions should
	 * be run on either side of buffer capture enable / disable.
	 */
	indio_dev->setup_ops = &iio_simple_dummy_buffer_setup_ops;

	/*
	 * Configure a polling function.
	 * When a trigger event with this polling function connected
	 * occurs, this function is run. Typically this grabs data
	 * from the device.
	 *
<<<<<<< HEAD
	 * NULL for the top half. This is normally implemented only if we
=======
	 * NULL for the bottom half. This is normally implemented only if we
>>>>>>> refs/remotes/origin/master
	 * either want to ping a capture now pin (no sleeping) or grab
	 * a timestamp as close as possible to a data ready trigger firing.
	 *
	 * IRQF_ONESHOT ensures irqs are masked such that only one instance
	 * of the handler can run at a time.
	 *
	 * "iio_simple_dummy_consumer%d" formatting string for the irq 'name'
	 * as seen under /proc/interrupts. Remaining parameters as per printk.
	 */
	indio_dev->pollfunc = iio_alloc_pollfunc(NULL,
						 &iio_simple_dummy_trigger_h,
						 IRQF_ONESHOT,
						 indio_dev,
						 "iio_simple_dummy_consumer%d",
						 indio_dev->id);

	if (indio_dev->pollfunc == NULL) {
		ret = -ENOMEM;
		goto error_free_buffer;
	}

	/*
	 * Notify the core that this device is capable of buffered capture
	 * driven by a trigger.
	 */
	indio_dev->modes |= INDIO_BUFFER_TRIGGERED;
<<<<<<< HEAD
	return 0;

=======

	ret = iio_buffer_register(indio_dev, channels, num_channels);
	if (ret)
		goto error_dealloc_pollfunc;

	return 0;

error_dealloc_pollfunc:
	iio_dealloc_pollfunc(indio_dev->pollfunc);
>>>>>>> refs/remotes/origin/master
error_free_buffer:
	iio_kfifo_free(indio_dev->buffer);
error_ret:
	return ret;

}

/**
 * iio_simple_dummy_unconfigure_buffer() - release buffer resources
 * @indo_dev: device instance state
 */
void iio_simple_dummy_unconfigure_buffer(struct iio_dev *indio_dev)
{
<<<<<<< HEAD
=======
	iio_buffer_unregister(indio_dev);
>>>>>>> refs/remotes/origin/master
	iio_dealloc_pollfunc(indio_dev->pollfunc);
	iio_kfifo_free(indio_dev->buffer);
}
