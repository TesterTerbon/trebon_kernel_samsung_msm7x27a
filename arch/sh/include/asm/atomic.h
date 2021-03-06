#ifndef __ASM_SH_ATOMIC_H
#define __ASM_SH_ATOMIC_H

/*
 * Atomic operations that C can't guarantee us.  Useful for
 * resource counting etc..
 *
 */

#include <linux/compiler.h>
#include <linux/types.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/system.h>

#define ATOMIC_INIT(i)	( (atomic_t) { (i) } )
=======
#include <asm/cmpxchg.h>

#define ATOMIC_INIT(i)	{ (i) }
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/cmpxchg.h>

#define ATOMIC_INIT(i)	{ (i) }
>>>>>>> refs/remotes/origin/master

#define atomic_read(v)		(*(volatile int *)&(v)->counter)
#define atomic_set(v,i)		((v)->counter = (i))

#if defined(CONFIG_GUSA_RB)
#include <asm/atomic-grb.h>
#elif defined(CONFIG_CPU_SH4A)
#include <asm/atomic-llsc.h>
#else
#include <asm/atomic-irq.h>
#endif

#define atomic_add_negative(a, v)	(atomic_add_return((a), (v)) < 0)
#define atomic_dec_return(v)		atomic_sub_return(1, (v))
#define atomic_inc_return(v)		atomic_add_return(1, (v))
#define atomic_inc_and_test(v)		(atomic_inc_return(v) == 0)
#define atomic_sub_and_test(i,v)	(atomic_sub_return((i), (v)) == 0)
#define atomic_dec_and_test(v)		(atomic_sub_return(1, (v)) == 0)
<<<<<<< HEAD
<<<<<<< HEAD
#define atomic_inc_not_zero(v)		atomic_add_unless((v), 1, 0)
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#define atomic_inc(v)			atomic_add(1, (v))
#define atomic_dec(v)			atomic_sub(1, (v))

#define atomic_xchg(v, new)		(xchg(&((v)->counter), new))
#define atomic_cmpxchg(v, o, n)		(cmpxchg(&((v)->counter), (o), (n)))

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * atomic_add_unless - add unless the number is a given value
=======
 * __atomic_add_unless - add unless the number is a given value
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * __atomic_add_unless - add unless the number is a given value
>>>>>>> refs/remotes/origin/master
 * @v: pointer of type atomic_t
 * @a: the amount to add to v...
 * @u: ...unless v is equal to u.
 *
 * Atomically adds @a to @v, so long as it was not @u.
<<<<<<< HEAD
<<<<<<< HEAD
 * Returns non-zero if @v was not @u, and zero otherwise.
 */
static inline int atomic_add_unless(atomic_t *v, int a, int u)
=======
 * Returns the old value of @v.
 */
static inline int __atomic_add_unless(atomic_t *v, int a, int u)
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Returns the old value of @v.
 */
static inline int __atomic_add_unless(atomic_t *v, int a, int u)
>>>>>>> refs/remotes/origin/master
{
	int c, old;
	c = atomic_read(v);
	for (;;) {
		if (unlikely(c == (u)))
			break;
		old = atomic_cmpxchg((v), c, c + (a));
		if (likely(old == c))
			break;
		c = old;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	return c != (u);
=======
	return c;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return c;
>>>>>>> refs/remotes/origin/master
}

#define smp_mb__before_atomic_dec()	smp_mb()
#define smp_mb__after_atomic_dec()	smp_mb()
#define smp_mb__before_atomic_inc()	smp_mb()
#define smp_mb__after_atomic_inc()	smp_mb()

<<<<<<< HEAD
<<<<<<< HEAD
#include <asm-generic/atomic-long.h>
#include <asm-generic/atomic64.h>

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#endif /* __ASM_SH_ATOMIC_H */
