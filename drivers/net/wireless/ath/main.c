/*
 * Copyright (c) 2009 Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

<<<<<<< HEAD
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> refs/remotes/origin/master
#include <linux/kernel.h>
#include <linux/module.h>

#include "ath.h"

MODULE_AUTHOR("Atheros Communications");
MODULE_DESCRIPTION("Shared library for Atheros wireless LAN cards.");
MODULE_LICENSE("Dual BSD/GPL");

struct sk_buff *ath_rxbuf_alloc(struct ath_common *common,
				u32 len,
				gfp_t gfp_mask)
{
	struct sk_buff *skb;
	u32 off;

	/*
	 * Cache-line-align.  This is important (for the
	 * 5210 at least) as not doing so causes bogus data
	 * in rx'd frames.
	 */

	/* Note: the kernel can allocate a value greater than
	 * what we ask it to give us. We really only need 4 KB as that
	 * is this hardware supports and in fact we need at least 3849
	 * as that is the MAX AMSDU size this hardware supports.
	 * Unfortunately this means we may get 8 KB here from the
	 * kernel... and that is actually what is observed on some
	 * systems :( */
	skb = __dev_alloc_skb(len + common->cachelsz - 1, gfp_mask);
	if (skb != NULL) {
		off = ((unsigned long) skb->data) % common->cachelsz;
		if (off != 0)
			skb_reserve(skb, common->cachelsz - off);
	} else {
<<<<<<< HEAD
		printk(KERN_ERR "skbuff alloc of size %u failed\n", len);
=======
		pr_err("skbuff alloc of size %u failed\n", len);
>>>>>>> refs/remotes/origin/master
		return NULL;
	}

	return skb;
}
EXPORT_SYMBOL(ath_rxbuf_alloc);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
#if 0 // by bbelief
void ath_printk(const char *level, const char *fmt, ...)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36))
	struct va_format vaf;
#endif
=======
void ath_printk(const char *level, const struct ath_common* common,
		const char *fmt, ...)
=======
#if 0 // by bbelief
void ath_printk(const char *level, const char *fmt, ...)
>>>>>>> refs/remotes/origin/cm-11.0
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36))
	struct va_format vaf;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
#endif
>>>>>>> refs/remotes/origin/cm-11.0
	va_list args;

	va_start(args, fmt);

<<<<<<< HEAD
<<<<<<< HEAD
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36))
	vaf.fmt = fmt;
	vaf.va = &args;

	printk("%sath: %pV", level, &vaf);
#else
	printk("%sath: ", level);
	vprintk(fmt, args);
#endif

	va_end(args);
}
#else
>>>>>>> refs/remotes/origin/cm-10.0
int ath_printk(const char *level, struct ath_common *common,
	       const char *fmt, ...)
{
	struct va_format vaf;
	va_list args;
	int rtn;

	va_start(args, fmt);

	vaf.fmt = fmt;
	vaf.va = &args;

	rtn = printk("%sath: %pV", level, &vaf);

	va_end(args);

	return rtn;
}
<<<<<<< HEAD
=======

#endif
>>>>>>> refs/remotes/origin/cm-10.0
=======
=======
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36))
>>>>>>> refs/remotes/origin/cm-11.0
	vaf.fmt = fmt;
	vaf.va = &args;

	printk("%sath: %pV", level, &vaf);
#else
	printk("%sath: ", level);
	vprintk(fmt, args);
#endif

	va_end(args);
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
#else
int ath_printk(const char *level, struct ath_common *common,
	       const char *fmt, ...)
{
	struct va_format vaf;
	va_list args;
	int rtn;

	va_start(args, fmt);

	vaf.fmt = fmt;
	vaf.va = &args;

	rtn = printk("%sath: %pV", level, &vaf);

	va_end(args);

	return rtn;
}

#endif
>>>>>>> refs/remotes/origin/cm-11.0
EXPORT_SYMBOL(ath_printk);
