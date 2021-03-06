/**
 * \file drm_fops.c
 * File operations for DRM
 *
 * \author Rickard E. (Rik) Faith <faith@valinux.com>
 * \author Daryll Strauss <daryll@valinux.com>
 * \author Gareth Hughes <gareth@valinux.com>
 */

/*
 * Created: Mon Jan  4 08:58:31 1999 by faith@valinux.com
 *
 * Copyright 1999 Precision Insight, Inc., Cedar Park, Texas.
 * Copyright 2000 VA Linux Systems, Inc., Sunnyvale, California.
 * All Rights Reserved.
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
 * VA LINUX SYSTEMS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

<<<<<<< HEAD
#include "drmP.h"
#include <linux/poll.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <drm/drmP.h>
#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/module.h>
>>>>>>> refs/remotes/origin/master

/* from BKL pushdown: note that nothing else serializes idr_find() */
DEFINE_MUTEX(drm_global_mutex);
EXPORT_SYMBOL(drm_global_mutex);

static int drm_open_helper(struct inode *inode, struct file *filp,
			   struct drm_device * dev);

static int drm_setup(struct drm_device * dev)
{
<<<<<<< HEAD
	int i;
	int ret;

	if (dev->driver->firstopen) {
=======
	int ret;

	if (dev->driver->firstopen &&
	    !drm_core_check_feature(dev, DRIVER_MODESET)) {
>>>>>>> refs/remotes/origin/master
		ret = dev->driver->firstopen(dev);
		if (ret != 0)
			return ret;
	}

<<<<<<< HEAD
	atomic_set(&dev->ioctl_count, 0);
	atomic_set(&dev->vma_count, 0);

	if (drm_core_check_feature(dev, DRIVER_HAVE_DMA) &&
	    !drm_core_check_feature(dev, DRIVER_MODESET)) {
		dev->buf_use = 0;
		atomic_set(&dev->buf_alloc, 0);

		i = drm_dma_setup(dev);
		if (i < 0)
			return i;
	}

	for (i = 0; i < ARRAY_SIZE(dev->counts); i++)
		atomic_set(&dev->counts[i], 0);

	dev->sigdata.lock = NULL;

	dev->queue_count = 0;
	dev->queue_reserved = 0;
	dev->queue_slots = 0;
	dev->queuelist = NULL;
	dev->context_flag = 0;
	dev->interrupt_flag = 0;
	dev->dma_flag = 0;
	dev->last_context = 0;
	dev->last_switch = 0;
	dev->last_checked = 0;
	init_waitqueue_head(&dev->context_wait);
	dev->if_version = 0;

	dev->ctx_start = 0;
	dev->lck_start = 0;

	dev->buf_async = NULL;
	init_waitqueue_head(&dev->buf_readers);
	init_waitqueue_head(&dev->buf_writers);

	DRM_DEBUG("\n");

	/*
	 * The kernel's context could be created here, but is now created
	 * in drm_dma_enqueue.  This is more resource-efficient for
	 * hardware that does not do DMA, but may mean that
	 * drm_select_queue fails between the time the interrupt is
	 * initialized and the time the queues are initialized.
	 */

=======
	ret = drm_legacy_dma_setup(dev);
	if (ret < 0)
		return ret;


	DRM_DEBUG("\n");
>>>>>>> refs/remotes/origin/master
	return 0;
}

/**
 * Open file.
 *
 * \param inode device inode
 * \param filp file pointer.
 * \return zero on success or a negative number on failure.
 *
 * Searches the DRM device with the same minor number, calls open_helper(), and
 * increments the device open count. If the open count was previous at zero,
 * i.e., it's the first that the device is open, then calls setup().
 */
int drm_open(struct inode *inode, struct file *filp)
{
	struct drm_device *dev = NULL;
	int minor_id = iminor(inode);
	struct drm_minor *minor;
	int retcode = 0;
<<<<<<< HEAD
=======
	int need_setup = 0;
	struct address_space *old_mapping;
	struct address_space *old_imapping;
>>>>>>> refs/remotes/origin/master

	minor = idr_find(&drm_minors_idr, minor_id);
	if (!minor)
		return -ENODEV;

	if (!(dev = minor->dev))
		return -ENODEV;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (drm_device_is_unplugged(dev))
		return -ENODEV;

>>>>>>> refs/remotes/origin/cm-10.0
	retcode = drm_open_helper(inode, filp, dev);
	if (!retcode) {
		atomic_inc(&dev->counts[_DRM_STAT_OPENS]);
		if (!dev->open_count++) {
			retcode = drm_setup(dev);
			if (retcode)
				dev->open_count--;
		}
	}
	if (!retcode) {
		mutex_lock(&dev->struct_mutex);
		if (minor->type == DRM_MINOR_LEGACY) {
			if (dev->dev_mapping == NULL)
				dev->dev_mapping = inode->i_mapping;
			else if (dev->dev_mapping != inode->i_mapping)
				retcode = -ENODEV;
		}
		mutex_unlock(&dev->struct_mutex);
	}

=======
	if (drm_device_is_unplugged(dev))
		return -ENODEV;

	if (!dev->open_count++)
		need_setup = 1;
	mutex_lock(&dev->struct_mutex);
	old_imapping = inode->i_mapping;
	old_mapping = dev->dev_mapping;
	if (old_mapping == NULL)
		dev->dev_mapping = &inode->i_data;
	/* ihold ensures nobody can remove inode with our i_data */
	ihold(container_of(dev->dev_mapping, struct inode, i_data));
	inode->i_mapping = dev->dev_mapping;
	filp->f_mapping = dev->dev_mapping;
	mutex_unlock(&dev->struct_mutex);

	retcode = drm_open_helper(inode, filp, dev);
	if (retcode)
		goto err_undo;
	if (need_setup) {
		retcode = drm_setup(dev);
		if (retcode)
			goto err_undo;
	}
	return 0;

err_undo:
	mutex_lock(&dev->struct_mutex);
	filp->f_mapping = old_imapping;
	inode->i_mapping = old_imapping;
	iput(container_of(dev->dev_mapping, struct inode, i_data));
	dev->dev_mapping = old_mapping;
	mutex_unlock(&dev->struct_mutex);
	dev->open_count--;
>>>>>>> refs/remotes/origin/master
	return retcode;
}
EXPORT_SYMBOL(drm_open);

/**
 * File \c open operation.
 *
 * \param inode device inode.
 * \param filp file pointer.
 *
 * Puts the dev->fops corresponding to the device minor number into
 * \p filp, call the \c open method, and restore the file operations.
 */
int drm_stub_open(struct inode *inode, struct file *filp)
{
	struct drm_device *dev = NULL;
	struct drm_minor *minor;
	int minor_id = iminor(inode);
	int err = -ENODEV;
<<<<<<< HEAD
	const struct file_operations *old_fops;
=======
	const struct file_operations *new_fops;
>>>>>>> refs/remotes/origin/master

	DRM_DEBUG("\n");

	mutex_lock(&drm_global_mutex);
	minor = idr_find(&drm_minors_idr, minor_id);
	if (!minor)
		goto out;

	if (!(dev = minor->dev))
		goto out;

<<<<<<< HEAD
<<<<<<< HEAD
	old_fops = filp->f_op;
	filp->f_op = fops_get(&dev->driver->fops);
=======
	if (drm_device_is_unplugged(dev))
		goto out;

	old_fops = filp->f_op;
	filp->f_op = fops_get(dev->driver->fops);
>>>>>>> refs/remotes/origin/cm-10.0
	if (filp->f_op == NULL) {
		filp->f_op = old_fops;
		goto out;
	}
	if (filp->f_op->open && (err = filp->f_op->open(inode, filp))) {
		fops_put(filp->f_op);
		filp->f_op = fops_get(old_fops);
	}
	fops_put(old_fops);

=======
	if (drm_device_is_unplugged(dev))
		goto out;

	new_fops = fops_get(dev->driver->fops);
	if (!new_fops)
		goto out;

	replace_fops(filp, new_fops);
	if (filp->f_op->open)
		err = filp->f_op->open(inode, filp);
>>>>>>> refs/remotes/origin/master
out:
	mutex_unlock(&drm_global_mutex);
	return err;
}

/**
 * Check whether DRI will run on this CPU.
 *
 * \return non-zero if the DRI will run on this CPU, or zero otherwise.
 */
static int drm_cpu_valid(void)
{
#if defined(__i386__)
	if (boot_cpu_data.x86 == 3)
		return 0;	/* No cmpxchg on a 386 */
#endif
#if defined(__sparc__) && !defined(__sparc_v9__)
	return 0;		/* No cmpxchg before v9 sparc. */
#endif
	return 1;
}

/**
 * Called whenever a process opens /dev/drm.
 *
 * \param inode device inode.
 * \param filp file pointer.
 * \param dev device.
 * \return zero on success or a negative number on failure.
 *
 * Creates and initializes a drm_file structure for the file private data in \p
 * filp and add it into the double linked list in \p dev.
 */
static int drm_open_helper(struct inode *inode, struct file *filp,
			   struct drm_device * dev)
{
	int minor_id = iminor(inode);
	struct drm_file *priv;
	int ret;

	if (filp->f_flags & O_EXCL)
		return -EBUSY;	/* No exclusive opens */
	if (!drm_cpu_valid())
		return -EINVAL;
<<<<<<< HEAD
	if (dev->switch_power_state != DRM_SWITCH_POWER_ON)
=======
	if (dev->switch_power_state != DRM_SWITCH_POWER_ON && dev->switch_power_state != DRM_SWITCH_POWER_DYNAMIC_OFF)
>>>>>>> refs/remotes/origin/master
		return -EINVAL;

	DRM_DEBUG("pid = %d, minor = %d\n", task_pid_nr(current), minor_id);

	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	filp->private_data = priv;
	priv->filp = filp;
	priv->uid = current_euid();
<<<<<<< HEAD
	priv->pid = task_pid_nr(current);
	priv->minor = idr_find(&drm_minors_idr, minor_id);
	priv->ioctl_count = 0;
	/* for compatibility root is always authenticated */
	priv->authenticated = capable(CAP_SYS_ADMIN);
=======
	priv->pid = get_pid(task_pid(current));
	priv->minor = idr_find(&drm_minors_idr, minor_id);
	if (!priv->minor) {
		ret = -ENODEV;
		goto out_put_pid;
	}

	priv->ioctl_count = 0;
	/* for compatibility root is always authenticated */
	priv->always_authenticated = capable(CAP_SYS_ADMIN);
	priv->authenticated = priv->always_authenticated;
>>>>>>> refs/remotes/origin/master
	priv->lock_count = 0;

	INIT_LIST_HEAD(&priv->lhead);
	INIT_LIST_HEAD(&priv->fbs);
<<<<<<< HEAD
=======
	mutex_init(&priv->fbs_lock);
>>>>>>> refs/remotes/origin/master
	INIT_LIST_HEAD(&priv->event_list);
	init_waitqueue_head(&priv->event_wait);
	priv->event_space = 4096; /* set aside 4k for event buffer */

	if (dev->driver->driver_features & DRIVER_GEM)
		drm_gem_open(dev, priv);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (drm_core_check_feature(dev, DRIVER_PRIME))
		drm_prime_init_file_private(&priv->prime);

>>>>>>> refs/remotes/origin/cm-10.0
	if (dev->driver->open) {
		ret = dev->driver->open(dev, priv);
		if (ret < 0)
			goto out_free;
	}


	/* if there is no current master make this fd it */
	mutex_lock(&dev->struct_mutex);
	if (!priv->minor->master) {
=======
	if (drm_core_check_feature(dev, DRIVER_PRIME))
		drm_prime_init_file_private(&priv->prime);

	if (dev->driver->open) {
		ret = dev->driver->open(dev, priv);
		if (ret < 0)
			goto out_prime_destroy;
	}

	/* if there is no current master make this fd it, but do not create
	 * any master object for render clients */
	mutex_lock(&dev->struct_mutex);
	if (!priv->minor->master && !drm_is_render_client(priv)) {
>>>>>>> refs/remotes/origin/master
		/* create a new master */
		priv->minor->master = drm_master_create(priv->minor);
		if (!priv->minor->master) {
			mutex_unlock(&dev->struct_mutex);
			ret = -ENOMEM;
<<<<<<< HEAD
			goto out_free;
=======
			goto out_close;
>>>>>>> refs/remotes/origin/master
		}

		priv->is_master = 1;
		/* take another reference for the copy in the local file priv */
		priv->master = drm_master_get(priv->minor->master);

		priv->authenticated = 1;

		mutex_unlock(&dev->struct_mutex);
		if (dev->driver->master_create) {
			ret = dev->driver->master_create(dev, priv->master);
			if (ret) {
				mutex_lock(&dev->struct_mutex);
				/* drop both references if this fails */
				drm_master_put(&priv->minor->master);
				drm_master_put(&priv->master);
				mutex_unlock(&dev->struct_mutex);
<<<<<<< HEAD
				goto out_free;
=======
				goto out_close;
>>>>>>> refs/remotes/origin/master
			}
		}
		mutex_lock(&dev->struct_mutex);
		if (dev->driver->master_set) {
			ret = dev->driver->master_set(dev, priv, true);
			if (ret) {
				/* drop both references if this fails */
				drm_master_put(&priv->minor->master);
				drm_master_put(&priv->master);
				mutex_unlock(&dev->struct_mutex);
<<<<<<< HEAD
				goto out_free;
			}
		}
		mutex_unlock(&dev->struct_mutex);
	} else {
		/* get a reference to the master */
		priv->master = drm_master_get(priv->minor->master);
		mutex_unlock(&dev->struct_mutex);
	}
=======
				goto out_close;
			}
		}
	} else if (!drm_is_render_client(priv)) {
		/* get a reference to the master */
		priv->master = drm_master_get(priv->minor->master);
	}
	mutex_unlock(&dev->struct_mutex);
>>>>>>> refs/remotes/origin/master

	mutex_lock(&dev->struct_mutex);
	list_add(&priv->lhead, &dev->filelist);
	mutex_unlock(&dev->struct_mutex);

#ifdef __alpha__
	/*
	 * Default the hose
	 */
	if (!dev->hose) {
		struct pci_dev *pci_dev;
		pci_dev = pci_get_class(PCI_CLASS_DISPLAY_VGA << 8, NULL);
		if (pci_dev) {
			dev->hose = pci_dev->sysdata;
			pci_dev_put(pci_dev);
		}
		if (!dev->hose) {
			struct pci_bus *b = pci_bus_b(pci_root_buses.next);
			if (b)
				dev->hose = b->sysdata;
		}
	}
#endif

	return 0;
<<<<<<< HEAD
      out_free:
=======

out_close:
	if (dev->driver->postclose)
		dev->driver->postclose(dev, priv);
out_prime_destroy:
	if (drm_core_check_feature(dev, DRIVER_PRIME))
		drm_prime_destroy_file_private(&priv->prime);
	if (dev->driver->driver_features & DRIVER_GEM)
		drm_gem_release(dev, priv);
out_put_pid:
	put_pid(priv->pid);
>>>>>>> refs/remotes/origin/master
	kfree(priv);
	filp->private_data = NULL;
	return ret;
}

<<<<<<< HEAD
/** No-op. */
int drm_fasync(int fd, struct file *filp, int on)
{
	struct drm_file *priv = filp->private_data;
	struct drm_device *dev = priv->minor->dev;

	DRM_DEBUG("fd = %d, device = 0x%lx\n", fd,
		  (long)old_encode_dev(priv->minor->device));
	return fasync_helper(fd, filp, on, &dev->buf_async);
}
EXPORT_SYMBOL(drm_fasync);

/*
 * Reclaim locked buffers; note that this may be a bad idea if the current
 * context doesn't have the hw lock...
 */
static void drm_reclaim_locked_buffers(struct drm_device *dev, struct file *f)
{
	struct drm_file *file_priv = f->private_data;

	if (drm_i_have_hw_lock(dev, file_priv)) {
		dev->driver->reclaim_buffers_locked(dev, file_priv);
	} else {
		unsigned long _end = jiffies + 3 * DRM_HZ;
		int locked = 0;

		drm_idlelock_take(&file_priv->master->lock);

		/*
		 * Wait for a while.
		 */
		do {
			spin_lock_bh(&file_priv->master->lock.spinlock);
			locked = file_priv->master->lock.idle_has_lock;
			spin_unlock_bh(&file_priv->master->lock.spinlock);
			if (locked)
				break;
			schedule();
		} while (!time_after_eq(jiffies, _end));

		if (!locked) {
			DRM_ERROR("reclaim_buffers_locked() deadlock. Please rework this\n"
				  "\tdriver to use reclaim_buffers_idlelocked() instead.\n"
				  "\tI will go on reclaiming the buffers anyway.\n");
		}

		dev->driver->reclaim_buffers_locked(dev, file_priv);
		drm_idlelock_release(&file_priv->master->lock);
	}
}

=======
>>>>>>> refs/remotes/origin/master
static void drm_master_release(struct drm_device *dev, struct file *filp)
{
	struct drm_file *file_priv = filp->private_data;

<<<<<<< HEAD
	if (dev->driver->reclaim_buffers_locked &&
	    file_priv->master->lock.hw_lock)
		drm_reclaim_locked_buffers(dev, filp);

	if (dev->driver->reclaim_buffers_idlelocked &&
	    file_priv->master->lock.hw_lock) {
		drm_idlelock_take(&file_priv->master->lock);
		dev->driver->reclaim_buffers_idlelocked(dev, file_priv);
		drm_idlelock_release(&file_priv->master->lock);
	}


=======
>>>>>>> refs/remotes/origin/master
	if (drm_i_have_hw_lock(dev, file_priv)) {
		DRM_DEBUG("File %p released, freeing lock for context %d\n",
			  filp, _DRM_LOCKING_CONTEXT(file_priv->master->lock.hw_lock->lock));
		drm_lock_free(&file_priv->master->lock,
			      _DRM_LOCKING_CONTEXT(file_priv->master->lock.hw_lock->lock));
	}
<<<<<<< HEAD

	if (drm_core_check_feature(dev, DRIVER_HAVE_DMA) &&
	    !dev->driver->reclaim_buffers_locked) {
		dev->driver->reclaim_buffers(dev, file_priv);
	}
=======
>>>>>>> refs/remotes/origin/master
}

static void drm_events_release(struct drm_file *file_priv)
{
	struct drm_device *dev = file_priv->minor->dev;
	struct drm_pending_event *e, *et;
	struct drm_pending_vblank_event *v, *vt;
	unsigned long flags;

	spin_lock_irqsave(&dev->event_lock, flags);

	/* Remove pending flips */
	list_for_each_entry_safe(v, vt, &dev->vblank_event_list, base.link)
		if (v->base.file_priv == file_priv) {
			list_del(&v->base.link);
			drm_vblank_put(dev, v->pipe);
			v->base.destroy(&v->base);
		}

	/* Remove unconsumed events */
<<<<<<< HEAD
	list_for_each_entry_safe(e, et, &file_priv->event_list, link)
		e->destroy(e);
=======
	list_for_each_entry_safe(e, et, &file_priv->event_list, link) {
		list_del(&e->link);
		e->destroy(e);
	}
>>>>>>> refs/remotes/origin/master

	spin_unlock_irqrestore(&dev->event_lock, flags);
}

/**
<<<<<<< HEAD
=======
 * drm_legacy_dev_reinit
 *
 * Reinitializes a legacy/ums drm device in it's lastclose function.
 */
static void drm_legacy_dev_reinit(struct drm_device *dev)
{
	if (drm_core_check_feature(dev, DRIVER_MODESET))
		return;

	atomic_set(&dev->ioctl_count, 0);
	atomic_set(&dev->vma_count, 0);

	dev->sigdata.lock = NULL;

	dev->context_flag = 0;
	dev->last_context = 0;
	dev->if_version = 0;
}

/**
 * Take down the DRM device.
 *
 * \param dev DRM device structure.
 *
 * Frees every resource in \p dev.
 *
 * \sa drm_device
 */
int drm_lastclose(struct drm_device * dev)
{
	struct drm_vma_entry *vma, *vma_temp;

	DRM_DEBUG("\n");

	if (dev->driver->lastclose)
		dev->driver->lastclose(dev);
	DRM_DEBUG("driver lastclose completed\n");

	if (dev->irq_enabled && !drm_core_check_feature(dev, DRIVER_MODESET))
		drm_irq_uninstall(dev);

	mutex_lock(&dev->struct_mutex);

	drm_agp_clear(dev);

	drm_legacy_sg_cleanup(dev);

	/* Clear vma list (only built for debugging) */
	list_for_each_entry_safe(vma, vma_temp, &dev->vmalist, head) {
		list_del(&vma->head);
		kfree(vma);
	}

	drm_legacy_dma_takedown(dev);

	dev->dev_mapping = NULL;
	mutex_unlock(&dev->struct_mutex);

	drm_legacy_dev_reinit(dev);

	DRM_DEBUG("lastclose completed\n");
	return 0;
}

/**
>>>>>>> refs/remotes/origin/master
 * Release file.
 *
 * \param inode device inode
 * \param file_priv DRM file private.
 * \return zero on success or a negative number on failure.
 *
 * If the hardware lock is held then free it, and take it again for the kernel
 * context since it's necessary to reclaim buffers. Unlink the file private
 * data from its list and free it. Decreases the open count and if it reaches
 * zero calls drm_lastclose().
 */
int drm_release(struct inode *inode, struct file *filp)
{
	struct drm_file *file_priv = filp->private_data;
	struct drm_device *dev = file_priv->minor->dev;
	int retcode = 0;

	mutex_lock(&drm_global_mutex);

	DRM_DEBUG("open_count = %d\n", dev->open_count);

	if (dev->driver->preclose)
		dev->driver->preclose(dev, file_priv);

	/* ========================================================
	 * Begin inline drm_release
	 */

	DRM_DEBUG("pid = %d, device = 0x%lx, open_count = %d\n",
		  task_pid_nr(current),
		  (long)old_encode_dev(file_priv->minor->device),
		  dev->open_count);

	/* Release any auth tokens that might point to this file_priv,
	   (do that under the drm_global_mutex) */
	if (file_priv->magic)
		(void) drm_remove_magic(file_priv->master, file_priv->magic);

	/* if the master has gone away we can't do anything with the lock */
	if (file_priv->minor->master)
		drm_master_release(dev, filp);

<<<<<<< HEAD
	drm_events_release(file_priv);

<<<<<<< HEAD
	if (dev->driver->driver_features & DRIVER_GEM)
		drm_gem_release(dev, file_priv);

	if (dev->driver->driver_features & DRIVER_MODESET)
		drm_fb_release(file_priv);

=======
=======
	if (drm_core_check_feature(dev, DRIVER_HAVE_DMA))
		drm_core_reclaim_buffers(dev, file_priv);

	drm_events_release(file_priv);

>>>>>>> refs/remotes/origin/master
	if (dev->driver->driver_features & DRIVER_MODESET)
		drm_fb_release(file_priv);

	if (dev->driver->driver_features & DRIVER_GEM)
		drm_gem_release(dev, file_priv);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	mutex_lock(&dev->ctxlist_mutex);
	if (!list_empty(&dev->ctxlist)) {
		struct drm_ctx_list *pos, *n;

		list_for_each_entry_safe(pos, n, &dev->ctxlist, head) {
			if (pos->tag == file_priv &&
			    pos->handle != DRM_KERNEL_CONTEXT) {
				if (dev->driver->context_dtor)
					dev->driver->context_dtor(dev,
								  pos->handle);

				drm_ctxbitmap_free(dev, pos->handle);

				list_del(&pos->head);
				kfree(pos);
<<<<<<< HEAD
				--dev->ctx_count;
=======
>>>>>>> refs/remotes/origin/master
			}
		}
	}
	mutex_unlock(&dev->ctxlist_mutex);

	mutex_lock(&dev->struct_mutex);

	if (file_priv->is_master) {
		struct drm_master *master = file_priv->master;
		struct drm_file *temp;
		list_for_each_entry(temp, &dev->filelist, lhead) {
			if ((temp->master == file_priv->master) &&
			    (temp != file_priv))
<<<<<<< HEAD
				temp->authenticated = 0;
=======
				temp->authenticated = temp->always_authenticated;
>>>>>>> refs/remotes/origin/master
		}

		/**
		 * Since the master is disappearing, so is the
		 * possibility to lock.
		 */

		if (master->lock.hw_lock) {
			if (dev->sigdata.lock == master->lock.hw_lock)
				dev->sigdata.lock = NULL;
			master->lock.hw_lock = NULL;
			master->lock.file_priv = NULL;
			wake_up_interruptible_all(&master->lock.lock_queue);
		}

		if (file_priv->minor->master == file_priv->master) {
			/* drop the reference held my the minor */
			if (dev->driver->master_drop)
				dev->driver->master_drop(dev, file_priv, true);
			drm_master_put(&file_priv->minor->master);
		}
	}

<<<<<<< HEAD
	/* drop the reference held my the file priv */
	drm_master_put(&file_priv->master);
=======
	BUG_ON(dev->dev_mapping == NULL);
	iput(container_of(dev->dev_mapping, struct inode, i_data));

	/* drop the reference held my the file priv */
	if (file_priv->master)
		drm_master_put(&file_priv->master);
>>>>>>> refs/remotes/origin/master
	file_priv->is_master = 0;
	list_del(&file_priv->lhead);
	mutex_unlock(&dev->struct_mutex);

	if (dev->driver->postclose)
		dev->driver->postclose(dev, file_priv);
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======

>>>>>>> refs/remotes/origin/master

	if (drm_core_check_feature(dev, DRIVER_PRIME))
		drm_prime_destroy_file_private(&file_priv->prime);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	put_pid(file_priv->pid);
>>>>>>> refs/remotes/origin/master
	kfree(file_priv);

	/* ========================================================
	 * End inline drm_release
	 */

<<<<<<< HEAD
	atomic_inc(&dev->counts[_DRM_STAT_CLOSES]);
=======
>>>>>>> refs/remotes/origin/master
	if (!--dev->open_count) {
		if (atomic_read(&dev->ioctl_count)) {
			DRM_ERROR("Device busy: %d\n",
				  atomic_read(&dev->ioctl_count));
			retcode = -EBUSY;
		} else
			retcode = drm_lastclose(dev);
<<<<<<< HEAD
<<<<<<< HEAD
=======
		if (drm_device_is_unplugged(dev))
			drm_put_dev(dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (drm_device_is_unplugged(dev))
			drm_put_dev(dev);
>>>>>>> refs/remotes/origin/master
	}
	mutex_unlock(&drm_global_mutex);

	return retcode;
}
EXPORT_SYMBOL(drm_release);

static bool
drm_dequeue_event(struct drm_file *file_priv,
		  size_t total, size_t max, struct drm_pending_event **out)
{
	struct drm_device *dev = file_priv->minor->dev;
	struct drm_pending_event *e;
	unsigned long flags;
	bool ret = false;

	spin_lock_irqsave(&dev->event_lock, flags);

	*out = NULL;
	if (list_empty(&file_priv->event_list))
		goto out;
	e = list_first_entry(&file_priv->event_list,
			     struct drm_pending_event, link);
	if (e->event->length + total > max)
		goto out;

	file_priv->event_space += e->event->length;
	list_del(&e->link);
	*out = e;
	ret = true;

out:
	spin_unlock_irqrestore(&dev->event_lock, flags);
	return ret;
}

ssize_t drm_read(struct file *filp, char __user *buffer,
		 size_t count, loff_t *offset)
{
	struct drm_file *file_priv = filp->private_data;
	struct drm_pending_event *e;
	size_t total;
	ssize_t ret;

	ret = wait_event_interruptible(file_priv->event_wait,
				       !list_empty(&file_priv->event_list));
	if (ret < 0)
		return ret;

	total = 0;
	while (drm_dequeue_event(file_priv, total, count, &e)) {
		if (copy_to_user(buffer + total,
				 e->event, e->event->length)) {
			total = -EFAULT;
			break;
		}

		total += e->event->length;
		e->destroy(e);
	}

	return total;
}
EXPORT_SYMBOL(drm_read);

unsigned int drm_poll(struct file *filp, struct poll_table_struct *wait)
{
	struct drm_file *file_priv = filp->private_data;
	unsigned int mask = 0;

	poll_wait(filp, &file_priv->event_wait, wait);

	if (!list_empty(&file_priv->event_list))
		mask |= POLLIN | POLLRDNORM;

	return mask;
}
EXPORT_SYMBOL(drm_poll);
