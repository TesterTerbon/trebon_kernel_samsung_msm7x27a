#ifndef _M68K_CHECKSUM_H
#define _M68K_CHECKSUM_H

#include <linux/in6.h>

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_GENERIC_CSUM
#include <asm-generic/checksum.h>
#else

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * computes the checksum of a memory block at buff, length len,
 * and adds in "sum" (32-bit)
 *
 * returns a 32-bit number suitable for feeding into itself
 * or csum_tcpudp_magic
 *
 * this function must be called with even lengths, except
 * for the last fragment, which may be odd
 *
 * it's best to have buff aligned on a 32-bit boundary
 */
__wsum csum_partial(const void *buff, int len, __wsum sum);

/*
 * the same as csum_partial, but copies from src while it
 * checksums
 *
 * here even more important to align src and dst on a 32-bit (or even
 * better 64-bit) boundary
 */

extern __wsum csum_partial_copy_from_user(const void __user *src,
						void *dst,
						int len, __wsum sum,
						int *csum_err);

extern __wsum csum_partial_copy_nocheck(const void *src,
					      void *dst, int len,
					      __wsum sum);

<<<<<<< HEAD
<<<<<<< HEAD

#ifdef CONFIG_COLDFIRE

/*
 *	The ColdFire cores don't support all the 68k instructions used
 *	in the optimized checksum code below. So it reverts back to using
 *	more standard C coded checksums. The fast checksum code is
 *	significantly larger than the optimized version, so it is not
 *	inlined here.
 */
__sum16 ip_fast_csum(const void *iph, unsigned int ihl);

static inline __sum16 csum_fold(__wsum sum)
{
	unsigned int tmp = (__force u32)sum;

	tmp = (tmp & 0xffff) + (tmp >> 16);
	tmp = (tmp & 0xffff) + (tmp >> 16);

	return (__force __sum16)~tmp;
}

#else

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 *	This is a version of ip_fast_csum() optimized for IP headers,
 *	which always checksum on 4 octet boundaries.
 */
static inline __sum16 ip_fast_csum(const void *iph, unsigned int ihl)
{
	unsigned int sum = 0;
	unsigned long tmp;

	__asm__ ("subqw #1,%2\n"
		 "1:\t"
		 "movel %1@+,%3\n\t"
		 "addxl %3,%0\n\t"
		 "dbra  %2,1b\n\t"
		 "movel %0,%3\n\t"
		 "swap  %3\n\t"
		 "addxw %3,%0\n\t"
		 "clrw  %3\n\t"
		 "addxw %3,%0\n\t"
		 : "=d" (sum), "=&a" (iph), "=&d" (ihl), "=&d" (tmp)
		 : "0" (sum), "1" (iph), "2" (ihl)
		 : "memory");
	return (__force __sum16)~sum;
}

static inline __sum16 csum_fold(__wsum sum)
{
	unsigned int tmp = (__force u32)sum;

	__asm__("swap %1\n\t"
		"addw %1, %0\n\t"
		"clrw %1\n\t"
		"addxw %1, %0"
		: "=&d" (sum), "=&d" (tmp)
		: "0" (sum), "1" (tmp));

	return (__force __sum16)~sum;
}

<<<<<<< HEAD
<<<<<<< HEAD
#endif /* CONFIG_COLDFIRE */

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static inline __wsum
csum_tcpudp_nofold(__be32 saddr, __be32 daddr, unsigned short len,
		  unsigned short proto, __wsum sum)
{
	__asm__ ("addl  %2,%0\n\t"
		 "addxl %3,%0\n\t"
		 "addxl %4,%0\n\t"
		 "clrl %1\n\t"
		 "addxl %1,%0"
		 : "=&d" (sum), "=d" (saddr)
		 : "g" (daddr), "1" (saddr), "d" (len + proto),
		   "0" (sum));
	return sum;
}


/*
 * computes the checksum of the TCP/UDP pseudo-header
 * returns a 16-bit checksum, already complemented
 */
static inline __sum16
csum_tcpudp_magic(__be32 saddr, __be32 daddr, unsigned short len,
		  unsigned short proto, __wsum sum)
{
	return csum_fold(csum_tcpudp_nofold(saddr,daddr,len,proto,sum));
}

/*
 * this routine is used for miscellaneous IP-like checksums, mainly
 * in icmp.c
 */

static inline __sum16 ip_compute_csum(const void *buff, int len)
{
	return csum_fold (csum_partial(buff, len, 0));
}

#define _HAVE_ARCH_IPV6_CSUM
static __inline__ __sum16
csum_ipv6_magic(const struct in6_addr *saddr, const struct in6_addr *daddr,
		__u32 len, unsigned short proto, __wsum sum)
{
	register unsigned long tmp;
	__asm__("addl %2@,%0\n\t"
		"movel %2@(4),%1\n\t"
		"addxl %1,%0\n\t"
		"movel %2@(8),%1\n\t"
		"addxl %1,%0\n\t"
		"movel %2@(12),%1\n\t"
		"addxl %1,%0\n\t"
		"movel %3@,%1\n\t"
		"addxl %1,%0\n\t"
		"movel %3@(4),%1\n\t"
		"addxl %1,%0\n\t"
		"movel %3@(8),%1\n\t"
		"addxl %1,%0\n\t"
		"movel %3@(12),%1\n\t"
		"addxl %1,%0\n\t"
		"addxl %4,%0\n\t"
		"clrl %1\n\t"
		"addxl %1,%0"
		: "=&d" (sum), "=&d" (tmp)
		: "a" (saddr), "a" (daddr), "d" (len + proto),
		  "0" (sum));

	return csum_fold(sum);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
#endif /* CONFIG_GENERIC_CSUM */
>>>>>>> refs/remotes/origin/cm-10.0
=======
#endif /* CONFIG_GENERIC_CSUM */
>>>>>>> refs/remotes/origin/master
#endif /* _M68K_CHECKSUM_H */
