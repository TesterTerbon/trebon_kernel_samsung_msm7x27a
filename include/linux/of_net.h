/*
 * OF helpers for network devices.
 *
 * This file is released under the GPLv2
 */

#ifndef __LINUX_OF_NET_H
#define __LINUX_OF_NET_H

#ifdef CONFIG_OF_NET
#include <linux/of.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
extern const int of_get_phy_mode(struct device_node *np);
>>>>>>> refs/remotes/origin/cm-10.0
extern const void *of_get_mac_address(struct device_node *np);
=======
extern int of_get_phy_mode(struct device_node *np);
extern const void *of_get_mac_address(struct device_node *np);
#else
static inline int of_get_phy_mode(struct device_node *np)
{
	return -ENODEV;
}

static inline const void *of_get_mac_address(struct device_node *np)
{
	return NULL;
}
>>>>>>> refs/remotes/origin/master
#endif

#endif /* __LINUX_OF_NET_H */
