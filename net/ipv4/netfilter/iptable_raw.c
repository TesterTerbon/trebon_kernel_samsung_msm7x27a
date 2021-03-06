/*
 * 'raw' table, which is the very first hooked in at PRE_ROUTING and LOCAL_OUT .
 *
 * Copyright (C) 2003 Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>
 */
#include <linux/module.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/slab.h>
#include <net/ip.h>

#define RAW_VALID_HOOKS ((1 << NF_INET_PRE_ROUTING) | (1 << NF_INET_LOCAL_OUT))

static const struct xt_table packet_raw = {
	.name = "raw",
	.valid_hooks =  RAW_VALID_HOOKS,
	.me = THIS_MODULE,
	.af = NFPROTO_IPV4,
	.priority = NF_IP_PRI_RAW,
};

/* The work comes in here from netfilter.c. */
static unsigned int
<<<<<<< HEAD
iptable_raw_hook(unsigned int hook, struct sk_buff *skb,
=======
iptable_raw_hook(const struct nf_hook_ops *ops, struct sk_buff *skb,
>>>>>>> refs/remotes/origin/master
		 const struct net_device *in, const struct net_device *out,
		 int (*okfn)(struct sk_buff *))
{
	const struct net *net;

<<<<<<< HEAD
	if (hook == NF_INET_LOCAL_OUT && 
=======
	if (ops->hooknum == NF_INET_LOCAL_OUT &&
>>>>>>> refs/remotes/origin/master
	    (skb->len < sizeof(struct iphdr) ||
	     ip_hdrlen(skb) < sizeof(struct iphdr)))
		/* root is playing with raw sockets. */
		return NF_ACCEPT;

	net = dev_net((in != NULL) ? in : out);
<<<<<<< HEAD
	return ipt_do_table(skb, hook, in, out, net->ipv4.iptable_raw);
=======
	return ipt_do_table(skb, ops->hooknum, in, out, net->ipv4.iptable_raw);
>>>>>>> refs/remotes/origin/master
}

static struct nf_hook_ops *rawtable_ops __read_mostly;

static int __net_init iptable_raw_net_init(struct net *net)
{
	struct ipt_replace *repl;

	repl = ipt_alloc_initial_table(&packet_raw);
	if (repl == NULL)
		return -ENOMEM;
	net->ipv4.iptable_raw =
		ipt_register_table(net, &packet_raw, repl);
	kfree(repl);
<<<<<<< HEAD
	if (IS_ERR(net->ipv4.iptable_raw))
		return PTR_ERR(net->ipv4.iptable_raw);
	return 0;
=======
	return PTR_ERR_OR_ZERO(net->ipv4.iptable_raw);
>>>>>>> refs/remotes/origin/master
}

static void __net_exit iptable_raw_net_exit(struct net *net)
{
	ipt_unregister_table(net, net->ipv4.iptable_raw);
}

static struct pernet_operations iptable_raw_net_ops = {
	.init = iptable_raw_net_init,
	.exit = iptable_raw_net_exit,
};

static int __init iptable_raw_init(void)
{
	int ret;

	ret = register_pernet_subsys(&iptable_raw_net_ops);
	if (ret < 0)
		return ret;

	/* Register hooks */
	rawtable_ops = xt_hook_link(&packet_raw, iptable_raw_hook);
	if (IS_ERR(rawtable_ops)) {
		ret = PTR_ERR(rawtable_ops);
<<<<<<< HEAD
		goto cleanup_table;
	}

	return ret;

 cleanup_table:
	unregister_pernet_subsys(&iptable_raw_net_ops);
	return ret;
=======
		unregister_pernet_subsys(&iptable_raw_net_ops);
	}

	return ret;
>>>>>>> refs/remotes/origin/master
}

static void __exit iptable_raw_fini(void)
{
	xt_hook_unlink(&packet_raw, rawtable_ops);
	unregister_pernet_subsys(&iptable_raw_net_ops);
}

module_init(iptable_raw_init);
module_exit(iptable_raw_fini);
MODULE_LICENSE("GPL");
