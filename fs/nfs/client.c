/* client.c: NFS client sharing and management code
 *
 * Copyright (C) 2006 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */


#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/stat.h>
#include <linux/errno.h>
#include <linux/unistd.h>
#include <linux/sunrpc/clnt.h>
#include <linux/sunrpc/stats.h>
#include <linux/sunrpc/metrics.h>
#include <linux/sunrpc/xprtsock.h>
#include <linux/sunrpc/xprtrdma.h>
#include <linux/nfs_fs.h>
#include <linux/nfs_mount.h>
#include <linux/nfs4_mount.h>
#include <linux/lockd/bind.h>
#include <linux/seq_file.h>
#include <linux/mount.h>
#include <linux/nfs_idmap.h>
#include <linux/vfs.h>
#include <linux/inet.h>
#include <linux/in6.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <net/ipv6.h>
#include <linux/nfs_xdr.h>
#include <linux/sunrpc/bc_xprt.h>

#include <asm/system.h>
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/idr.h>
#include <net/ipv6.h>
#include <linux/nfs_xdr.h>
#include <linux/sunrpc/bc_xprt.h>
#include <linux/nsproxy.h>
#include <linux/pid_namespace.h>

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#include "nfs4_fs.h"
#include "callback.h"
#include "delegation.h"
#include "iostat.h"
#include "internal.h"
#include "fscache.h"
#include "pnfs.h"
<<<<<<< HEAD
<<<<<<< HEAD

#define NFSDBG_FACILITY		NFSDBG_CLIENT

static DEFINE_SPINLOCK(nfs_client_lock);
static LIST_HEAD(nfs_client_list);
static LIST_HEAD(nfs_volume_list);
static DECLARE_WAIT_QUEUE_HEAD(nfs_client_active_wq);
#ifdef CONFIG_NFS_V4
static DEFINE_IDR(cb_ident_idr); /* Protected by nfs_client_lock */
=======
=======
#include "nfs.h"
>>>>>>> refs/remotes/origin/master
#include "netns.h"

#define NFSDBG_FACILITY		NFSDBG_CLIENT

static DECLARE_WAIT_QUEUE_HEAD(nfs_client_active_wq);
<<<<<<< HEAD
#ifdef CONFIG_NFS_V4
>>>>>>> refs/remotes/origin/cm-10.0

/*
 * Get a unique NFSv4.0 callback identifier which will be used
 * by the V4.0 callback service to lookup the nfs_client struct
 */
static int nfs_get_cb_ident_idr(struct nfs_client *clp, int minorversion)
{
	int ret = 0;
<<<<<<< HEAD
=======
	struct nfs_net *nn = net_generic(clp->net, nfs_net_id);
>>>>>>> refs/remotes/origin/cm-10.0

	if (clp->rpc_ops->version != 4 || minorversion != 0)
		return ret;
retry:
<<<<<<< HEAD
	if (!idr_pre_get(&cb_ident_idr, GFP_KERNEL))
		return -ENOMEM;
	spin_lock(&nfs_client_lock);
	ret = idr_get_new(&cb_ident_idr, clp, &clp->cl_cb_ident);
	spin_unlock(&nfs_client_lock);
=======
	if (!idr_pre_get(&nn->cb_ident_idr, GFP_KERNEL))
		return -ENOMEM;
	spin_lock(&nn->nfs_client_lock);
	ret = idr_get_new(&nn->cb_ident_idr, clp, &clp->cl_cb_ident);
	spin_unlock(&nn->nfs_client_lock);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret == -EAGAIN)
		goto retry;
	return ret;
}
#endif /* CONFIG_NFS_V4 */

/*
 * Turn off NFSv4 uid/gid mapping when using AUTH_SYS
 */
<<<<<<< HEAD
static int nfs4_disable_idmapping = 0;
=======
static bool nfs4_disable_idmapping = true;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static DEFINE_SPINLOCK(nfs_version_lock);
static DEFINE_MUTEX(nfs_version_mutex);
static LIST_HEAD(nfs_versions);
>>>>>>> refs/remotes/origin/master

/*
 * RPC cruft for NFS
 */
<<<<<<< HEAD
<<<<<<< HEAD
static struct rpc_version *nfs_version[5] = {
=======
static const struct rpc_version *nfs_version[5] = {
>>>>>>> refs/remotes/origin/cm-10.0
	[2]			= &nfs_version2,
#ifdef CONFIG_NFS_V3
	[3]			= &nfs_version3,
#endif
#ifdef CONFIG_NFS_V4
	[4]			= &nfs_version4,
#endif
};

<<<<<<< HEAD
struct rpc_program nfs_program = {
=======
const struct rpc_program nfs_program = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static const struct rpc_version *nfs_version[5] = {
	[2] = NULL,
	[3] = NULL,
	[4] = NULL,
};

const struct rpc_program nfs_program = {
>>>>>>> refs/remotes/origin/master
	.name			= "nfs",
	.number			= NFS_PROGRAM,
	.nrvers			= ARRAY_SIZE(nfs_version),
	.version		= nfs_version,
	.stats			= &nfs_rpcstat,
<<<<<<< HEAD
<<<<<<< HEAD
	.pipe_dir_name		= "/nfs",
=======
	.pipe_dir_name		= NFS_PIPE_DIRNAME,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.pipe_dir_name		= NFS_PIPE_DIRNAME,
>>>>>>> refs/remotes/origin/master
};

struct rpc_stat nfs_rpcstat = {
	.program		= &nfs_program
};

<<<<<<< HEAD

#ifdef CONFIG_NFS_V3_ACL
static struct rpc_stat		nfsacl_rpcstat = { &nfsacl_program };
<<<<<<< HEAD
static struct rpc_version *	nfsacl_version[] = {
	[3]			= &nfsacl_version3,
};

struct rpc_program		nfsacl_program = {
=======
static const struct rpc_version *nfsacl_version[] = {
	[3]			= &nfsacl_version3,
};

const struct rpc_program nfsacl_program = {
>>>>>>> refs/remotes/origin/cm-10.0
	.name			= "nfsacl",
	.number			= NFS_ACL_PROGRAM,
	.nrvers			= ARRAY_SIZE(nfsacl_version),
	.version		= nfsacl_version,
	.stats			= &nfsacl_rpcstat,
};
#endif  /* CONFIG_NFS_V3_ACL */

struct nfs_client_initdata {
	const char *hostname;
	const struct sockaddr *addr;
	size_t addrlen;
	const struct nfs_rpc_ops *rpc_ops;
	int proto;
	u32 minorversion;
<<<<<<< HEAD
=======
	struct net *net;
>>>>>>> refs/remotes/origin/cm-10.0
};
=======
static struct nfs_subversion *find_nfs_version(unsigned int version)
{
	struct nfs_subversion *nfs;
	spin_lock(&nfs_version_lock);

	list_for_each_entry(nfs, &nfs_versions, list) {
		if (nfs->rpc_ops->version == version) {
			spin_unlock(&nfs_version_lock);
			return nfs;
		}
	}

	spin_unlock(&nfs_version_lock);
	return ERR_PTR(-EPROTONOSUPPORT);
}

struct nfs_subversion *get_nfs_version(unsigned int version)
{
	struct nfs_subversion *nfs = find_nfs_version(version);

	if (IS_ERR(nfs)) {
		mutex_lock(&nfs_version_mutex);
		request_module("nfsv%d", version);
		nfs = find_nfs_version(version);
		mutex_unlock(&nfs_version_mutex);
	}

	if (!IS_ERR(nfs))
		try_module_get(nfs->owner);
	return nfs;
}

void put_nfs_version(struct nfs_subversion *nfs)
{
	module_put(nfs->owner);
}

void register_nfs_version(struct nfs_subversion *nfs)
{
	spin_lock(&nfs_version_lock);

	list_add(&nfs->list, &nfs_versions);
	nfs_version[nfs->rpc_ops->version] = nfs->rpc_vers;

	spin_unlock(&nfs_version_lock);
}
EXPORT_SYMBOL_GPL(register_nfs_version);

void unregister_nfs_version(struct nfs_subversion *nfs)
{
	spin_lock(&nfs_version_lock);

	nfs_version[nfs->rpc_ops->version] = NULL;
	list_del(&nfs->list);

	spin_unlock(&nfs_version_lock);
}
EXPORT_SYMBOL_GPL(unregister_nfs_version);
>>>>>>> refs/remotes/origin/master

/*
 * Allocate a shared client record
 *
 * Since these are allocated/deallocated very rarely, we don't
 * bother putting them in a slab cache...
 */
<<<<<<< HEAD
static struct nfs_client *nfs_alloc_client(const struct nfs_client_initdata *cl_init)
=======
struct nfs_client *nfs_alloc_client(const struct nfs_client_initdata *cl_init)
>>>>>>> refs/remotes/origin/master
{
	struct nfs_client *clp;
	struct rpc_cred *cred;
	int err = -ENOMEM;

	if ((clp = kzalloc(sizeof(*clp), GFP_KERNEL)) == NULL)
		goto error_0;

<<<<<<< HEAD
	clp->rpc_ops = cl_init->rpc_ops;
=======
	clp->cl_nfs_mod = cl_init->nfs_mod;
	try_module_get(clp->cl_nfs_mod->owner);

	clp->rpc_ops = clp->cl_nfs_mod->rpc_ops;
>>>>>>> refs/remotes/origin/master

	atomic_set(&clp->cl_count, 1);
	clp->cl_cons_state = NFS_CS_INITING;

	memcpy(&clp->cl_addr, cl_init->addr, cl_init->addrlen);
	clp->cl_addrlen = cl_init->addrlen;

	if (cl_init->hostname) {
		err = -ENOMEM;
		clp->cl_hostname = kstrdup(cl_init->hostname, GFP_KERNEL);
		if (!clp->cl_hostname)
			goto error_cleanup;
	}

	INIT_LIST_HEAD(&clp->cl_superblocks);
	clp->cl_rpcclient = ERR_PTR(-EINVAL);

	clp->cl_proto = cl_init->proto;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	clp->net = get_net(cl_init->net);
>>>>>>> refs/remotes/origin/cm-10.0

#ifdef CONFIG_NFS_V4
	err = nfs_get_cb_ident_idr(clp, cl_init->minorversion);
	if (err)
		goto error_cleanup;

	spin_lock_init(&clp->cl_lock);
	INIT_DELAYED_WORK(&clp->cl_renewd, nfs4_renew_state);
	rpc_init_wait_queue(&clp->cl_rpcwaitq, "NFS client");
	clp->cl_boot_time = CURRENT_TIME;
	clp->cl_state = 1 << NFS4CLNT_LEASE_EXPIRED;
	clp->cl_minorversion = cl_init->minorversion;
	clp->cl_mvops = nfs_v4_minor_ops[cl_init->minorversion];
#endif
<<<<<<< HEAD
	cred = rpc_lookup_machine_cred();
	if (!IS_ERR(cred))
		clp->cl_machine_cred = cred;
#if defined(CONFIG_NFS_V4_1)
	INIT_LIST_HEAD(&clp->cl_layouts);
#endif
=======
	cred = rpc_lookup_machine_cred("*");
	if (!IS_ERR(cred))
		clp->cl_machine_cred = cred;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	clp->cl_net = get_net(cl_init->net);

	cred = rpc_lookup_machine_cred("*");
	if (!IS_ERR(cred))
		clp->cl_machine_cred = cred;
>>>>>>> refs/remotes/origin/master
	nfs_fscache_get_client_cookie(clp);

	return clp;

error_cleanup:
<<<<<<< HEAD
=======
	put_nfs_version(clp->cl_nfs_mod);
>>>>>>> refs/remotes/origin/master
	kfree(clp);
error_0:
	return ERR_PTR(err);
}
<<<<<<< HEAD

#ifdef CONFIG_NFS_V4
#ifdef CONFIG_NFS_V4_1
static void nfs4_shutdown_session(struct nfs_client *clp)
{
<<<<<<< HEAD
	if (nfs4_has_session(clp))
		nfs4_destroy_session(clp->cl_session);
=======
	if (nfs4_has_session(clp)) {
		nfs4_deviceid_purge_client(clp);
		nfs4_destroy_session(clp->cl_session);
	}

>>>>>>> refs/remotes/origin/cm-10.0
}
#else /* CONFIG_NFS_V4_1 */
static void nfs4_shutdown_session(struct nfs_client *clp)
{
}
#endif /* CONFIG_NFS_V4_1 */

/*
 * Destroy the NFS4 callback service
 */
static void nfs4_destroy_callback(struct nfs_client *clp)
{
	if (__test_and_clear_bit(NFS_CS_CALLBACK, &clp->cl_res_state))
		nfs_callback_down(clp->cl_mvops->minor_version);
}

static void nfs4_shutdown_client(struct nfs_client *clp)
{
	if (__test_and_clear_bit(NFS_CS_RENEWD, &clp->cl_res_state))
		nfs4_kill_renewd(clp);
	nfs4_shutdown_session(clp);
	nfs4_destroy_callback(clp);
	if (__test_and_clear_bit(NFS_CS_IDMAP, &clp->cl_res_state))
		nfs_idmap_delete(clp);

	rpc_destroy_wait_queue(&clp->cl_rpcwaitq);
}

/* idr_remove_all is not needed as all id's are removed by nfs_put_client */
<<<<<<< HEAD
void nfs_cleanup_cb_ident_idr(void)
{
	idr_destroy(&cb_ident_idr);
=======
=======
EXPORT_SYMBOL_GPL(nfs_alloc_client);

#if IS_ENABLED(CONFIG_NFS_V4)
>>>>>>> refs/remotes/origin/master
void nfs_cleanup_cb_ident_idr(struct net *net)
{
	struct nfs_net *nn = net_generic(net, nfs_net_id);

	idr_destroy(&nn->cb_ident_idr);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/* nfs_client_lock held */
static void nfs_cb_idr_remove_locked(struct nfs_client *clp)
{
<<<<<<< HEAD
<<<<<<< HEAD
	if (clp->cl_cb_ident)
		idr_remove(&cb_ident_idr, clp->cl_cb_ident);
=======
	struct nfs_net *nn = net_generic(clp->net, nfs_net_id);

	if (clp->cl_cb_ident)
		idr_remove(&nn->cb_ident_idr, clp->cl_cb_ident);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct nfs_net *nn = net_generic(clp->cl_net, nfs_net_id);

	if (clp->cl_cb_ident)
		idr_remove(&nn->cb_ident_idr, clp->cl_cb_ident);
>>>>>>> refs/remotes/origin/master
}

static void pnfs_init_server(struct nfs_server *server)
{
	rpc_init_wait_queue(&server->roc_rpcwaitq, "pNFS ROC");
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
static void nfs4_destroy_server(struct nfs_server *server)
{
	nfs4_purge_state_owners(server);
}

>>>>>>> refs/remotes/origin/cm-10.0
#else
static void nfs4_shutdown_client(struct nfs_client *clp)
{
}

<<<<<<< HEAD
void nfs_cleanup_cb_ident_idr(void)
=======
void nfs_cleanup_cb_ident_idr(struct net *net)
>>>>>>> refs/remotes/origin/cm-10.0
=======
#else
void nfs_cleanup_cb_ident_idr(struct net *net)
>>>>>>> refs/remotes/origin/master
{
}

static void nfs_cb_idr_remove_locked(struct nfs_client *clp)
{
}

static void pnfs_init_server(struct nfs_server *server)
{
}

#endif /* CONFIG_NFS_V4 */

/*
 * Destroy a shared client record
 */
<<<<<<< HEAD
static void nfs_free_client(struct nfs_client *clp)
{
	dprintk("--> nfs_free_client(%u)\n", clp->rpc_ops->version);

	nfs4_shutdown_client(clp);

=======
void nfs_free_client(struct nfs_client *clp)
{
	dprintk("--> nfs_free_client(%u)\n", clp->rpc_ops->version);

>>>>>>> refs/remotes/origin/master
	nfs_fscache_release_client_cookie(clp);

	/* -EIO all pending I/O */
	if (!IS_ERR(clp->cl_rpcclient))
		rpc_shutdown_client(clp->cl_rpcclient);

	if (clp->cl_machine_cred != NULL)
		put_rpccred(clp->cl_machine_cred);

<<<<<<< HEAD
<<<<<<< HEAD
	nfs4_deviceid_purge_client(clp);

	kfree(clp->cl_hostname);
=======
	put_net(clp->net);
	kfree(clp->cl_hostname);
	kfree(clp->server_scope);
	kfree(clp->impl_id);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	put_net(clp->cl_net);
	put_nfs_version(clp->cl_nfs_mod);
	kfree(clp->cl_hostname);
>>>>>>> refs/remotes/origin/master
	kfree(clp);

	dprintk("<-- nfs_free_client()\n");
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(nfs_free_client);
>>>>>>> refs/remotes/origin/master

/*
 * Release a reference to a shared client record
 */
void nfs_put_client(struct nfs_client *clp)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct nfs_net *nn;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct nfs_net *nn;

>>>>>>> refs/remotes/origin/master
	if (!clp)
		return;

	dprintk("--> nfs_put_client({%d})\n", atomic_read(&clp->cl_count));
<<<<<<< HEAD
<<<<<<< HEAD

	if (atomic_dec_and_lock(&clp->cl_count, &nfs_client_lock)) {
		list_del(&clp->cl_share_link);
		nfs_cb_idr_remove_locked(clp);
		spin_unlock(&nfs_client_lock);
=======
	nn = net_generic(clp->net, nfs_net_id);
=======
	nn = net_generic(clp->cl_net, nfs_net_id);
>>>>>>> refs/remotes/origin/master

	if (atomic_dec_and_lock(&clp->cl_count, &nn->nfs_client_lock)) {
		list_del(&clp->cl_share_link);
		nfs_cb_idr_remove_locked(clp);
		spin_unlock(&nn->nfs_client_lock);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

		BUG_ON(!list_empty(&clp->cl_superblocks));

		nfs_free_client(clp);
=======

		WARN_ON_ONCE(!list_empty(&clp->cl_superblocks));

		clp->rpc_ops->free_client(clp);
>>>>>>> refs/remotes/origin/master
	}
}
EXPORT_SYMBOL_GPL(nfs_put_client);

#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
/*
 * Test if two ip6 socket addresses refer to the same socket by
 * comparing relevant fields. The padding bytes specifically, are not
 * compared. sin6_flowinfo is not compared because it only affects QoS
 * and sin6_scope_id is only compared if the address is "link local"
 * because "link local" addresses need only be unique to a specific
 * link. Conversely, ordinary unicast addresses might have different
 * sin6_scope_id.
 *
 * The caller should ensure both socket addresses are AF_INET6.
 */
static int nfs_sockaddr_match_ipaddr6(const struct sockaddr *sa1,
				      const struct sockaddr *sa2)
{
	const struct sockaddr_in6 *sin1 = (const struct sockaddr_in6 *)sa1;
	const struct sockaddr_in6 *sin2 = (const struct sockaddr_in6 *)sa2;

<<<<<<< HEAD
<<<<<<< HEAD
	if (ipv6_addr_scope(&sin1->sin6_addr) == IPV6_ADDR_SCOPE_LINKLOCAL &&
	    sin1->sin6_scope_id != sin2->sin6_scope_id)
		return 0;

	return ipv6_addr_equal(&sin1->sin6_addr, &sin2->sin6_addr);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (!ipv6_addr_equal(&sin1->sin6_addr, &sin2->sin6_addr))
		return 0;
	else if (ipv6_addr_type(&sin1->sin6_addr) & IPV6_ADDR_LINKLOCAL)
		return sin1->sin6_scope_id == sin2->sin6_scope_id;

	return 1;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}
#else	/* !defined(CONFIG_IPV6) && !defined(CONFIG_IPV6_MODULE) */
static int nfs_sockaddr_match_ipaddr6(const struct sockaddr *sa1,
				      const struct sockaddr *sa2)
{
	return 0;
}
#endif

/*
 * Test if two ip4 socket addresses refer to the same socket, by
 * comparing relevant fields. The padding bytes specifically, are
 * not compared.
 *
 * The caller should ensure both socket addresses are AF_INET.
 */
static int nfs_sockaddr_match_ipaddr4(const struct sockaddr *sa1,
				      const struct sockaddr *sa2)
{
	const struct sockaddr_in *sin1 = (const struct sockaddr_in *)sa1;
	const struct sockaddr_in *sin2 = (const struct sockaddr_in *)sa2;

	return sin1->sin_addr.s_addr == sin2->sin_addr.s_addr;
}

static int nfs_sockaddr_cmp_ip6(const struct sockaddr *sa1,
				const struct sockaddr *sa2)
{
	const struct sockaddr_in6 *sin1 = (const struct sockaddr_in6 *)sa1;
	const struct sockaddr_in6 *sin2 = (const struct sockaddr_in6 *)sa2;

	return nfs_sockaddr_match_ipaddr6(sa1, sa2) &&
		(sin1->sin6_port == sin2->sin6_port);
}

static int nfs_sockaddr_cmp_ip4(const struct sockaddr *sa1,
				const struct sockaddr *sa2)
{
	const struct sockaddr_in *sin1 = (const struct sockaddr_in *)sa1;
	const struct sockaddr_in *sin2 = (const struct sockaddr_in *)sa2;

	return nfs_sockaddr_match_ipaddr4(sa1, sa2) &&
		(sin1->sin_port == sin2->sin_port);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
#if defined(CONFIG_NFS_V4_1)
>>>>>>> refs/remotes/origin/cm-10.0
=======
#if defined(CONFIG_NFS_V4_1)
>>>>>>> refs/remotes/origin/master
/*
 * Test if two socket addresses represent the same actual socket,
 * by comparing (only) relevant fields, excluding the port number.
 */
<<<<<<< HEAD
static int nfs_sockaddr_match_ipaddr(const struct sockaddr *sa1,
				     const struct sockaddr *sa2)
=======
int nfs_sockaddr_match_ipaddr(const struct sockaddr *sa1,
			      const struct sockaddr *sa2)
>>>>>>> refs/remotes/origin/master
{
	if (sa1->sa_family != sa2->sa_family)
		return 0;

	switch (sa1->sa_family) {
	case AF_INET:
		return nfs_sockaddr_match_ipaddr4(sa1, sa2);
	case AF_INET6:
		return nfs_sockaddr_match_ipaddr6(sa1, sa2);
	}
	return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
#endif /* CONFIG_NFS_V4_1 */
>>>>>>> refs/remotes/origin/cm-10.0
=======
EXPORT_SYMBOL_GPL(nfs_sockaddr_match_ipaddr);
#endif /* CONFIG_NFS_V4_1 */
>>>>>>> refs/remotes/origin/master

/*
 * Test if two socket addresses represent the same actual socket,
 * by comparing (only) relevant fields, including the port number.
 */
static int nfs_sockaddr_cmp(const struct sockaddr *sa1,
			    const struct sockaddr *sa2)
{
	if (sa1->sa_family != sa2->sa_family)
		return 0;

	switch (sa1->sa_family) {
	case AF_INET:
		return nfs_sockaddr_cmp_ip4(sa1, sa2);
	case AF_INET6:
		return nfs_sockaddr_cmp_ip6(sa1, sa2);
	}
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
/* Common match routine for v4.0 and v4.1 callback services */
bool
nfs4_cb_match_client(const struct sockaddr *addr, struct nfs_client *clp,
		     u32 minorversion)
=======
#if defined(CONFIG_NFS_V4_1)
/* Common match routine for v4.0 and v4.1 callback services */
static bool nfs4_cb_match_client(const struct sockaddr *addr,
		struct nfs_client *clp, u32 minorversion)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct sockaddr *clap = (struct sockaddr *)&clp->cl_addr;

	/* Don't match clients that failed to initialise */
	if (!(clp->cl_cons_state == NFS_CS_READY ||
	    clp->cl_cons_state == NFS_CS_SESSION_INITING))
		return false;

	/* Match the version and minorversion */
	if (clp->rpc_ops->version != 4 ||
	    clp->cl_minorversion != minorversion)
		return false;

	/* Match only the IP address, not the port number */
	if (!nfs_sockaddr_match_ipaddr(addr, clap))
		return false;

	return true;
}
<<<<<<< HEAD
=======
#endif /* CONFIG_NFS_V4_1 */
>>>>>>> refs/remotes/origin/cm-10.0

=======
>>>>>>> refs/remotes/origin/master
/*
 * Find an nfs_client on the list that matches the initialisation data
 * that is supplied.
 */
static struct nfs_client *nfs_match_client(const struct nfs_client_initdata *data)
{
	struct nfs_client *clp;
	const struct sockaddr *sap = data->addr;
<<<<<<< HEAD
<<<<<<< HEAD

	list_for_each_entry(clp, &nfs_client_list, cl_share_link) {
=======
	struct nfs_net *nn = net_generic(data->net, nfs_net_id);

	list_for_each_entry(clp, &nn->nfs_client_list, cl_share_link) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct nfs_net *nn = net_generic(data->net, nfs_net_id);

	list_for_each_entry(clp, &nn->nfs_client_list, cl_share_link) {
>>>>>>> refs/remotes/origin/master
	        const struct sockaddr *clap = (struct sockaddr *)&clp->cl_addr;
		/* Don't match clients that failed to initialise properly */
		if (clp->cl_cons_state < 0)
			continue;

		/* Different NFS versions cannot share the same nfs_client */
<<<<<<< HEAD
		if (clp->rpc_ops != data->rpc_ops)
=======
		if (clp->rpc_ops != data->nfs_mod->rpc_ops)
>>>>>>> refs/remotes/origin/master
			continue;

		if (clp->cl_proto != data->proto)
			continue;
		/* Match nfsv4 minorversion */
		if (clp->cl_minorversion != data->minorversion)
			continue;
		/* Match the full socket address */
		if (!nfs_sockaddr_cmp(sap, clap))
			continue;

		atomic_inc(&clp->cl_count);
		return clp;
	}
	return NULL;
}

<<<<<<< HEAD
=======
static bool nfs_client_init_is_complete(const struct nfs_client *clp)
{
	return clp->cl_cons_state != NFS_CS_INITING;
}

int nfs_wait_client_init_complete(const struct nfs_client *clp)
{
	return wait_event_killable(nfs_client_active_wq,
			nfs_client_init_is_complete(clp));
}
EXPORT_SYMBOL_GPL(nfs_wait_client_init_complete);

/*
 * Found an existing client.  Make sure it's ready before returning.
 */
static struct nfs_client *
nfs_found_client(const struct nfs_client_initdata *cl_init,
		 struct nfs_client *clp)
{
	int error;

	error = nfs_wait_client_init_complete(clp);
	if (error < 0) {
		nfs_put_client(clp);
		return ERR_PTR(-ERESTARTSYS);
	}

	if (clp->cl_cons_state < NFS_CS_READY) {
		error = clp->cl_cons_state;
		nfs_put_client(clp);
		return ERR_PTR(error);
	}

	smp_rmb();

	dprintk("<-- %s found nfs_client %p for %s\n",
		__func__, clp, cl_init->hostname ?: "");
	return clp;
}

>>>>>>> refs/remotes/origin/master
/*
 * Look up a client by IP address and protocol version
 * - creates a new record if one doesn't yet exist
 */
<<<<<<< HEAD
static struct nfs_client *
nfs_get_client(const struct nfs_client_initdata *cl_init,
	       const struct rpc_timeout *timeparms,
	       const char *ip_addr,
	       rpc_authflavor_t authflavour,
	       int noresvport)
{
	struct nfs_client *clp, *new = NULL;
	int error;
<<<<<<< HEAD
=======
	struct nfs_net *nn = net_generic(cl_init->net, nfs_net_id);
>>>>>>> refs/remotes/origin/cm-10.0

	dprintk("--> nfs_get_client(%s,v%u)\n",
		cl_init->hostname ?: "", cl_init->rpc_ops->version);

	/* see if the client already exists */
	do {
<<<<<<< HEAD
		spin_lock(&nfs_client_lock);
=======
		spin_lock(&nn->nfs_client_lock);
>>>>>>> refs/remotes/origin/cm-10.0

		clp = nfs_match_client(cl_init);
		if (clp)
			goto found_client;
		if (new)
			goto install_client;

<<<<<<< HEAD
		spin_unlock(&nfs_client_lock);
=======
		spin_unlock(&nn->nfs_client_lock);
>>>>>>> refs/remotes/origin/cm-10.0

		new = nfs_alloc_client(cl_init);
	} while (!IS_ERR(new));

	dprintk("--> nfs_get_client() = %ld [failed]\n", PTR_ERR(new));
	return new;

	/* install a new client and return with it unready */
install_client:
	clp = new;
<<<<<<< HEAD
	list_add(&clp->cl_share_link, &nfs_client_list);
	spin_unlock(&nfs_client_lock);
=======
	list_add(&clp->cl_share_link, &nn->nfs_client_list);
	spin_unlock(&nn->nfs_client_lock);
>>>>>>> refs/remotes/origin/cm-10.0

	error = cl_init->rpc_ops->init_client(clp, timeparms, ip_addr,
					      authflavour, noresvport);
	if (error < 0) {
		nfs_put_client(clp);
		return ERR_PTR(error);
	}
	dprintk("--> nfs_get_client() = %p [new]\n", clp);
	return clp;

	/* found an existing client
	 * - make sure it's ready before returning
	 */
found_client:
<<<<<<< HEAD
	spin_unlock(&nfs_client_lock);
=======
	spin_unlock(&nn->nfs_client_lock);
>>>>>>> refs/remotes/origin/cm-10.0

	if (new)
		nfs_free_client(new);

	error = wait_event_killable(nfs_client_active_wq,
				clp->cl_cons_state < NFS_CS_INITING);
	if (error < 0) {
		nfs_put_client(clp);
		return ERR_PTR(-ERESTARTSYS);
	}

	if (clp->cl_cons_state < NFS_CS_READY) {
		error = clp->cl_cons_state;
		nfs_put_client(clp);
		return ERR_PTR(error);
	}

	BUG_ON(clp->cl_cons_state != NFS_CS_READY);

	dprintk("--> nfs_get_client() = %p [share]\n", clp);
	return clp;
}
=======
struct nfs_client *
nfs_get_client(const struct nfs_client_initdata *cl_init,
	       const struct rpc_timeout *timeparms,
	       const char *ip_addr,
	       rpc_authflavor_t authflavour)
{
	struct nfs_client *clp, *new = NULL;
	struct nfs_net *nn = net_generic(cl_init->net, nfs_net_id);
	const struct nfs_rpc_ops *rpc_ops = cl_init->nfs_mod->rpc_ops;

	dprintk("--> nfs_get_client(%s,v%u)\n",
		cl_init->hostname ?: "", rpc_ops->version);

	/* see if the client already exists */
	do {
		spin_lock(&nn->nfs_client_lock);

		clp = nfs_match_client(cl_init);
		if (clp) {
			spin_unlock(&nn->nfs_client_lock);
			if (new)
				new->rpc_ops->free_client(new);
			return nfs_found_client(cl_init, clp);
		}
		if (new) {
			list_add_tail(&new->cl_share_link,
					&nn->nfs_client_list);
			spin_unlock(&nn->nfs_client_lock);
			new->cl_flags = cl_init->init_flags;
			return rpc_ops->init_client(new, timeparms, ip_addr);
		}

		spin_unlock(&nn->nfs_client_lock);

		new = rpc_ops->alloc_client(cl_init);
	} while (!IS_ERR(new));

	dprintk("<-- nfs_get_client() Failed to find %s (%ld)\n",
		cl_init->hostname ?: "", PTR_ERR(new));
	return new;
}
EXPORT_SYMBOL_GPL(nfs_get_client);
>>>>>>> refs/remotes/origin/master

/*
 * Mark a server as ready or failed
 */
void nfs_mark_client_ready(struct nfs_client *clp, int state)
{
<<<<<<< HEAD
	clp->cl_cons_state = state;
	wake_up_all(&nfs_client_active_wq);
}

/*
 * With sessions, the client is not marked ready until after a
 * successful EXCHANGE_ID and CREATE_SESSION.
 *
 * Map errors cl_cons_state errors to EPROTONOSUPPORT to indicate
 * other versions of NFS can be tried.
 */
int nfs4_check_client_ready(struct nfs_client *clp)
{
	if (!nfs4_has_session(clp))
		return 0;
	if (clp->cl_cons_state < NFS_CS_READY)
		return -EPROTONOSUPPORT;
	return 0;
}
=======
	smp_wmb();
	clp->cl_cons_state = state;
	wake_up_all(&nfs_client_active_wq);
}
EXPORT_SYMBOL_GPL(nfs_mark_client_ready);
>>>>>>> refs/remotes/origin/master

/*
 * Initialise the timeout values for a connection
 */
<<<<<<< HEAD
static void nfs_init_timeout_values(struct rpc_timeout *to, int proto,
=======
void nfs_init_timeout_values(struct rpc_timeout *to, int proto,
>>>>>>> refs/remotes/origin/master
				    unsigned int timeo, unsigned int retrans)
{
	to->to_initval = timeo * HZ / 10;
	to->to_retries = retrans;

	switch (proto) {
	case XPRT_TRANSPORT_TCP:
	case XPRT_TRANSPORT_RDMA:
		if (to->to_retries == 0)
			to->to_retries = NFS_DEF_TCP_RETRANS;
		if (to->to_initval == 0)
			to->to_initval = NFS_DEF_TCP_TIMEO * HZ / 10;
		if (to->to_initval > NFS_MAX_TCP_TIMEOUT)
			to->to_initval = NFS_MAX_TCP_TIMEOUT;
		to->to_increment = to->to_initval;
		to->to_maxval = to->to_initval + (to->to_increment * to->to_retries);
		if (to->to_maxval > NFS_MAX_TCP_TIMEOUT)
			to->to_maxval = NFS_MAX_TCP_TIMEOUT;
		if (to->to_maxval < to->to_initval)
			to->to_maxval = to->to_initval;
		to->to_exponential = 0;
		break;
	case XPRT_TRANSPORT_UDP:
		if (to->to_retries == 0)
			to->to_retries = NFS_DEF_UDP_RETRANS;
		if (!to->to_initval)
			to->to_initval = NFS_DEF_UDP_TIMEO * HZ / 10;
		if (to->to_initval > NFS_MAX_UDP_TIMEOUT)
			to->to_initval = NFS_MAX_UDP_TIMEOUT;
		to->to_maxval = NFS_MAX_UDP_TIMEOUT;
		to->to_exponential = 1;
		break;
	default:
		BUG();
	}
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(nfs_init_timeout_values);
>>>>>>> refs/remotes/origin/master

/*
 * Create an RPC client handle
 */
<<<<<<< HEAD
static int nfs_create_rpc_client(struct nfs_client *clp,
				 const struct rpc_timeout *timeparms,
				 rpc_authflavor_t flavor,
				 int discrtry, int noresvport)
{
	struct rpc_clnt		*clnt = NULL;
	struct rpc_create_args args = {
<<<<<<< HEAD
		.net		= &init_net,
=======
		.net		= clp->net,
>>>>>>> refs/remotes/origin/cm-10.0
=======
int nfs_create_rpc_client(struct nfs_client *clp,
			  const struct rpc_timeout *timeparms,
			  rpc_authflavor_t flavor)
{
	struct rpc_clnt		*clnt = NULL;
	struct rpc_create_args args = {
		.net		= clp->cl_net,
>>>>>>> refs/remotes/origin/master
		.protocol	= clp->cl_proto,
		.address	= (struct sockaddr *)&clp->cl_addr,
		.addrsize	= clp->cl_addrlen,
		.timeout	= timeparms,
		.servername	= clp->cl_hostname,
		.program	= &nfs_program,
		.version	= clp->rpc_ops->version,
		.authflavor	= flavor,
	};

<<<<<<< HEAD
	if (discrtry)
		args.flags |= RPC_CLNT_CREATE_DISCRTRY;
	if (noresvport)
		args.flags |= RPC_CLNT_CREATE_NONPRIVPORT;
=======
	if (test_bit(NFS_CS_DISCRTRY, &clp->cl_flags))
		args.flags |= RPC_CLNT_CREATE_DISCRTRY;
	if (test_bit(NFS_CS_NO_RETRANS_TIMEOUT, &clp->cl_flags))
		args.flags |= RPC_CLNT_CREATE_NO_RETRANS_TIMEOUT;
	if (test_bit(NFS_CS_NORESVPORT, &clp->cl_flags))
		args.flags |= RPC_CLNT_CREATE_NONPRIVPORT;
	if (test_bit(NFS_CS_INFINITE_SLOTS, &clp->cl_flags))
		args.flags |= RPC_CLNT_CREATE_INFINITE_SLOTS;
>>>>>>> refs/remotes/origin/master

	if (!IS_ERR(clp->cl_rpcclient))
		return 0;

	clnt = rpc_create(&args);
	if (IS_ERR(clnt)) {
		dprintk("%s: cannot create RPC client. Error = %ld\n",
				__func__, PTR_ERR(clnt));
		return PTR_ERR(clnt);
	}

	clp->cl_rpcclient = clnt;
	return 0;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(nfs_create_rpc_client);
>>>>>>> refs/remotes/origin/master

/*
 * Version 2 or 3 client destruction
 */
static void nfs_destroy_server(struct nfs_server *server)
{
	if (server->nlm_host)
		nlmclnt_done(server->nlm_host);
}

/*
 * Version 2 or 3 lockd setup
 */
static int nfs_start_lockd(struct nfs_server *server)
{
	struct nlm_host *host;
	struct nfs_client *clp = server->nfs_client;
	struct nlmclnt_initdata nlm_init = {
		.hostname	= clp->cl_hostname,
		.address	= (struct sockaddr *)&clp->cl_addr,
		.addrlen	= clp->cl_addrlen,
		.nfs_version	= clp->rpc_ops->version,
		.noresvport	= server->flags & NFS_MOUNT_NORESVPORT ?
					1 : 0,
<<<<<<< HEAD
<<<<<<< HEAD
=======
		.net		= clp->net,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.net		= clp->cl_net,
>>>>>>> refs/remotes/origin/master
	};

	if (nlm_init.nfs_version > 3)
		return 0;
	if ((server->flags & NFS_MOUNT_LOCAL_FLOCK) &&
			(server->flags & NFS_MOUNT_LOCAL_FCNTL))
		return 0;

	switch (clp->cl_proto) {
		default:
			nlm_init.protocol = IPPROTO_TCP;
			break;
		case XPRT_TRANSPORT_UDP:
			nlm_init.protocol = IPPROTO_UDP;
	}

	host = nlmclnt_init(&nlm_init);
	if (IS_ERR(host))
		return PTR_ERR(host);

	server->nlm_host = host;
	server->destroy = nfs_destroy_server;
	return 0;
}

/*
<<<<<<< HEAD
 * Initialise an NFSv3 ACL client connection
 */
#ifdef CONFIG_NFS_V3_ACL
static void nfs_init_server_aclclient(struct nfs_server *server)
{
	if (server->nfs_client->rpc_ops->version != 3)
		goto out_noacl;
	if (server->flags & NFS_MOUNT_NOACL)
		goto out_noacl;

	server->client_acl = rpc_bind_new_program(server->client, &nfsacl_program, 3);
	if (IS_ERR(server->client_acl))
		goto out_noacl;

	/* No errors! Assume that Sun nfsacls are supported */
	server->caps |= NFS_CAP_ACLS;
	return;

out_noacl:
	server->caps &= ~NFS_CAP_ACLS;
}
#else
static inline void nfs_init_server_aclclient(struct nfs_server *server)
{
	server->flags &= ~NFS_MOUNT_NOACL;
	server->caps &= ~NFS_CAP_ACLS;
}
#endif

/*
 * Create a general RPC client
 */
static int nfs_init_server_rpcclient(struct nfs_server *server,
=======
 * Create a general RPC client
 */
int nfs_init_server_rpcclient(struct nfs_server *server,
>>>>>>> refs/remotes/origin/master
		const struct rpc_timeout *timeo,
		rpc_authflavor_t pseudoflavour)
{
	struct nfs_client *clp = server->nfs_client;

<<<<<<< HEAD
	server->client = rpc_clone_client(clp->cl_rpcclient);
=======
	server->client = rpc_clone_client_set_auth(clp->cl_rpcclient,
							pseudoflavour);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(server->client)) {
		dprintk("%s: couldn't create rpc_client!\n", __func__);
		return PTR_ERR(server->client);
	}

	memcpy(&server->client->cl_timeout_default,
			timeo,
			sizeof(server->client->cl_timeout_default));
	server->client->cl_timeout = &server->client->cl_timeout_default;
<<<<<<< HEAD

	if (pseudoflavour != clp->cl_rpcclient->cl_auth->au_flavor) {
		struct rpc_auth *auth;

		auth = rpcauth_create(pseudoflavour, server->client);
		if (IS_ERR(auth)) {
			dprintk("%s: couldn't create credcache!\n", __func__);
			return PTR_ERR(auth);
		}
	}
=======
>>>>>>> refs/remotes/origin/master
	server->client->cl_softrtry = 0;
	if (server->flags & NFS_MOUNT_SOFT)
		server->client->cl_softrtry = 1;

	return 0;
}
<<<<<<< HEAD

/*
 * Initialise an NFS2 or NFS3 client
 */
int nfs_init_client(struct nfs_client *clp, const struct rpc_timeout *timeparms,
		    const char *ip_addr, rpc_authflavor_t authflavour,
		    int noresvport)
=======
EXPORT_SYMBOL_GPL(nfs_init_server_rpcclient);

/**
 * nfs_init_client - Initialise an NFS2 or NFS3 client
 *
 * @clp: nfs_client to initialise
 * @timeparms: timeout parameters for underlying RPC transport
 * @ip_addr: IP presentation address (not used)
 *
 * Returns pointer to an NFS client, or an ERR_PTR value.
 */
struct nfs_client *nfs_init_client(struct nfs_client *clp,
		    const struct rpc_timeout *timeparms,
		    const char *ip_addr)
>>>>>>> refs/remotes/origin/master
{
	int error;

	if (clp->cl_cons_state == NFS_CS_READY) {
		/* the client is already initialised */
		dprintk("<-- nfs_init_client() = 0 [already %p]\n", clp);
<<<<<<< HEAD
		return 0;
=======
		return clp;
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * Create a client RPC handle for doing FSSTAT with UNIX auth only
	 * - RFC 2623, sec 2.3.2
	 */
<<<<<<< HEAD
	error = nfs_create_rpc_client(clp, timeparms, RPC_AUTH_UNIX,
				      0, noresvport);
	if (error < 0)
		goto error;
	nfs_mark_client_ready(clp, NFS_CS_READY);
	return 0;

error:
	nfs_mark_client_ready(clp, error);
	dprintk("<-- nfs_init_client() = xerror %d\n", error);
	return error;
}
=======
	error = nfs_create_rpc_client(clp, timeparms, RPC_AUTH_UNIX);
	if (error < 0)
		goto error;
	nfs_mark_client_ready(clp, NFS_CS_READY);
	return clp;

error:
	nfs_mark_client_ready(clp, error);
	nfs_put_client(clp);
	dprintk("<-- nfs_init_client() = xerror %d\n", error);
	return ERR_PTR(error);
}
EXPORT_SYMBOL_GPL(nfs_init_client);
>>>>>>> refs/remotes/origin/master

/*
 * Create a version 2 or 3 client
 */
static int nfs_init_server(struct nfs_server *server,
<<<<<<< HEAD
			   const struct nfs_parsed_mount_data *data)
=======
			   const struct nfs_parsed_mount_data *data,
			   struct nfs_subversion *nfs_mod)
>>>>>>> refs/remotes/origin/master
{
	struct nfs_client_initdata cl_init = {
		.hostname = data->nfs_server.hostname,
		.addr = (const struct sockaddr *)&data->nfs_server.address,
		.addrlen = data->nfs_server.addrlen,
<<<<<<< HEAD
		.rpc_ops = &nfs_v2_clientops,
		.proto = data->nfs_server.protocol,
<<<<<<< HEAD
=======
		.net = data->net,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.nfs_mod = nfs_mod,
		.proto = data->nfs_server.protocol,
		.net = data->net,
>>>>>>> refs/remotes/origin/master
	};
	struct rpc_timeout timeparms;
	struct nfs_client *clp;
	int error;

	dprintk("--> nfs_init_server()\n");

<<<<<<< HEAD
#ifdef CONFIG_NFS_V3
	if (data->version == 3)
		cl_init.rpc_ops = &nfs_v3_clientops;
#endif

	nfs_init_timeout_values(&timeparms, data->nfs_server.protocol,
			data->timeo, data->retrans);

	/* Allocate or find a client reference we can use */
	clp = nfs_get_client(&cl_init, &timeparms, NULL, RPC_AUTH_UNIX,
			     data->flags & NFS_MOUNT_NORESVPORT);
=======
	nfs_init_timeout_values(&timeparms, data->nfs_server.protocol,
			data->timeo, data->retrans);
	if (data->flags & NFS_MOUNT_NORESVPORT)
		set_bit(NFS_CS_NORESVPORT, &cl_init.init_flags);

	/* Allocate or find a client reference we can use */
	clp = nfs_get_client(&cl_init, &timeparms, NULL, RPC_AUTH_UNIX);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(clp)) {
		dprintk("<-- nfs_init_server() = error %ld\n", PTR_ERR(clp));
		return PTR_ERR(clp);
	}

	server->nfs_client = clp;

	/* Initialise the client representation from the mount data */
	server->flags = data->flags;
	server->options = data->options;
	server->caps |= NFS_CAP_HARDLINKS|NFS_CAP_SYMLINKS|NFS_CAP_FILEID|
		NFS_CAP_MODE|NFS_CAP_NLINK|NFS_CAP_OWNER|NFS_CAP_OWNER_GROUP|
<<<<<<< HEAD
		NFS_CAP_ATIME|NFS_CAP_CTIME|NFS_CAP_MTIME;
=======
		NFS_CAP_ATIME|NFS_CAP_CTIME|NFS_CAP_MTIME|NFS_CAP_CHANGE_ATTR;
>>>>>>> refs/remotes/origin/master

	if (data->rsize)
		server->rsize = nfs_block_size(data->rsize, NULL);
	if (data->wsize)
		server->wsize = nfs_block_size(data->wsize, NULL);

	server->acregmin = data->acregmin * HZ;
	server->acregmax = data->acregmax * HZ;
	server->acdirmin = data->acdirmin * HZ;
	server->acdirmax = data->acdirmax * HZ;

	/* Start lockd here, before we might error out */
	error = nfs_start_lockd(server);
	if (error < 0)
		goto error;

	server->port = data->nfs_server.port;
<<<<<<< HEAD

	error = nfs_init_server_rpcclient(server, &timeparms, data->auth_flavors[0]);
=======
	server->auth_info = data->auth_info;

	error = nfs_init_server_rpcclient(server, &timeparms,
					  data->selected_flavor);
>>>>>>> refs/remotes/origin/master
	if (error < 0)
		goto error;

	/* Preserve the values of mount_server-related mount options */
	if (data->mount_server.addrlen) {
		memcpy(&server->mountd_address, &data->mount_server.address,
			data->mount_server.addrlen);
		server->mountd_addrlen = data->mount_server.addrlen;
	}
	server->mountd_version = data->mount_server.version;
	server->mountd_port = data->mount_server.port;
	server->mountd_protocol = data->mount_server.protocol;

	server->namelen  = data->namlen;
<<<<<<< HEAD
	/* Create a client RPC handle for the NFSv3 ACL management interface */
	nfs_init_server_aclclient(server);
=======
>>>>>>> refs/remotes/origin/master
	dprintk("<-- nfs_init_server() = 0 [new %p]\n", clp);
	return 0;

error:
	server->nfs_client = NULL;
	nfs_put_client(clp);
	dprintk("<-- nfs_init_server() = xerror %d\n", error);
	return error;
}

/*
 * Load up the server record from information gained in an fsinfo record
 */
<<<<<<< HEAD
<<<<<<< HEAD
static void nfs_server_set_fsinfo(struct nfs_server *server, struct nfs_fsinfo *fsinfo)
=======
static void nfs_server_set_fsinfo(struct nfs_server *server,
				  struct nfs_fh *mntfh,
				  struct nfs_fsinfo *fsinfo)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void nfs_server_set_fsinfo(struct nfs_server *server,
				  struct nfs_fh *mntfh,
				  struct nfs_fsinfo *fsinfo)
>>>>>>> refs/remotes/origin/master
{
	unsigned long max_rpc_payload;

	/* Work out a lot of parameters */
	if (server->rsize == 0)
		server->rsize = nfs_block_size(fsinfo->rtpref, NULL);
	if (server->wsize == 0)
		server->wsize = nfs_block_size(fsinfo->wtpref, NULL);

	if (fsinfo->rtmax >= 512 && server->rsize > fsinfo->rtmax)
		server->rsize = nfs_block_size(fsinfo->rtmax, NULL);
	if (fsinfo->wtmax >= 512 && server->wsize > fsinfo->wtmax)
		server->wsize = nfs_block_size(fsinfo->wtmax, NULL);

	max_rpc_payload = nfs_block_size(rpc_max_payload(server->client), NULL);
	if (server->rsize > max_rpc_payload)
		server->rsize = max_rpc_payload;
	if (server->rsize > NFS_MAX_FILE_IO_SIZE)
		server->rsize = NFS_MAX_FILE_IO_SIZE;
	server->rpages = (server->rsize + PAGE_CACHE_SIZE - 1) >> PAGE_CACHE_SHIFT;

	server->backing_dev_info.name = "nfs";
	server->backing_dev_info.ra_pages = server->rpages * NFS_MAX_READAHEAD;

	if (server->wsize > max_rpc_payload)
		server->wsize = max_rpc_payload;
	if (server->wsize > NFS_MAX_FILE_IO_SIZE)
		server->wsize = NFS_MAX_FILE_IO_SIZE;
	server->wpages = (server->wsize + PAGE_CACHE_SIZE - 1) >> PAGE_CACHE_SHIFT;
<<<<<<< HEAD
<<<<<<< HEAD
	set_pnfs_layoutdriver(server, fsinfo->layouttype);
=======
	server->pnfs_blksize = fsinfo->blksize;
	set_pnfs_layoutdriver(server, mntfh, fsinfo->layouttype);
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	server->wtmult = nfs_block_bits(fsinfo->wtmult, NULL);

	server->dtsize = nfs_block_size(fsinfo->dtpref, NULL);
	if (server->dtsize > PAGE_CACHE_SIZE * NFS_MAX_READDIR_PAGES)
		server->dtsize = PAGE_CACHE_SIZE * NFS_MAX_READDIR_PAGES;
	if (server->dtsize > server->rsize)
		server->dtsize = server->rsize;

	if (server->flags & NFS_MOUNT_NOAC) {
		server->acregmin = server->acregmax = 0;
		server->acdirmin = server->acdirmax = 0;
	}

	server->maxfilesize = fsinfo->maxfilesize;

	server->time_delta = fsinfo->time_delta;

	/* We're airborne Set socket buffersize */
	rpc_setbufsize(server->client, server->wsize + 100, server->rsize + 100);
}

/*
 * Probe filesystem information, including the FSID on v2/v3
 */
<<<<<<< HEAD
static int nfs_probe_fsinfo(struct nfs_server *server, struct nfs_fh *mntfh, struct nfs_fattr *fattr)
=======
int nfs_probe_fsinfo(struct nfs_server *server, struct nfs_fh *mntfh, struct nfs_fattr *fattr)
>>>>>>> refs/remotes/origin/master
{
	struct nfs_fsinfo fsinfo;
	struct nfs_client *clp = server->nfs_client;
	int error;

	dprintk("--> nfs_probe_fsinfo()\n");

	if (clp->rpc_ops->set_capabilities != NULL) {
		error = clp->rpc_ops->set_capabilities(server, mntfh);
		if (error < 0)
			goto out_error;
	}

	fsinfo.fattr = fattr;
	fsinfo.layouttype = 0;
	error = clp->rpc_ops->fsinfo(server, mntfh, &fsinfo);
	if (error < 0)
		goto out_error;

<<<<<<< HEAD
<<<<<<< HEAD
	nfs_server_set_fsinfo(server, &fsinfo);
=======
	nfs_server_set_fsinfo(server, mntfh, &fsinfo);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	nfs_server_set_fsinfo(server, mntfh, &fsinfo);
>>>>>>> refs/remotes/origin/master

	/* Get some general file system info */
	if (server->namelen == 0) {
		struct nfs_pathconf pathinfo;

		pathinfo.fattr = fattr;
		nfs_fattr_init(fattr);

		if (clp->rpc_ops->pathconf(server, mntfh, &pathinfo) >= 0)
			server->namelen = pathinfo.max_namelen;
	}

	dprintk("<-- nfs_probe_fsinfo() = 0\n");
	return 0;

out_error:
	dprintk("nfs_probe_fsinfo: error = %d\n", -error);
	return error;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(nfs_probe_fsinfo);
>>>>>>> refs/remotes/origin/master

/*
 * Copy useful information when duplicating a server record
 */
<<<<<<< HEAD
static void nfs_server_copy_userdata(struct nfs_server *target, struct nfs_server *source)
=======
void nfs_server_copy_userdata(struct nfs_server *target, struct nfs_server *source)
>>>>>>> refs/remotes/origin/master
{
	target->flags = source->flags;
	target->rsize = source->rsize;
	target->wsize = source->wsize;
	target->acregmin = source->acregmin;
	target->acregmax = source->acregmax;
	target->acdirmin = source->acdirmin;
	target->acdirmax = source->acdirmax;
	target->caps = source->caps;
	target->options = source->options;
<<<<<<< HEAD
}

static void nfs_server_insert_lists(struct nfs_server *server)
{
	struct nfs_client *clp = server->nfs_client;
<<<<<<< HEAD

	spin_lock(&nfs_client_lock);
	list_add_tail_rcu(&server->client_link, &clp->cl_superblocks);
	list_add_tail(&server->master_link, &nfs_volume_list);
	clear_bit(NFS_CS_STOP_RENEW, &clp->cl_res_state);
	spin_unlock(&nfs_client_lock);
=======
	struct nfs_net *nn = net_generic(clp->net, nfs_net_id);
=======
	target->auth_info = source->auth_info;
}
EXPORT_SYMBOL_GPL(nfs_server_copy_userdata);

void nfs_server_insert_lists(struct nfs_server *server)
{
	struct nfs_client *clp = server->nfs_client;
	struct nfs_net *nn = net_generic(clp->cl_net, nfs_net_id);
>>>>>>> refs/remotes/origin/master

	spin_lock(&nn->nfs_client_lock);
	list_add_tail_rcu(&server->client_link, &clp->cl_superblocks);
	list_add_tail(&server->master_link, &nn->nfs_volume_list);
	clear_bit(NFS_CS_STOP_RENEW, &clp->cl_res_state);
	spin_unlock(&nn->nfs_client_lock);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

}

static void nfs_server_remove_lists(struct nfs_server *server)
{
	struct nfs_client *clp = server->nfs_client;
<<<<<<< HEAD

	spin_lock(&nfs_client_lock);
	list_del_rcu(&server->client_link);
	if (clp && list_empty(&clp->cl_superblocks))
		set_bit(NFS_CS_STOP_RENEW, &clp->cl_res_state);
	list_del(&server->master_link);
	spin_unlock(&nfs_client_lock);
=======
=======

}
EXPORT_SYMBOL_GPL(nfs_server_insert_lists);

void nfs_server_remove_lists(struct nfs_server *server)
{
	struct nfs_client *clp = server->nfs_client;
>>>>>>> refs/remotes/origin/master
	struct nfs_net *nn;

	if (clp == NULL)
		return;
<<<<<<< HEAD
	nn = net_generic(clp->net, nfs_net_id);
=======
	nn = net_generic(clp->cl_net, nfs_net_id);
>>>>>>> refs/remotes/origin/master
	spin_lock(&nn->nfs_client_lock);
	list_del_rcu(&server->client_link);
	if (list_empty(&clp->cl_superblocks))
		set_bit(NFS_CS_STOP_RENEW, &clp->cl_res_state);
	list_del(&server->master_link);
	spin_unlock(&nn->nfs_client_lock);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	synchronize_rcu();
}
=======

	synchronize_rcu();
}
EXPORT_SYMBOL_GPL(nfs_server_remove_lists);
>>>>>>> refs/remotes/origin/master

/*
 * Allocate and initialise a server record
 */
<<<<<<< HEAD
static struct nfs_server *nfs_alloc_server(void)
=======
struct nfs_server *nfs_alloc_server(void)
>>>>>>> refs/remotes/origin/master
{
	struct nfs_server *server;

	server = kzalloc(sizeof(struct nfs_server), GFP_KERNEL);
	if (!server)
		return NULL;

	server->client = server->client_acl = ERR_PTR(-EINVAL);

	/* Zero out the NFS state stuff */
	INIT_LIST_HEAD(&server->client_link);
	INIT_LIST_HEAD(&server->master_link);
	INIT_LIST_HEAD(&server->delegations);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	INIT_LIST_HEAD(&server->layouts);
	INIT_LIST_HEAD(&server->state_owners_lru);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	INIT_LIST_HEAD(&server->layouts);
	INIT_LIST_HEAD(&server->state_owners_lru);
>>>>>>> refs/remotes/origin/master

	atomic_set(&server->active, 0);

	server->io_stats = nfs_alloc_iostats();
	if (!server->io_stats) {
		kfree(server);
		return NULL;
	}

	if (bdi_init(&server->backing_dev_info)) {
		nfs_free_iostats(server->io_stats);
		kfree(server);
		return NULL;
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
	ida_init(&server->openowner_id);
	ida_init(&server->lockowner_id);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ida_init(&server->openowner_id);
	ida_init(&server->lockowner_id);
>>>>>>> refs/remotes/origin/master
	pnfs_init_server(server);

	return server;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(nfs_alloc_server);
>>>>>>> refs/remotes/origin/master

/*
 * Free up a server record
 */
void nfs_free_server(struct nfs_server *server)
{
	dprintk("--> nfs_free_server()\n");

	nfs_server_remove_lists(server);
<<<<<<< HEAD
	unset_pnfs_layoutdriver(server);
=======
>>>>>>> refs/remotes/origin/master

	if (server->destroy != NULL)
		server->destroy(server);

	if (!IS_ERR(server->client_acl))
		rpc_shutdown_client(server->client_acl);
	if (!IS_ERR(server->client))
		rpc_shutdown_client(server->client);

	nfs_put_client(server->nfs_client);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	ida_destroy(&server->lockowner_id);
	ida_destroy(&server->openowner_id);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ida_destroy(&server->lockowner_id);
	ida_destroy(&server->openowner_id);
>>>>>>> refs/remotes/origin/master
	nfs_free_iostats(server->io_stats);
	bdi_destroy(&server->backing_dev_info);
	kfree(server);
	nfs_release_automount_timer();
	dprintk("<-- nfs_free_server()\n");
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(nfs_free_server);
>>>>>>> refs/remotes/origin/master

/*
 * Create a version 2 or 3 volume record
 * - keyed on server and FSID
 */
<<<<<<< HEAD
struct nfs_server *nfs_create_server(const struct nfs_parsed_mount_data *data,
				     struct nfs_fh *mntfh)
=======
struct nfs_server *nfs_create_server(struct nfs_mount_info *mount_info,
				     struct nfs_subversion *nfs_mod)
>>>>>>> refs/remotes/origin/master
{
	struct nfs_server *server;
	struct nfs_fattr *fattr;
	int error;

	server = nfs_alloc_server();
	if (!server)
		return ERR_PTR(-ENOMEM);

	error = -ENOMEM;
	fattr = nfs_alloc_fattr();
	if (fattr == NULL)
		goto error;

	/* Get a client representation */
<<<<<<< HEAD
	error = nfs_init_server(server, data);
	if (error < 0)
		goto error;

	BUG_ON(!server->nfs_client);
	BUG_ON(!server->nfs_client->rpc_ops);
	BUG_ON(!server->nfs_client->rpc_ops->file_inode_ops);

	/* Probe the root fh to retrieve its FSID */
	error = nfs_probe_fsinfo(server, mntfh, fattr);
=======
	error = nfs_init_server(server, mount_info->parsed, nfs_mod);
	if (error < 0)
		goto error;

	/* Probe the root fh to retrieve its FSID */
	error = nfs_probe_fsinfo(server, mount_info->mntfh, fattr);
>>>>>>> refs/remotes/origin/master
	if (error < 0)
		goto error;
	if (server->nfs_client->rpc_ops->version == 3) {
		if (server->namelen == 0 || server->namelen > NFS3_MAXNAMLEN)
			server->namelen = NFS3_MAXNAMLEN;
<<<<<<< HEAD
		if (!(data->flags & NFS_MOUNT_NORDIRPLUS))
=======
		if (!(mount_info->parsed->flags & NFS_MOUNT_NORDIRPLUS))
>>>>>>> refs/remotes/origin/master
			server->caps |= NFS_CAP_READDIRPLUS;
	} else {
		if (server->namelen == 0 || server->namelen > NFS2_MAXNAMLEN)
			server->namelen = NFS2_MAXNAMLEN;
	}

	if (!(fattr->valid & NFS_ATTR_FATTR)) {
<<<<<<< HEAD
		error = server->nfs_client->rpc_ops->getattr(server, mntfh, fattr);
=======
		error = nfs_mod->rpc_ops->getattr(server, mount_info->mntfh, fattr, NULL);
>>>>>>> refs/remotes/origin/master
		if (error < 0) {
			dprintk("nfs_create_server: getattr error = %d\n", -error);
			goto error;
		}
	}
	memcpy(&server->fsid, &fattr->fsid, sizeof(server->fsid));

	dprintk("Server FSID: %llx:%llx\n",
		(unsigned long long) server->fsid.major,
		(unsigned long long) server->fsid.minor);

	nfs_server_insert_lists(server);
	server->mount_time = jiffies;
	nfs_free_fattr(fattr);
	return server;

error:
	nfs_free_fattr(fattr);
	nfs_free_server(server);
	return ERR_PTR(error);
}
<<<<<<< HEAD

#ifdef CONFIG_NFS_V4
/*
 * NFSv4.0 callback thread helper
 *
<<<<<<< HEAD
 * Find a client by IP address, protocol version, and minorversion
 *
 * Called from the pg_authenticate method. The callback identifier
 * is not used as it has not been decoded.
 *
 * Returns NULL if no such client
 */
struct nfs_client *
nfs4_find_client_no_ident(const struct sockaddr *addr)
{
	struct nfs_client *clp;

	spin_lock(&nfs_client_lock);
	list_for_each_entry(clp, &nfs_client_list, cl_share_link) {
		if (nfs4_cb_match_client(addr, clp, 0) == false)
			continue;
		atomic_inc(&clp->cl_count);
		spin_unlock(&nfs_client_lock);
		return clp;
	}
	spin_unlock(&nfs_client_lock);
	return NULL;
}

/*
 * NFSv4.0 callback thread helper
 *
 * Find a client by callback identifier
 */
struct nfs_client *
nfs4_find_client_ident(int cb_ident)
{
	struct nfs_client *clp;

	spin_lock(&nfs_client_lock);
	clp = idr_find(&cb_ident_idr, cb_ident);
	if (clp)
		atomic_inc(&clp->cl_count);
	spin_unlock(&nfs_client_lock);
=======
 * Find a client by callback identifier
 */
struct nfs_client *
nfs4_find_client_ident(struct net *net, int cb_ident)
{
	struct nfs_client *clp;
	struct nfs_net *nn = net_generic(net, nfs_net_id);

	spin_lock(&nn->nfs_client_lock);
	clp = idr_find(&nn->cb_ident_idr, cb_ident);
	if (clp)
		atomic_inc(&clp->cl_count);
	spin_unlock(&nn->nfs_client_lock);
>>>>>>> refs/remotes/origin/cm-10.0
	return clp;
}

#if defined(CONFIG_NFS_V4_1)
/*
 * NFSv4.1 callback thread helper
 * For CB_COMPOUND calls, find a client by IP address, protocol version,
 * minorversion, and sessionID
 *
 * Returns NULL if no such client
 */
struct nfs_client *
<<<<<<< HEAD
nfs4_find_client_sessionid(const struct sockaddr *addr,
			   struct nfs4_sessionid *sid)
{
	struct nfs_client *clp;

	spin_lock(&nfs_client_lock);
	list_for_each_entry(clp, &nfs_client_list, cl_share_link) {
=======
nfs4_find_client_sessionid(struct net *net, const struct sockaddr *addr,
			   struct nfs4_sessionid *sid)
{
	struct nfs_client *clp;
	struct nfs_net *nn = net_generic(net, nfs_net_id);

	spin_lock(&nn->nfs_client_lock);
	list_for_each_entry(clp, &nn->nfs_client_list, cl_share_link) {
>>>>>>> refs/remotes/origin/cm-10.0
		if (nfs4_cb_match_client(addr, clp, 1) == false)
			continue;

		if (!nfs4_has_session(clp))
			continue;

		/* Match sessionid*/
		if (memcmp(clp->cl_session->sess_id.data,
		    sid->data, NFS4_MAX_SESSIONID_LEN) != 0)
			continue;

		atomic_inc(&clp->cl_count);
<<<<<<< HEAD
		spin_unlock(&nfs_client_lock);
		return clp;
	}
	spin_unlock(&nfs_client_lock);
=======
		spin_unlock(&nn->nfs_client_lock);
		return clp;
	}
	spin_unlock(&nn->nfs_client_lock);
>>>>>>> refs/remotes/origin/cm-10.0
	return NULL;
}

#else /* CONFIG_NFS_V4_1 */

struct nfs_client *
<<<<<<< HEAD
nfs4_find_client_sessionid(const struct sockaddr *addr,
=======
nfs4_find_client_sessionid(struct net *net, const struct sockaddr *addr,
>>>>>>> refs/remotes/origin/cm-10.0
			   struct nfs4_sessionid *sid)
{
	return NULL;
}
#endif /* CONFIG_NFS_V4_1 */

/*
 * Initialize the NFS4 callback service
 */
static int nfs4_init_callback(struct nfs_client *clp)
{
	int error;

	if (clp->rpc_ops->version == 4) {
<<<<<<< HEAD
		if (nfs4_has_session(clp)) {
			error = xprt_setup_backchannel(
						clp->cl_rpcclient->cl_xprt,
=======
		struct rpc_xprt *xprt;

		xprt = rcu_dereference_raw(clp->cl_rpcclient->cl_xprt);

		if (nfs4_has_session(clp)) {
			error = xprt_setup_backchannel(xprt,
>>>>>>> refs/remotes/origin/cm-10.0
						NFS41_BC_MIN_CALLBACKS);
			if (error < 0)
				return error;
		}

<<<<<<< HEAD
		error = nfs_callback_up(clp->cl_mvops->minor_version,
					clp->cl_rpcclient->cl_xprt);
=======
		error = nfs_callback_up(clp->cl_mvops->minor_version, xprt);
>>>>>>> refs/remotes/origin/cm-10.0
		if (error < 0) {
			dprintk("%s: failed to start callback. Error = %d\n",
				__func__, error);
			return error;
		}
		__set_bit(NFS_CS_CALLBACK, &clp->cl_res_state);
	}
	return 0;
}

/*
 * Initialize the minor version specific parts of an NFS4 client record
 */
static int nfs4_init_client_minor_version(struct nfs_client *clp)
{
#if defined(CONFIG_NFS_V4_1)
	if (clp->cl_mvops->minor_version) {
		struct nfs4_session *session = NULL;
		/*
		 * Create the session and mark it expired.
		 * When a SEQUENCE operation encounters the expired session
		 * it will do session recovery to initialize it.
		 */
		session = nfs4_alloc_session(clp);
		if (!session)
			return -ENOMEM;

		clp->cl_session = session;
		/*
		 * The create session reply races with the server back
		 * channel probe. Mark the client NFS_CS_SESSION_INITING
		 * so that the client back channel can find the
		 * nfs_client struct
		 */
		clp->cl_cons_state = NFS_CS_SESSION_INITING;
	}
#endif /* CONFIG_NFS_V4_1 */

	return nfs4_init_callback(clp);
}

/*
 * Initialise an NFS4 client record
 */
int nfs4_init_client(struct nfs_client *clp,
		     const struct rpc_timeout *timeparms,
		     const char *ip_addr,
		     rpc_authflavor_t authflavour,
		     int noresvport)
{
<<<<<<< HEAD
=======
	char buf[INET6_ADDRSTRLEN + 1];
>>>>>>> refs/remotes/origin/cm-10.0
	int error;

	if (clp->cl_cons_state == NFS_CS_READY) {
		/* the client is initialised already */
		dprintk("<-- nfs4_init_client() = 0 [already %p]\n", clp);
		return 0;
	}

	/* Check NFS protocol revision and initialize RPC op vector */
	clp->rpc_ops = &nfs_v4_clientops;

	error = nfs_create_rpc_client(clp, timeparms, authflavour,
				      1, noresvport);
	if (error < 0)
		goto error;
<<<<<<< HEAD
=======

	/* If no clientaddr= option was specified, find a usable cb address */
	if (ip_addr == NULL) {
		struct sockaddr_storage cb_addr;
		struct sockaddr *sap = (struct sockaddr *)&cb_addr;

		error = rpc_localaddr(clp->cl_rpcclient, sap, sizeof(cb_addr));
		if (error < 0)
			goto error;
		error = rpc_ntop(sap, buf, sizeof(buf));
		if (error < 0)
			goto error;
		ip_addr = (const char *)buf;
	}
>>>>>>> refs/remotes/origin/cm-10.0
	strlcpy(clp->cl_ipaddr, ip_addr, sizeof(clp->cl_ipaddr));

	error = nfs_idmap_new(clp);
	if (error < 0) {
		dprintk("%s: failed to create idmapper. Error = %d\n",
			__func__, error);
		goto error;
	}
	__set_bit(NFS_CS_IDMAP, &clp->cl_res_state);

	error = nfs4_init_client_minor_version(clp);
	if (error < 0)
		goto error;

	if (!nfs4_has_session(clp))
		nfs_mark_client_ready(clp, NFS_CS_READY);
	return 0;

error:
	nfs_mark_client_ready(clp, error);
	dprintk("<-- nfs4_init_client() = xerror %d\n", error);
	return error;
}

/*
 * Set up an NFS4 client
 */
static int nfs4_set_client(struct nfs_server *server,
		const char *hostname,
		const struct sockaddr *addr,
		const size_t addrlen,
		const char *ip_addr,
		rpc_authflavor_t authflavour,
		int proto, const struct rpc_timeout *timeparms,
<<<<<<< HEAD
		u32 minorversion)
=======
		u32 minorversion, struct net *net)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct nfs_client_initdata cl_init = {
		.hostname = hostname,
		.addr = addr,
		.addrlen = addrlen,
		.rpc_ops = &nfs_v4_clientops,
		.proto = proto,
		.minorversion = minorversion,
<<<<<<< HEAD
=======
		.net = net,
>>>>>>> refs/remotes/origin/cm-10.0
	};
	struct nfs_client *clp;
	int error;

	dprintk("--> nfs4_set_client()\n");

	/* Allocate or find a client reference we can use */
	clp = nfs_get_client(&cl_init, timeparms, ip_addr, authflavour,
			     server->flags & NFS_MOUNT_NORESVPORT);
	if (IS_ERR(clp)) {
		error = PTR_ERR(clp);
		goto error;
	}

	/*
	 * Query for the lease time on clientid setup or renewal
	 *
	 * Note that this will be set on nfs_clients that were created
	 * only for the DS role and did not set this bit, but now will
	 * serve a dual role.
	 */
	set_bit(NFS_CS_CHECK_LEASE_TIME, &clp->cl_res_state);

	server->nfs_client = clp;
	dprintk("<-- nfs4_set_client() = 0 [new %p]\n", clp);
	return 0;
error:
	dprintk("<-- nfs4_set_client() = xerror %d\n", error);
	return error;
}

/*
 * Set up a pNFS Data Server client.
 *
 * Return any existing nfs_client that matches server address,port,version
 * and minorversion.
 *
 * For a new nfs_client, use a soft mount (default), a low retrans and a
 * low timeout interval so that if a connection is lost, we retry through
 * the MDS.
 */
struct nfs_client *nfs4_set_ds_client(struct nfs_client* mds_clp,
		const struct sockaddr *ds_addr,
		int ds_addrlen, int ds_proto)
{
	struct nfs_client_initdata cl_init = {
		.addr = ds_addr,
		.addrlen = ds_addrlen,
		.rpc_ops = &nfs_v4_clientops,
		.proto = ds_proto,
		.minorversion = mds_clp->cl_minorversion,
<<<<<<< HEAD
=======
		.net = mds_clp->net,
>>>>>>> refs/remotes/origin/cm-10.0
	};
	struct rpc_timeout ds_timeout = {
		.to_initval = 15 * HZ,
		.to_maxval = 15 * HZ,
		.to_retries = 1,
		.to_exponential = 1,
	};
	struct nfs_client *clp;

	/*
	 * Set an authflavor equual to the MDS value. Use the MDS nfs_client
	 * cl_ipaddr so as to use the same EXCHANGE_ID co_ownerid as the MDS
	 * (section 13.1 RFC 5661).
	 */
	clp = nfs_get_client(&cl_init, &ds_timeout, mds_clp->cl_ipaddr,
			     mds_clp->cl_rpcclient->cl_auth->au_flavor, 0);

	dprintk("<-- %s %p\n", __func__, clp);
	return clp;
}
<<<<<<< HEAD
EXPORT_SYMBOL(nfs4_set_ds_client);
=======
EXPORT_SYMBOL_GPL(nfs4_set_ds_client);
>>>>>>> refs/remotes/origin/cm-10.0

/*
 * Session has been established, and the client marked ready.
 * Set the mount rsize and wsize with negotiated fore channel
 * attributes which will be bound checked in nfs_server_set_fsinfo.
 */
static void nfs4_session_set_rwsize(struct nfs_server *server)
{
#ifdef CONFIG_NFS_V4_1
	struct nfs4_session *sess;
	u32 server_resp_sz;
	u32 server_rqst_sz;

	if (!nfs4_has_session(server->nfs_client))
		return;
	sess = server->nfs_client->cl_session;
	server_resp_sz = sess->fc_attrs.max_resp_sz - nfs41_maxread_overhead;
	server_rqst_sz = sess->fc_attrs.max_rqst_sz - nfs41_maxwrite_overhead;

	if (server->rsize > server_resp_sz)
		server->rsize = server_resp_sz;
	if (server->wsize > server_rqst_sz)
		server->wsize = server_rqst_sz;
#endif /* CONFIG_NFS_V4_1 */
}

static int nfs4_server_common_setup(struct nfs_server *server,
		struct nfs_fh *mntfh)
{
	struct nfs_fattr *fattr;
	int error;

	BUG_ON(!server->nfs_client);
	BUG_ON(!server->nfs_client->rpc_ops);
	BUG_ON(!server->nfs_client->rpc_ops->file_inode_ops);

	/* data servers support only a subset of NFSv4.1 */
	if (is_ds_only_client(server->nfs_client))
		return -EPROTONOSUPPORT;

	fattr = nfs_alloc_fattr();
	if (fattr == NULL)
		return -ENOMEM;

	/* We must ensure the session is initialised first */
	error = nfs4_init_session(server);
	if (error < 0)
		goto out;

	/* Probe the root fh to retrieve its FSID and filehandle */
	error = nfs4_get_rootfh(server, mntfh);
	if (error < 0)
		goto out;

	dprintk("Server FSID: %llx:%llx\n",
			(unsigned long long) server->fsid.major,
			(unsigned long long) server->fsid.minor);
	dprintk("Mount FH: %d\n", mntfh->size);

	nfs4_session_set_rwsize(server);

	error = nfs_probe_fsinfo(server, mntfh, fattr);
	if (error < 0)
		goto out;

	if (server->namelen == 0 || server->namelen > NFS4_MAXNAMLEN)
		server->namelen = NFS4_MAXNAMLEN;

	nfs_server_insert_lists(server);
	server->mount_time = jiffies;
<<<<<<< HEAD
=======
	server->destroy = nfs4_destroy_server;
>>>>>>> refs/remotes/origin/cm-10.0
out:
	nfs_free_fattr(fattr);
	return error;
}

/*
 * Create a version 4 volume record
 */
static int nfs4_init_server(struct nfs_server *server,
		const struct nfs_parsed_mount_data *data)
{
	struct rpc_timeout timeparms;
	int error;

	dprintk("--> nfs4_init_server()\n");

	nfs_init_timeout_values(&timeparms, data->nfs_server.protocol,
			data->timeo, data->retrans);

	/* Initialise the client representation from the mount data */
	server->flags = data->flags;
	server->caps |= NFS_CAP_ATOMIC_OPEN|NFS_CAP_CHANGE_ATTR|NFS_CAP_POSIX_LOCK;
	if (!(data->flags & NFS_MOUNT_NORDIRPLUS))
			server->caps |= NFS_CAP_READDIRPLUS;
	server->options = data->options;

	/* Get a client record */
	error = nfs4_set_client(server,
			data->nfs_server.hostname,
			(const struct sockaddr *)&data->nfs_server.address,
			data->nfs_server.addrlen,
			data->client_address,
			data->auth_flavors[0],
			data->nfs_server.protocol,
			&timeparms,
<<<<<<< HEAD
			data->minorversion);
=======
			data->minorversion,
			data->net);
>>>>>>> refs/remotes/origin/cm-10.0
	if (error < 0)
		goto error;

	/*
	 * Don't use NFS uid/gid mapping if we're using AUTH_SYS or lower
	 * authentication.
	 */
	if (nfs4_disable_idmapping && data->auth_flavors[0] == RPC_AUTH_UNIX)
		server->caps |= NFS_CAP_UIDGID_NOMAP;

	if (data->rsize)
		server->rsize = nfs_block_size(data->rsize, NULL);
	if (data->wsize)
		server->wsize = nfs_block_size(data->wsize, NULL);

	server->acregmin = data->acregmin * HZ;
	server->acregmax = data->acregmax * HZ;
	server->acdirmin = data->acdirmin * HZ;
	server->acdirmax = data->acdirmax * HZ;

	server->port = data->nfs_server.port;

	error = nfs_init_server_rpcclient(server, &timeparms, data->auth_flavors[0]);

error:
	/* Done */
	dprintk("<-- nfs4_init_server() = %d\n", error);
	return error;
}

/*
 * Create a version 4 volume record
 * - keyed on server and FSID
 */
struct nfs_server *nfs4_create_server(const struct nfs_parsed_mount_data *data,
				      struct nfs_fh *mntfh)
{
	struct nfs_server *server;
	int error;

	dprintk("--> nfs4_create_server()\n");

	server = nfs_alloc_server();
	if (!server)
		return ERR_PTR(-ENOMEM);

	/* set up the general RPC client */
	error = nfs4_init_server(server, data);
	if (error < 0)
		goto error;

	error = nfs4_server_common_setup(server, mntfh);
	if (error < 0)
		goto error;

	dprintk("<-- nfs4_create_server() = %p\n", server);
	return server;

error:
	nfs_free_server(server);
	dprintk("<-- nfs4_create_server() = error %d\n", error);
	return ERR_PTR(error);
}

/*
 * Create an NFS4 referral server record
 */
struct nfs_server *nfs4_create_referral_server(struct nfs_clone_mount *data,
					       struct nfs_fh *mntfh)
{
	struct nfs_client *parent_client;
	struct nfs_server *server, *parent_server;
	int error;

	dprintk("--> nfs4_create_referral_server()\n");

	server = nfs_alloc_server();
	if (!server)
		return ERR_PTR(-ENOMEM);

	parent_server = NFS_SB(data->sb);
	parent_client = parent_server->nfs_client;

	/* Initialise the client representation from the parent server */
	nfs_server_copy_userdata(server, parent_server);
	server->caps |= NFS_CAP_ATOMIC_OPEN|NFS_CAP_CHANGE_ATTR;

	/* Get a client representation.
	 * Note: NFSv4 always uses TCP, */
	error = nfs4_set_client(server, data->hostname,
				data->addr,
				data->addrlen,
				parent_client->cl_ipaddr,
				data->authflavor,
<<<<<<< HEAD
				parent_server->client->cl_xprt->prot,
				parent_server->client->cl_timeout,
				parent_client->cl_mvops->minor_version);
=======
				rpc_protocol(parent_server->client),
				parent_server->client->cl_timeout,
				parent_client->cl_mvops->minor_version,
				parent_client->net);
>>>>>>> refs/remotes/origin/cm-10.0
	if (error < 0)
		goto error;

	error = nfs_init_server_rpcclient(server, parent_server->client->cl_timeout, data->authflavor);
	if (error < 0)
		goto error;

	error = nfs4_server_common_setup(server, mntfh);
	if (error < 0)
		goto error;

	dprintk("<-- nfs_create_referral_server() = %p\n", server);
	return server;

error:
	nfs_free_server(server);
	dprintk("<-- nfs4_create_referral_server() = error %d\n", error);
	return ERR_PTR(error);
}

#endif /* CONFIG_NFS_V4 */
=======
EXPORT_SYMBOL_GPL(nfs_create_server);
>>>>>>> refs/remotes/origin/master

/*
 * Clone an NFS2, NFS3 or NFS4 server record
 */
struct nfs_server *nfs_clone_server(struct nfs_server *source,
				    struct nfs_fh *fh,
<<<<<<< HEAD
<<<<<<< HEAD
				    struct nfs_fattr *fattr)
=======
				    struct nfs_fattr *fattr,
				    rpc_authflavor_t flavor)
>>>>>>> refs/remotes/origin/cm-10.0
=======
				    struct nfs_fattr *fattr,
				    rpc_authflavor_t flavor)
>>>>>>> refs/remotes/origin/master
{
	struct nfs_server *server;
	struct nfs_fattr *fattr_fsinfo;
	int error;

	dprintk("--> nfs_clone_server(,%llx:%llx,)\n",
		(unsigned long long) fattr->fsid.major,
		(unsigned long long) fattr->fsid.minor);

	server = nfs_alloc_server();
	if (!server)
		return ERR_PTR(-ENOMEM);

	error = -ENOMEM;
	fattr_fsinfo = nfs_alloc_fattr();
	if (fattr_fsinfo == NULL)
		goto out_free_server;

	/* Copy data from the source */
	server->nfs_client = source->nfs_client;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	server->destroy = source->destroy;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	server->destroy = source->destroy;
>>>>>>> refs/remotes/origin/master
	atomic_inc(&server->nfs_client->cl_count);
	nfs_server_copy_userdata(server, source);

	server->fsid = fattr->fsid;

	error = nfs_init_server_rpcclient(server,
			source->client->cl_timeout,
<<<<<<< HEAD
<<<<<<< HEAD
			source->client->cl_auth->au_flavor);
=======
			flavor);
>>>>>>> refs/remotes/origin/cm-10.0
	if (error < 0)
		goto out_free_server;
	if (!IS_ERR(source->client_acl))
		nfs_init_server_aclclient(server);
=======
			flavor);
	if (error < 0)
		goto out_free_server;
>>>>>>> refs/remotes/origin/master

	/* probe the filesystem info for this server filesystem */
	error = nfs_probe_fsinfo(server, fh, fattr_fsinfo);
	if (error < 0)
		goto out_free_server;

	if (server->namelen == 0 || server->namelen > NFS4_MAXNAMLEN)
		server->namelen = NFS4_MAXNAMLEN;

	dprintk("Cloned FSID: %llx:%llx\n",
		(unsigned long long) server->fsid.major,
		(unsigned long long) server->fsid.minor);

	error = nfs_start_lockd(server);
	if (error < 0)
		goto out_free_server;

	nfs_server_insert_lists(server);
	server->mount_time = jiffies;

	nfs_free_fattr(fattr_fsinfo);
	dprintk("<-- nfs_clone_server() = %p\n", server);
	return server;

out_free_server:
	nfs_free_fattr(fattr_fsinfo);
	nfs_free_server(server);
	dprintk("<-- nfs_clone_server() = error %d\n", error);
	return ERR_PTR(error);
}
<<<<<<< HEAD

<<<<<<< HEAD
=======
=======
EXPORT_SYMBOL_GPL(nfs_clone_server);

>>>>>>> refs/remotes/origin/master
void nfs_clients_init(struct net *net)
{
	struct nfs_net *nn = net_generic(net, nfs_net_id);

	INIT_LIST_HEAD(&nn->nfs_client_list);
	INIT_LIST_HEAD(&nn->nfs_volume_list);
<<<<<<< HEAD
#ifdef CONFIG_NFS_V4
	idr_init(&nn->cb_ident_idr);
#endif
	spin_lock_init(&nn->nfs_client_lock);
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
#if IS_ENABLED(CONFIG_NFS_V4)
	idr_init(&nn->cb_ident_idr);
#endif
	spin_lock_init(&nn->nfs_client_lock);
	nn->boot_time = CURRENT_TIME;
}

>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_PROC_FS
static struct proc_dir_entry *proc_fs_nfs;

static int nfs_server_list_open(struct inode *inode, struct file *file);
static void *nfs_server_list_start(struct seq_file *p, loff_t *pos);
static void *nfs_server_list_next(struct seq_file *p, void *v, loff_t *pos);
static void nfs_server_list_stop(struct seq_file *p, void *v);
static int nfs_server_list_show(struct seq_file *m, void *v);

static const struct seq_operations nfs_server_list_ops = {
	.start	= nfs_server_list_start,
	.next	= nfs_server_list_next,
	.stop	= nfs_server_list_stop,
	.show	= nfs_server_list_show,
};

static const struct file_operations nfs_server_list_fops = {
	.open		= nfs_server_list_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
	.owner		= THIS_MODULE,
};

static int nfs_volume_list_open(struct inode *inode, struct file *file);
static void *nfs_volume_list_start(struct seq_file *p, loff_t *pos);
static void *nfs_volume_list_next(struct seq_file *p, void *v, loff_t *pos);
static void nfs_volume_list_stop(struct seq_file *p, void *v);
static int nfs_volume_list_show(struct seq_file *m, void *v);

static const struct seq_operations nfs_volume_list_ops = {
	.start	= nfs_volume_list_start,
	.next	= nfs_volume_list_next,
	.stop	= nfs_volume_list_stop,
	.show	= nfs_volume_list_show,
};

static const struct file_operations nfs_volume_list_fops = {
	.open		= nfs_volume_list_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
	.owner		= THIS_MODULE,
};

/*
 * open "/proc/fs/nfsfs/servers" which provides a summary of servers with which
 * we're dealing
 */
static int nfs_server_list_open(struct inode *inode, struct file *file)
{
	struct seq_file *m;
	int ret;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct pid_namespace *pid_ns = file->f_dentry->d_sb->s_fs_info;
	struct net *net = pid_ns->child_reaper->nsproxy->net_ns;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct pid_namespace *pid_ns = file->f_dentry->d_sb->s_fs_info;
	struct net *net = pid_ns->child_reaper->nsproxy->net_ns;
>>>>>>> refs/remotes/origin/master

	ret = seq_open(file, &nfs_server_list_ops);
	if (ret < 0)
		return ret;

	m = file->private_data;
<<<<<<< HEAD
<<<<<<< HEAD
	m->private = PDE(inode)->data;
=======
	m->private = net;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	m->private = net;
>>>>>>> refs/remotes/origin/master

	return 0;
}

/*
 * set up the iterator to start reading from the server list and return the first item
 */
static void *nfs_server_list_start(struct seq_file *m, loff_t *_pos)
{
<<<<<<< HEAD
<<<<<<< HEAD
	/* lock the list against modification */
	spin_lock(&nfs_client_lock);
	return seq_list_start_head(&nfs_client_list, *_pos);
=======
=======
>>>>>>> refs/remotes/origin/master
	struct nfs_net *nn = net_generic(m->private, nfs_net_id);

	/* lock the list against modification */
	spin_lock(&nn->nfs_client_lock);
	return seq_list_start_head(&nn->nfs_client_list, *_pos);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/*
 * move to next server
 */
static void *nfs_server_list_next(struct seq_file *p, void *v, loff_t *pos)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return seq_list_next(v, &nfs_client_list, pos);
=======
	struct nfs_net *nn = net_generic(p->private, nfs_net_id);

	return seq_list_next(v, &nn->nfs_client_list, pos);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct nfs_net *nn = net_generic(p->private, nfs_net_id);

	return seq_list_next(v, &nn->nfs_client_list, pos);
>>>>>>> refs/remotes/origin/master
}

/*
 * clean up after reading from the transports list
 */
static void nfs_server_list_stop(struct seq_file *p, void *v)
{
<<<<<<< HEAD
<<<<<<< HEAD
	spin_unlock(&nfs_client_lock);
=======
	struct nfs_net *nn = net_generic(p->private, nfs_net_id);

	spin_unlock(&nn->nfs_client_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct nfs_net *nn = net_generic(p->private, nfs_net_id);

	spin_unlock(&nn->nfs_client_lock);
>>>>>>> refs/remotes/origin/master
}

/*
 * display a header line followed by a load of call lines
 */
static int nfs_server_list_show(struct seq_file *m, void *v)
{
	struct nfs_client *clp;
<<<<<<< HEAD
<<<<<<< HEAD

	/* display header on line 1 */
	if (v == &nfs_client_list) {
=======
=======
>>>>>>> refs/remotes/origin/master
	struct nfs_net *nn = net_generic(m->private, nfs_net_id);

	/* display header on line 1 */
	if (v == &nn->nfs_client_list) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		seq_puts(m, "NV SERVER   PORT USE HOSTNAME\n");
		return 0;
	}

	/* display one transport per line on subsequent lines */
	clp = list_entry(v, struct nfs_client, cl_share_link);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	/* Check if the client is initialized */
	if (clp->cl_cons_state != NFS_CS_READY)
		return 0;

	rcu_read_lock();
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	seq_printf(m, "v%u %s %s %3d %s\n",
		   clp->rpc_ops->version,
		   rpc_peeraddr2str(clp->cl_rpcclient, RPC_DISPLAY_HEX_ADDR),
		   rpc_peeraddr2str(clp->cl_rpcclient, RPC_DISPLAY_HEX_PORT),
		   atomic_read(&clp->cl_count),
		   clp->cl_hostname);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	rcu_read_unlock();
>>>>>>> refs/remotes/origin/cm-10.0
=======
	rcu_read_unlock();
>>>>>>> refs/remotes/origin/master

	return 0;
}

/*
 * open "/proc/fs/nfsfs/volumes" which provides a summary of extant volumes
 */
static int nfs_volume_list_open(struct inode *inode, struct file *file)
{
	struct seq_file *m;
	int ret;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct pid_namespace *pid_ns = file->f_dentry->d_sb->s_fs_info;
	struct net *net = pid_ns->child_reaper->nsproxy->net_ns;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct pid_namespace *pid_ns = file->f_dentry->d_sb->s_fs_info;
	struct net *net = pid_ns->child_reaper->nsproxy->net_ns;
>>>>>>> refs/remotes/origin/master

	ret = seq_open(file, &nfs_volume_list_ops);
	if (ret < 0)
		return ret;

	m = file->private_data;
<<<<<<< HEAD
<<<<<<< HEAD
	m->private = PDE(inode)->data;
=======
	m->private = net;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	m->private = net;
>>>>>>> refs/remotes/origin/master

	return 0;
}

/*
 * set up the iterator to start reading from the volume list and return the first item
 */
static void *nfs_volume_list_start(struct seq_file *m, loff_t *_pos)
{
<<<<<<< HEAD
<<<<<<< HEAD
	/* lock the list against modification */
	spin_lock(&nfs_client_lock);
	return seq_list_start_head(&nfs_volume_list, *_pos);
=======
=======
>>>>>>> refs/remotes/origin/master
	struct nfs_net *nn = net_generic(m->private, nfs_net_id);

	/* lock the list against modification */
	spin_lock(&nn->nfs_client_lock);
	return seq_list_start_head(&nn->nfs_volume_list, *_pos);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/*
 * move to next volume
 */
static void *nfs_volume_list_next(struct seq_file *p, void *v, loff_t *pos)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return seq_list_next(v, &nfs_volume_list, pos);
=======
	struct nfs_net *nn = net_generic(p->private, nfs_net_id);

	return seq_list_next(v, &nn->nfs_volume_list, pos);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct nfs_net *nn = net_generic(p->private, nfs_net_id);

	return seq_list_next(v, &nn->nfs_volume_list, pos);
>>>>>>> refs/remotes/origin/master
}

/*
 * clean up after reading from the transports list
 */
static void nfs_volume_list_stop(struct seq_file *p, void *v)
{
<<<<<<< HEAD
<<<<<<< HEAD
	spin_unlock(&nfs_client_lock);
=======
	struct nfs_net *nn = net_generic(p->private, nfs_net_id);

	spin_unlock(&nn->nfs_client_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct nfs_net *nn = net_generic(p->private, nfs_net_id);

	spin_unlock(&nn->nfs_client_lock);
>>>>>>> refs/remotes/origin/master
}

/*
 * display a header line followed by a load of call lines
 */
static int nfs_volume_list_show(struct seq_file *m, void *v)
{
	struct nfs_server *server;
	struct nfs_client *clp;
	char dev[8], fsid[17];
<<<<<<< HEAD
<<<<<<< HEAD

	/* display header on line 1 */
	if (v == &nfs_volume_list) {
=======
=======
>>>>>>> refs/remotes/origin/master
	struct nfs_net *nn = net_generic(m->private, nfs_net_id);

	/* display header on line 1 */
	if (v == &nn->nfs_volume_list) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		seq_puts(m, "NV SERVER   PORT DEV     FSID              FSC\n");
		return 0;
	}
	/* display one transport per line on subsequent lines */
	server = list_entry(v, struct nfs_server, master_link);
	clp = server->nfs_client;

	snprintf(dev, 8, "%u:%u",
		 MAJOR(server->s_dev), MINOR(server->s_dev));

	snprintf(fsid, 17, "%llx:%llx",
		 (unsigned long long) server->fsid.major,
		 (unsigned long long) server->fsid.minor);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	rcu_read_lock();
>>>>>>> refs/remotes/origin/cm-10.0
=======
	rcu_read_lock();
>>>>>>> refs/remotes/origin/master
	seq_printf(m, "v%u %s %s %-7s %-17s %s\n",
		   clp->rpc_ops->version,
		   rpc_peeraddr2str(clp->cl_rpcclient, RPC_DISPLAY_HEX_ADDR),
		   rpc_peeraddr2str(clp->cl_rpcclient, RPC_DISPLAY_HEX_PORT),
		   dev,
		   fsid,
		   nfs_server_fscache_state(server));
<<<<<<< HEAD
<<<<<<< HEAD
=======
	rcu_read_unlock();
>>>>>>> refs/remotes/origin/cm-10.0
=======
	rcu_read_unlock();
>>>>>>> refs/remotes/origin/master

	return 0;
}

/*
 * initialise the /proc/fs/nfsfs/ directory
 */
int __init nfs_fs_proc_init(void)
{
	struct proc_dir_entry *p;

	proc_fs_nfs = proc_mkdir("fs/nfsfs", NULL);
	if (!proc_fs_nfs)
		goto error_0;

	/* a file of servers with which we're dealing */
	p = proc_create("servers", S_IFREG|S_IRUGO,
			proc_fs_nfs, &nfs_server_list_fops);
	if (!p)
		goto error_1;

	/* a file of volumes that we have mounted */
	p = proc_create("volumes", S_IFREG|S_IRUGO,
			proc_fs_nfs, &nfs_volume_list_fops);
	if (!p)
		goto error_2;
	return 0;

error_2:
	remove_proc_entry("servers", proc_fs_nfs);
error_1:
	remove_proc_entry("fs/nfsfs", NULL);
error_0:
	return -ENOMEM;
}

/*
 * clean up the /proc/fs/nfsfs/ directory
 */
void nfs_fs_proc_exit(void)
{
	remove_proc_entry("volumes", proc_fs_nfs);
	remove_proc_entry("servers", proc_fs_nfs);
	remove_proc_entry("fs/nfsfs", NULL);
}

#endif /* CONFIG_PROC_FS */
<<<<<<< HEAD

module_param(nfs4_disable_idmapping, bool, 0644);
MODULE_PARM_DESC(nfs4_disable_idmapping,
		"Turn off NFSv4 idmapping when using 'sec=sys'");
=======
>>>>>>> refs/remotes/origin/master
