#ifndef __BNX2FC_DEBUG__
#define __BNX2FC_DEBUG__

/* Log level bit mask */
#define LOG_IO		0x01	/* scsi cmd error, cleanup */
#define LOG_TGT		0x02	/* Session setup, cleanup, etc' */
#define LOG_HBA		0x04	/* lport events, link, mtu, etc' */
#define LOG_ELS		0x08	/* ELS logs */
#define LOG_MISC	0x10	/* fcoe L2 frame related logs*/
#define LOG_ALL		0xff	/* LOG all messages */

extern unsigned int bnx2fc_debug_level;

<<<<<<< HEAD
#define BNX2FC_CHK_LOGGING(LEVEL, CMD)					\
	do {								\
		if (unlikely(bnx2fc_debug_level & LEVEL))		\
			do {						\
				CMD;					\
			} while (0);					\
	} while (0)

#define BNX2FC_ELS_DBG(fmt, arg...)					\
	BNX2FC_CHK_LOGGING(LOG_ELS,					\
<<<<<<< HEAD
			   printk(KERN_ALERT PFX fmt, ##arg))

#define BNX2FC_MISC_DBG(fmt, arg...)					\
	BNX2FC_CHK_LOGGING(LOG_MISC,					\
			   printk(KERN_ALERT PFX fmt, ##arg))
=======
			   printk(KERN_INFO PFX fmt, ##arg))

#define BNX2FC_MISC_DBG(fmt, arg...)					\
	BNX2FC_CHK_LOGGING(LOG_MISC,					\
			   printk(KERN_INFO PFX fmt, ##arg))
>>>>>>> refs/remotes/origin/cm-10.0

#define BNX2FC_IO_DBG(io_req, fmt, arg...)				\
	do {								\
		if (!io_req || !io_req->port || !io_req->port->lport ||	\
		    !io_req->port->lport->host)				\
			BNX2FC_CHK_LOGGING(LOG_IO,			\
<<<<<<< HEAD
			   printk(KERN_ALERT PFX "NULL " fmt, ##arg));	\
		else							\
			BNX2FC_CHK_LOGGING(LOG_IO,			\
			   shost_printk(KERN_ALERT,			\
=======
			   printk(KERN_INFO PFX "NULL " fmt, ##arg));	\
		else							\
			BNX2FC_CHK_LOGGING(LOG_IO,			\
			   shost_printk(KERN_INFO,			\
>>>>>>> refs/remotes/origin/cm-10.0
				   (io_req)->port->lport->host,		\
				   PFX "xid:0x%x " fmt,			\
				   (io_req)->xid, ##arg));		\
	} while (0)

#define BNX2FC_TGT_DBG(tgt, fmt, arg...)				\
	do {								\
		if (!tgt || !tgt->port || !tgt->port->lport ||		\
		    !tgt->port->lport->host || !tgt->rport)		\
			BNX2FC_CHK_LOGGING(LOG_TGT,			\
<<<<<<< HEAD
			   printk(KERN_ALERT PFX "NULL " fmt, ##arg));	\
		else							\
			BNX2FC_CHK_LOGGING(LOG_TGT,			\
			   shost_printk(KERN_ALERT,			\
=======
			   printk(KERN_INFO PFX "NULL " fmt, ##arg));	\
		else							\
			BNX2FC_CHK_LOGGING(LOG_TGT,			\
			   shost_printk(KERN_INFO,			\
>>>>>>> refs/remotes/origin/cm-10.0
				   (tgt)->port->lport->host,		\
				   PFX "port:%x " fmt,			\
				   (tgt)->rport->port_id, ##arg));	\
	} while (0)


#define BNX2FC_HBA_DBG(lport, fmt, arg...)				\
	do {								\
		if (!lport || !lport->host)				\
			BNX2FC_CHK_LOGGING(LOG_HBA,			\
<<<<<<< HEAD
			   printk(KERN_ALERT PFX "NULL " fmt, ##arg));	\
		else							\
			BNX2FC_CHK_LOGGING(LOG_HBA,			\
			   shost_printk(KERN_ALERT, lport->host,	\
=======
			   printk(KERN_INFO PFX "NULL " fmt, ##arg));	\
		else							\
			BNX2FC_CHK_LOGGING(LOG_HBA,			\
			   shost_printk(KERN_INFO, lport->host,	\
>>>>>>> refs/remotes/origin/cm-10.0
				   PFX fmt, ##arg));			\
	} while (0)
=======
#define BNX2FC_ELS_DBG(fmt, ...)				\
do {								\
	if (unlikely(bnx2fc_debug_level & LOG_ELS))		\
		pr_info(fmt, ##__VA_ARGS__);			\
} while (0)

#define BNX2FC_MISC_DBG(fmt, ...)				\
do {								\
	if (unlikely(bnx2fc_debug_level & LOG_MISC))		\
		pr_info(fmt, ##__VA_ARGS__);			\
} while (0)

__printf(2, 3)
void BNX2FC_IO_DBG(const struct bnx2fc_cmd *io_req, const char *fmt, ...);
__printf(2, 3)
void BNX2FC_TGT_DBG(const struct bnx2fc_rport *tgt, const char *fmt, ...);
__printf(2, 3)
void BNX2FC_HBA_DBG(const struct fc_lport *lport, const char *fmt, ...);
>>>>>>> refs/remotes/origin/master

#endif
