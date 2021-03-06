/* types.h: FRV types
 *
 * Copyright (C) 2004 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
<<<<<<< HEAD

#ifndef _ASM_TYPES_H
#define _ASM_TYPES_H

#include <asm-generic/int-ll64.h>

<<<<<<< HEAD
#ifndef __ASSEMBLY__

typedef unsigned short umode_t;

#endif /* __ASSEMBLY__ */

=======
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * These aren't exported outside the kernel to avoid name space clashes
 */
#ifdef __KERNEL__

#define BITS_PER_LONG 32

#endif /* __KERNEL__ */

=======
#ifndef _ASM_TYPES_H
#define _ASM_TYPES_H

#include <uapi/asm/types.h>

/*
 * These aren't exported outside the kernel to avoid name space clashes
 */

#define BITS_PER_LONG 32

>>>>>>> refs/remotes/origin/master
#endif /* _ASM_TYPES_H */
