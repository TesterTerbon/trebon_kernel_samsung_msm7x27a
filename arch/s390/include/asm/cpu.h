/*
<<<<<<< HEAD
 *    Copyright IBM Corp. 2000,2009
=======
 *    Copyright IBM Corp. 2000, 2009
>>>>>>> refs/remotes/origin/master
 *    Author(s): Hartmut Penner <hp@de.ibm.com>,
 *		 Martin Schwidefsky <schwidefsky@de.ibm.com>,
 *		 Christian Ehrhardt <ehrhardt@de.ibm.com>,
 */

#ifndef _ASM_S390_CPU_H
#define _ASM_S390_CPU_H

#define MAX_CPU_ADDRESS 255

#ifndef __ASSEMBLY__

#include <linux/types.h>

struct cpuid
{
	unsigned int version :	8;
	unsigned int ident   : 24;
	unsigned int machine : 16;
	unsigned int unused  : 16;
} __attribute__ ((packed, aligned(8)));

#endif /* __ASSEMBLY__ */
#endif /* _ASM_S390_CPU_H */
