/*
 * fs/direct-io.c
 *
 * Copyright (C) 2002, Linus Torvalds.
 *
 * O_DIRECT
 *
 * 04Jul2002	Andrew Morton
 *		Initial version
 * 11Sep2002	janetinc@us.ibm.com
 * 		added readv/writev support.
 * 29Oct2002	Andrew Morton
 *		rewrote bio_add_page() support.
 * 30Oct2002	pbadari@us.ibm.com
 *		added support for non-aligned IO.
 * 06Nov2002	pbadari@us.ibm.com
 *		added asynchronous IO support.
 * 21Jul2003	nathans@sgi.com
 *		added IO completion notifier.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/highmem.h>
#include <linux/pagemap.h>
#include <linux/task_io_accounting_ops.h>
#include <linux/bio.h>
#include <linux/wait.h>
#include <linux/err.h>
#include <linux/blkdev.h>
#include <linux/buffer_head.h>
#include <linux/rwsem.h>
#include <linux/uio.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/atomic.h>

/*
 * How many user pages to map in one call to get_user_pages().  This determines
 * the size of a structure on the stack.
=======
#include <linux/atomic.h>
#include <linux/prefetch.h>
=======
#include <linux/atomic.h>
#include <linux/prefetch.h>
#include <linux/aio.h>
>>>>>>> refs/remotes/origin/master

/*
 * How many user pages to map in one call to get_user_pages().  This determines
 * the size of a structure in the slab cache
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 */
#define DIO_PAGES	64

/*
 * This code generally works in units of "dio_blocks".  A dio_block is
 * somewhere between the hard sector size and the filesystem block size.  it
 * is determined on a per-invocation basis.   When talking to the filesystem
 * we need to convert dio_blocks to fs_blocks by scaling the dio_block quantity
 * down by dio->blkfactor.  Similarly, fs-blocksize quantities are converted
 * to bio_block quantities by shifting left by blkfactor.
 *
 * If blkfactor is zero then the user's request was aligned to the filesystem's
 * blocksize.
 */

<<<<<<< HEAD
<<<<<<< HEAD
struct dio {
	/* BIO submission state */
	struct bio *bio;		/* bio under assembly */
	struct inode *inode;
	int rw;
	loff_t i_size;			/* i_size when submitted */
	int flags;			/* doesn't change */
=======
=======
>>>>>>> refs/remotes/origin/master
/* dio_state only used in the submission path */

struct dio_submit {
	struct bio *bio;		/* bio under assembly */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	unsigned blkbits;		/* doesn't change */
	unsigned blkfactor;		/* When we're using an alignment which
					   is finer than the filesystem's soft
					   blocksize, this specifies how much
					   finer.  blkfactor=2 means 1/4-block
					   alignment.  Does not change */
	unsigned start_zero_done;	/* flag: sub-blocksize zeroing has
					   been performed at the start of a
					   write */
	int pages_in_io;		/* approximate total IO pages */
	size_t	size;			/* total request size (doesn't change)*/
	sector_t block_in_file;		/* Current offset into the underlying
					   file in dio_block units. */
	unsigned blocks_available;	/* At block_in_file.  changes */
<<<<<<< HEAD
<<<<<<< HEAD
	sector_t final_block_in_request;/* doesn't change */
	unsigned first_block_in_page;	/* doesn't change, Used only once */
	int boundary;			/* prev block is at a boundary */
	int reap_counter;		/* rate limit reaping */
	get_block_t *get_block;		/* block mapping function */
	dio_iodone_t *end_io;		/* IO completion function */
	dio_submit_t *submit_io;	/* IO submition function */
=======
=======
>>>>>>> refs/remotes/origin/master
	int reap_counter;		/* rate limit reaping */
	sector_t final_block_in_request;/* doesn't change */
	unsigned first_block_in_page;	/* doesn't change, Used only once */
	int boundary;			/* prev block is at a boundary */
	get_block_t *get_block;		/* block mapping function */
	dio_submit_t *submit_io;	/* IO submition function */

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	loff_t logical_offset_in_bio;	/* current first logical block in bio */
	sector_t final_block_in_bio;	/* current final block in bio + 1 */
	sector_t next_block_for_io;	/* next block to be put under IO,
					   in dio_blocks units */
<<<<<<< HEAD
<<<<<<< HEAD
	struct buffer_head map_bh;	/* last get_block() result */
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/*
	 * Deferred addition of a page to the dio.  These variables are
	 * private to dio_send_cur_page(), submit_page_section() and
	 * dio_bio_add_page().
	 */
	struct page *cur_page;		/* The page */
	unsigned cur_page_offset;	/* Offset into it, in bytes */
	unsigned cur_page_len;		/* Nr of bytes at cur_page_offset */
	sector_t cur_page_block;	/* Where it starts */
	loff_t cur_page_fs_offset;	/* Offset in file */

<<<<<<< HEAD
<<<<<<< HEAD
	/* BIO completion state */
	spinlock_t bio_lock;		/* protects BIO fields below */
	unsigned long refcount;		/* direct_io_worker() and bios */
	struct bio *bio_list;		/* singly linked via bi_private */
	struct task_struct *waiter;	/* waiting task (NULL if none) */

	/* AIO related stuff */
	struct kiocb *iocb;		/* kiocb */
	int is_async;			/* is IO async ? */
	int io_error;			/* IO error in completion path */
	ssize_t result;                 /* IO result */

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Page fetching state. These variables belong to dio_refill_pages().
	 */
	int curr_page;			/* changes */
	int total_pages;		/* doesn't change */
	unsigned long curr_user_address;/* changes */

	/*
	 * Page queue.  These variables belong to dio_refill_pages() and
	 * dio_get_page().
	 */
	unsigned head;			/* next page to process */
	unsigned tail;			/* last valid page + 1 */
<<<<<<< HEAD
<<<<<<< HEAD
	int page_errors;		/* errno from get_user_pages() */
=======
=======
>>>>>>> refs/remotes/origin/master
};

/* dio_state communicated between submission path and end_io */
struct dio {
	int flags;			/* doesn't change */
	int rw;
	struct inode *inode;
	loff_t i_size;			/* i_size when submitted */
	dio_iodone_t *end_io;		/* IO completion function */

	void *private;			/* copy from map_bh.b_private */

	/* BIO completion state */
	spinlock_t bio_lock;		/* protects BIO fields below */
	int page_errors;		/* errno from get_user_pages() */
	int is_async;			/* is IO async ? */
<<<<<<< HEAD
=======
	bool defer_completion;		/* defer AIO completion to workqueue? */
>>>>>>> refs/remotes/origin/master
	int io_error;			/* IO error in completion path */
	unsigned long refcount;		/* direct_io_worker() and bios */
	struct bio *bio_list;		/* singly linked via bi_private */
	struct task_struct *waiter;	/* waiting task (NULL if none) */

	/* AIO related stuff */
	struct kiocb *iocb;		/* kiocb */
	ssize_t result;                 /* IO result */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/*
	 * pages[] (and any fields placed after it) are not zeroed out at
	 * allocation time.  Don't add new fields after pages[] unless you
	 * wish that they not be zeroed.
	 */
<<<<<<< HEAD
	struct page *pages[DIO_PAGES];	/* page buffer */
<<<<<<< HEAD
};
=======
=======
	union {
		struct page *pages[DIO_PAGES];	/* page buffer */
		struct work_struct complete_work;/* deferred AIO completion */
	};
>>>>>>> refs/remotes/origin/master
} ____cacheline_aligned_in_smp;

static struct kmem_cache *dio_cache __read_mostly;

<<<<<<< HEAD
static void __inode_dio_wait(struct inode *inode)
{
	wait_queue_head_t *wq = bit_waitqueue(&inode->i_state, __I_DIO_WAKEUP);
	DEFINE_WAIT_BIT(q, &inode->i_state, __I_DIO_WAKEUP);

	do {
		prepare_to_wait(wq, &q.wait, TASK_UNINTERRUPTIBLE);
		if (atomic_read(&inode->i_dio_count))
			schedule();
	} while (atomic_read(&inode->i_dio_count));
	finish_wait(wq, &q.wait);
}

/**
 * inode_dio_wait - wait for outstanding DIO requests to finish
 * @inode: inode to wait for
 *
 * Waits for all pending direct I/O requests to finish so that we can
 * proceed with a truncate or equivalent operation.
 *
 * Must be called under a lock that serializes taking new references
 * to i_dio_count, usually by inode->i_mutex.
 */
void inode_dio_wait(struct inode *inode)
{
	if (atomic_read(&inode->i_dio_count))
		__inode_dio_wait(inode);
}
EXPORT_SYMBOL(inode_dio_wait);

/*
 * inode_dio_done - signal finish of a direct I/O requests
 * @inode: inode the direct I/O happens on
 *
 * This is called once we've finished processing a direct I/O request,
 * and is used to wake up callers waiting for direct I/O to be quiesced.
 */
void inode_dio_done(struct inode *inode)
{
	if (atomic_dec_and_test(&inode->i_dio_count))
		wake_up_bit(&inode->i_state, __I_DIO_WAKEUP);
}
EXPORT_SYMBOL(inode_dio_done);
>>>>>>> refs/remotes/origin/cm-10.0

/*
 * How many pages are in the queue?
 */
<<<<<<< HEAD
static inline unsigned dio_pages_present(struct dio *dio)
{
	return dio->tail - dio->head;
=======
static inline unsigned dio_pages_present(struct dio_submit *sdio)
{
	return sdio->tail - sdio->head;
>>>>>>> refs/remotes/origin/cm-10.0
=======
/*
 * How many pages are in the queue?
 */
static inline unsigned dio_pages_present(struct dio_submit *sdio)
{
	return sdio->tail - sdio->head;
>>>>>>> refs/remotes/origin/master
}

/*
 * Go grab and pin some userspace pages.   Typically we'll get 64 at a time.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int dio_refill_pages(struct dio *dio)
=======
static inline int dio_refill_pages(struct dio *dio, struct dio_submit *sdio)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline int dio_refill_pages(struct dio *dio, struct dio_submit *sdio)
>>>>>>> refs/remotes/origin/master
{
	int ret;
	int nr_pages;

<<<<<<< HEAD
<<<<<<< HEAD
	nr_pages = min(dio->total_pages - dio->curr_page, DIO_PAGES);
	ret = get_user_pages_fast(
		dio->curr_user_address,		/* Where from? */
=======
	nr_pages = min(sdio->total_pages - sdio->curr_page, DIO_PAGES);
	ret = get_user_pages_fast(
		sdio->curr_user_address,		/* Where from? */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	nr_pages = min(sdio->total_pages - sdio->curr_page, DIO_PAGES);
	ret = get_user_pages_fast(
		sdio->curr_user_address,		/* Where from? */
>>>>>>> refs/remotes/origin/master
		nr_pages,			/* How many pages? */
		dio->rw == READ,		/* Write to memory? */
		&dio->pages[0]);		/* Put results here */

<<<<<<< HEAD
<<<<<<< HEAD
	if (ret < 0 && dio->blocks_available && (dio->rw & WRITE)) {
=======
	if (ret < 0 && sdio->blocks_available && (dio->rw & WRITE)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (ret < 0 && sdio->blocks_available && (dio->rw & WRITE)) {
>>>>>>> refs/remotes/origin/master
		struct page *page = ZERO_PAGE(0);
		/*
		 * A memory fault, but the filesystem has some outstanding
		 * mapped blocks.  We need to use those blocks up to avoid
		 * leaking stale data in the file.
		 */
		if (dio->page_errors == 0)
			dio->page_errors = ret;
		page_cache_get(page);
		dio->pages[0] = page;
<<<<<<< HEAD
<<<<<<< HEAD
		dio->head = 0;
		dio->tail = 1;
=======
		sdio->head = 0;
		sdio->tail = 1;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		sdio->head = 0;
		sdio->tail = 1;
>>>>>>> refs/remotes/origin/master
		ret = 0;
		goto out;
	}

	if (ret >= 0) {
<<<<<<< HEAD
<<<<<<< HEAD
		dio->curr_user_address += ret * PAGE_SIZE;
		dio->curr_page += ret;
		dio->head = 0;
		dio->tail = ret;
=======
=======
>>>>>>> refs/remotes/origin/master
		sdio->curr_user_address += ret * PAGE_SIZE;
		sdio->curr_page += ret;
		sdio->head = 0;
		sdio->tail = ret;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		ret = 0;
	}
out:
	return ret;	
}

/*
 * Get another userspace page.  Returns an ERR_PTR on error.  Pages are
 * buffered inside the dio so that we can call get_user_pages() against a
 * decent number of pages, less frequently.  To provide nicer use of the
 * L1 cache.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static struct page *dio_get_page(struct dio *dio)
{
	if (dio_pages_present(dio) == 0) {
		int ret;

		ret = dio_refill_pages(dio);
		if (ret)
			return ERR_PTR(ret);
		BUG_ON(dio_pages_present(dio) == 0);
	}
	return dio->pages[dio->head++];
=======
=======
>>>>>>> refs/remotes/origin/master
static inline struct page *dio_get_page(struct dio *dio,
		struct dio_submit *sdio)
{
	if (dio_pages_present(sdio) == 0) {
		int ret;

		ret = dio_refill_pages(dio, sdio);
		if (ret)
			return ERR_PTR(ret);
		BUG_ON(dio_pages_present(sdio) == 0);
	}
	return dio->pages[sdio->head++];
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/**
 * dio_complete() - called when all DIO BIO I/O has been completed
 * @offset: the byte offset in the file of the completed operation
 *
<<<<<<< HEAD
 * This releases locks as dictated by the locking type, lets interested parties
 * know that a DIO operation has completed, and calculates the resulting return
 * code for the operation.
=======
 * This drops i_dio_count, lets interested parties know that a DIO operation
 * has completed, and calculates the resulting return code for the operation.
>>>>>>> refs/remotes/origin/master
 *
 * It lets the filesystem know if it registered an interest earlier via
 * get_block.  Pass the private field of the map buffer_head so that
 * filesystems can use it to hold additional state between get_block calls and
 * dio_complete.
 */
<<<<<<< HEAD
static ssize_t dio_complete(struct dio *dio, loff_t offset, ssize_t ret, bool is_async)
=======
static ssize_t dio_complete(struct dio *dio, loff_t offset, ssize_t ret,
		bool is_async)
>>>>>>> refs/remotes/origin/master
{
	ssize_t transferred = 0;

	/*
	 * AIO submission can race with bio completion to get here while
	 * expecting to have the last io completed by bio completion.
	 * In that case -EIOCBQUEUED is in fact not an error we want
	 * to preserve through this call.
	 */
	if (ret == -EIOCBQUEUED)
		ret = 0;

	if (dio->result) {
		transferred = dio->result;

		/* Check for short read case */
		if ((dio->rw == READ) && ((offset + transferred) > dio->i_size))
			transferred = dio->i_size - offset;
	}

	if (ret == 0)
		ret = dio->page_errors;
	if (ret == 0)
		ret = dio->io_error;
	if (ret == 0)
		ret = transferred;

<<<<<<< HEAD
	if (dio->end_io && dio->result) {
		dio->end_io(dio->iocb, offset, transferred,
<<<<<<< HEAD
			    dio->map_bh.b_private, ret, is_async);
	} else if (is_async) {
		aio_complete(dio->iocb, ret, 0);
	}

	if (dio->flags & DIO_LOCKING)
		/* lockdep: non-owner release */
		up_read_non_owner(&dio->inode->i_alloc_sem);

=======
			    dio->private, ret, is_async);
	} else {
		inode_dio_done(dio->inode);
		if (is_async)
			aio_complete(dio->iocb, ret, 0);
	}

>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

static int dio_bio_complete(struct dio *dio, struct bio *bio);
=======
	if (dio->end_io && dio->result)
		dio->end_io(dio->iocb, offset, transferred, dio->private);

	inode_dio_done(dio->inode);
	if (is_async) {
		if (dio->rw & WRITE) {
			int err;

			err = generic_write_sync(dio->iocb->ki_filp, offset,
						 transferred);
			if (err < 0 && ret > 0)
				ret = err;
		}

		aio_complete(dio->iocb, ret, 0);
	}

	kmem_cache_free(dio_cache, dio);
	return ret;
}

static void dio_aio_complete_work(struct work_struct *work)
{
	struct dio *dio = container_of(work, struct dio, complete_work);

	dio_complete(dio, dio->iocb->ki_pos, 0, true);
}

static int dio_bio_complete(struct dio *dio, struct bio *bio);

>>>>>>> refs/remotes/origin/master
/*
 * Asynchronous IO callback. 
 */
static void dio_bio_end_aio(struct bio *bio, int error)
{
	struct dio *dio = bio->bi_private;
	unsigned long remaining;
	unsigned long flags;

	/* cleanup the bio */
	dio_bio_complete(dio, bio);

	spin_lock_irqsave(&dio->bio_lock, flags);
	remaining = --dio->refcount;
	if (remaining == 1 && dio->waiter)
		wake_up_process(dio->waiter);
	spin_unlock_irqrestore(&dio->bio_lock, flags);

	if (remaining == 0) {
<<<<<<< HEAD
		dio_complete(dio, dio->iocb->ki_pos, 0, true);
<<<<<<< HEAD
		kfree(dio);
=======
		kmem_cache_free(dio_cache, dio);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (dio->result && dio->defer_completion) {
			INIT_WORK(&dio->complete_work, dio_aio_complete_work);
			queue_work(dio->inode->i_sb->s_dio_done_wq,
				   &dio->complete_work);
		} else {
			dio_complete(dio, dio->iocb->ki_pos, 0, true);
		}
>>>>>>> refs/remotes/origin/master
	}
}

/*
 * The BIO completion handler simply queues the BIO up for the process-context
 * handler.
 *
 * During I/O bi_private points at the dio.  After I/O, bi_private is used to
 * implement a singly-linked list of completed BIOs, at dio->bio_list.
 */
static void dio_bio_end_io(struct bio *bio, int error)
{
	struct dio *dio = bio->bi_private;
	unsigned long flags;

	spin_lock_irqsave(&dio->bio_lock, flags);
	bio->bi_private = dio->bio_list;
	dio->bio_list = bio;
	if (--dio->refcount == 1 && dio->waiter)
		wake_up_process(dio->waiter);
	spin_unlock_irqrestore(&dio->bio_lock, flags);
}

/**
 * dio_end_io - handle the end io action for the given bio
 * @bio: The direct io bio thats being completed
 * @error: Error if there was one
 *
 * This is meant to be called by any filesystem that uses their own dio_submit_t
 * so that the DIO specific endio actions are dealt with after the filesystem
 * has done it's completion work.
 */
void dio_end_io(struct bio *bio, int error)
{
	struct dio *dio = bio->bi_private;

	if (dio->is_async)
		dio_bio_end_aio(bio, error);
	else
		dio_bio_end_io(bio, error);
}
EXPORT_SYMBOL_GPL(dio_end_io);

<<<<<<< HEAD
<<<<<<< HEAD
static void
dio_bio_alloc(struct dio *dio, struct block_device *bdev,
		sector_t first_sector, int nr_vecs)
=======
=======
>>>>>>> refs/remotes/origin/master
static inline void
dio_bio_alloc(struct dio *dio, struct dio_submit *sdio,
	      struct block_device *bdev,
	      sector_t first_sector, int nr_vecs)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	struct bio *bio;

	/*
	 * bio_alloc() is guaranteed to return a bio when called with
	 * __GFP_WAIT and we request a valid number of vectors.
	 */
	bio = bio_alloc(GFP_KERNEL, nr_vecs);

	bio->bi_bdev = bdev;
	bio->bi_sector = first_sector;
	if (dio->is_async)
		bio->bi_end_io = dio_bio_end_aio;
	else
		bio->bi_end_io = dio_bio_end_io;

<<<<<<< HEAD
<<<<<<< HEAD
	dio->bio = bio;
	dio->logical_offset_in_bio = dio->cur_page_fs_offset;
=======
	sdio->bio = bio;
	sdio->logical_offset_in_bio = sdio->cur_page_fs_offset;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	sdio->bio = bio;
	sdio->logical_offset_in_bio = sdio->cur_page_fs_offset;
>>>>>>> refs/remotes/origin/master
}

/*
 * In the AIO read case we speculatively dirty the pages before starting IO.
 * During IO completion, any of these pages which happen to have been written
 * back will be redirtied by bio_check_pages_dirty().
 *
 * bios hold a dio reference between submit_bio and ->end_io.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static void dio_bio_submit(struct dio *dio)
{
	struct bio *bio = dio->bio;
=======
static inline void dio_bio_submit(struct dio *dio, struct dio_submit *sdio)
{
	struct bio *bio = sdio->bio;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline void dio_bio_submit(struct dio *dio, struct dio_submit *sdio)
{
	struct bio *bio = sdio->bio;
>>>>>>> refs/remotes/origin/master
	unsigned long flags;

	bio->bi_private = dio;

	spin_lock_irqsave(&dio->bio_lock, flags);
	dio->refcount++;
	spin_unlock_irqrestore(&dio->bio_lock, flags);

	if (dio->is_async && dio->rw == READ)
		bio_set_pages_dirty(bio);

<<<<<<< HEAD
<<<<<<< HEAD
	if (dio->submit_io)
		dio->submit_io(dio->rw, bio, dio->inode,
			       dio->logical_offset_in_bio);
	else
		submit_bio(dio->rw, bio);

	dio->bio = NULL;
	dio->boundary = 0;
	dio->logical_offset_in_bio = 0;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (sdio->submit_io)
		sdio->submit_io(dio->rw, bio, dio->inode,
			       sdio->logical_offset_in_bio);
	else
		submit_bio(dio->rw, bio);

	sdio->bio = NULL;
	sdio->boundary = 0;
	sdio->logical_offset_in_bio = 0;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/*
 * Release any resources in case of a failure
 */
<<<<<<< HEAD
<<<<<<< HEAD
static void dio_cleanup(struct dio *dio)
{
	while (dio_pages_present(dio))
		page_cache_release(dio_get_page(dio));
=======
=======
>>>>>>> refs/remotes/origin/master
static inline void dio_cleanup(struct dio *dio, struct dio_submit *sdio)
{
	while (dio_pages_present(sdio))
		page_cache_release(dio_get_page(dio, sdio));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/*
 * Wait for the next BIO to complete.  Remove it and return it.  NULL is
 * returned once all BIOs have been completed.  This must only be called once
 * all bios have been issued so that dio->refcount can only decrease.  This
 * requires that that the caller hold a reference on the dio.
 */
static struct bio *dio_await_one(struct dio *dio)
{
	unsigned long flags;
	struct bio *bio = NULL;

	spin_lock_irqsave(&dio->bio_lock, flags);

	/*
	 * Wait as long as the list is empty and there are bios in flight.  bio
	 * completion drops the count, maybe adds to the list, and wakes while
	 * holding the bio_lock so we don't need set_current_state()'s barrier
	 * and can call it after testing our condition.
	 */
	while (dio->refcount > 1 && dio->bio_list == NULL) {
		__set_current_state(TASK_UNINTERRUPTIBLE);
		dio->waiter = current;
		spin_unlock_irqrestore(&dio->bio_lock, flags);
		io_schedule();
		/* wake up sets us TASK_RUNNING */
		spin_lock_irqsave(&dio->bio_lock, flags);
		dio->waiter = NULL;
	}
	if (dio->bio_list) {
		bio = dio->bio_list;
		dio->bio_list = bio->bi_private;
	}
	spin_unlock_irqrestore(&dio->bio_lock, flags);
	return bio;
}

/*
 * Process one completed BIO.  No locks are held.
 */
static int dio_bio_complete(struct dio *dio, struct bio *bio)
{
	const int uptodate = test_bit(BIO_UPTODATE, &bio->bi_flags);
<<<<<<< HEAD
	struct bio_vec *bvec = bio->bi_io_vec;
	int page_no;
=======
	struct bio_vec *bvec;
	unsigned i;
>>>>>>> refs/remotes/origin/master

	if (!uptodate)
		dio->io_error = -EIO;

	if (dio->is_async && dio->rw == READ) {
		bio_check_pages_dirty(bio);	/* transfers ownership */
	} else {
<<<<<<< HEAD
		for (page_no = 0; page_no < bio->bi_vcnt; page_no++) {
			struct page *page = bvec[page_no].bv_page;
=======
		bio_for_each_segment_all(bvec, bio, i) {
			struct page *page = bvec->bv_page;
>>>>>>> refs/remotes/origin/master

			if (dio->rw == READ && !PageCompound(page))
				set_page_dirty_lock(page);
			page_cache_release(page);
		}
		bio_put(bio);
	}
	return uptodate ? 0 : -EIO;
}

/*
 * Wait on and process all in-flight BIOs.  This must only be called once
 * all bios have been issued so that the refcount can only decrease.
 * This just waits for all bios to make it through dio_bio_complete.  IO
 * errors are propagated through dio->io_error and should be propagated via
 * dio_complete().
 */
static void dio_await_completion(struct dio *dio)
{
	struct bio *bio;
	do {
		bio = dio_await_one(dio);
		if (bio)
			dio_bio_complete(dio, bio);
	} while (bio);
}

/*
 * A really large O_DIRECT read or write can generate a lot of BIOs.  So
 * to keep the memory consumption sane we periodically reap any completed BIOs
 * during the BIO generation phase.
 *
 * This also helps to limit the peak amount of pinned userspace memory.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int dio_bio_reap(struct dio *dio)
{
	int ret = 0;

	if (dio->reap_counter++ >= 64) {
=======
=======
>>>>>>> refs/remotes/origin/master
static inline int dio_bio_reap(struct dio *dio, struct dio_submit *sdio)
{
	int ret = 0;

	if (sdio->reap_counter++ >= 64) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		while (dio->bio_list) {
			unsigned long flags;
			struct bio *bio;
			int ret2;

			spin_lock_irqsave(&dio->bio_lock, flags);
			bio = dio->bio_list;
			dio->bio_list = bio->bi_private;
			spin_unlock_irqrestore(&dio->bio_lock, flags);
			ret2 = dio_bio_complete(dio, bio);
			if (ret == 0)
				ret = ret2;
		}
<<<<<<< HEAD
<<<<<<< HEAD
		dio->reap_counter = 0;
=======
		sdio->reap_counter = 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		sdio->reap_counter = 0;
>>>>>>> refs/remotes/origin/master
	}
	return ret;
}

/*
<<<<<<< HEAD
 * Call into the fs to map some more disk blocks.  We record the current number
<<<<<<< HEAD
 * of available blocks at dio->blocks_available.  These are in units of the
=======
 * of available blocks at sdio->blocks_available.  These are in units of the
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Create workqueue for deferred direct IO completions. We allocate the
 * workqueue when it's first needed. This avoids creating workqueue for
 * filesystems that don't need it and also allows us to create the workqueue
 * late enough so the we can include s_id in the name of the workqueue.
 */
static int sb_init_dio_done_wq(struct super_block *sb)
{
	struct workqueue_struct *old;
	struct workqueue_struct *wq = alloc_workqueue("dio/%s",
						      WQ_MEM_RECLAIM, 0,
						      sb->s_id);
	if (!wq)
		return -ENOMEM;
	/*
	 * This has to be atomic as more DIOs can race to create the workqueue
	 */
	old = cmpxchg(&sb->s_dio_done_wq, NULL, wq);
	/* Someone created workqueue before us? Free ours... */
	if (old)
		destroy_workqueue(wq);
	return 0;
}

static int dio_set_defer_completion(struct dio *dio)
{
	struct super_block *sb = dio->inode->i_sb;

	if (dio->defer_completion)
		return 0;
	dio->defer_completion = true;
	if (!sb->s_dio_done_wq)
		return sb_init_dio_done_wq(sb);
	return 0;
}

/*
 * Call into the fs to map some more disk blocks.  We record the current number
 * of available blocks at sdio->blocks_available.  These are in units of the
>>>>>>> refs/remotes/origin/master
 * fs blocksize, (1 << inode->i_blkbits).
 *
 * The fs is allowed to map lots of blocks at once.  If it wants to do that,
 * it uses the passed inode-relative block number as the file offset, as usual.
 *
 * get_block() is passed the number of i_blkbits-sized blocks which direct_io
 * has remaining to do.  The fs should not map more than this number of blocks.
 *
 * If the fs has mapped a lot of blocks, it should populate bh->b_size to
 * indicate how much contiguous disk space has been made available at
 * bh->b_blocknr.
 *
 * If *any* of the mapped blocks are new, then the fs must set buffer_new().
 * This isn't very efficient...
 *
 * In the case of filesystem holes: the fs may return an arbitrarily-large
 * hole by returning an appropriate value in b_size and by clearing
 * buffer_mapped().  However the direct-io code will only process holes one
 * block at a time - it will repeatedly call get_block() as it walks the hole.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int get_more_blocks(struct dio *dio)
{
	int ret;
	struct buffer_head *map_bh = &dio->map_bh;
	sector_t fs_startblk;	/* Into file, in filesystem-sized blocks */
	unsigned long fs_count;	/* Number of filesystem-sized blocks */
	unsigned long dio_count;/* Number of dio_block-sized blocks */
	unsigned long blkmask;
=======
=======
>>>>>>> refs/remotes/origin/master
static int get_more_blocks(struct dio *dio, struct dio_submit *sdio,
			   struct buffer_head *map_bh)
{
	int ret;
	sector_t fs_startblk;	/* Into file, in filesystem-sized blocks */
	sector_t fs_endblk;	/* Into file, in filesystem-sized blocks */
	unsigned long fs_count;	/* Number of filesystem-sized blocks */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	int create;
=======
	int create;
	unsigned int i_blkbits = sdio->blkbits + sdio->blkfactor;
>>>>>>> refs/remotes/origin/master

	/*
	 * If there was a memory error and we've overwritten all the
	 * mapped blocks then we can now return that memory error
	 */
	ret = dio->page_errors;
	if (ret == 0) {
<<<<<<< HEAD
<<<<<<< HEAD
		BUG_ON(dio->block_in_file >= dio->final_block_in_request);
		fs_startblk = dio->block_in_file >> dio->blkfactor;
		dio_count = dio->final_block_in_request - dio->block_in_file;
		fs_count = dio_count >> dio->blkfactor;
		blkmask = (1 << dio->blkfactor) - 1;
		if (dio_count & blkmask)	
			fs_count++;
=======
=======
>>>>>>> refs/remotes/origin/master
		BUG_ON(sdio->block_in_file >= sdio->final_block_in_request);
		fs_startblk = sdio->block_in_file >> sdio->blkfactor;
		fs_endblk = (sdio->final_block_in_request - 1) >>
					sdio->blkfactor;
		fs_count = fs_endblk - fs_startblk + 1;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

		map_bh->b_state = 0;
		map_bh->b_size = fs_count << dio->inode->i_blkbits;
=======

		map_bh->b_state = 0;
		map_bh->b_size = fs_count << i_blkbits;
>>>>>>> refs/remotes/origin/master

		/*
		 * For writes inside i_size on a DIO_SKIP_HOLES filesystem we
		 * forbid block creations: only overwrites are permitted.
		 * We will return early to the caller once we see an
		 * unmapped buffer head returned, and the caller will fall
		 * back to buffered I/O.
		 *
		 * Otherwise the decision is left to the get_blocks method,
		 * which may decide to handle it or also return an unmapped
		 * buffer head.
		 */
		create = dio->rw & WRITE;
		if (dio->flags & DIO_SKIP_HOLES) {
<<<<<<< HEAD
<<<<<<< HEAD
			if (dio->block_in_file < (i_size_read(dio->inode) >>
							dio->blkbits))
				create = 0;
		}

		ret = (*dio->get_block)(dio->inode, fs_startblk,
						map_bh, create);
=======
=======
>>>>>>> refs/remotes/origin/master
			if (sdio->block_in_file < (i_size_read(dio->inode) >>
							sdio->blkbits))
				create = 0;
		}

		ret = (*sdio->get_block)(dio->inode, fs_startblk,
						map_bh, create);

		/* Store for completion */
		dio->private = map_bh->b_private;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======

		if (ret == 0 && buffer_defer_completion(map_bh))
			ret = dio_set_defer_completion(dio);
>>>>>>> refs/remotes/origin/master
	}
	return ret;
}

/*
 * There is no bio.  Make one now.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int dio_new_bio(struct dio *dio, sector_t start_sector)
=======
static inline int dio_new_bio(struct dio *dio, struct dio_submit *sdio,
		sector_t start_sector, struct buffer_head *map_bh)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline int dio_new_bio(struct dio *dio, struct dio_submit *sdio,
		sector_t start_sector, struct buffer_head *map_bh)
>>>>>>> refs/remotes/origin/master
{
	sector_t sector;
	int ret, nr_pages;

<<<<<<< HEAD
<<<<<<< HEAD
	ret = dio_bio_reap(dio);
	if (ret)
		goto out;
	sector = start_sector << (dio->blkbits - 9);
	nr_pages = min(dio->pages_in_io, bio_get_nr_vecs(dio->map_bh.b_bdev));
	nr_pages = min(nr_pages, BIO_MAX_PAGES);
	BUG_ON(nr_pages <= 0);
	dio_bio_alloc(dio, dio->map_bh.b_bdev, sector, nr_pages);
	dio->boundary = 0;
=======
=======
>>>>>>> refs/remotes/origin/master
	ret = dio_bio_reap(dio, sdio);
	if (ret)
		goto out;
	sector = start_sector << (sdio->blkbits - 9);
	nr_pages = min(sdio->pages_in_io, bio_get_nr_vecs(map_bh->b_bdev));
	nr_pages = min(nr_pages, BIO_MAX_PAGES);
	BUG_ON(nr_pages <= 0);
	dio_bio_alloc(dio, sdio, map_bh->b_bdev, sector, nr_pages);
	sdio->boundary = 0;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
out:
	return ret;
}

/*
 * Attempt to put the current chunk of 'cur_page' into the current BIO.  If
 * that was successful then update final_block_in_bio and take a ref against
 * the just-added page.
 *
 * Return zero on success.  Non-zero means the caller needs to start a new BIO.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int dio_bio_add_page(struct dio *dio)
{
	int ret;

	ret = bio_add_page(dio->bio, dio->cur_page,
			dio->cur_page_len, dio->cur_page_offset);
	if (ret == dio->cur_page_len) {
		/*
		 * Decrement count only, if we are done with this page
		 */
		if ((dio->cur_page_len + dio->cur_page_offset) == PAGE_SIZE)
			dio->pages_in_io--;
		page_cache_get(dio->cur_page);
		dio->final_block_in_bio = dio->cur_page_block +
			(dio->cur_page_len >> dio->blkbits);
=======
=======
>>>>>>> refs/remotes/origin/master
static inline int dio_bio_add_page(struct dio_submit *sdio)
{
	int ret;

	ret = bio_add_page(sdio->bio, sdio->cur_page,
			sdio->cur_page_len, sdio->cur_page_offset);
	if (ret == sdio->cur_page_len) {
		/*
		 * Decrement count only, if we are done with this page
		 */
		if ((sdio->cur_page_len + sdio->cur_page_offset) == PAGE_SIZE)
			sdio->pages_in_io--;
		page_cache_get(sdio->cur_page);
		sdio->final_block_in_bio = sdio->cur_page_block +
			(sdio->cur_page_len >> sdio->blkbits);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		ret = 0;
	} else {
		ret = 1;
	}
	return ret;
}
		
/*
 * Put cur_page under IO.  The section of cur_page which is described by
 * cur_page_offset,cur_page_len is put into a BIO.  The section of cur_page
 * starts on-disk at cur_page_block.
 *
 * We take a ref against the page here (on behalf of its presence in the bio).
 *
 * The caller of this function is responsible for removing cur_page from the
 * dio, and for dropping the refcount which came from that presence.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int dio_send_cur_page(struct dio *dio)
{
	int ret = 0;

	if (dio->bio) {
		loff_t cur_offset = dio->cur_page_fs_offset;
		loff_t bio_next_offset = dio->logical_offset_in_bio +
			dio->bio->bi_size;
=======
=======
>>>>>>> refs/remotes/origin/master
static inline int dio_send_cur_page(struct dio *dio, struct dio_submit *sdio,
		struct buffer_head *map_bh)
{
	int ret = 0;

	if (sdio->bio) {
		loff_t cur_offset = sdio->cur_page_fs_offset;
		loff_t bio_next_offset = sdio->logical_offset_in_bio +
			sdio->bio->bi_size;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

		/*
		 * See whether this new request is contiguous with the old.
		 *
		 * Btrfs cannot handle having logically non-contiguous requests
		 * submitted.  For example if you have
		 *
		 * Logical:  [0-4095][HOLE][8192-12287]
		 * Physical: [0-4095]      [4096-8191]
		 *
		 * We cannot submit those pages together as one BIO.  So if our
		 * current logical offset in the file does not equal what would
		 * be the next logical offset in the bio, submit the bio we
		 * have.
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		if (dio->final_block_in_bio != dio->cur_page_block ||
		    cur_offset != bio_next_offset)
			dio_bio_submit(dio);
=======
		if (sdio->final_block_in_bio != sdio->cur_page_block ||
		    cur_offset != bio_next_offset)
			dio_bio_submit(dio, sdio);
>>>>>>> refs/remotes/origin/cm-10.0
		/*
		 * Submit now if the underlying fs is about to perform a
		 * metadata read
		 */
<<<<<<< HEAD
		else if (dio->boundary)
			dio_bio_submit(dio);
	}

	if (dio->bio == NULL) {
		ret = dio_new_bio(dio, dio->cur_page_block);
=======
		else if (sdio->boundary)
			dio_bio_submit(dio, sdio);
=======
		if (sdio->final_block_in_bio != sdio->cur_page_block ||
		    cur_offset != bio_next_offset)
			dio_bio_submit(dio, sdio);
>>>>>>> refs/remotes/origin/master
	}

	if (sdio->bio == NULL) {
		ret = dio_new_bio(dio, sdio, sdio->cur_page_block, map_bh);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (ret)
			goto out;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (dio_bio_add_page(dio) != 0) {
		dio_bio_submit(dio);
		ret = dio_new_bio(dio, dio->cur_page_block);
		if (ret == 0) {
			ret = dio_bio_add_page(dio);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (dio_bio_add_page(sdio) != 0) {
		dio_bio_submit(dio, sdio);
		ret = dio_new_bio(dio, sdio, sdio->cur_page_block, map_bh);
		if (ret == 0) {
			ret = dio_bio_add_page(sdio);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			BUG_ON(ret != 0);
		}
	}
out:
	return ret;
}

/*
 * An autonomous function to put a chunk of a page under deferred IO.
 *
 * The caller doesn't actually know (or care) whether this piece of page is in
 * a BIO, or is under IO or whatever.  We just take care of all possible 
 * situations here.  The separation between the logic of do_direct_IO() and
 * that of submit_page_section() is important for clarity.  Please don't break.
 *
 * The chunk of page starts on-disk at blocknr.
 *
 * We perform deferred IO, by recording the last-submitted page inside our
 * private part of the dio structure.  If possible, we just expand the IO
 * across that page here.
 *
 * If that doesn't work out then we put the old page into the bio and add this
 * page to the dio instead.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int
submit_page_section(struct dio *dio, struct page *page,
		unsigned offset, unsigned len, sector_t blocknr)
=======
=======
>>>>>>> refs/remotes/origin/master
static inline int
submit_page_section(struct dio *dio, struct dio_submit *sdio, struct page *page,
		    unsigned offset, unsigned len, sector_t blocknr,
		    struct buffer_head *map_bh)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	int ret = 0;

	if (dio->rw & WRITE) {
		/*
		 * Read accounting is performed in submit_bio()
		 */
		task_io_account_write(len);
	}

	/*
	 * Can we just grow the current page's presence in the dio?
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (	(dio->cur_page == page) &&
		(dio->cur_page_offset + dio->cur_page_len == offset) &&
		(dio->cur_page_block +
			(dio->cur_page_len >> dio->blkbits) == blocknr)) {
		dio->cur_page_len += len;

		/*
		 * If dio->boundary then we want to schedule the IO now to
		 * avoid metadata seeks.
		 */
		if (dio->boundary) {
			ret = dio_send_cur_page(dio);
			page_cache_release(dio->cur_page);
			dio->cur_page = NULL;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (sdio->cur_page == page &&
	    sdio->cur_page_offset + sdio->cur_page_len == offset &&
	    sdio->cur_page_block +
	    (sdio->cur_page_len >> sdio->blkbits) == blocknr) {
		sdio->cur_page_len += len;
<<<<<<< HEAD

		/*
		 * If sdio->boundary then we want to schedule the IO now to
		 * avoid metadata seeks.
		 */
		if (sdio->boundary) {
			ret = dio_send_cur_page(dio, sdio, map_bh);
			page_cache_release(sdio->cur_page);
			sdio->cur_page = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
		}
=======
>>>>>>> refs/remotes/origin/master
		goto out;
	}

	/*
	 * If there's a deferred page already there then send it.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (dio->cur_page) {
		ret = dio_send_cur_page(dio);
		page_cache_release(dio->cur_page);
		dio->cur_page = NULL;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (sdio->cur_page) {
		ret = dio_send_cur_page(dio, sdio, map_bh);
		page_cache_release(sdio->cur_page);
		sdio->cur_page = NULL;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		if (ret)
			goto out;
	}

	page_cache_get(page);		/* It is in dio */
<<<<<<< HEAD
	dio->cur_page = page;
	dio->cur_page_offset = offset;
	dio->cur_page_len = len;
	dio->cur_page_block = blocknr;
	dio->cur_page_fs_offset = dio->block_in_file << dio->blkbits;
=======
=======
		if (ret)
			return ret;
	}

	page_cache_get(page);		/* It is in dio */
>>>>>>> refs/remotes/origin/master
	sdio->cur_page = page;
	sdio->cur_page_offset = offset;
	sdio->cur_page_len = len;
	sdio->cur_page_block = blocknr;
	sdio->cur_page_fs_offset = sdio->block_in_file << sdio->blkbits;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
out:
=======
out:
	/*
	 * If sdio->boundary then we want to schedule the IO now to
	 * avoid metadata seeks.
	 */
	if (sdio->boundary) {
		ret = dio_send_cur_page(dio, sdio, map_bh);
		dio_bio_submit(dio, sdio);
		page_cache_release(sdio->cur_page);
		sdio->cur_page = NULL;
	}
>>>>>>> refs/remotes/origin/master
	return ret;
}

/*
 * Clean any dirty buffers in the blockdev mapping which alias newly-created
 * file blocks.  Only called for S_ISREG files - blockdevs do not set
 * buffer_new
 */
<<<<<<< HEAD
<<<<<<< HEAD
static void clean_blockdev_aliases(struct dio *dio)
=======
static void clean_blockdev_aliases(struct dio *dio, struct buffer_head *map_bh)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void clean_blockdev_aliases(struct dio *dio, struct buffer_head *map_bh)
>>>>>>> refs/remotes/origin/master
{
	unsigned i;
	unsigned nblocks;

<<<<<<< HEAD
<<<<<<< HEAD
	nblocks = dio->map_bh.b_size >> dio->inode->i_blkbits;

	for (i = 0; i < nblocks; i++) {
		unmap_underlying_metadata(dio->map_bh.b_bdev,
					dio->map_bh.b_blocknr + i);
=======
=======
>>>>>>> refs/remotes/origin/master
	nblocks = map_bh->b_size >> dio->inode->i_blkbits;

	for (i = 0; i < nblocks; i++) {
		unmap_underlying_metadata(map_bh->b_bdev,
					  map_bh->b_blocknr + i);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
}

/*
 * If we are not writing the entire block and get_block() allocated
 * the block for us, we need to fill-in the unused portion of the
 * block with zeros. This happens only if user-buffer, fileoffset or
 * io length is not filesystem block-size multiple.
 *
 * `end' is zero if we're doing the start of the IO, 1 at the end of the
 * IO.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static void dio_zero_block(struct dio *dio, int end)
=======
static inline void dio_zero_block(struct dio *dio, struct dio_submit *sdio,
		int end, struct buffer_head *map_bh)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline void dio_zero_block(struct dio *dio, struct dio_submit *sdio,
		int end, struct buffer_head *map_bh)
>>>>>>> refs/remotes/origin/master
{
	unsigned dio_blocks_per_fs_block;
	unsigned this_chunk_blocks;	/* In dio_blocks */
	unsigned this_chunk_bytes;
	struct page *page;

<<<<<<< HEAD
<<<<<<< HEAD
	dio->start_zero_done = 1;
	if (!dio->blkfactor || !buffer_new(&dio->map_bh))
		return;

	dio_blocks_per_fs_block = 1 << dio->blkfactor;
	this_chunk_blocks = dio->block_in_file & (dio_blocks_per_fs_block - 1);
=======
=======
>>>>>>> refs/remotes/origin/master
	sdio->start_zero_done = 1;
	if (!sdio->blkfactor || !buffer_new(map_bh))
		return;

	dio_blocks_per_fs_block = 1 << sdio->blkfactor;
	this_chunk_blocks = sdio->block_in_file & (dio_blocks_per_fs_block - 1);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (!this_chunk_blocks)
		return;

	/*
	 * We need to zero out part of an fs block.  It is either at the
	 * beginning or the end of the fs block.
	 */
	if (end) 
		this_chunk_blocks = dio_blocks_per_fs_block - this_chunk_blocks;

<<<<<<< HEAD
<<<<<<< HEAD
	this_chunk_bytes = this_chunk_blocks << dio->blkbits;

	page = ZERO_PAGE(0);
	if (submit_page_section(dio, page, 0, this_chunk_bytes, 
				dio->next_block_for_io))
		return;

	dio->next_block_for_io += this_chunk_blocks;
=======
=======
>>>>>>> refs/remotes/origin/master
	this_chunk_bytes = this_chunk_blocks << sdio->blkbits;

	page = ZERO_PAGE(0);
	if (submit_page_section(dio, sdio, page, 0, this_chunk_bytes,
				sdio->next_block_for_io, map_bh))
		return;

	sdio->next_block_for_io += this_chunk_blocks;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/*
 * Walk the user pages, and the file, mapping blocks to disk and generating
 * a sequence of (page,offset,len,block) mappings.  These mappings are injected
 * into submit_page_section(), which takes care of the next stage of submission
 *
 * Direct IO against a blockdev is different from a file.  Because we can
 * happily perform page-sized but 512-byte aligned IOs.  It is important that
 * blockdev IO be able to have fine alignment and large sizes.
 *
 * So what we do is to permit the ->get_block function to populate bh.b_size
 * with the size of IO which is permitted at this offset and this i_blkbits.
 *
 * For best results, the blockdev should be set up with 512-byte i_blkbits and
 * it should set b_size to PAGE_SIZE or more inside get_block().  This gives
 * fine alignment but still allows this function to work in PAGE_SIZE units.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int do_direct_IO(struct dio *dio)
{
	const unsigned blkbits = dio->blkbits;
	const unsigned blocks_per_page = PAGE_SIZE >> blkbits;
	struct page *page;
	unsigned block_in_page;
	struct buffer_head *map_bh = &dio->map_bh;
	int ret = 0;

	/* The I/O can start at any block offset within the first page */
	block_in_page = dio->first_block_in_page;

	while (dio->block_in_file < dio->final_block_in_request) {
		page = dio_get_page(dio);
=======
=======
>>>>>>> refs/remotes/origin/master
static int do_direct_IO(struct dio *dio, struct dio_submit *sdio,
			struct buffer_head *map_bh)
{
	const unsigned blkbits = sdio->blkbits;
	const unsigned blocks_per_page = PAGE_SIZE >> blkbits;
	struct page *page;
	unsigned block_in_page;
	int ret = 0;

	/* The I/O can start at any block offset within the first page */
	block_in_page = sdio->first_block_in_page;

	while (sdio->block_in_file < sdio->final_block_in_request) {
		page = dio_get_page(dio, sdio);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (IS_ERR(page)) {
			ret = PTR_ERR(page);
			goto out;
		}

		while (block_in_page < blocks_per_page) {
			unsigned offset_in_page = block_in_page << blkbits;
			unsigned this_chunk_bytes;	/* # of bytes mapped */
			unsigned this_chunk_blocks;	/* # of blocks */
			unsigned u;

<<<<<<< HEAD
<<<<<<< HEAD
			if (dio->blocks_available == 0) {
=======
			if (sdio->blocks_available == 0) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
			if (sdio->blocks_available == 0) {
>>>>>>> refs/remotes/origin/master
				/*
				 * Need to go and map some more disk
				 */
				unsigned long blkmask;
				unsigned long dio_remainder;

<<<<<<< HEAD
<<<<<<< HEAD
				ret = get_more_blocks(dio);
=======
				ret = get_more_blocks(dio, sdio, map_bh);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				ret = get_more_blocks(dio, sdio, map_bh);
>>>>>>> refs/remotes/origin/master
				if (ret) {
					page_cache_release(page);
					goto out;
				}
				if (!buffer_mapped(map_bh))
					goto do_holes;

<<<<<<< HEAD
<<<<<<< HEAD
				dio->blocks_available =
						map_bh->b_size >> dio->blkbits;
				dio->next_block_for_io =
					map_bh->b_blocknr << dio->blkfactor;
				if (buffer_new(map_bh))
					clean_blockdev_aliases(dio);

				if (!dio->blkfactor)
					goto do_holes;

				blkmask = (1 << dio->blkfactor) - 1;
				dio_remainder = (dio->block_in_file & blkmask);
=======
=======
>>>>>>> refs/remotes/origin/master
				sdio->blocks_available =
						map_bh->b_size >> sdio->blkbits;
				sdio->next_block_for_io =
					map_bh->b_blocknr << sdio->blkfactor;
				if (buffer_new(map_bh))
					clean_blockdev_aliases(dio, map_bh);

				if (!sdio->blkfactor)
					goto do_holes;

				blkmask = (1 << sdio->blkfactor) - 1;
				dio_remainder = (sdio->block_in_file & blkmask);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

				/*
				 * If we are at the start of IO and that IO
				 * starts partway into a fs-block,
				 * dio_remainder will be non-zero.  If the IO
				 * is a read then we can simply advance the IO
				 * cursor to the first block which is to be
				 * read.  But if the IO is a write and the
				 * block was newly allocated we cannot do that;
				 * the start of the fs block must be zeroed out
				 * on-disk
				 */
				if (!buffer_new(map_bh))
<<<<<<< HEAD
<<<<<<< HEAD
					dio->next_block_for_io += dio_remainder;
				dio->blocks_available -= dio_remainder;
=======
					sdio->next_block_for_io += dio_remainder;
				sdio->blocks_available -= dio_remainder;
>>>>>>> refs/remotes/origin/cm-10.0
=======
					sdio->next_block_for_io += dio_remainder;
				sdio->blocks_available -= dio_remainder;
>>>>>>> refs/remotes/origin/master
			}
do_holes:
			/* Handle holes */
			if (!buffer_mapped(map_bh)) {
				loff_t i_size_aligned;

				/* AKPM: eargh, -ENOTBLK is a hack */
				if (dio->rw & WRITE) {
					page_cache_release(page);
					return -ENOTBLK;
				}

				/*
				 * Be sure to account for a partial block as the
				 * last block in the file
				 */
				i_size_aligned = ALIGN(i_size_read(dio->inode),
							1 << blkbits);
<<<<<<< HEAD
<<<<<<< HEAD
				if (dio->block_in_file >=
=======
				if (sdio->block_in_file >=
>>>>>>> refs/remotes/origin/cm-10.0
=======
				if (sdio->block_in_file >=
>>>>>>> refs/remotes/origin/master
						i_size_aligned >> blkbits) {
					/* We hit eof */
					page_cache_release(page);
					goto out;
				}
				zero_user(page, block_in_page << blkbits,
						1 << blkbits);
<<<<<<< HEAD
<<<<<<< HEAD
				dio->block_in_file++;
=======
				sdio->block_in_file++;
>>>>>>> refs/remotes/origin/cm-10.0
=======
				sdio->block_in_file++;
>>>>>>> refs/remotes/origin/master
				block_in_page++;
				goto next_block;
			}

			/*
			 * If we're performing IO which has an alignment which
			 * is finer than the underlying fs, go check to see if
			 * we must zero out the start of this block.
			 */
<<<<<<< HEAD
<<<<<<< HEAD
			if (unlikely(dio->blkfactor && !dio->start_zero_done))
				dio_zero_block(dio, 0);
=======
			if (unlikely(sdio->blkfactor && !sdio->start_zero_done))
				dio_zero_block(dio, sdio, 0, map_bh);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			if (unlikely(sdio->blkfactor && !sdio->start_zero_done))
				dio_zero_block(dio, sdio, 0, map_bh);
>>>>>>> refs/remotes/origin/master

			/*
			 * Work out, in this_chunk_blocks, how much disk we
			 * can add to this page
			 */
<<<<<<< HEAD
<<<<<<< HEAD
			this_chunk_blocks = dio->blocks_available;
			u = (PAGE_SIZE - offset_in_page) >> blkbits;
			if (this_chunk_blocks > u)
				this_chunk_blocks = u;
			u = dio->final_block_in_request - dio->block_in_file;
=======
=======
>>>>>>> refs/remotes/origin/master
			this_chunk_blocks = sdio->blocks_available;
			u = (PAGE_SIZE - offset_in_page) >> blkbits;
			if (this_chunk_blocks > u)
				this_chunk_blocks = u;
			u = sdio->final_block_in_request - sdio->block_in_file;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			if (this_chunk_blocks > u)
				this_chunk_blocks = u;
			this_chunk_bytes = this_chunk_blocks << blkbits;
			BUG_ON(this_chunk_bytes == 0);

<<<<<<< HEAD
<<<<<<< HEAD
			dio->boundary = buffer_boundary(map_bh);
			ret = submit_page_section(dio, page, offset_in_page,
				this_chunk_bytes, dio->next_block_for_io);
=======
			sdio->boundary = buffer_boundary(map_bh);
=======
			if (this_chunk_blocks == sdio->blocks_available)
				sdio->boundary = buffer_boundary(map_bh);
>>>>>>> refs/remotes/origin/master
			ret = submit_page_section(dio, sdio, page,
						  offset_in_page,
						  this_chunk_bytes,
						  sdio->next_block_for_io,
						  map_bh);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			if (ret) {
				page_cache_release(page);
				goto out;
			}
<<<<<<< HEAD
<<<<<<< HEAD
			dio->next_block_for_io += this_chunk_blocks;

			dio->block_in_file += this_chunk_blocks;
			block_in_page += this_chunk_blocks;
			dio->blocks_available -= this_chunk_blocks;
next_block:
			BUG_ON(dio->block_in_file > dio->final_block_in_request);
			if (dio->block_in_file == dio->final_block_in_request)
=======
=======
>>>>>>> refs/remotes/origin/master
			sdio->next_block_for_io += this_chunk_blocks;

			sdio->block_in_file += this_chunk_blocks;
			block_in_page += this_chunk_blocks;
			sdio->blocks_available -= this_chunk_blocks;
next_block:
			BUG_ON(sdio->block_in_file > sdio->final_block_in_request);
			if (sdio->block_in_file == sdio->final_block_in_request)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				break;
		}

		/* Drop the ref which was taken in get_user_pages() */
		page_cache_release(page);
		block_in_page = 0;
	}
out:
	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * Releases both i_mutex and i_alloc_sem
 */
static ssize_t
direct_io_worker(int rw, struct kiocb *iocb, struct inode *inode, 
	const struct iovec *iov, loff_t offset, unsigned long nr_segs, 
	unsigned blkbits, get_block_t get_block, dio_iodone_t end_io,
	dio_submit_t submit_io, struct dio *dio)
{
	unsigned long user_addr; 
	unsigned long flags;
	int seg;
	ssize_t ret = 0;
	ssize_t ret2;
	size_t bytes;

	dio->inode = inode;
	dio->rw = rw;
	dio->blkbits = blkbits;
	dio->blkfactor = inode->i_blkbits - blkbits;
	dio->block_in_file = offset >> blkbits;

	dio->get_block = get_block;
	dio->end_io = end_io;
	dio->submit_io = submit_io;
	dio->final_block_in_bio = -1;
	dio->next_block_for_io = -1;

	dio->iocb = iocb;
	dio->i_size = i_size_read(inode);

	spin_lock_init(&dio->bio_lock);
	dio->refcount = 1;

	/*
	 * In case of non-aligned buffers, we may need 2 more
	 * pages since we need to zero out first and last block.
	 */
	if (unlikely(dio->blkfactor))
		dio->pages_in_io = 2;

	for (seg = 0; seg < nr_segs; seg++) {
		user_addr = (unsigned long)iov[seg].iov_base;
		dio->pages_in_io +=
			((user_addr+iov[seg].iov_len +PAGE_SIZE-1)/PAGE_SIZE
				- user_addr/PAGE_SIZE);
	}

	for (seg = 0; seg < nr_segs; seg++) {
		user_addr = (unsigned long)iov[seg].iov_base;
		dio->size += bytes = iov[seg].iov_len;

		/* Index into the first page of the first block */
		dio->first_block_in_page = (user_addr & ~PAGE_MASK) >> blkbits;
		dio->final_block_in_request = dio->block_in_file +
						(bytes >> blkbits);
		/* Page fetching state */
		dio->head = 0;
		dio->tail = 0;
		dio->curr_page = 0;

		dio->total_pages = 0;
		if (user_addr & (PAGE_SIZE-1)) {
			dio->total_pages++;
			bytes -= PAGE_SIZE - (user_addr & (PAGE_SIZE - 1));
		}
		dio->total_pages += (bytes + PAGE_SIZE - 1) / PAGE_SIZE;
		dio->curr_user_address = user_addr;
	
		ret = do_direct_IO(dio);

		dio->result += iov[seg].iov_len -
			((dio->final_block_in_request - dio->block_in_file) <<
					blkbits);

		if (ret) {
			dio_cleanup(dio);
			break;
		}
	} /* end iovec loop */

	if (ret == -ENOTBLK) {
		/*
		 * The remaining part of the request will be
		 * be handled by buffered I/O when we return
		 */
		ret = 0;
	}
	/*
	 * There may be some unwritten disk at the end of a part-written
	 * fs-block-sized block.  Go zero that now.
	 */
	dio_zero_block(dio, 1);

	if (dio->cur_page) {
		ret2 = dio_send_cur_page(dio);
		if (ret == 0)
			ret = ret2;
		page_cache_release(dio->cur_page);
		dio->cur_page = NULL;
	}
	if (dio->bio)
		dio_bio_submit(dio);

	/*
	 * It is possible that, we return short IO due to end of file.
	 * In that case, we need to release all the pages we got hold on.
	 */
	dio_cleanup(dio);

	/*
	 * All block lookups have been performed. For READ requests
	 * we can let i_mutex go now that its achieved its purpose
	 * of protecting us from looking up uninitialized blocks.
	 */
	if (rw == READ && (dio->flags & DIO_LOCKING))
		mutex_unlock(&dio->inode->i_mutex);

	/*
	 * The only time we want to leave bios in flight is when a successful
	 * partial aio read or full aio write have been setup.  In that case
	 * bio completion will call aio_complete.  The only time it's safe to
	 * call aio_complete is when we return -EIOCBQUEUED, so we key on that.
	 * This had *better* be the only place that raises -EIOCBQUEUED.
	 */
	BUG_ON(ret == -EIOCBQUEUED);
	if (dio->is_async && ret == 0 && dio->result &&
	    ((rw & READ) || (dio->result == dio->size)))
		ret = -EIOCBQUEUED;

	if (ret != -EIOCBQUEUED)
		dio_await_completion(dio);
=======
=======
>>>>>>> refs/remotes/origin/master
static inline int drop_refcount(struct dio *dio)
{
	int ret2;
	unsigned long flags;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/*
	 * Sync will always be dropping the final ref and completing the
	 * operation.  AIO can if it was a broken operation described above or
	 * in fact if all the bios race to complete before we get here.  In
	 * that case dio_complete() translates the EIOCBQUEUED into the proper
	 * return code that the caller will hand to aio_complete().
	 *
	 * This is managed by the bio_lock instead of being an atomic_t so that
	 * completion paths can drop their ref and use the remaining count to
	 * decide to wake the submission path atomically.
	 */
	spin_lock_irqsave(&dio->bio_lock, flags);
	ret2 = --dio->refcount;
	spin_unlock_irqrestore(&dio->bio_lock, flags);
<<<<<<< HEAD
<<<<<<< HEAD

	if (ret2 == 0) {
		ret = dio_complete(dio, offset, ret, false);
		kfree(dio);
	} else
		BUG_ON(ret != -EIOCBQUEUED);

	return ret;
=======
	return ret2;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return ret2;
>>>>>>> refs/remotes/origin/master
}

/*
 * This is a library function for use by filesystem drivers.
 *
 * The locking rules are governed by the flags parameter:
 *  - if the flags value contains DIO_LOCKING we use a fancy locking
 *    scheme for dumb filesystems.
 *    For writes this function is called under i_mutex and returns with
 *    i_mutex held, for reads, i_mutex is not held on entry, but it is
 *    taken and dropped again before returning.
<<<<<<< HEAD
<<<<<<< HEAD
 *    For reads and writes i_alloc_sem is taken in shared mode and released
 *    on I/O completion (which may happen asynchronously after returning to
 *    the caller).
 *
 *  - if the flags value does NOT contain DIO_LOCKING we don't use any
 *    internal locking but rather rely on the filesystem to synchronize
 *    direct I/O reads/writes versus each other and truncate.
 *    For reads and writes both i_mutex and i_alloc_sem are not held on
 *    entry and are never taken.
 */
ssize_t
__blockdev_direct_IO(int rw, struct kiocb *iocb, struct inode *inode,
=======
=======
>>>>>>> refs/remotes/origin/master
 *  - if the flags value does NOT contain DIO_LOCKING we don't use any
 *    internal locking but rather rely on the filesystem to synchronize
 *    direct I/O reads/writes versus each other and truncate.
 *
 * To help with locking against truncate we incremented the i_dio_count
 * counter before starting direct I/O, and decrement it once we are done.
 * Truncate can wait for it to reach zero to provide exclusion.  It is
 * expected that filesystem provide exclusion between new direct I/O
 * and truncates.  For DIO_LOCKING filesystems this is done by i_mutex,
 * but other filesystems need to take care of this on their own.
 *
 * NOTE: if you pass "sdio" to anything by pointer make sure that function
 * is always inlined. Otherwise gcc is unable to split the structure into
 * individual fields and will generate much worse code. This is important
 * for the whole file.
 */
static inline ssize_t
do_blockdev_direct_IO(int rw, struct kiocb *iocb, struct inode *inode,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct block_device *bdev, const struct iovec *iov, loff_t offset, 
	unsigned long nr_segs, get_block_t get_block, dio_iodone_t end_io,
	dio_submit_t submit_io,	int flags)
{
	int seg;
	size_t size;
	unsigned long addr;
<<<<<<< HEAD
	unsigned blkbits = inode->i_blkbits;
<<<<<<< HEAD
	unsigned bdev_blkbits = 0;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned i_blkbits = ACCESS_ONCE(inode->i_blkbits);
	unsigned blkbits = i_blkbits;
>>>>>>> refs/remotes/origin/master
	unsigned blocksize_mask = (1 << blkbits) - 1;
	ssize_t retval = -EINVAL;
	loff_t end = offset;
	struct dio *dio;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	struct dio_submit sdio = { 0, };
	unsigned long user_addr;
	size_t bytes;
	struct buffer_head map_bh = { 0, };
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct blk_plug plug;
>>>>>>> refs/remotes/origin/master

	if (rw & WRITE)
		rw = WRITE_ODIRECT;

<<<<<<< HEAD
<<<<<<< HEAD
	if (bdev)
		bdev_blkbits = blksize_bits(bdev_logical_block_size(bdev));

	if (offset & blocksize_mask) {
		if (bdev)
			 blkbits = bdev_blkbits;
=======
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Avoid references to bdev if not absolutely needed to give
	 * the early prefetch in the caller enough time.
	 */

	if (offset & blocksize_mask) {
		if (bdev)
			blkbits = blksize_bits(bdev_logical_block_size(bdev));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		blocksize_mask = (1 << blkbits) - 1;
		if (offset & blocksize_mask)
			goto out;
	}

	/* Check the memory alignment.  Blocks cannot straddle pages */
	for (seg = 0; seg < nr_segs; seg++) {
		addr = (unsigned long)iov[seg].iov_base;
		size = iov[seg].iov_len;
		end += size;
<<<<<<< HEAD
<<<<<<< HEAD
		if ((addr & blocksize_mask) || (size & blocksize_mask))  {
			if (bdev)
				 blkbits = bdev_blkbits;
			blocksize_mask = (1 << blkbits) - 1;
			if ((addr & blocksize_mask) || (size & blocksize_mask))  
=======
=======
>>>>>>> refs/remotes/origin/master
		if (unlikely((addr & blocksize_mask) ||
			     (size & blocksize_mask))) {
			if (bdev)
				blkbits = blksize_bits(
					 bdev_logical_block_size(bdev));
			blocksize_mask = (1 << blkbits) - 1;
			if ((addr & blocksize_mask) || (size & blocksize_mask))
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				goto out;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	dio = kmalloc(sizeof(*dio), GFP_KERNEL);
=======
=======
>>>>>>> refs/remotes/origin/master
	/* watch out for a 0 len io from a tricksy fs */
	if (rw == READ && end == offset)
		return 0;

	dio = kmem_cache_alloc(dio_cache, GFP_KERNEL);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	retval = -ENOMEM;
	if (!dio)
		goto out;
	/*
	 * Believe it or not, zeroing out the page array caused a .5%
	 * performance regression in a database benchmark.  So, we take
	 * care to only zero out what's needed.
	 */
	memset(dio, 0, offsetof(struct dio, pages));

	dio->flags = flags;
	if (dio->flags & DIO_LOCKING) {
<<<<<<< HEAD
<<<<<<< HEAD
		/* watch out for a 0 len io from a tricksy fs */
		if (rw == READ && end > offset) {
=======
		if (rw == READ) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (rw == READ) {
>>>>>>> refs/remotes/origin/master
			struct address_space *mapping =
					iocb->ki_filp->f_mapping;

			/* will be released by direct_io_worker */
			mutex_lock(&inode->i_mutex);

			retval = filemap_write_and_wait_range(mapping, offset,
							      end - 1);
			if (retval) {
				mutex_unlock(&inode->i_mutex);
<<<<<<< HEAD
<<<<<<< HEAD
				kfree(dio);
				goto out;
			}
		}

		/*
		 * Will be released at I/O completion, possibly in a
		 * different thread.
		 */
		down_read_non_owner(&inode->i_alloc_sem);
	}

	/*
=======
=======
>>>>>>> refs/remotes/origin/master
				kmem_cache_free(dio_cache, dio);
				goto out;
			}
		}
	}

	/*
<<<<<<< HEAD
	 * Will be decremented at I/O completion time.
	 */
	atomic_inc(&inode->i_dio_count);

	/*
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	 * For file extending writes updating i_size before data
	 * writeouts complete can expose uninitialized blocks. So
	 * even for AIO, we need to wait for i/o to complete before
	 * returning in this case.
	 */
	dio->is_async = !is_sync_kiocb(iocb) && !((rw & WRITE) &&
		(end > i_size_read(inode)));
<<<<<<< HEAD

<<<<<<< HEAD
	retval = direct_io_worker(rw, iocb, inode, iov, offset,
				nr_segs, blkbits, get_block, end_io,
				submit_io, dio);
=======
	retval = 0;

	dio->inode = inode;
	dio->rw = rw;
	sdio.blkbits = blkbits;
	sdio.blkfactor = inode->i_blkbits - blkbits;
=======
	dio->inode = inode;
	dio->rw = rw;

	/*
	 * For AIO O_(D)SYNC writes we need to defer completions to a workqueue
	 * so that we can call ->fsync.
	 */
	if (dio->is_async && (rw & WRITE) &&
	    ((iocb->ki_filp->f_flags & O_DSYNC) ||
	     IS_SYNC(iocb->ki_filp->f_mapping->host))) {
		retval = dio_set_defer_completion(dio);
		if (retval) {
			/*
			 * We grab i_mutex only for reads so we don't have
			 * to release it here
			 */
			kmem_cache_free(dio_cache, dio);
			goto out;
		}
	}

	/*
	 * Will be decremented at I/O completion time.
	 */
	atomic_inc(&inode->i_dio_count);

	retval = 0;
	sdio.blkbits = blkbits;
	sdio.blkfactor = i_blkbits - blkbits;
>>>>>>> refs/remotes/origin/master
	sdio.block_in_file = offset >> blkbits;

	sdio.get_block = get_block;
	dio->end_io = end_io;
	sdio.submit_io = submit_io;
	sdio.final_block_in_bio = -1;
	sdio.next_block_for_io = -1;

	dio->iocb = iocb;
	dio->i_size = i_size_read(inode);

	spin_lock_init(&dio->bio_lock);
	dio->refcount = 1;

	/*
	 * In case of non-aligned buffers, we may need 2 more
	 * pages since we need to zero out first and last block.
	 */
	if (unlikely(sdio.blkfactor))
		sdio.pages_in_io = 2;

	for (seg = 0; seg < nr_segs; seg++) {
		user_addr = (unsigned long)iov[seg].iov_base;
		sdio.pages_in_io +=
			((user_addr + iov[seg].iov_len + PAGE_SIZE-1) /
				PAGE_SIZE - user_addr / PAGE_SIZE);
	}

<<<<<<< HEAD
=======
	blk_start_plug(&plug);

>>>>>>> refs/remotes/origin/master
	for (seg = 0; seg < nr_segs; seg++) {
		user_addr = (unsigned long)iov[seg].iov_base;
		sdio.size += bytes = iov[seg].iov_len;

		/* Index into the first page of the first block */
		sdio.first_block_in_page = (user_addr & ~PAGE_MASK) >> blkbits;
		sdio.final_block_in_request = sdio.block_in_file +
						(bytes >> blkbits);
		/* Page fetching state */
		sdio.head = 0;
		sdio.tail = 0;
		sdio.curr_page = 0;

		sdio.total_pages = 0;
		if (user_addr & (PAGE_SIZE-1)) {
			sdio.total_pages++;
			bytes -= PAGE_SIZE - (user_addr & (PAGE_SIZE - 1));
		}
		sdio.total_pages += (bytes + PAGE_SIZE - 1) / PAGE_SIZE;
		sdio.curr_user_address = user_addr;

		retval = do_direct_IO(dio, &sdio, &map_bh);

		dio->result += iov[seg].iov_len -
			((sdio.final_block_in_request - sdio.block_in_file) <<
					blkbits);

		if (retval) {
			dio_cleanup(dio, &sdio);
			break;
		}
	} /* end iovec loop */

	if (retval == -ENOTBLK) {
		/*
		 * The remaining part of the request will be
		 * be handled by buffered I/O when we return
		 */
		retval = 0;
	}
	/*
	 * There may be some unwritten disk at the end of a part-written
	 * fs-block-sized block.  Go zero that now.
	 */
	dio_zero_block(dio, &sdio, 1, &map_bh);

	if (sdio.cur_page) {
		ssize_t ret2;

		ret2 = dio_send_cur_page(dio, &sdio, &map_bh);
		if (retval == 0)
			retval = ret2;
		page_cache_release(sdio.cur_page);
		sdio.cur_page = NULL;
	}
	if (sdio.bio)
		dio_bio_submit(dio, &sdio);

<<<<<<< HEAD
=======
	blk_finish_plug(&plug);

>>>>>>> refs/remotes/origin/master
	/*
	 * It is possible that, we return short IO due to end of file.
	 * In that case, we need to release all the pages we got hold on.
	 */
	dio_cleanup(dio, &sdio);

	/*
	 * All block lookups have been performed. For READ requests
	 * we can let i_mutex go now that its achieved its purpose
	 * of protecting us from looking up uninitialized blocks.
	 */
	if (rw == READ && (dio->flags & DIO_LOCKING))
		mutex_unlock(&dio->inode->i_mutex);

	/*
	 * The only time we want to leave bios in flight is when a successful
	 * partial aio read or full aio write have been setup.  In that case
	 * bio completion will call aio_complete.  The only time it's safe to
	 * call aio_complete is when we return -EIOCBQUEUED, so we key on that.
	 * This had *better* be the only place that raises -EIOCBQUEUED.
	 */
	BUG_ON(retval == -EIOCBQUEUED);
	if (dio->is_async && retval == 0 && dio->result &&
<<<<<<< HEAD
	    ((rw & READ) || (dio->result == sdio.size)))
=======
	    ((rw == READ) || (dio->result == sdio.size)))
>>>>>>> refs/remotes/origin/master
		retval = -EIOCBQUEUED;

	if (retval != -EIOCBQUEUED)
		dio_await_completion(dio);

	if (drop_refcount(dio) == 0) {
		retval = dio_complete(dio, offset, retval, false);
<<<<<<< HEAD
		kmem_cache_free(dio_cache, dio);
	} else
		BUG_ON(retval != -EIOCBQUEUED);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	} else
		BUG_ON(retval != -EIOCBQUEUED);
>>>>>>> refs/remotes/origin/master

out:
	return retval;
}
<<<<<<< HEAD
<<<<<<< HEAD
EXPORT_SYMBOL(__blockdev_direct_IO);
=======
=======
>>>>>>> refs/remotes/origin/master

ssize_t
__blockdev_direct_IO(int rw, struct kiocb *iocb, struct inode *inode,
	struct block_device *bdev, const struct iovec *iov, loff_t offset,
	unsigned long nr_segs, get_block_t get_block, dio_iodone_t end_io,
	dio_submit_t submit_io,	int flags)
{
	/*
	 * The block device state is needed in the end to finally
	 * submit everything.  Since it's likely to be cache cold
	 * prefetch it here as first thing to hide some of the
	 * latency.
	 *
	 * Attempt to prefetch the pieces we likely need later.
	 */
	prefetch(&bdev->bd_disk->part_tbl);
	prefetch(bdev->bd_queue);
	prefetch((char *)bdev->bd_queue + SMP_CACHE_BYTES);

	return do_blockdev_direct_IO(rw, iocb, inode, bdev, iov, offset,
				     nr_segs, get_block, end_io,
				     submit_io, flags);
}

EXPORT_SYMBOL(__blockdev_direct_IO);

static __init int dio_init(void)
{
	dio_cache = KMEM_CACHE(dio, SLAB_PANIC);
	return 0;
}
module_init(dio_init)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
