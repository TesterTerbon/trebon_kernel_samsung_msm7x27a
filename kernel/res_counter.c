/*
 * resource cgroups
 *
 * Copyright 2007 OpenVZ SWsoft Inc
 *
 * Author: Pavel Emelianov <xemul@openvz.org>
 *
 */

#include <linux/types.h>
#include <linux/parser.h>
#include <linux/fs.h>
#include <linux/res_counter.h>
#include <linux/uaccess.h>
#include <linux/mm.h>

void res_counter_init(struct res_counter *counter, struct res_counter *parent)
{
	spin_lock_init(&counter->lock);
<<<<<<< HEAD
	counter->limit = RESOURCE_MAX;
	counter->soft_limit = RESOURCE_MAX;
	counter->parent = parent;
}

int res_counter_charge_locked(struct res_counter *counter, unsigned long val)
{
	if (counter->usage + val > counter->limit) {
		counter->failcnt++;
		return -ENOMEM;
=======
	counter->limit = RES_COUNTER_MAX;
	counter->soft_limit = RES_COUNTER_MAX;
	counter->parent = parent;
}

int res_counter_charge_locked(struct res_counter *counter, unsigned long val,
			      bool force)
{
	int ret = 0;

	if (counter->usage + val > counter->limit) {
		counter->failcnt++;
		ret = -ENOMEM;
		if (!force)
			return ret;
>>>>>>> refs/remotes/origin/master
	}

	counter->usage += val;
	if (counter->usage > counter->max_usage)
		counter->max_usage = counter->usage;
<<<<<<< HEAD
	return 0;
}

int res_counter_charge(struct res_counter *counter, unsigned long val,
			struct res_counter **limit_fail_at)
{
	int ret;
	unsigned long flags;
	struct res_counter *c, *u;

=======
	return ret;
}

static int __res_counter_charge(struct res_counter *counter, unsigned long val,
				struct res_counter **limit_fail_at, bool force)
{
	int ret, r;
	unsigned long flags;
	struct res_counter *c, *u;

	r = ret = 0;
>>>>>>> refs/remotes/origin/master
	*limit_fail_at = NULL;
	local_irq_save(flags);
	for (c = counter; c != NULL; c = c->parent) {
		spin_lock(&c->lock);
<<<<<<< HEAD
		ret = res_counter_charge_locked(c, val);
		spin_unlock(&c->lock);
		if (ret < 0) {
			*limit_fail_at = c;
			goto undo;
		}
	}
	ret = 0;
	goto done;
undo:
	for (u = counter; u != c; u = u->parent) {
		spin_lock(&u->lock);
		res_counter_uncharge_locked(u, val);
		spin_unlock(&u->lock);
	}
done:
	local_irq_restore(flags);
	return ret;
}

<<<<<<< HEAD
=======
int res_counter_charge_nofail(struct res_counter *counter, unsigned long val,
			      struct res_counter **limit_fail_at)
{
	int ret, r;
	unsigned long flags;
	struct res_counter *c;

	r = ret = 0;
	*limit_fail_at = NULL;
	local_irq_save(flags);
	for (c = counter; c != NULL; c = c->parent) {
		spin_lock(&c->lock);
		r = res_counter_charge_locked(c, val);
		if (r)
			c->usage += val;
		spin_unlock(&c->lock);
		if (r < 0 && ret == 0) {
			*limit_fail_at = c;
			ret = r;
		}
	}
	local_irq_restore(flags);

	return ret;
}
>>>>>>> refs/remotes/origin/cm-10.0
void res_counter_uncharge_locked(struct res_counter *counter, unsigned long val)
=======
		r = res_counter_charge_locked(c, val, force);
		spin_unlock(&c->lock);
		if (r < 0 && !ret) {
			ret = r;
			*limit_fail_at = c;
			if (!force)
				break;
		}
	}

	if (ret < 0 && !force) {
		for (u = counter; u != c; u = u->parent) {
			spin_lock(&u->lock);
			res_counter_uncharge_locked(u, val);
			spin_unlock(&u->lock);
		}
	}
	local_irq_restore(flags);

	return ret;
}

int res_counter_charge(struct res_counter *counter, unsigned long val,
			struct res_counter **limit_fail_at)
{
	return __res_counter_charge(counter, val, limit_fail_at, false);
}

int res_counter_charge_nofail(struct res_counter *counter, unsigned long val,
			      struct res_counter **limit_fail_at)
{
	return __res_counter_charge(counter, val, limit_fail_at, true);
}

u64 res_counter_uncharge_locked(struct res_counter *counter, unsigned long val)
>>>>>>> refs/remotes/origin/master
{
	if (WARN_ON(counter->usage < val))
		val = counter->usage;

	counter->usage -= val;
<<<<<<< HEAD
}

void res_counter_uncharge(struct res_counter *counter, unsigned long val)
{
	unsigned long flags;
	struct res_counter *c;

	local_irq_save(flags);
	for (c = counter; c != NULL; c = c->parent) {
		spin_lock(&c->lock);
		res_counter_uncharge_locked(c, val);
		spin_unlock(&c->lock);
	}
	local_irq_restore(flags);
}

=======
	return counter->usage;
}

u64 res_counter_uncharge_until(struct res_counter *counter,
			       struct res_counter *top,
			       unsigned long val)
{
	unsigned long flags;
	struct res_counter *c;
	u64 ret = 0;

	local_irq_save(flags);
	for (c = counter; c != top; c = c->parent) {
		u64 r;
		spin_lock(&c->lock);
		r = res_counter_uncharge_locked(c, val);
		if (c == counter)
			ret = r;
		spin_unlock(&c->lock);
	}
	local_irq_restore(flags);
	return ret;
}

u64 res_counter_uncharge(struct res_counter *counter, unsigned long val)
{
	return res_counter_uncharge_until(counter, NULL, val);
}
>>>>>>> refs/remotes/origin/master

static inline unsigned long long *
res_counter_member(struct res_counter *counter, int member)
{
	switch (member) {
	case RES_USAGE:
		return &counter->usage;
	case RES_MAX_USAGE:
		return &counter->max_usage;
	case RES_LIMIT:
		return &counter->limit;
	case RES_FAILCNT:
		return &counter->failcnt;
	case RES_SOFT_LIMIT:
		return &counter->soft_limit;
	};

	BUG();
	return NULL;
}

ssize_t res_counter_read(struct res_counter *counter, int member,
		const char __user *userbuf, size_t nbytes, loff_t *pos,
		int (*read_strategy)(unsigned long long val, char *st_buf))
{
	unsigned long long *val;
	char buf[64], *s;

	s = buf;
	val = res_counter_member(counter, member);
	if (read_strategy)
		s += read_strategy(*val, s);
	else
		s += sprintf(s, "%llu\n", *val);
	return simple_read_from_buffer((void __user *)userbuf, nbytes,
			pos, buf, s - buf);
}

#if BITS_PER_LONG == 32
u64 res_counter_read_u64(struct res_counter *counter, int member)
{
	unsigned long flags;
	u64 ret;

	spin_lock_irqsave(&counter->lock, flags);
	ret = *res_counter_member(counter, member);
	spin_unlock_irqrestore(&counter->lock, flags);

	return ret;
}
#else
u64 res_counter_read_u64(struct res_counter *counter, int member)
{
	return *res_counter_member(counter, member);
}
#endif

int res_counter_memparse_write_strategy(const char *buf,
<<<<<<< HEAD
					unsigned long long *res)
{
	char *end;

	/* return RESOURCE_MAX(unlimited) if "-1" is specified */
	if (*buf == '-') {
		*res = simple_strtoull(buf + 1, &end, 10);
		if (*res != 1 || *end != '\0')
			return -EINVAL;
		*res = RESOURCE_MAX;
		return 0;
	}

<<<<<<< HEAD
	/* FIXME - make memparse() take const char* args */
	*res = memparse((char *)buf, &end);
=======
	*res = memparse(buf, &end);
>>>>>>> refs/remotes/origin/cm-10.0
	if (*end != '\0')
		return -EINVAL;

	*res = PAGE_ALIGN(*res);
	return 0;
}

int res_counter_write(struct res_counter *counter, int member,
		      const char *buf, write_strategy_fn write_strategy)
{
	char *end;
	unsigned long flags;
	unsigned long long tmp, *val;

	if (write_strategy) {
		if (write_strategy(buf, &tmp))
			return -EINVAL;
	} else {
		tmp = simple_strtoull(buf, &end, 10);
		if (*end != '\0')
			return -EINVAL;
	}
	spin_lock_irqsave(&counter->lock, flags);
	val = res_counter_member(counter, member);
	*val = tmp;
	spin_unlock_irqrestore(&counter->lock, flags);
=======
					unsigned long long *resp)
{
	char *end;
	unsigned long long res;

	/* return RES_COUNTER_MAX(unlimited) if "-1" is specified */
	if (*buf == '-') {
		res = simple_strtoull(buf + 1, &end, 10);
		if (res != 1 || *end != '\0')
			return -EINVAL;
		*resp = RES_COUNTER_MAX;
		return 0;
	}

	res = memparse(buf, &end);
	if (*end != '\0')
		return -EINVAL;

	if (PAGE_ALIGN(res) >= res)
		res = PAGE_ALIGN(res);
	else
		res = RES_COUNTER_MAX;

	*resp = res;

>>>>>>> refs/remotes/origin/master
	return 0;
}
