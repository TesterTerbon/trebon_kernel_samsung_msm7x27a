<<<<<<< HEAD
#ifndef __ARCH_S390_ATOMIC__
#define __ARCH_S390_ATOMIC__

/*
 * Copyright 1999,2009 IBM Corp.
=======
/*
 * Copyright IBM Corp. 1999, 2009
>>>>>>> refs/remotes/origin/master
 * Author(s): Martin Schwidefsky <schwidefsky@de.ibm.com>,
 *	      Denis Joseph Barrow,
 *	      Arnd Bergmann <arndb@de.ibm.com>,
 *
 * Atomic operations that C can't guarantee us.
 * Useful for resource counting etc.
 * s390 uses 'Compare And Swap' for atomicity in SMP environment.
 *
 */

<<<<<<< HEAD
#include <linux/compiler.h>
#include <linux/types.h>
<<<<<<< HEAD
#include <asm/system.h>
=======
#include <asm/cmpxchg.h>
>>>>>>> refs/remotes/origin/cm-10.0

#define ATOMIC_INIT(i)  { (i) }

#define __CS_LOOP(ptr, op_val, op_string) ({				\
	int old_val, new_val;						\
=======
#ifndef __ARCH_S390_ATOMIC__
#define __ARCH_S390_ATOMIC__

#include <linux/compiler.h>
#include <linux/types.h>
#include <asm/cmpxchg.h>

#define ATOMIC_INIT(i)  { (i) }

#ifdef CONFIG_HAVE_MARCH_Z196_FEATURES

#define __ATOMIC_OR	"lao"
#define __ATOMIC_AND	"lan"
#define __ATOMIC_ADD	"laa"

#define __ATOMIC_LOOP(ptr, op_val, op_string)				\
({									\
	int old_val;							\
									\
	typecheck(atomic_t *, ptr);					\
	asm volatile(							\
		op_string "	%0,%2,%1\n"				\
		: "=d" (old_val), "+Q" ((ptr)->counter)			\
		: "d" (op_val)						\
		: "cc", "memory");					\
	old_val;							\
})

#else /* CONFIG_HAVE_MARCH_Z196_FEATURES */

#define __ATOMIC_OR	"or"
#define __ATOMIC_AND	"nr"
#define __ATOMIC_ADD	"ar"

#define __ATOMIC_LOOP(ptr, op_val, op_string)				\
({									\
	int old_val, new_val;						\
									\
	typecheck(atomic_t *, ptr);					\
>>>>>>> refs/remotes/origin/master
	asm volatile(							\
		"	l	%0,%2\n"				\
		"0:	lr	%1,%0\n"				\
		op_string "	%1,%3\n"				\
		"	cs	%0,%1,%2\n"				\
		"	jl	0b"					\
<<<<<<< HEAD
		: "=&d" (old_val), "=&d" (new_val),			\
		  "=Q" (((atomic_t *)(ptr))->counter)			\
		: "d" (op_val),	 "Q" (((atomic_t *)(ptr))->counter)	\
		: "cc", "memory");					\
	new_val;							\
})

=======
		: "=&d" (old_val), "=&d" (new_val), "+Q" ((ptr)->counter)\
		: "d" (op_val)						\
		: "cc", "memory");					\
	old_val;							\
})

#endif /* CONFIG_HAVE_MARCH_Z196_FEATURES */

>>>>>>> refs/remotes/origin/master
static inline int atomic_read(const atomic_t *v)
{
	int c;

	asm volatile(
		"	l	%0,%1\n"
		: "=d" (c) : "Q" (v->counter));
	return c;
}

static inline void atomic_set(atomic_t *v, int i)
{
	asm volatile(
		"	st	%1,%0\n"
		: "=Q" (v->counter) : "d" (i));
}

static inline int atomic_add_return(int i, atomic_t *v)
{
<<<<<<< HEAD
	return __CS_LOOP(v, i, "ar");
}
#define atomic_add(_i, _v)		atomic_add_return(_i, _v)
#define atomic_add_negative(_i, _v)	(atomic_add_return(_i, _v) < 0)
#define atomic_inc(_v)			atomic_add_return(1, _v)
#define atomic_inc_return(_v)		atomic_add_return(1, _v)
#define atomic_inc_and_test(_v)		(atomic_add_return(1, _v) == 0)

static inline int atomic_sub_return(int i, atomic_t *v)
{
	return __CS_LOOP(v, i, "sr");
}
#define atomic_sub(_i, _v)		atomic_sub_return(_i, _v)
#define atomic_sub_and_test(_i, _v)	(atomic_sub_return(_i, _v) == 0)
#define atomic_dec(_v)			atomic_sub_return(1, _v)
#define atomic_dec_return(_v)		atomic_sub_return(1, _v)
#define atomic_dec_and_test(_v)		(atomic_sub_return(1, _v) == 0)

static inline void atomic_clear_mask(unsigned long mask, atomic_t *v)
{
	__CS_LOOP(v, ~mask, "nr");
}

static inline void atomic_set_mask(unsigned long mask, atomic_t *v)
{
	__CS_LOOP(v, mask, "or");
=======
	return __ATOMIC_LOOP(v, i, __ATOMIC_ADD) + i;
}

static inline void atomic_add(int i, atomic_t *v)
{
#ifdef CONFIG_HAVE_MARCH_Z196_FEATURES
	if (__builtin_constant_p(i) && (i > -129) && (i < 128)) {
		asm volatile(
			"asi	%0,%1\n"
			: "+Q" (v->counter)
			: "i" (i)
			: "cc", "memory");
	} else {
		atomic_add_return(i, v);
	}
#else
	atomic_add_return(i, v);
#endif
}

#define atomic_add_negative(_i, _v)	(atomic_add_return(_i, _v) < 0)
#define atomic_inc(_v)			atomic_add(1, _v)
#define atomic_inc_return(_v)		atomic_add_return(1, _v)
#define atomic_inc_and_test(_v)		(atomic_add_return(1, _v) == 0)
#define atomic_sub(_i, _v)		atomic_add(-(int)(_i), _v)
#define atomic_sub_return(_i, _v)	atomic_add_return(-(int)(_i), _v)
#define atomic_sub_and_test(_i, _v)	(atomic_sub_return(_i, _v) == 0)
#define atomic_dec(_v)			atomic_sub(1, _v)
#define atomic_dec_return(_v)		atomic_sub_return(1, _v)
#define atomic_dec_and_test(_v)		(atomic_sub_return(1, _v) == 0)

static inline void atomic_clear_mask(unsigned int mask, atomic_t *v)
{
	__ATOMIC_LOOP(v, ~mask, __ATOMIC_AND);
}

static inline void atomic_set_mask(unsigned int mask, atomic_t *v)
{
	__ATOMIC_LOOP(v, mask, __ATOMIC_OR);
>>>>>>> refs/remotes/origin/master
}

#define atomic_xchg(v, new) (xchg(&((v)->counter), new))

static inline int atomic_cmpxchg(atomic_t *v, int old, int new)
{
	asm volatile(
		"	cs	%0,%2,%1"
<<<<<<< HEAD
		: "+d" (old), "=Q" (v->counter)
		: "d" (new), "Q" (v->counter)
=======
		: "+d" (old), "+Q" (v->counter)
		: "d" (new)
>>>>>>> refs/remotes/origin/master
		: "cc", "memory");
	return old;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline int atomic_add_unless(atomic_t *v, int a, int u)
=======
static inline int __atomic_add_unless(atomic_t *v, int a, int u)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline int __atomic_add_unless(atomic_t *v, int a, int u)
>>>>>>> refs/remotes/origin/master
{
	int c, old;
	c = atomic_read(v);
	for (;;) {
		if (unlikely(c == u))
			break;
		old = atomic_cmpxchg(v, c, c + a);
		if (likely(old == c))
			break;
		c = old;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	return c != u;
}

#define atomic_inc_not_zero(v) atomic_add_unless((v), 1, 0)
=======
	return c;
}

>>>>>>> refs/remotes/origin/cm-10.0

#undef __CS_LOOP
=======
	return c;
}


#undef __ATOMIC_LOOP
>>>>>>> refs/remotes/origin/master

#define ATOMIC64_INIT(i)  { (i) }

#ifdef CONFIG_64BIT

<<<<<<< HEAD
#define __CSG_LOOP(ptr, op_val, op_string) ({				\
	long long old_val, new_val;					\
=======
#ifdef CONFIG_HAVE_MARCH_Z196_FEATURES

#define __ATOMIC64_OR	"laog"
#define __ATOMIC64_AND	"lang"
#define __ATOMIC64_ADD	"laag"

#define __ATOMIC64_LOOP(ptr, op_val, op_string)				\
({									\
	long long old_val;						\
									\
	typecheck(atomic64_t *, ptr);					\
	asm volatile(							\
		op_string "	%0,%2,%1\n"				\
		: "=d" (old_val), "+Q" ((ptr)->counter)			\
		: "d" (op_val)						\
		: "cc", "memory");					\
	old_val;							\
})

#else /* CONFIG_HAVE_MARCH_Z196_FEATURES */

#define __ATOMIC64_OR	"ogr"
#define __ATOMIC64_AND	"ngr"
#define __ATOMIC64_ADD	"agr"

#define __ATOMIC64_LOOP(ptr, op_val, op_string)				\
({									\
	long long old_val, new_val;					\
									\
	typecheck(atomic64_t *, ptr);					\
>>>>>>> refs/remotes/origin/master
	asm volatile(							\
		"	lg	%0,%2\n"				\
		"0:	lgr	%1,%0\n"				\
		op_string "	%1,%3\n"				\
		"	csg	%0,%1,%2\n"				\
		"	jl	0b"					\
<<<<<<< HEAD
		: "=&d" (old_val), "=&d" (new_val),			\
		  "=Q" (((atomic_t *)(ptr))->counter)			\
		: "d" (op_val),	"Q" (((atomic_t *)(ptr))->counter)	\
		: "cc", "memory");					\
	new_val;							\
})

=======
		: "=&d" (old_val), "=&d" (new_val), "+Q" ((ptr)->counter)\
		: "d" (op_val)						\
		: "cc", "memory");					\
	old_val;							\
})

#endif /* CONFIG_HAVE_MARCH_Z196_FEATURES */

>>>>>>> refs/remotes/origin/master
static inline long long atomic64_read(const atomic64_t *v)
{
	long long c;

	asm volatile(
		"	lg	%0,%1\n"
		: "=d" (c) : "Q" (v->counter));
	return c;
}

static inline void atomic64_set(atomic64_t *v, long long i)
{
	asm volatile(
		"	stg	%1,%0\n"
		: "=Q" (v->counter) : "d" (i));
}

static inline long long atomic64_add_return(long long i, atomic64_t *v)
{
<<<<<<< HEAD
	return __CSG_LOOP(v, i, "agr");
}

static inline long long atomic64_sub_return(long long i, atomic64_t *v)
{
	return __CSG_LOOP(v, i, "sgr");
=======
	return __ATOMIC64_LOOP(v, i, __ATOMIC64_ADD) + i;
>>>>>>> refs/remotes/origin/master
}

static inline void atomic64_clear_mask(unsigned long mask, atomic64_t *v)
{
<<<<<<< HEAD
	__CSG_LOOP(v, ~mask, "ngr");
=======
	__ATOMIC64_LOOP(v, ~mask, __ATOMIC64_AND);
>>>>>>> refs/remotes/origin/master
}

static inline void atomic64_set_mask(unsigned long mask, atomic64_t *v)
{
<<<<<<< HEAD
	__CSG_LOOP(v, mask, "ogr");
=======
	__ATOMIC64_LOOP(v, mask, __ATOMIC64_OR);
>>>>>>> refs/remotes/origin/master
}

#define atomic64_xchg(v, new) (xchg(&((v)->counter), new))

static inline long long atomic64_cmpxchg(atomic64_t *v,
					     long long old, long long new)
{
	asm volatile(
		"	csg	%0,%2,%1"
<<<<<<< HEAD
		: "+d" (old), "=Q" (v->counter)
		: "d" (new), "Q" (v->counter)
=======
		: "+d" (old), "+Q" (v->counter)
		: "d" (new)
>>>>>>> refs/remotes/origin/master
		: "cc", "memory");
	return old;
}

<<<<<<< HEAD
#undef __CSG_LOOP
=======
#undef __ATOMIC64_LOOP
>>>>>>> refs/remotes/origin/master

#else /* CONFIG_64BIT */

typedef struct {
	long long counter;
} atomic64_t;

static inline long long atomic64_read(const atomic64_t *v)
{
	register_pair rp;

	asm volatile(
		"	lm	%0,%N0,%1"
		: "=&d" (rp) : "Q" (v->counter)	);
	return rp.pair;
}

static inline void atomic64_set(atomic64_t *v, long long i)
{
	register_pair rp = {.pair = i};

	asm volatile(
		"	stm	%1,%N1,%0"
		: "=Q" (v->counter) : "d" (rp) );
}

static inline long long atomic64_xchg(atomic64_t *v, long long new)
{
	register_pair rp_new = {.pair = new};
	register_pair rp_old;

	asm volatile(
		"	lm	%0,%N0,%1\n"
		"0:	cds	%0,%2,%1\n"
		"	jl	0b\n"
<<<<<<< HEAD
		: "=&d" (rp_old), "=Q" (v->counter)
		: "d" (rp_new), "Q" (v->counter)
=======
		: "=&d" (rp_old), "+Q" (v->counter)
		: "d" (rp_new)
>>>>>>> refs/remotes/origin/master
		: "cc");
	return rp_old.pair;
}

static inline long long atomic64_cmpxchg(atomic64_t *v,
					 long long old, long long new)
{
	register_pair rp_old = {.pair = old};
	register_pair rp_new = {.pair = new};

	asm volatile(
		"	cds	%0,%2,%1"
<<<<<<< HEAD
		: "+&d" (rp_old), "=Q" (v->counter)
		: "d" (rp_new), "Q" (v->counter)
=======
		: "+&d" (rp_old), "+Q" (v->counter)
		: "d" (rp_new)
>>>>>>> refs/remotes/origin/master
		: "cc");
	return rp_old.pair;
}


static inline long long atomic64_add_return(long long i, atomic64_t *v)
{
	long long old, new;

	do {
		old = atomic64_read(v);
		new = old + i;
	} while (atomic64_cmpxchg(v, old, new) != old);
	return new;
}

<<<<<<< HEAD
static inline long long atomic64_sub_return(long long i, atomic64_t *v)
{
	long long old, new;

	do {
		old = atomic64_read(v);
		new = old - i;
	} while (atomic64_cmpxchg(v, old, new) != old);
	return new;
}

=======
>>>>>>> refs/remotes/origin/master
static inline void atomic64_set_mask(unsigned long long mask, atomic64_t *v)
{
	long long old, new;

	do {
		old = atomic64_read(v);
		new = old | mask;
	} while (atomic64_cmpxchg(v, old, new) != old);
}

static inline void atomic64_clear_mask(unsigned long long mask, atomic64_t *v)
{
	long long old, new;

	do {
		old = atomic64_read(v);
		new = old & mask;
	} while (atomic64_cmpxchg(v, old, new) != old);
}

#endif /* CONFIG_64BIT */

<<<<<<< HEAD
static inline int atomic64_add_unless(atomic64_t *v, long long a, long long u)
=======
static inline void atomic64_add(long long i, atomic64_t *v)
{
#ifdef CONFIG_HAVE_MARCH_Z196_FEATURES
	if (__builtin_constant_p(i) && (i > -129) && (i < 128)) {
		asm volatile(
			"agsi	%0,%1\n"
			: "+Q" (v->counter)
			: "i" (i)
			: "cc", "memory");
	} else {
		atomic64_add_return(i, v);
	}
#else
	atomic64_add_return(i, v);
#endif
}

static inline int atomic64_add_unless(atomic64_t *v, long long i, long long u)
>>>>>>> refs/remotes/origin/master
{
	long long c, old;

	c = atomic64_read(v);
	for (;;) {
		if (unlikely(c == u))
			break;
<<<<<<< HEAD
		old = atomic64_cmpxchg(v, c, c + a);
=======
		old = atomic64_cmpxchg(v, c, c + i);
>>>>>>> refs/remotes/origin/master
		if (likely(old == c))
			break;
		c = old;
	}
	return c != u;
}

static inline long long atomic64_dec_if_positive(atomic64_t *v)
{
	long long c, old, dec;

	c = atomic64_read(v);
	for (;;) {
		dec = c - 1;
		if (unlikely(dec < 0))
			break;
		old = atomic64_cmpxchg((v), c, dec);
		if (likely(old == c))
			break;
		c = old;
	}
	return dec;
}

<<<<<<< HEAD
#define atomic64_add(_i, _v)		atomic64_add_return(_i, _v)
#define atomic64_add_negative(_i, _v)	(atomic64_add_return(_i, _v) < 0)
#define atomic64_inc(_v)		atomic64_add_return(1, _v)
#define atomic64_inc_return(_v)		atomic64_add_return(1, _v)
#define atomic64_inc_and_test(_v)	(atomic64_add_return(1, _v) == 0)
#define atomic64_sub(_i, _v)		atomic64_sub_return(_i, _v)
#define atomic64_sub_and_test(_i, _v)	(atomic64_sub_return(_i, _v) == 0)
#define atomic64_dec(_v)		atomic64_sub_return(1, _v)
=======
#define atomic64_add_negative(_i, _v)	(atomic64_add_return(_i, _v) < 0)
#define atomic64_inc(_v)		atomic64_add(1, _v)
#define atomic64_inc_return(_v)		atomic64_add_return(1, _v)
#define atomic64_inc_and_test(_v)	(atomic64_add_return(1, _v) == 0)
#define atomic64_sub_return(_i, _v)	atomic64_add_return(-(long long)(_i), _v)
#define atomic64_sub(_i, _v)		atomic64_add(-(long long)(_i), _v)
#define atomic64_sub_and_test(_i, _v)	(atomic64_sub_return(_i, _v) == 0)
#define atomic64_dec(_v)		atomic64_sub(1, _v)
>>>>>>> refs/remotes/origin/master
#define atomic64_dec_return(_v)		atomic64_sub_return(1, _v)
#define atomic64_dec_and_test(_v)	(atomic64_sub_return(1, _v) == 0)
#define atomic64_inc_not_zero(v)	atomic64_add_unless((v), 1, 0)

#define smp_mb__before_atomic_dec()	smp_mb()
#define smp_mb__after_atomic_dec()	smp_mb()
#define smp_mb__before_atomic_inc()	smp_mb()
#define smp_mb__after_atomic_inc()	smp_mb()

<<<<<<< HEAD
<<<<<<< HEAD
#include <asm-generic/atomic-long.h>

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#endif /* __ARCH_S390_ATOMIC__  */
