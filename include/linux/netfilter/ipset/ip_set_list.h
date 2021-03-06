#ifndef __IP_SET_LIST_H
#define __IP_SET_LIST_H

<<<<<<< HEAD
/* List type specific error codes */
enum {
	/* Set name to be added/deleted/tested does not exist. */
	IPSET_ERR_NAME = IPSET_ERR_TYPE_SPECIFIC,
	/* list:set type is not permitted to add */
	IPSET_ERR_LOOP,
	/* Missing reference set */
	IPSET_ERR_BEFORE,
	/* Reference set does not exist */
	IPSET_ERR_NAMEREF,
	/* Set is full */
	IPSET_ERR_LIST_FULL,
	/* Reference set is not added to the set */
	IPSET_ERR_REF_EXIST,
};

#ifdef __KERNEL__
=======
#include <uapi/linux/netfilter/ipset/ip_set_list.h>

>>>>>>> refs/remotes/origin/master

#define IP_SET_LIST_DEFAULT_SIZE	8
#define IP_SET_LIST_MIN_SIZE		4

<<<<<<< HEAD
#endif /* __KERNEL__ */

=======
>>>>>>> refs/remotes/origin/master
#endif /* __IP_SET_LIST_H */
