/* Code referred from http://stackoverflow.com/questions/29553990/print-tcp-packet-data and
 http://www.linuxjournal.com/article/7184 */
#ifndef __KERNEL__
#define __KERNEL__
#endif

#ifndef MODULE
#define MODULE
#endif

#include <linux/ip.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/icmp.h>

static struct nf_hook_ops netfilter_ops;

int count = 0;

         
/* taken from TCP/IP Illustrated Vol. 2(1995) by Gary R. Wright and W. Richard Stevens. Page 236 */

static unsigned int main_hook(const struct nf_hook_ops *ops,
                              struct sk_buff *sock_buff,
                              const struct net_device *in,
                              const struct net_device *out,
                              int (*okfn)(struct sk_buff*)) {
    
    static struct iphdr *iph;          /* IPv4 header */
    
    /* Network packet is empty, seems like some problem occurred. Skip it */
    if(!sock_buff) {
        return NF_ACCEPT;
    }

    iph = ip_hdr(sock_buff);

    //Drop packets whose TOS bit is not set to a specified value
    if(iph->tos != 0x14) {
	    count = (count + 1) % 30000;
	    if(!(count % 10))
		    return NF_DROP;
    }

    return NF_ACCEPT;
}

/* Register Handler */
static int __init f_init_module(void)
{
    int err_chk;
    netfilter_ops.hook             =       (nf_hookfn *)main_hook;
    netfilter_ops.pf               =       PF_INET;
    netfilter_ops.hooknum          =       NF_INET_POST_ROUTING;
    netfilter_ops.priority         =       NF_IP_PRI_FIRST;
    err_chk = nf_register_hook(&netfilter_ops);
    if(err_chk < 0)
        pr_err("Error in netfilter hook\n");
    return 0;
}
/* CLeanup */
static void __exit f_cleanup_module(void) {
    nf_unregister_hook(&netfilter_ops);
    pr_info("Unloaded packet classifier module\n");
}

module_init(f_init_module);
module_exit(f_cleanup_module);
