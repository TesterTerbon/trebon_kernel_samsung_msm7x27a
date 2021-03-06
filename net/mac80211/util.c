/*
 * Copyright 2002-2005, Instant802 Networks, Inc.
 * Copyright 2005-2006, Devicescape Software, Inc.
 * Copyright 2006-2007	Jiri Benc <jbenc@suse.cz>
 * Copyright 2007	Johannes Berg <johannes@sipsolutions.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * utilities for mac80211
 */

#include <net/mac80211.h>
#include <linux/netdevice.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/skbuff.h>
#include <linux/etherdevice.h>
#include <linux/if_arp.h>
#include <linux/bitmap.h>
#include <linux/crc32.h>
#include <net/net_namespace.h>
#include <net/cfg80211.h>
#include <net/rtnetlink.h>

#include "ieee80211_i.h"
#include "driver-ops.h"
#include "rate.h"
#include "mesh.h"
#include "wme.h"
#include "led.h"
#include "wep.h"

/* privid for wiphys to determine whether they belong to us or not */
void *mac80211_wiphy_privid = &mac80211_wiphy_privid;

struct ieee80211_hw *wiphy_to_ieee80211_hw(struct wiphy *wiphy)
{
	struct ieee80211_local *local;
	BUG_ON(!wiphy);

	local = wiphy_priv(wiphy);
	return &local->hw;
}
EXPORT_SYMBOL(wiphy_to_ieee80211_hw);

u8 *ieee80211_get_bssid(struct ieee80211_hdr *hdr, size_t len,
			enum nl80211_iftype type)
{
	__le16 fc = hdr->frame_control;

	 /* drop ACK/CTS frames and incorrect hdr len (ctrl) */
	if (len < 16)
		return NULL;

	if (ieee80211_is_data(fc)) {
		if (len < 24) /* drop incorrect hdr len (data) */
			return NULL;

		if (ieee80211_has_a4(fc))
			return NULL;
		if (ieee80211_has_tods(fc))
			return hdr->addr1;
		if (ieee80211_has_fromds(fc))
			return hdr->addr2;

		return hdr->addr3;
	}

	if (ieee80211_is_mgmt(fc)) {
		if (len < 24) /* drop incorrect hdr len (mgmt) */
			return NULL;
		return hdr->addr3;
	}

	if (ieee80211_is_ctl(fc)) {
		if(ieee80211_is_pspoll(fc))
			return hdr->addr1;

		if (ieee80211_is_back_req(fc)) {
			switch (type) {
			case NL80211_IFTYPE_STATION:
				return hdr->addr2;
			case NL80211_IFTYPE_AP:
			case NL80211_IFTYPE_AP_VLAN:
				return hdr->addr1;
			default:
				break; /* fall through to the return */
			}
		}
	}

	return NULL;
}

void ieee80211_tx_set_protected(struct ieee80211_tx_data *tx)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct sk_buff *skb = tx->skb;
	struct ieee80211_hdr *hdr;

	do {
		hdr = (struct ieee80211_hdr *) skb->data;
		hdr->frame_control |= cpu_to_le16(IEEE80211_FCTL_PROTECTED);
	} while ((skb = skb->next));
=======
=======
>>>>>>> refs/remotes/origin/master
	struct sk_buff *skb;
	struct ieee80211_hdr *hdr;

	skb_queue_walk(&tx->skbs, skb) {
		hdr = (struct ieee80211_hdr *) skb->data;
		hdr->frame_control |= cpu_to_le16(IEEE80211_FCTL_PROTECTED);
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

int ieee80211_frame_duration(struct ieee80211_local *local, size_t len,
			     int rate, int erp, int short_preamble)
=======
}

int ieee80211_frame_duration(enum ieee80211_band band, size_t len,
			     int rate, int erp, int short_preamble,
			     int shift)
>>>>>>> refs/remotes/origin/master
{
	int dur;

	/* calculate duration (in microseconds, rounded up to next higher
	 * integer if it includes a fractional microsecond) to send frame of
	 * len bytes (does not include FCS) at the given rate. Duration will
	 * also include SIFS.
	 *
	 * rate is in 100 kbps, so divident is multiplied by 10 in the
	 * DIV_ROUND_UP() operations.
<<<<<<< HEAD
	 */

	if (local->hw.conf.channel->band == IEEE80211_BAND_5GHZ || erp) {
=======
	 *
	 * shift may be 2 for 5 MHz channels or 1 for 10 MHz channels, and
	 * is assumed to be 0 otherwise.
	 */

	if (band == IEEE80211_BAND_5GHZ || erp) {
>>>>>>> refs/remotes/origin/master
		/*
		 * OFDM:
		 *
		 * N_DBPS = DATARATE x 4
		 * N_SYM = Ceiling((16+8xLENGTH+6) / N_DBPS)
		 *	(16 = SIGNAL time, 6 = tail bits)
		 * TXTIME = T_PREAMBLE + T_SIGNAL + T_SYM x N_SYM + Signal Ext
		 *
		 * T_SYM = 4 usec
<<<<<<< HEAD
		 * 802.11a - 17.5.2: aSIFSTime = 16 usec
=======
		 * 802.11a - 18.5.2: aSIFSTime = 16 usec
>>>>>>> refs/remotes/origin/master
		 * 802.11g - 19.8.4: aSIFSTime = 10 usec +
		 *	signal ext = 6 usec
		 */
		dur = 16; /* SIFS + signal ext */
<<<<<<< HEAD
		dur += 16; /* 17.3.2.3: T_PREAMBLE = 16 usec */
		dur += 4; /* 17.3.2.3: T_SIGNAL = 4 usec */
=======
		dur += 16; /* IEEE 802.11-2012 18.3.2.4: T_PREAMBLE = 16 usec */
		dur += 4; /* IEEE 802.11-2012 18.3.2.4: T_SIGNAL = 4 usec */

		/* IEEE 802.11-2012 18.3.2.4: all values above are:
		 *  * times 4 for 5 MHz
		 *  * times 2 for 10 MHz
		 */
		dur *= 1 << shift;

		/* rates should already consider the channel bandwidth,
		 * don't apply divisor again.
		 */
>>>>>>> refs/remotes/origin/master
		dur += 4 * DIV_ROUND_UP((16 + 8 * (len + 4) + 6) * 10,
					4 * rate); /* T_SYM x N_SYM */
	} else {
		/*
		 * 802.11b or 802.11g with 802.11b compatibility:
		 * 18.3.4: TXTIME = PreambleLength + PLCPHeaderTime +
		 * Ceiling(((LENGTH+PBCC)x8)/DATARATE). PBCC=0.
		 *
		 * 802.11 (DS): 15.3.3, 802.11b: 18.3.4
		 * aSIFSTime = 10 usec
		 * aPreambleLength = 144 usec or 72 usec with short preamble
		 * aPLCPHeaderLength = 48 usec or 24 usec with short preamble
		 */
		dur = 10; /* aSIFSTime = 10 usec */
		dur += short_preamble ? (72 + 24) : (144 + 48);

		dur += DIV_ROUND_UP(8 * (len + 4) * 10, rate);
	}

	return dur;
}

/* Exported duration function for driver use */
__le16 ieee80211_generic_frame_duration(struct ieee80211_hw *hw,
					struct ieee80211_vif *vif,
<<<<<<< HEAD
					size_t frame_len,
					struct ieee80211_rate *rate)
{
	struct ieee80211_local *local = hw_to_local(hw);
	struct ieee80211_sub_if_data *sdata;
	u16 dur;
	int erp;
=======
					enum ieee80211_band band,
					size_t frame_len,
					struct ieee80211_rate *rate)
{
	struct ieee80211_sub_if_data *sdata;
	u16 dur;
	int erp, shift = 0;
>>>>>>> refs/remotes/origin/master
	bool short_preamble = false;

	erp = 0;
	if (vif) {
		sdata = vif_to_sdata(vif);
		short_preamble = sdata->vif.bss_conf.use_short_preamble;
		if (sdata->flags & IEEE80211_SDATA_OPERATING_GMODE)
			erp = rate->flags & IEEE80211_RATE_ERP_G;
<<<<<<< HEAD
	}

	dur = ieee80211_frame_duration(local, frame_len, rate->bitrate, erp,
				       short_preamble);
=======
		shift = ieee80211_vif_get_shift(vif);
	}

	dur = ieee80211_frame_duration(band, frame_len, rate->bitrate, erp,
				       short_preamble, shift);
>>>>>>> refs/remotes/origin/master

	return cpu_to_le16(dur);
}
EXPORT_SYMBOL(ieee80211_generic_frame_duration);

__le16 ieee80211_rts_duration(struct ieee80211_hw *hw,
			      struct ieee80211_vif *vif, size_t frame_len,
			      const struct ieee80211_tx_info *frame_txctl)
{
	struct ieee80211_local *local = hw_to_local(hw);
	struct ieee80211_rate *rate;
	struct ieee80211_sub_if_data *sdata;
	bool short_preamble;
<<<<<<< HEAD
	int erp;
	u16 dur;
	struct ieee80211_supported_band *sband;

	sband = local->hw.wiphy->bands[local->hw.conf.channel->band];
=======
	int erp, shift = 0, bitrate;
	u16 dur;
	struct ieee80211_supported_band *sband;

	sband = local->hw.wiphy->bands[frame_txctl->band];
>>>>>>> refs/remotes/origin/master

	short_preamble = false;

	rate = &sband->bitrates[frame_txctl->control.rts_cts_rate_idx];

	erp = 0;
	if (vif) {
		sdata = vif_to_sdata(vif);
		short_preamble = sdata->vif.bss_conf.use_short_preamble;
		if (sdata->flags & IEEE80211_SDATA_OPERATING_GMODE)
			erp = rate->flags & IEEE80211_RATE_ERP_G;
<<<<<<< HEAD
	}

	/* CTS duration */
	dur = ieee80211_frame_duration(local, 10, rate->bitrate,
				       erp, short_preamble);
	/* Data frame duration */
	dur += ieee80211_frame_duration(local, frame_len, rate->bitrate,
					erp, short_preamble);
	/* ACK duration */
	dur += ieee80211_frame_duration(local, 10, rate->bitrate,
					erp, short_preamble);
=======
		shift = ieee80211_vif_get_shift(vif);
	}

	bitrate = DIV_ROUND_UP(rate->bitrate, 1 << shift);

	/* CTS duration */
	dur = ieee80211_frame_duration(sband->band, 10, bitrate,
				       erp, short_preamble, shift);
	/* Data frame duration */
	dur += ieee80211_frame_duration(sband->band, frame_len, bitrate,
					erp, short_preamble, shift);
	/* ACK duration */
	dur += ieee80211_frame_duration(sband->band, 10, bitrate,
					erp, short_preamble, shift);
>>>>>>> refs/remotes/origin/master

	return cpu_to_le16(dur);
}
EXPORT_SYMBOL(ieee80211_rts_duration);

__le16 ieee80211_ctstoself_duration(struct ieee80211_hw *hw,
				    struct ieee80211_vif *vif,
				    size_t frame_len,
				    const struct ieee80211_tx_info *frame_txctl)
{
	struct ieee80211_local *local = hw_to_local(hw);
	struct ieee80211_rate *rate;
	struct ieee80211_sub_if_data *sdata;
	bool short_preamble;
<<<<<<< HEAD
	int erp;
	u16 dur;
	struct ieee80211_supported_band *sband;

	sband = local->hw.wiphy->bands[local->hw.conf.channel->band];
=======
	int erp, shift = 0, bitrate;
	u16 dur;
	struct ieee80211_supported_band *sband;

	sband = local->hw.wiphy->bands[frame_txctl->band];
>>>>>>> refs/remotes/origin/master

	short_preamble = false;

	rate = &sband->bitrates[frame_txctl->control.rts_cts_rate_idx];
	erp = 0;
	if (vif) {
		sdata = vif_to_sdata(vif);
		short_preamble = sdata->vif.bss_conf.use_short_preamble;
		if (sdata->flags & IEEE80211_SDATA_OPERATING_GMODE)
			erp = rate->flags & IEEE80211_RATE_ERP_G;
<<<<<<< HEAD
	}

	/* Data frame duration */
	dur = ieee80211_frame_duration(local, frame_len, rate->bitrate,
				       erp, short_preamble);
	if (!(frame_txctl->flags & IEEE80211_TX_CTL_NO_ACK)) {
		/* ACK duration */
		dur += ieee80211_frame_duration(local, 10, rate->bitrate,
						erp, short_preamble);
=======
		shift = ieee80211_vif_get_shift(vif);
	}

	bitrate = DIV_ROUND_UP(rate->bitrate, 1 << shift);

	/* Data frame duration */
	dur = ieee80211_frame_duration(sband->band, frame_len, bitrate,
				       erp, short_preamble, shift);
	if (!(frame_txctl->flags & IEEE80211_TX_CTL_NO_ACK)) {
		/* ACK duration */
		dur += ieee80211_frame_duration(sband->band, 10, bitrate,
						erp, short_preamble, shift);
>>>>>>> refs/remotes/origin/master
	}

	return cpu_to_le16(dur);
}
EXPORT_SYMBOL(ieee80211_ctstoself_duration);

<<<<<<< HEAD
=======
void ieee80211_propagate_queue_wake(struct ieee80211_local *local, int queue)
{
	struct ieee80211_sub_if_data *sdata;
	int n_acs = IEEE80211_NUM_ACS;

	if (local->hw.queues < IEEE80211_NUM_ACS)
		n_acs = 1;

	list_for_each_entry_rcu(sdata, &local->interfaces, list) {
		int ac;

		if (!sdata->dev)
			continue;

		if (sdata->vif.cab_queue != IEEE80211_INVAL_HW_QUEUE &&
		    local->queue_stop_reasons[sdata->vif.cab_queue] != 0)
			continue;

		for (ac = 0; ac < n_acs; ac++) {
			int ac_queue = sdata->vif.hw_queue[ac];

			if (ac_queue == queue ||
			    (sdata->vif.cab_queue == queue &&
			     local->queue_stop_reasons[ac_queue] == 0 &&
			     skb_queue_empty(&local->pending[ac_queue])))
				netif_wake_subqueue(sdata->dev, ac);
		}
	}
}

>>>>>>> refs/remotes/origin/master
static void __ieee80211_wake_queue(struct ieee80211_hw *hw, int queue,
				   enum queue_stop_reason reason)
{
	struct ieee80211_local *local = hw_to_local(hw);
<<<<<<< HEAD
	struct ieee80211_sub_if_data *sdata;
=======
>>>>>>> refs/remotes/origin/master

	trace_wake_queue(local, queue, reason);

	if (WARN_ON(queue >= hw->queues))
		return;

<<<<<<< HEAD
=======
	if (!test_bit(reason, &local->queue_stop_reasons[queue]))
		return;

>>>>>>> refs/remotes/origin/master
	__clear_bit(reason, &local->queue_stop_reasons[queue]);

	if (local->queue_stop_reasons[queue] != 0)
		/* someone still has this queue stopped */
		return;

	if (skb_queue_empty(&local->pending[queue])) {
		rcu_read_lock();
<<<<<<< HEAD
		list_for_each_entry_rcu(sdata, &local->interfaces, list) {
			if (test_bit(SDATA_STATE_OFFCHANNEL, &sdata->state))
				continue;
			netif_wake_subqueue(sdata->dev, queue);
		}
=======
		ieee80211_propagate_queue_wake(local, queue);
>>>>>>> refs/remotes/origin/master
		rcu_read_unlock();
	} else
		tasklet_schedule(&local->tx_pending_tasklet);
}

void ieee80211_wake_queue_by_reason(struct ieee80211_hw *hw, int queue,
				    enum queue_stop_reason reason)
{
	struct ieee80211_local *local = hw_to_local(hw);
	unsigned long flags;

	spin_lock_irqsave(&local->queue_stop_reason_lock, flags);
	__ieee80211_wake_queue(hw, queue, reason);
	spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);
}

void ieee80211_wake_queue(struct ieee80211_hw *hw, int queue)
{
	ieee80211_wake_queue_by_reason(hw, queue,
				       IEEE80211_QUEUE_STOP_REASON_DRIVER);
}
EXPORT_SYMBOL(ieee80211_wake_queue);

static void __ieee80211_stop_queue(struct ieee80211_hw *hw, int queue,
				   enum queue_stop_reason reason)
{
	struct ieee80211_local *local = hw_to_local(hw);
	struct ieee80211_sub_if_data *sdata;
<<<<<<< HEAD
=======
	int n_acs = IEEE80211_NUM_ACS;
>>>>>>> refs/remotes/origin/master

	trace_stop_queue(local, queue, reason);

	if (WARN_ON(queue >= hw->queues))
		return;

<<<<<<< HEAD
	__set_bit(reason, &local->queue_stop_reasons[queue]);

	rcu_read_lock();
	list_for_each_entry_rcu(sdata, &local->interfaces, list)
		netif_stop_subqueue(sdata->dev, queue);
=======
	if (test_bit(reason, &local->queue_stop_reasons[queue]))
		return;

	__set_bit(reason, &local->queue_stop_reasons[queue]);

	if (local->hw.queues < IEEE80211_NUM_ACS)
		n_acs = 1;

	rcu_read_lock();
	list_for_each_entry_rcu(sdata, &local->interfaces, list) {
		int ac;

		if (!sdata->dev)
			continue;

		for (ac = 0; ac < n_acs; ac++) {
			if (sdata->vif.hw_queue[ac] == queue ||
			    sdata->vif.cab_queue == queue)
				netif_stop_subqueue(sdata->dev, ac);
		}
	}
>>>>>>> refs/remotes/origin/master
	rcu_read_unlock();
}

void ieee80211_stop_queue_by_reason(struct ieee80211_hw *hw, int queue,
				    enum queue_stop_reason reason)
{
	struct ieee80211_local *local = hw_to_local(hw);
	unsigned long flags;

	spin_lock_irqsave(&local->queue_stop_reason_lock, flags);
	__ieee80211_stop_queue(hw, queue, reason);
	spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);
}

void ieee80211_stop_queue(struct ieee80211_hw *hw, int queue)
{
	ieee80211_stop_queue_by_reason(hw, queue,
				       IEEE80211_QUEUE_STOP_REASON_DRIVER);
}
EXPORT_SYMBOL(ieee80211_stop_queue);

void ieee80211_add_pending_skb(struct ieee80211_local *local,
			       struct sk_buff *skb)
{
	struct ieee80211_hw *hw = &local->hw;
	unsigned long flags;
<<<<<<< HEAD
	int queue = skb_get_queue_mapping(skb);
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);

	if (WARN_ON(!info->control.vif)) {
		kfree_skb(skb);
=======
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	int queue = info->hw_queue;

	if (WARN_ON(!info->control.vif)) {
		ieee80211_free_txskb(&local->hw, skb);
>>>>>>> refs/remotes/origin/master
		return;
	}

	spin_lock_irqsave(&local->queue_stop_reason_lock, flags);
	__ieee80211_stop_queue(hw, queue, IEEE80211_QUEUE_STOP_REASON_SKB_ADD);
	__skb_queue_tail(&local->pending[queue], skb);
	__ieee80211_wake_queue(hw, queue, IEEE80211_QUEUE_STOP_REASON_SKB_ADD);
	spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);
}

<<<<<<< HEAD
<<<<<<< HEAD
int ieee80211_add_pending_skbs_fn(struct ieee80211_local *local,
				  struct sk_buff_head *skbs,
				  void (*fn)(void *data), void *data)
=======
void ieee80211_add_pending_skbs_fn(struct ieee80211_local *local,
				   struct sk_buff_head *skbs,
				   void (*fn)(void *data), void *data)
>>>>>>> refs/remotes/origin/cm-10.0
=======
void ieee80211_add_pending_skbs_fn(struct ieee80211_local *local,
				   struct sk_buff_head *skbs,
				   void (*fn)(void *data), void *data)
>>>>>>> refs/remotes/origin/master
{
	struct ieee80211_hw *hw = &local->hw;
	struct sk_buff *skb;
	unsigned long flags;
<<<<<<< HEAD
<<<<<<< HEAD
	int queue, ret = 0, i;
=======
	int queue, i;
>>>>>>> refs/remotes/origin/cm-10.0

	spin_lock_irqsave(&local->queue_stop_reason_lock, flags);
	for (i = 0; i < hw->queues; i++)
		__ieee80211_stop_queue(hw, i,
			IEEE80211_QUEUE_STOP_REASON_SKB_ADD);

=======
	int queue, i;

	spin_lock_irqsave(&local->queue_stop_reason_lock, flags);
>>>>>>> refs/remotes/origin/master
	while ((skb = skb_dequeue(skbs))) {
		struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);

		if (WARN_ON(!info->control.vif)) {
<<<<<<< HEAD
			kfree_skb(skb);
			continue;
		}

<<<<<<< HEAD
		ret++;
=======
>>>>>>> refs/remotes/origin/cm-10.0
		queue = skb_get_queue_mapping(skb);
=======
			ieee80211_free_txskb(&local->hw, skb);
			continue;
		}

		queue = info->hw_queue;

		__ieee80211_stop_queue(hw, queue,
				IEEE80211_QUEUE_STOP_REASON_SKB_ADD);

>>>>>>> refs/remotes/origin/master
		__skb_queue_tail(&local->pending[queue], skb);
	}

	if (fn)
		fn(data);

	for (i = 0; i < hw->queues; i++)
		__ieee80211_wake_queue(hw, i,
			IEEE80211_QUEUE_STOP_REASON_SKB_ADD);
	spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);
<<<<<<< HEAD
<<<<<<< HEAD

	return ret;
}

int ieee80211_add_pending_skbs(struct ieee80211_local *local,
			       struct sk_buff_head *skbs)
{
	return ieee80211_add_pending_skbs_fn(local, skbs, NULL, NULL);
=======
}

void ieee80211_add_pending_skbs(struct ieee80211_local *local,
				struct sk_buff_head *skbs)
{
	ieee80211_add_pending_skbs_fn(local, skbs, NULL, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
}

void ieee80211_stop_queues_by_reason(struct ieee80211_hw *hw,
				    enum queue_stop_reason reason)
=======
}

void ieee80211_stop_queues_by_reason(struct ieee80211_hw *hw,
				     unsigned long queues,
				     enum queue_stop_reason reason)
>>>>>>> refs/remotes/origin/master
{
	struct ieee80211_local *local = hw_to_local(hw);
	unsigned long flags;
	int i;

	spin_lock_irqsave(&local->queue_stop_reason_lock, flags);

<<<<<<< HEAD
	for (i = 0; i < hw->queues; i++)
=======
	for_each_set_bit(i, &queues, hw->queues)
>>>>>>> refs/remotes/origin/master
		__ieee80211_stop_queue(hw, i, reason);

	spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);
}

void ieee80211_stop_queues(struct ieee80211_hw *hw)
{
<<<<<<< HEAD
	ieee80211_stop_queues_by_reason(hw,
=======
	ieee80211_stop_queues_by_reason(hw, IEEE80211_MAX_QUEUE_MAP,
>>>>>>> refs/remotes/origin/master
					IEEE80211_QUEUE_STOP_REASON_DRIVER);
}
EXPORT_SYMBOL(ieee80211_stop_queues);

int ieee80211_queue_stopped(struct ieee80211_hw *hw, int queue)
{
	struct ieee80211_local *local = hw_to_local(hw);
	unsigned long flags;
	int ret;

	if (WARN_ON(queue >= hw->queues))
		return true;

	spin_lock_irqsave(&local->queue_stop_reason_lock, flags);
<<<<<<< HEAD
	ret = !!local->queue_stop_reasons[queue];
=======
	ret = test_bit(IEEE80211_QUEUE_STOP_REASON_DRIVER,
		       &local->queue_stop_reasons[queue]);
>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);
	return ret;
}
EXPORT_SYMBOL(ieee80211_queue_stopped);

void ieee80211_wake_queues_by_reason(struct ieee80211_hw *hw,
<<<<<<< HEAD
=======
				     unsigned long queues,
>>>>>>> refs/remotes/origin/master
				     enum queue_stop_reason reason)
{
	struct ieee80211_local *local = hw_to_local(hw);
	unsigned long flags;
	int i;

	spin_lock_irqsave(&local->queue_stop_reason_lock, flags);

<<<<<<< HEAD
	for (i = 0; i < hw->queues; i++)
=======
	for_each_set_bit(i, &queues, hw->queues)
>>>>>>> refs/remotes/origin/master
		__ieee80211_wake_queue(hw, i, reason);

	spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);
}

void ieee80211_wake_queues(struct ieee80211_hw *hw)
{
<<<<<<< HEAD
	ieee80211_wake_queues_by_reason(hw, IEEE80211_QUEUE_STOP_REASON_DRIVER);
}
EXPORT_SYMBOL(ieee80211_wake_queues);

void ieee80211_iterate_active_interfaces(
	struct ieee80211_hw *hw,
	void (*iterator)(void *data, u8 *mac,
			 struct ieee80211_vif *vif),
	void *data)
{
	struct ieee80211_local *local = hw_to_local(hw);
	struct ieee80211_sub_if_data *sdata;

	mutex_lock(&local->iflist_mtx);

	list_for_each_entry(sdata, &local->interfaces, list) {
		switch (sdata->vif.type) {
		case NL80211_IFTYPE_MONITOR:
=======
	ieee80211_wake_queues_by_reason(hw, IEEE80211_MAX_QUEUE_MAP,
					IEEE80211_QUEUE_STOP_REASON_DRIVER);
}
EXPORT_SYMBOL(ieee80211_wake_queues);

void ieee80211_flush_queues(struct ieee80211_local *local,
			    struct ieee80211_sub_if_data *sdata)
{
	u32 queues;

	if (!local->ops->flush)
		return;

	if (sdata && local->hw.flags & IEEE80211_HW_QUEUE_CONTROL) {
		int ac;

		queues = 0;

		for (ac = 0; ac < IEEE80211_NUM_ACS; ac++)
			queues |= BIT(sdata->vif.hw_queue[ac]);
		if (sdata->vif.cab_queue != IEEE80211_INVAL_HW_QUEUE)
			queues |= BIT(sdata->vif.cab_queue);
	} else {
		/* all queues */
		queues = BIT(local->hw.queues) - 1;
	}

	ieee80211_stop_queues_by_reason(&local->hw, IEEE80211_MAX_QUEUE_MAP,
					IEEE80211_QUEUE_STOP_REASON_FLUSH);

	drv_flush(local, queues, false);

	ieee80211_wake_queues_by_reason(&local->hw, IEEE80211_MAX_QUEUE_MAP,
					IEEE80211_QUEUE_STOP_REASON_FLUSH);
}

static void __iterate_active_interfaces(struct ieee80211_local *local,
					u32 iter_flags,
					void (*iterator)(void *data, u8 *mac,
						struct ieee80211_vif *vif),
					void *data)
{
	struct ieee80211_sub_if_data *sdata;

	list_for_each_entry_rcu(sdata, &local->interfaces, list) {
		switch (sdata->vif.type) {
		case NL80211_IFTYPE_MONITOR:
			if (!(sdata->u.mntr_flags & MONITOR_FLAG_ACTIVE))
				continue;
			break;
>>>>>>> refs/remotes/origin/master
		case NL80211_IFTYPE_AP_VLAN:
			continue;
		default:
			break;
		}
<<<<<<< HEAD
=======
		if (!(iter_flags & IEEE80211_IFACE_ITER_RESUME_ALL) &&
		    !(sdata->flags & IEEE80211_SDATA_IN_DRIVER))
			continue;
>>>>>>> refs/remotes/origin/master
		if (ieee80211_sdata_running(sdata))
			iterator(data, sdata->vif.addr,
				 &sdata->vif);
	}

<<<<<<< HEAD
=======
	sdata = rcu_dereference_check(local->monitor_sdata,
				      lockdep_is_held(&local->iflist_mtx) ||
				      lockdep_rtnl_is_held());
	if (sdata &&
	    (iter_flags & IEEE80211_IFACE_ITER_RESUME_ALL ||
	     sdata->flags & IEEE80211_SDATA_IN_DRIVER))
		iterator(data, sdata->vif.addr, &sdata->vif);
}

void ieee80211_iterate_active_interfaces(
	struct ieee80211_hw *hw, u32 iter_flags,
	void (*iterator)(void *data, u8 *mac,
			 struct ieee80211_vif *vif),
	void *data)
{
	struct ieee80211_local *local = hw_to_local(hw);

	mutex_lock(&local->iflist_mtx);
	__iterate_active_interfaces(local, iter_flags, iterator, data);
>>>>>>> refs/remotes/origin/master
	mutex_unlock(&local->iflist_mtx);
}
EXPORT_SYMBOL_GPL(ieee80211_iterate_active_interfaces);

void ieee80211_iterate_active_interfaces_atomic(
<<<<<<< HEAD
	struct ieee80211_hw *hw,
=======
	struct ieee80211_hw *hw, u32 iter_flags,
>>>>>>> refs/remotes/origin/master
	void (*iterator)(void *data, u8 *mac,
			 struct ieee80211_vif *vif),
	void *data)
{
	struct ieee80211_local *local = hw_to_local(hw);
<<<<<<< HEAD
	struct ieee80211_sub_if_data *sdata;

	rcu_read_lock();

	list_for_each_entry_rcu(sdata, &local->interfaces, list) {
		switch (sdata->vif.type) {
		case NL80211_IFTYPE_MONITOR:
		case NL80211_IFTYPE_AP_VLAN:
			continue;
		default:
			break;
		}
		if (ieee80211_sdata_running(sdata))
			iterator(data, sdata->vif.addr,
				 &sdata->vif);
	}

=======

	rcu_read_lock();
	__iterate_active_interfaces(local, iter_flags, iterator, data);
>>>>>>> refs/remotes/origin/master
	rcu_read_unlock();
}
EXPORT_SYMBOL_GPL(ieee80211_iterate_active_interfaces_atomic);

<<<<<<< HEAD
=======
void ieee80211_iterate_active_interfaces_rtnl(
	struct ieee80211_hw *hw, u32 iter_flags,
	void (*iterator)(void *data, u8 *mac,
			 struct ieee80211_vif *vif),
	void *data)
{
	struct ieee80211_local *local = hw_to_local(hw);

	ASSERT_RTNL();

	__iterate_active_interfaces(local, iter_flags, iterator, data);
}
EXPORT_SYMBOL_GPL(ieee80211_iterate_active_interfaces_rtnl);

>>>>>>> refs/remotes/origin/master
/*
 * Nothing should have been stuffed into the workqueue during
 * the suspend->resume cycle. If this WARN is seen then there
 * is a bug with either the driver suspend or something in
 * mac80211 stuffing into the workqueue which we haven't yet
 * cleared during mac80211's suspend cycle.
 */
static bool ieee80211_can_queue_work(struct ieee80211_local *local)
{
	if (WARN(local->suspended && !local->resuming,
		 "queueing ieee80211 work while going to suspend\n"))
		return false;

	return true;
}

void ieee80211_queue_work(struct ieee80211_hw *hw, struct work_struct *work)
{
	struct ieee80211_local *local = hw_to_local(hw);

	if (!ieee80211_can_queue_work(local))
		return;

	queue_work(local->workqueue, work);
}
EXPORT_SYMBOL(ieee80211_queue_work);

void ieee80211_queue_delayed_work(struct ieee80211_hw *hw,
				  struct delayed_work *dwork,
				  unsigned long delay)
{
	struct ieee80211_local *local = hw_to_local(hw);

	if (!ieee80211_can_queue_work(local))
		return;

	queue_delayed_work(local->workqueue, dwork, delay);
}
EXPORT_SYMBOL(ieee80211_queue_delayed_work);

<<<<<<< HEAD
<<<<<<< HEAD
void ieee802_11_parse_elems(u8 *start, size_t len,
			    struct ieee802_11_elems *elems)
{
	ieee802_11_parse_elems_crc(start, len, elems, 0, 0);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
u32 ieee802_11_parse_elems_crc(u8 *start, size_t len,
=======
u32 ieee802_11_parse_elems_crc(const u8 *start, size_t len, bool action,
>>>>>>> refs/remotes/origin/master
			       struct ieee802_11_elems *elems,
			       u64 filter, u32 crc)
{
	size_t left = len;
<<<<<<< HEAD
	u8 *pos = start;
	bool calc_crc = filter != 0;
<<<<<<< HEAD

=======
	DECLARE_BITMAP(seen_elems, 256);

	bitmap_zero(seen_elems, 256);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	const u8 *pos = start;
	bool calc_crc = filter != 0;
	DECLARE_BITMAP(seen_elems, 256);
	const u8 *ie;

	bitmap_zero(seen_elems, 256);
>>>>>>> refs/remotes/origin/master
	memset(elems, 0, sizeof(*elems));
	elems->ie_start = start;
	elems->total_len = len;

	while (left >= 2) {
		u8 id, elen;
<<<<<<< HEAD
<<<<<<< HEAD
=======
		bool elem_parse_failed;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		bool elem_parse_failed;
>>>>>>> refs/remotes/origin/master

		id = *pos++;
		elen = *pos++;
		left -= 2;

<<<<<<< HEAD
<<<<<<< HEAD
		if (elen > left)
			break;
=======
=======
>>>>>>> refs/remotes/origin/master
		if (elen > left) {
			elems->parse_error = true;
			break;
		}

		switch (id) {
		case WLAN_EID_SSID:
		case WLAN_EID_SUPP_RATES:
		case WLAN_EID_FH_PARAMS:
		case WLAN_EID_DS_PARAMS:
		case WLAN_EID_CF_PARAMS:
		case WLAN_EID_TIM:
		case WLAN_EID_IBSS_PARAMS:
		case WLAN_EID_CHALLENGE:
		case WLAN_EID_RSN:
		case WLAN_EID_ERP_INFO:
		case WLAN_EID_EXT_SUPP_RATES:
		case WLAN_EID_HT_CAPABILITY:
<<<<<<< HEAD
<<<<<<< HEAD
=======
		case WLAN_EID_HT_OPERATION:
		case WLAN_EID_VHT_CAPABILITY:
		case WLAN_EID_VHT_OPERATION:
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
		case WLAN_EID_MESH_ID:
		case WLAN_EID_MESH_CONFIG:
		case WLAN_EID_PEER_MGMT:
		case WLAN_EID_PREQ:
		case WLAN_EID_PREP:
		case WLAN_EID_PERR:
		case WLAN_EID_RANN:
		case WLAN_EID_CHANNEL_SWITCH:
		case WLAN_EID_EXT_CHANSWITCH_ANN:
		case WLAN_EID_COUNTRY:
		case WLAN_EID_PWR_CONSTRAINT:
		case WLAN_EID_TIMEOUT_INTERVAL:
<<<<<<< HEAD
<<<<<<< HEAD
=======
		case WLAN_EID_SECONDARY_CHANNEL_OFFSET:
		case WLAN_EID_WIDE_BW_CHANNEL_SWITCH:
		case WLAN_EID_CHAN_SWITCH_PARAM:
		/*
		 * not listing WLAN_EID_CHANNEL_SWITCH_WRAPPER -- it seems possible
		 * that if the content gets bigger it might be needed more than once
		 */
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
			if (test_bit(id, seen_elems)) {
				elems->parse_error = true;
				left -= elen;
				pos += elen;
				continue;
			}
			break;
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

		if (calc_crc && id < 64 && (filter & (1ULL << id)))
			crc = crc32_be(crc, pos - 2, elen + 2);

<<<<<<< HEAD
<<<<<<< HEAD
=======
		elem_parse_failed = false;

>>>>>>> refs/remotes/origin/cm-10.0
=======
		elem_parse_failed = false;

>>>>>>> refs/remotes/origin/master
		switch (id) {
		case WLAN_EID_SSID:
			elems->ssid = pos;
			elems->ssid_len = elen;
			break;
		case WLAN_EID_SUPP_RATES:
			elems->supp_rates = pos;
			elems->supp_rates_len = elen;
			break;
<<<<<<< HEAD
		case WLAN_EID_FH_PARAMS:
			elems->fh_params = pos;
			elems->fh_params_len = elen;
			break;
		case WLAN_EID_DS_PARAMS:
			elems->ds_params = pos;
			elems->ds_params_len = elen;
			break;
		case WLAN_EID_CF_PARAMS:
			elems->cf_params = pos;
			elems->cf_params_len = elen;
=======
		case WLAN_EID_DS_PARAMS:
			if (elen >= 1)
				elems->ds_params = pos;
			else
				elem_parse_failed = true;
>>>>>>> refs/remotes/origin/master
			break;
		case WLAN_EID_TIM:
			if (elen >= sizeof(struct ieee80211_tim_ie)) {
				elems->tim = (void *)pos;
				elems->tim_len = elen;
<<<<<<< HEAD
<<<<<<< HEAD
			}
=======
			} else
				elem_parse_failed = true;
>>>>>>> refs/remotes/origin/cm-10.0
			break;
		case WLAN_EID_IBSS_PARAMS:
			elems->ibss_params = pos;
			elems->ibss_params_len = elen;
=======
			} else
				elem_parse_failed = true;
>>>>>>> refs/remotes/origin/master
			break;
		case WLAN_EID_CHALLENGE:
			elems->challenge = pos;
			elems->challenge_len = elen;
			break;
		case WLAN_EID_VENDOR_SPECIFIC:
			if (elen >= 4 && pos[0] == 0x00 && pos[1] == 0x50 &&
			    pos[2] == 0xf2) {
				/* Microsoft OUI (00:50:F2) */

				if (calc_crc)
					crc = crc32_be(crc, pos - 2, elen + 2);

<<<<<<< HEAD
				if (pos[3] == 1) {
					/* OUI Type 1 - WPA IE */
					elems->wpa = pos;
					elems->wpa_len = elen;
				} else if (elen >= 5 && pos[3] == 2) {
=======
				if (elen >= 5 && pos[3] == 2) {
>>>>>>> refs/remotes/origin/master
					/* OUI Type 2 - WMM IE */
					if (pos[4] == 0) {
						elems->wmm_info = pos;
						elems->wmm_info_len = elen;
					} else if (pos[4] == 1) {
						elems->wmm_param = pos;
						elems->wmm_param_len = elen;
					}
				}
			}
			break;
		case WLAN_EID_RSN:
			elems->rsn = pos;
			elems->rsn_len = elen;
			break;
		case WLAN_EID_ERP_INFO:
<<<<<<< HEAD
			elems->erp_info = pos;
			elems->erp_info_len = elen;
=======
			if (elen >= 1)
				elems->erp_info = pos;
			else
				elem_parse_failed = true;
>>>>>>> refs/remotes/origin/master
			break;
		case WLAN_EID_EXT_SUPP_RATES:
			elems->ext_supp_rates = pos;
			elems->ext_supp_rates_len = elen;
			break;
		case WLAN_EID_HT_CAPABILITY:
			if (elen >= sizeof(struct ieee80211_ht_cap))
				elems->ht_cap_elem = (void *)pos;
<<<<<<< HEAD
<<<<<<< HEAD
=======
			else
				elem_parse_failed = true;
>>>>>>> refs/remotes/origin/cm-10.0
			break;
		case WLAN_EID_HT_INFORMATION:
			if (elen >= sizeof(struct ieee80211_ht_info))
				elems->ht_info_elem = (void *)pos;
<<<<<<< HEAD
=======
			else
				elem_parse_failed = true;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			else
				elem_parse_failed = true;
			break;
		case WLAN_EID_HT_OPERATION:
			if (elen >= sizeof(struct ieee80211_ht_operation))
				elems->ht_operation = (void *)pos;
			else
				elem_parse_failed = true;
			break;
		case WLAN_EID_VHT_CAPABILITY:
			if (elen >= sizeof(struct ieee80211_vht_cap))
				elems->vht_cap_elem = (void *)pos;
			else
				elem_parse_failed = true;
			break;
		case WLAN_EID_VHT_OPERATION:
			if (elen >= sizeof(struct ieee80211_vht_operation))
				elems->vht_operation = (void *)pos;
			else
				elem_parse_failed = true;
			break;
		case WLAN_EID_OPMODE_NOTIF:
			if (elen > 0)
				elems->opmode_notif = pos;
			else
				elem_parse_failed = true;
>>>>>>> refs/remotes/origin/master
			break;
		case WLAN_EID_MESH_ID:
			elems->mesh_id = pos;
			elems->mesh_id_len = elen;
			break;
		case WLAN_EID_MESH_CONFIG:
			if (elen >= sizeof(struct ieee80211_meshconf_ie))
				elems->mesh_config = (void *)pos;
<<<<<<< HEAD
<<<<<<< HEAD
			break;
		case WLAN_EID_PEER_LINK:
			elems->peer_link = pos;
			elems->peer_link_len = elen;
=======
=======
>>>>>>> refs/remotes/origin/master
			else
				elem_parse_failed = true;
			break;
		case WLAN_EID_PEER_MGMT:
			elems->peering = pos;
			elems->peering_len = elen;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
			break;
		case WLAN_EID_MESH_AWAKE_WINDOW:
			if (elen >= 2)
				elems->awake_window = (void *)pos;
>>>>>>> refs/remotes/origin/master
			break;
		case WLAN_EID_PREQ:
			elems->preq = pos;
			elems->preq_len = elen;
			break;
		case WLAN_EID_PREP:
			elems->prep = pos;
			elems->prep_len = elen;
			break;
		case WLAN_EID_PERR:
			elems->perr = pos;
			elems->perr_len = elen;
			break;
		case WLAN_EID_RANN:
			if (elen >= sizeof(struct ieee80211_rann_ie))
				elems->rann = (void *)pos;
<<<<<<< HEAD
<<<<<<< HEAD
=======
			else
				elem_parse_failed = true;
>>>>>>> refs/remotes/origin/cm-10.0
			break;
		case WLAN_EID_CHANNEL_SWITCH:
			elems->ch_switch_elem = pos;
			elems->ch_switch_elem_len = elen;
			break;
		case WLAN_EID_QUIET:
			if (!elems->quiet_elem) {
				elems->quiet_elem = pos;
				elems->quiet_elem_len = elen;
			}
			elems->num_of_quiet_elem++;
=======
			else
				elem_parse_failed = true;
			break;
		case WLAN_EID_CHANNEL_SWITCH:
			if (elen != sizeof(struct ieee80211_channel_sw_ie)) {
				elem_parse_failed = true;
				break;
			}
			elems->ch_switch_ie = (void *)pos;
			break;
		case WLAN_EID_EXT_CHANSWITCH_ANN:
			if (elen != sizeof(struct ieee80211_ext_chansw_ie)) {
				elem_parse_failed = true;
				break;
			}
			elems->ext_chansw_ie = (void *)pos;
			break;
		case WLAN_EID_SECONDARY_CHANNEL_OFFSET:
			if (elen != sizeof(struct ieee80211_sec_chan_offs_ie)) {
				elem_parse_failed = true;
				break;
			}
			elems->sec_chan_offs = (void *)pos;
			break;
		case WLAN_EID_CHAN_SWITCH_PARAM:
			if (elen !=
			    sizeof(*elems->mesh_chansw_params_ie)) {
				elem_parse_failed = true;
				break;
			}
			elems->mesh_chansw_params_ie = (void *)pos;
			break;
		case WLAN_EID_WIDE_BW_CHANNEL_SWITCH:
			if (!action ||
			    elen != sizeof(*elems->wide_bw_chansw_ie)) {
				elem_parse_failed = true;
				break;
			}
			elems->wide_bw_chansw_ie = (void *)pos;
			break;
		case WLAN_EID_CHANNEL_SWITCH_WRAPPER:
			if (action) {
				elem_parse_failed = true;
				break;
			}
			/*
			 * This is a bit tricky, but as we only care about
			 * the wide bandwidth channel switch element, so
			 * just parse it out manually.
			 */
			ie = cfg80211_find_ie(WLAN_EID_WIDE_BW_CHANNEL_SWITCH,
					      pos, elen);
			if (ie) {
				if (ie[1] == sizeof(*elems->wide_bw_chansw_ie))
					elems->wide_bw_chansw_ie =
						(void *)(ie + 2);
				else
					elem_parse_failed = true;
			}
>>>>>>> refs/remotes/origin/master
			break;
		case WLAN_EID_COUNTRY:
			elems->country_elem = pos;
			elems->country_elem_len = elen;
			break;
		case WLAN_EID_PWR_CONSTRAINT:
<<<<<<< HEAD
			elems->pwr_constr_elem = pos;
			elems->pwr_constr_elem_len = elen;
			break;
		case WLAN_EID_TIMEOUT_INTERVAL:
			elems->timeout_int = pos;
			elems->timeout_int_len = elen;
=======
			if (elen != 1) {
				elem_parse_failed = true;
				break;
			}
			elems->pwr_constr_elem = pos;
			break;
		case WLAN_EID_TIMEOUT_INTERVAL:
			if (elen >= sizeof(struct ieee80211_timeout_interval_ie))
				elems->timeout_int = (void *)pos;
			else
				elem_parse_failed = true;
>>>>>>> refs/remotes/origin/master
			break;
		default:
			break;
		}

<<<<<<< HEAD
<<<<<<< HEAD
=======
		if (elem_parse_failed)
			elems->parse_error = true;
		else
			set_bit(id, seen_elems);

>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (elem_parse_failed)
			elems->parse_error = true;
		else
			__set_bit(id, seen_elems);

>>>>>>> refs/remotes/origin/master
		left -= elen;
		pos += elen;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	return crc;
}

void ieee80211_set_wmm_default(struct ieee80211_sub_if_data *sdata)
=======
=======
>>>>>>> refs/remotes/origin/master
	if (left != 0)
		elems->parse_error = true;

	return crc;
}

<<<<<<< HEAD
void ieee802_11_parse_elems(u8 *start, size_t len,
			    struct ieee802_11_elems *elems)
{
	ieee802_11_parse_elems_crc(start, len, elems, 0, 0);
}

void ieee80211_set_wmm_default(struct ieee80211_sub_if_data *sdata,
			       bool bss_notify)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_tx_queue_params qparam;
	int queue;
	bool use_11b;
=======
void ieee80211_set_wmm_default(struct ieee80211_sub_if_data *sdata,
			       bool bss_notify)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_tx_queue_params qparam;
	struct ieee80211_chanctx_conf *chanctx_conf;
	int ac;
	bool use_11b, enable_qos;
>>>>>>> refs/remotes/origin/master
	int aCWmin, aCWmax;

	if (!local->ops->conf_tx)
		return;

<<<<<<< HEAD
	memset(&qparam, 0, sizeof(qparam));

	use_11b = (local->hw.conf.channel->band == IEEE80211_BAND_2GHZ) &&
		 !(sdata->flags & IEEE80211_SDATA_OPERATING_GMODE);

<<<<<<< HEAD
	for (queue = 0; queue < local_to_hw(local)->queues; queue++) {
=======
	for (queue = 0; queue < local->hw.queues; queue++) {
>>>>>>> refs/remotes/origin/cm-10.0
		/* Set defaults according to 802.11-2007 Table 7-37 */
		aCWmax = 1023;
		if (use_11b)
			aCWmin = 31;
		else
			aCWmin = 15;

		switch (queue) {
		case 3: /* AC_BK */
			qparam.cw_max = aCWmax;
			qparam.cw_min = aCWmin;
			qparam.txop = 0;
			qparam.aifs = 7;
			break;
		default: /* never happens but let's not leave undefined */
		case 2: /* AC_BE */
			qparam.cw_max = aCWmax;
			qparam.cw_min = aCWmin;
			qparam.txop = 0;
			qparam.aifs = 3;
			break;
		case 1: /* AC_VI */
			qparam.cw_max = aCWmin;
			qparam.cw_min = (aCWmin + 1) / 2 - 1;
			if (use_11b)
				qparam.txop = 6016/32;
			else
				qparam.txop = 3008/32;
			qparam.aifs = 2;
			break;
		case 0: /* AC_VO */
			qparam.cw_max = (aCWmin + 1) / 2 - 1;
			qparam.cw_min = (aCWmin + 1) / 4 - 1;
			if (use_11b)
				qparam.txop = 3264/32;
			else
				qparam.txop = 1504/32;
			qparam.aifs = 2;
			break;
=======
	if (local->hw.queues < IEEE80211_NUM_ACS)
		return;

	memset(&qparam, 0, sizeof(qparam));

	rcu_read_lock();
	chanctx_conf = rcu_dereference(sdata->vif.chanctx_conf);
	use_11b = (chanctx_conf &&
		   chanctx_conf->def.chan->band == IEEE80211_BAND_2GHZ) &&
		 !(sdata->flags & IEEE80211_SDATA_OPERATING_GMODE);
	rcu_read_unlock();

	/*
	 * By default disable QoS in STA mode for old access points, which do
	 * not support 802.11e. New APs will provide proper queue parameters,
	 * that we will configure later.
	 */
	enable_qos = (sdata->vif.type != NL80211_IFTYPE_STATION);

	/* Set defaults according to 802.11-2007 Table 7-37 */
	aCWmax = 1023;
	if (use_11b)
		aCWmin = 31;
	else
		aCWmin = 15;

	/* Confiure old 802.11b/g medium access rules. */
	qparam.cw_max = aCWmax;
	qparam.cw_min = aCWmin;
	qparam.txop = 0;
	qparam.aifs = 2;

	for (ac = 0; ac < IEEE80211_NUM_ACS; ac++) {
		/* Update if QoS is enabled. */
		if (enable_qos) {
			switch (ac) {
			case IEEE80211_AC_BK:
				qparam.cw_max = aCWmax;
				qparam.cw_min = aCWmin;
				qparam.txop = 0;
				qparam.aifs = 7;
				break;
			/* never happens but let's not leave undefined */
			default:
			case IEEE80211_AC_BE:
				qparam.cw_max = aCWmax;
				qparam.cw_min = aCWmin;
				qparam.txop = 0;
				qparam.aifs = 3;
				break;
			case IEEE80211_AC_VI:
				qparam.cw_max = aCWmin;
				qparam.cw_min = (aCWmin + 1) / 2 - 1;
				if (use_11b)
					qparam.txop = 6016/32;
				else
					qparam.txop = 3008/32;
				qparam.aifs = 2;
				break;
			case IEEE80211_AC_VO:
				qparam.cw_max = (aCWmin + 1) / 2 - 1;
				qparam.cw_min = (aCWmin + 1) / 4 - 1;
				if (use_11b)
					qparam.txop = 3264/32;
				else
					qparam.txop = 1504/32;
				qparam.aifs = 2;
				break;
			}
>>>>>>> refs/remotes/origin/master
		}

		qparam.uapsd = false;

<<<<<<< HEAD
<<<<<<< HEAD
		drv_conf_tx(local, queue, &qparam);
=======
		sdata->tx_conf[queue] = qparam;
		drv_conf_tx(local, sdata, queue, &qparam);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	/* after reinitialize QoS TX queues setting to default,
	 * disable QoS at all */

	if (sdata->vif.type != NL80211_IFTYPE_MONITOR) {
		sdata->vif.bss_conf.qos =
			sdata->vif.type != NL80211_IFTYPE_STATION;
<<<<<<< HEAD
		ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_QOS);
=======
		if (bss_notify)
			ieee80211_bss_info_change_notify(sdata,
							 BSS_CHANGED_QOS);
>>>>>>> refs/remotes/origin/cm-10.0
	}
}

void ieee80211_sta_def_wmm_params(struct ieee80211_sub_if_data *sdata,
				  const size_t supp_rates_len,
				  const u8 *supp_rates)
{
	struct ieee80211_local *local = sdata->local;
	int i, have_higher_than_11mbit = 0;

	/* cf. IEEE 802.11 9.2.12 */
	for (i = 0; i < supp_rates_len; i++)
		if ((supp_rates[i] & 0x7f) * 5 > 110)
			have_higher_than_11mbit = 1;

	if (local->hw.conf.channel->band == IEEE80211_BAND_2GHZ &&
	    have_higher_than_11mbit)
		sdata->flags |= IEEE80211_SDATA_OPERATING_GMODE;
	else
		sdata->flags &= ~IEEE80211_SDATA_OPERATING_GMODE;

<<<<<<< HEAD
	ieee80211_set_wmm_default(sdata);
=======
	ieee80211_set_wmm_default(sdata, true);
>>>>>>> refs/remotes/origin/cm-10.0
}

u32 ieee80211_mandatory_rates(struct ieee80211_local *local,
			      enum ieee80211_band band)
{
	struct ieee80211_supported_band *sband;
	struct ieee80211_rate *bitrates;
	u32 mandatory_rates;
	enum ieee80211_rate_flags mandatory_flag;
	int i;

	sband = local->hw.wiphy->bands[band];
	if (!sband) {
		WARN_ON(1);
		sband = local->hw.wiphy->bands[local->hw.conf.channel->band];
	}

	if (band == IEEE80211_BAND_2GHZ)
		mandatory_flag = IEEE80211_RATE_MANDATORY_B;
	else
		mandatory_flag = IEEE80211_RATE_MANDATORY_A;

	bitrates = sband->bitrates;
	mandatory_rates = 0;
	for (i = 0; i < sband->n_bitrates; i++)
		if (bitrates[i].flags & mandatory_flag)
			mandatory_rates |= BIT(i);
	return mandatory_rates;
}

void ieee80211_send_auth(struct ieee80211_sub_if_data *sdata,
			 u16 transaction, u16 auth_alg,
<<<<<<< HEAD
			 u8 *extra, size_t extra_len, const u8 *bssid,
			 const u8 *key, u8 key_len, u8 key_idx)
=======
			 u8 *extra, size_t extra_len, const u8 *da,
			 const u8 *bssid, const u8 *key, u8 key_len, u8 key_idx)
>>>>>>> refs/remotes/origin/cm-10.0
=======
		sdata->tx_conf[ac] = qparam;
		drv_conf_tx(local, sdata, ac, &qparam);
	}

	if (sdata->vif.type != NL80211_IFTYPE_MONITOR &&
	    sdata->vif.type != NL80211_IFTYPE_P2P_DEVICE) {
		sdata->vif.bss_conf.qos = enable_qos;
		if (bss_notify)
			ieee80211_bss_info_change_notify(sdata,
							 BSS_CHANGED_QOS);
	}
}

void ieee80211_send_auth(struct ieee80211_sub_if_data *sdata,
			 u16 transaction, u16 auth_alg, u16 status,
			 const u8 *extra, size_t extra_len, const u8 *da,
			 const u8 *bssid, const u8 *key, u8 key_len, u8 key_idx,
			 u32 tx_flags)
>>>>>>> refs/remotes/origin/master
{
	struct ieee80211_local *local = sdata->local;
	struct sk_buff *skb;
	struct ieee80211_mgmt *mgmt;
	int err;

<<<<<<< HEAD
	skb = dev_alloc_skb(local->hw.extra_tx_headroom +
			    sizeof(*mgmt) + 6 + extra_len);
<<<<<<< HEAD
	if (!skb) {
		printk(KERN_DEBUG "%s: failed to allocate buffer for auth "
		       "frame\n", sdata->name);
		return;
	}
=======
	if (!skb)
		return;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* 24 + 6 = header + auth_algo + auth_transaction + status_code */
	skb = dev_alloc_skb(local->hw.extra_tx_headroom + 24 + 6 + extra_len);
	if (!skb)
		return;

>>>>>>> refs/remotes/origin/master
	skb_reserve(skb, local->hw.extra_tx_headroom);

	mgmt = (struct ieee80211_mgmt *) skb_put(skb, 24 + 6);
	memset(mgmt, 0, 24 + 6);
	mgmt->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT |
					  IEEE80211_STYPE_AUTH);
<<<<<<< HEAD
<<<<<<< HEAD
	memcpy(mgmt->da, bssid, ETH_ALEN);
=======
	memcpy(mgmt->da, da, ETH_ALEN);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	memcpy(mgmt->da, da, ETH_ALEN);
>>>>>>> refs/remotes/origin/master
	memcpy(mgmt->sa, sdata->vif.addr, ETH_ALEN);
	memcpy(mgmt->bssid, bssid, ETH_ALEN);
	mgmt->u.auth.auth_alg = cpu_to_le16(auth_alg);
	mgmt->u.auth.auth_transaction = cpu_to_le16(transaction);
<<<<<<< HEAD
	mgmt->u.auth.status_code = cpu_to_le16(0);
=======
	mgmt->u.auth.status_code = cpu_to_le16(status);
>>>>>>> refs/remotes/origin/master
	if (extra)
		memcpy(skb_put(skb, extra_len), extra, extra_len);

	if (auth_alg == WLAN_AUTH_SHARED_KEY && transaction == 3) {
		mgmt->frame_control |= cpu_to_le16(IEEE80211_FCTL_PROTECTED);
		err = ieee80211_wep_encrypt(local, skb, key, key_len, key_idx);
		WARN_ON(err);
	}

<<<<<<< HEAD
	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;
	ieee80211_tx_skb(sdata, skb);
}

int ieee80211_build_preq_ies(struct ieee80211_local *local, u8 *buffer,
			     const u8 *ie, size_t ie_len,
			     enum ieee80211_band band, u32 rate_mask,
			     u8 channel)
{
	struct ieee80211_supported_band *sband;
	u8 *pos;
=======
	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT |
					tx_flags;
	ieee80211_tx_skb(sdata, skb);
}

void ieee80211_send_deauth_disassoc(struct ieee80211_sub_if_data *sdata,
				    const u8 *bssid, u16 stype, u16 reason,
				    bool send_frame, u8 *frame_buf)
{
	struct ieee80211_local *local = sdata->local;
	struct sk_buff *skb;
	struct ieee80211_mgmt *mgmt = (void *)frame_buf;

	/* build frame */
	mgmt->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT | stype);
	mgmt->duration = 0; /* initialize only */
	mgmt->seq_ctrl = 0; /* initialize only */
	memcpy(mgmt->da, bssid, ETH_ALEN);
	memcpy(mgmt->sa, sdata->vif.addr, ETH_ALEN);
	memcpy(mgmt->bssid, bssid, ETH_ALEN);
	/* u.deauth.reason_code == u.disassoc.reason_code */
	mgmt->u.deauth.reason_code = cpu_to_le16(reason);

	if (send_frame) {
		skb = dev_alloc_skb(local->hw.extra_tx_headroom +
				    IEEE80211_DEAUTH_FRAME_LEN);
		if (!skb)
			return;

		skb_reserve(skb, local->hw.extra_tx_headroom);

		/* copy in frame */
		memcpy(skb_put(skb, IEEE80211_DEAUTH_FRAME_LEN),
		       mgmt, IEEE80211_DEAUTH_FRAME_LEN);

		if (sdata->vif.type != NL80211_IFTYPE_STATION ||
		    !(sdata->u.mgd.flags & IEEE80211_STA_MFP_ENABLED))
			IEEE80211_SKB_CB(skb)->flags |=
				IEEE80211_TX_INTFL_DONT_ENCRYPT;

		ieee80211_tx_skb(sdata, skb);
	}
}

int ieee80211_build_preq_ies(struct ieee80211_local *local, u8 *buffer,
			     size_t buffer_len, const u8 *ie, size_t ie_len,
			     enum ieee80211_band band, u32 rate_mask,
			     struct cfg80211_chan_def *chandef)
{
	struct ieee80211_supported_band *sband;
	u8 *pos = buffer, *end = buffer + buffer_len;
>>>>>>> refs/remotes/origin/master
	size_t offset = 0, noffset;
	int supp_rates_len, i;
	u8 rates[32];
	int num_rates;
	int ext_rates_len;
<<<<<<< HEAD

	sband = local->hw.wiphy->bands[band];

	pos = buffer;
=======
	int shift;
	u32 rate_flags;

	sband = local->hw.wiphy->bands[band];
	if (WARN_ON_ONCE(!sband))
		return 0;

	rate_flags = ieee80211_chandef_rate_flags(chandef);
	shift = ieee80211_chandef_get_shift(chandef);
>>>>>>> refs/remotes/origin/master

	num_rates = 0;
	for (i = 0; i < sband->n_bitrates; i++) {
		if ((BIT(i) & rate_mask) == 0)
			continue; /* skip rate */
<<<<<<< HEAD
		rates[num_rates++] = (u8) (sband->bitrates[i].bitrate / 5);
=======
		if ((rate_flags & sband->bitrates[i].flags) != rate_flags)
			continue;

		rates[num_rates++] =
			(u8) DIV_ROUND_UP(sband->bitrates[i].bitrate,
					  (1 << shift) * 5);
>>>>>>> refs/remotes/origin/master
	}

	supp_rates_len = min_t(int, num_rates, 8);

<<<<<<< HEAD
=======
	if (end - pos < 2 + supp_rates_len)
		goto out_err;
>>>>>>> refs/remotes/origin/master
	*pos++ = WLAN_EID_SUPP_RATES;
	*pos++ = supp_rates_len;
	memcpy(pos, rates, supp_rates_len);
	pos += supp_rates_len;

	/* insert "request information" if in custom IEs */
	if (ie && ie_len) {
		static const u8 before_extrates[] = {
			WLAN_EID_SSID,
			WLAN_EID_SUPP_RATES,
			WLAN_EID_REQUEST,
		};
		noffset = ieee80211_ie_split(ie, ie_len,
					     before_extrates,
					     ARRAY_SIZE(before_extrates),
					     offset);
<<<<<<< HEAD
=======
		if (end - pos < noffset - offset)
			goto out_err;
>>>>>>> refs/remotes/origin/master
		memcpy(pos, ie + offset, noffset - offset);
		pos += noffset - offset;
		offset = noffset;
	}

	ext_rates_len = num_rates - supp_rates_len;
	if (ext_rates_len > 0) {
<<<<<<< HEAD
=======
		if (end - pos < 2 + ext_rates_len)
			goto out_err;
>>>>>>> refs/remotes/origin/master
		*pos++ = WLAN_EID_EXT_SUPP_RATES;
		*pos++ = ext_rates_len;
		memcpy(pos, rates + supp_rates_len, ext_rates_len);
		pos += ext_rates_len;
	}

<<<<<<< HEAD
	if (channel && sband->band == IEEE80211_BAND_2GHZ) {
		*pos++ = WLAN_EID_DS_PARAMS;
		*pos++ = 1;
		*pos++ = channel;
=======
	if (chandef->chan && sband->band == IEEE80211_BAND_2GHZ) {
		if (end - pos < 3)
			goto out_err;
		*pos++ = WLAN_EID_DS_PARAMS;
		*pos++ = 1;
		*pos++ = ieee80211_frequency_to_channel(
				chandef->chan->center_freq);
>>>>>>> refs/remotes/origin/master
	}

	/* insert custom IEs that go before HT */
	if (ie && ie_len) {
		static const u8 before_ht[] = {
			WLAN_EID_SSID,
			WLAN_EID_SUPP_RATES,
			WLAN_EID_REQUEST,
			WLAN_EID_EXT_SUPP_RATES,
			WLAN_EID_DS_PARAMS,
			WLAN_EID_SUPPORTED_REGULATORY_CLASSES,
		};
		noffset = ieee80211_ie_split(ie, ie_len,
					     before_ht, ARRAY_SIZE(before_ht),
					     offset);
<<<<<<< HEAD
=======
		if (end - pos < noffset - offset)
			goto out_err;
>>>>>>> refs/remotes/origin/master
		memcpy(pos, ie + offset, noffset - offset);
		pos += noffset - offset;
		offset = noffset;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (sband->ht_cap.ht_supported) {
		u16 cap = sband->ht_cap.cap;
		__le16 tmp;

		*pos++ = WLAN_EID_HT_CAPABILITY;
		*pos++ = sizeof(struct ieee80211_ht_cap);
		memset(pos, 0, sizeof(struct ieee80211_ht_cap));
		tmp = cpu_to_le16(cap);
		memcpy(pos, &tmp, sizeof(u16));
		pos += sizeof(u16);
		*pos++ = sband->ht_cap.ampdu_factor |
			 (sband->ht_cap.ampdu_density <<
				IEEE80211_HT_AMPDU_PARM_DENSITY_SHIFT);
		memcpy(pos, &sband->ht_cap.mcs, sizeof(sband->ht_cap.mcs));
		pos += sizeof(sband->ht_cap.mcs);
		pos += 2 + 4 + 1; /* ext info, BF cap, antsel */
	}
=======
	if (sband->ht_cap.ht_supported)
		pos = ieee80211_ie_build_ht_cap(pos, &sband->ht_cap,
						sband->ht_cap.cap);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (sband->ht_cap.ht_supported) {
		if (end - pos < 2 + sizeof(struct ieee80211_ht_cap))
			goto out_err;
		pos = ieee80211_ie_build_ht_cap(pos, &sband->ht_cap,
						sband->ht_cap.cap);
	}
>>>>>>> refs/remotes/origin/master

	/*
	 * If adding more here, adjust code in main.c
	 * that calculates local->scan_ies_len.
	 */

	/* add any remaining custom IEs */
	if (ie && ie_len) {
		noffset = ie_len;
<<<<<<< HEAD
=======
		if (end - pos < noffset - offset)
			goto out_err;
>>>>>>> refs/remotes/origin/master
		memcpy(pos, ie + offset, noffset - offset);
		pos += noffset - offset;
	}

<<<<<<< HEAD
=======
	if (sband->vht_cap.vht_supported) {
		if (end - pos < 2 + sizeof(struct ieee80211_vht_cap))
			goto out_err;
		pos = ieee80211_ie_build_vht_cap(pos, &sband->vht_cap,
						 sband->vht_cap.cap);
	}

	return pos - buffer;
 out_err:
	WARN_ONCE(1, "not enough space for preq IEs\n");
>>>>>>> refs/remotes/origin/master
	return pos - buffer;
}

struct sk_buff *ieee80211_build_probe_req(struct ieee80211_sub_if_data *sdata,
<<<<<<< HEAD
<<<<<<< HEAD
					  u8 *dst,
					  const u8 *ssid, size_t ssid_len,
					  const u8 *ie, size_t ie_len)
=======
					  u8 *dst, u32 ratemask,
					  const u8 *ssid, size_t ssid_len,
					  const u8 *ie, size_t ie_len,
					  bool directed)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct ieee80211_local *local = sdata->local;
	struct sk_buff *skb;
	struct ieee80211_mgmt *mgmt;
	size_t buf_len;
	u8 *buf;
	u8 chan;

	/* FIXME: come up with a proper value */
	buf = kmalloc(200 + ie_len, GFP_KERNEL);
<<<<<<< HEAD
	if (!buf) {
		printk(KERN_DEBUG "%s: failed to allocate temporary IE "
		       "buffer\n", sdata->name);
		return NULL;
	}

	chan = ieee80211_frequency_to_channel(
		local->hw.conf.channel->center_freq);

	buf_len = ieee80211_build_preq_ies(local, buf, ie, ie_len,
					   local->hw.conf.channel->band,
					   sdata->rc_rateidx_mask
					   [local->hw.conf.channel->band],
					   chan);
=======
	if (!buf)
		return NULL;
=======
					  u8 *dst, u32 ratemask,
					  struct ieee80211_channel *chan,
					  const u8 *ssid, size_t ssid_len,
					  const u8 *ie, size_t ie_len,
					  bool directed)
{
	struct ieee80211_local *local = sdata->local;
	struct cfg80211_chan_def chandef;
	struct sk_buff *skb;
	struct ieee80211_mgmt *mgmt;
	int ies_len;
>>>>>>> refs/remotes/origin/master

	/*
	 * Do not send DS Channel parameter for directed probe requests
	 * in order to maximize the chance that we get a response.  Some
	 * badly-behaved APs don't respond when this parameter is included.
	 */
<<<<<<< HEAD
	if (directed)
		chan = 0;
	else
		chan = ieee80211_frequency_to_channel(
			local->hw.conf.channel->center_freq);

	buf_len = ieee80211_build_preq_ies(local, buf, ie, ie_len,
					   local->hw.conf.channel->band,
					   ratemask, chan);
>>>>>>> refs/remotes/origin/cm-10.0

	skb = ieee80211_probereq_get(&local->hw, &sdata->vif,
				     ssid, ssid_len,
				     buf, buf_len);
	if (!skb)
		goto out;
=======
	chandef.width = sdata->vif.bss_conf.chandef.width;
	if (directed)
		chandef.chan = NULL;
	else
		chandef.chan = chan;

	skb = ieee80211_probereq_get(&local->hw, &sdata->vif,
				     ssid, ssid_len, 100 + ie_len);
	if (!skb)
		return NULL;

	ies_len = ieee80211_build_preq_ies(local, skb_tail_pointer(skb),
					   skb_tailroom(skb),
					   ie, ie_len, chan->band,
					   ratemask, &chandef);
	skb_put(skb, ies_len);
>>>>>>> refs/remotes/origin/master

	if (dst) {
		mgmt = (struct ieee80211_mgmt *) skb->data;
		memcpy(mgmt->da, dst, ETH_ALEN);
		memcpy(mgmt->bssid, dst, ETH_ALEN);
	}

	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;

<<<<<<< HEAD
 out:
	kfree(buf);

=======
>>>>>>> refs/remotes/origin/master
	return skb;
}

void ieee80211_send_probe_req(struct ieee80211_sub_if_data *sdata, u8 *dst,
			      const u8 *ssid, size_t ssid_len,
<<<<<<< HEAD
<<<<<<< HEAD
			      const u8 *ie, size_t ie_len)
{
	struct sk_buff *skb;

	skb = ieee80211_build_probe_req(sdata, dst, ssid, ssid_len, ie, ie_len);
	if (skb)
		ieee80211_tx_skb(sdata, skb);
=======
			      const u8 *ie, size_t ie_len,
			      u32 ratemask, bool directed, bool no_cck)
{
	struct sk_buff *skb;

	skb = ieee80211_build_probe_req(sdata, dst, ratemask, ssid, ssid_len,
					ie, ie_len, directed);
	if (skb) {
		if (no_cck)
			IEEE80211_SKB_CB(skb)->flags |=
				IEEE80211_TX_CTL_NO_CCK_RATE;
		ieee80211_tx_skb(sdata, skb);
	}
>>>>>>> refs/remotes/origin/cm-10.0
}

u32 ieee80211_sta_get_rates(struct ieee80211_local *local,
			    struct ieee802_11_elems *elems,
			    enum ieee80211_band band)
=======
			      const u8 *ie, size_t ie_len,
			      u32 ratemask, bool directed, u32 tx_flags,
			      struct ieee80211_channel *channel, bool scan)
{
	struct sk_buff *skb;

	skb = ieee80211_build_probe_req(sdata, dst, ratemask, channel,
					ssid, ssid_len,
					ie, ie_len, directed);
	if (skb) {
		IEEE80211_SKB_CB(skb)->flags |= tx_flags;
		if (scan)
			ieee80211_tx_skb_tid_band(sdata, skb, 7, channel->band);
		else
			ieee80211_tx_skb(sdata, skb);
	}
}

u32 ieee80211_sta_get_rates(struct ieee80211_sub_if_data *sdata,
			    struct ieee802_11_elems *elems,
			    enum ieee80211_band band, u32 *basic_rates)
>>>>>>> refs/remotes/origin/master
{
	struct ieee80211_supported_band *sband;
	struct ieee80211_rate *bitrates;
	size_t num_rates;
<<<<<<< HEAD
	u32 supp_rates;
	int i, j;
	sband = local->hw.wiphy->bands[band];

	if (!sband) {
		WARN_ON(1);
		sband = local->hw.wiphy->bands[local->hw.conf.channel->band];
	}
=======
	u32 supp_rates, rate_flags;
	int i, j, shift;
	sband = sdata->local->hw.wiphy->bands[band];

	rate_flags = ieee80211_chandef_rate_flags(&sdata->vif.bss_conf.chandef);
	shift = ieee80211_vif_get_shift(&sdata->vif);

	if (WARN_ON(!sband))
		return 1;
>>>>>>> refs/remotes/origin/master

	bitrates = sband->bitrates;
	num_rates = sband->n_bitrates;
	supp_rates = 0;
	for (i = 0; i < elems->supp_rates_len +
		     elems->ext_supp_rates_len; i++) {
		u8 rate = 0;
		int own_rate;
<<<<<<< HEAD
=======
		bool is_basic;
>>>>>>> refs/remotes/origin/master
		if (i < elems->supp_rates_len)
			rate = elems->supp_rates[i];
		else if (elems->ext_supp_rates)
			rate = elems->ext_supp_rates
				[i - elems->supp_rates_len];
		own_rate = 5 * (rate & 0x7f);
<<<<<<< HEAD
		for (j = 0; j < num_rates; j++)
			if (bitrates[j].bitrate == own_rate)
				supp_rates |= BIT(j);
=======
		is_basic = !!(rate & 0x80);

		if (is_basic && (rate & 0x7f) == BSS_MEMBERSHIP_SELECTOR_HT_PHY)
			continue;

		for (j = 0; j < num_rates; j++) {
			int brate;
			if ((rate_flags & sband->bitrates[j].flags)
			    != rate_flags)
				continue;

			brate = DIV_ROUND_UP(sband->bitrates[j].bitrate,
					     1 << shift);

			if (brate == own_rate) {
				supp_rates |= BIT(j);
				if (basic_rates && is_basic)
					*basic_rates |= BIT(j);
			}
		}
>>>>>>> refs/remotes/origin/master
	}
	return supp_rates;
}

void ieee80211_stop_device(struct ieee80211_local *local)
{
	ieee80211_led_radio(local, false);
	ieee80211_mod_tpt_led_trig(local, 0, IEEE80211_TPT_LEDTRIG_FL_RADIO);

	cancel_work_sync(&local->reconfig_filter);

	flush_workqueue(local->workqueue);
	drv_stop(local);
}

<<<<<<< HEAD
=======
static void ieee80211_assign_chanctx(struct ieee80211_local *local,
				     struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_chanctx_conf *conf;
	struct ieee80211_chanctx *ctx;

	if (!local->use_chanctx)
		return;

	mutex_lock(&local->chanctx_mtx);
	conf = rcu_dereference_protected(sdata->vif.chanctx_conf,
					 lockdep_is_held(&local->chanctx_mtx));
	if (conf) {
		ctx = container_of(conf, struct ieee80211_chanctx, conf);
		drv_assign_vif_chanctx(local, sdata, ctx);
	}
	mutex_unlock(&local->chanctx_mtx);
}

>>>>>>> refs/remotes/origin/master
int ieee80211_reconfig(struct ieee80211_local *local)
{
	struct ieee80211_hw *hw = &local->hw;
	struct ieee80211_sub_if_data *sdata;
<<<<<<< HEAD
	struct sta_info *sta;
<<<<<<< HEAD
	int res;
=======
	int res, i;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct ieee80211_chanctx *ctx;
	struct sta_info *sta;
	int res, i;
	bool reconfig_due_to_wowlan = false;
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_PM
	if (local->suspended)
		local->resuming = true;

	if (local->wowlan) {
<<<<<<< HEAD
		local->wowlan = false;
		res = drv_resume(local);
=======
		res = drv_resume(local);
		local->wowlan = false;
>>>>>>> refs/remotes/origin/master
		if (res < 0) {
			local->resuming = false;
			return res;
		}
		if (res == 0)
			goto wake_up;
		WARN_ON(res > 1);
		/*
		 * res is 1, which means the driver requested
		 * to go through a regular reset on wakeup.
		 */
<<<<<<< HEAD
	}
#endif
<<<<<<< HEAD

	/* restart hardware */
	if (local->open_count) {
		/*
		 * Upon resume hardware can sometimes be goofy due to
		 * various platform / driver / bus issues, so restarting
		 * the device may at times not work immediately. Propagate
		 * the error.
		 */
		res = drv_start(local);
		if (res) {
			WARN(local->suspended, "Hardware became unavailable "
			     "upon resume. This could be a software issue "
			     "prior to suspend or a hardware issue.\n");
			return res;
		}

		ieee80211_led_radio(local, true);
		ieee80211_mod_tpt_led_trig(local,
					   IEEE80211_TPT_LEDTRIG_FL_RADIO, 0);
	}

=======
=======
		reconfig_due_to_wowlan = true;
	}
#endif
>>>>>>> refs/remotes/origin/master
	/* everything else happens only if HW was up & running */
	if (!local->open_count)
		goto wake_up;

	/*
	 * Upon resume hardware can sometimes be goofy due to
	 * various platform / driver / bus issues, so restarting
	 * the device may at times not work immediately. Propagate
	 * the error.
	 */
	res = drv_start(local);
	if (res) {
		WARN(local->suspended, "Hardware became unavailable "
		     "upon resume. This could be a software issue "
		     "prior to suspend or a hardware issue.\n");
		return res;
	}

	/* setup fragmentation threshold */
	drv_set_frag_threshold(local, hw->wiphy->frag_threshold);

	/* setup RTS threshold */
	drv_set_rts_threshold(local, hw->wiphy->rts_threshold);

	/* reset coverage class */
	drv_set_coverage_class(local, hw->wiphy->coverage_class);

	ieee80211_led_radio(local, true);
	ieee80211_mod_tpt_led_trig(local,
				   IEEE80211_TPT_LEDTRIG_FL_RADIO, 0);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	/* add interfaces */
=======
	/* add interfaces */
	sdata = rtnl_dereference(local->monitor_sdata);
	if (sdata) {
		/* in HW restart it exists already */
		WARN_ON(local->resuming);
		res = drv_add_interface(local, sdata);
		if (WARN_ON(res)) {
			rcu_assign_pointer(local->monitor_sdata, NULL);
			synchronize_net();
			kfree(sdata);
		}
	}

>>>>>>> refs/remotes/origin/master
	list_for_each_entry(sdata, &local->interfaces, list) {
		if (sdata->vif.type != NL80211_IFTYPE_AP_VLAN &&
		    sdata->vif.type != NL80211_IFTYPE_MONITOR &&
		    ieee80211_sdata_running(sdata))
<<<<<<< HEAD
<<<<<<< HEAD
			res = drv_add_interface(local, &sdata->vif);
=======
			res = drv_add_interface(local, sdata);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	/* add STAs back */
	mutex_lock(&local->sta_mtx);
	list_for_each_entry(sta, &local->sta_list, list) {
		if (sta->uploaded) {
<<<<<<< HEAD
			sdata = sta->sdata;
			if (sdata->vif.type == NL80211_IFTYPE_AP_VLAN)
				sdata = container_of(sdata->bss,
					     struct ieee80211_sub_if_data,
					     u.ap);

			WARN_ON(drv_sta_add(local, sdata, &sta->sta));
=======
			enum ieee80211_sta_state state;

			for (state = IEEE80211_STA_NOTEXIST;
			     state < sta->sta_state; state++)
				WARN_ON(drv_sta_state(local, sta->sdata, sta,
						      state, state + 1));
>>>>>>> refs/remotes/origin/cm-10.0
		}
	}
	mutex_unlock(&local->sta_mtx);

<<<<<<< HEAD
	/* setup fragmentation threshold */
	drv_set_frag_threshold(local, hw->wiphy->frag_threshold);

	/* setup RTS threshold */
	drv_set_rts_threshold(local, hw->wiphy->rts_threshold);
=======
	/* reconfigure tx conf */
	list_for_each_entry(sdata, &local->interfaces, list) {
		if (sdata->vif.type == NL80211_IFTYPE_AP_VLAN ||
		    sdata->vif.type == NL80211_IFTYPE_MONITOR ||
		    !ieee80211_sdata_running(sdata))
			continue;

		for (i = 0; i < hw->queues; i++)
			drv_conf_tx(local, sdata, i, &sdata->tx_conf[i]);
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
			res = drv_add_interface(local, sdata);
	}

	/* add channel contexts */
	if (local->use_chanctx) {
		mutex_lock(&local->chanctx_mtx);
		list_for_each_entry(ctx, &local->chanctx_list, list)
			WARN_ON(drv_add_chanctx(local, ctx));
		mutex_unlock(&local->chanctx_mtx);
	}

	list_for_each_entry(sdata, &local->interfaces, list) {
		if (!ieee80211_sdata_running(sdata))
			continue;
		ieee80211_assign_chanctx(local, sdata);
	}

	sdata = rtnl_dereference(local->monitor_sdata);
	if (sdata && ieee80211_sdata_running(sdata))
		ieee80211_assign_chanctx(local, sdata);

	/* add STAs back */
	mutex_lock(&local->sta_mtx);
	list_for_each_entry(sta, &local->sta_list, list) {
		enum ieee80211_sta_state state;

		if (!sta->uploaded)
			continue;

		/* AP-mode stations will be added later */
		if (sta->sdata->vif.type == NL80211_IFTYPE_AP)
			continue;

		for (state = IEEE80211_STA_NOTEXIST;
		     state < sta->sta_state; state++)
			WARN_ON(drv_sta_state(local, sta->sdata, sta, state,
					      state + 1));
	}
	mutex_unlock(&local->sta_mtx);

	/* reconfigure tx conf */
	if (hw->queues >= IEEE80211_NUM_ACS) {
		list_for_each_entry(sdata, &local->interfaces, list) {
			if (sdata->vif.type == NL80211_IFTYPE_AP_VLAN ||
			    sdata->vif.type == NL80211_IFTYPE_MONITOR ||
			    !ieee80211_sdata_running(sdata))
				continue;

			for (i = 0; i < IEEE80211_NUM_ACS; i++)
				drv_conf_tx(local, sdata, i,
					    &sdata->tx_conf[i]);
		}
	}
>>>>>>> refs/remotes/origin/master

	/* reconfigure hardware */
	ieee80211_hw_config(local, ~0);

	ieee80211_configure_filter(local);

	/* Finally also reconfigure all the BSS information */
	list_for_each_entry(sdata, &local->interfaces, list) {
		u32 changed;

		if (!ieee80211_sdata_running(sdata))
			continue;

		/* common change flags for all interface types */
		changed = BSS_CHANGED_ERP_CTS_PROT |
			  BSS_CHANGED_ERP_PREAMBLE |
			  BSS_CHANGED_ERP_SLOT |
			  BSS_CHANGED_HT |
			  BSS_CHANGED_BASIC_RATES |
			  BSS_CHANGED_BEACON_INT |
			  BSS_CHANGED_BSSID |
			  BSS_CHANGED_CQM |
<<<<<<< HEAD
<<<<<<< HEAD
			  BSS_CHANGED_QOS;

		switch (sdata->vif.type) {
		case NL80211_IFTYPE_STATION:
			changed |= BSS_CHANGED_ASSOC;
=======
			  BSS_CHANGED_QOS |
			  BSS_CHANGED_IDLE;
=======
			  BSS_CHANGED_QOS |
			  BSS_CHANGED_IDLE |
			  BSS_CHANGED_TXPOWER;
>>>>>>> refs/remotes/origin/master

		switch (sdata->vif.type) {
		case NL80211_IFTYPE_STATION:
			changed |= BSS_CHANGED_ASSOC |
<<<<<<< HEAD
				   BSS_CHANGED_ARP_FILTER;
>>>>>>> refs/remotes/origin/cm-10.0
			mutex_lock(&sdata->u.mgd.mtx);
			ieee80211_bss_info_change_notify(sdata, changed);
			mutex_unlock(&sdata->u.mgd.mtx);
=======
				   BSS_CHANGED_ARP_FILTER |
				   BSS_CHANGED_PS;

			/* Re-send beacon info report to the driver */
			if (sdata->u.mgd.have_beacon)
				changed |= BSS_CHANGED_BEACON_INFO;

			sdata_lock(sdata);
			ieee80211_bss_info_change_notify(sdata, changed);
			sdata_unlock(sdata);
>>>>>>> refs/remotes/origin/master
			break;
		case NL80211_IFTYPE_ADHOC:
			changed |= BSS_CHANGED_IBSS;
			/* fall through */
		case NL80211_IFTYPE_AP:
<<<<<<< HEAD
<<<<<<< HEAD
=======
			changed |= BSS_CHANGED_SSID;

			if (sdata->vif.type == NL80211_IFTYPE_AP)
				changed |= BSS_CHANGED_AP_PROBE_RESP;

			/* fall through */
>>>>>>> refs/remotes/origin/cm-10.0
		case NL80211_IFTYPE_MESH_POINT:
			changed |= BSS_CHANGED_BEACON |
				   BSS_CHANGED_BEACON_ENABLED;
			ieee80211_bss_info_change_notify(sdata, changed);
=======
			changed |= BSS_CHANGED_SSID | BSS_CHANGED_P2P_PS;

			if (sdata->vif.type == NL80211_IFTYPE_AP) {
				changed |= BSS_CHANGED_AP_PROBE_RESP;

				if (rcu_access_pointer(sdata->u.ap.beacon))
					drv_start_ap(local, sdata);
			}

			/* fall through */
		case NL80211_IFTYPE_MESH_POINT:
			if (sdata->vif.bss_conf.enable_beacon) {
				changed |= BSS_CHANGED_BEACON |
					   BSS_CHANGED_BEACON_ENABLED;
				ieee80211_bss_info_change_notify(sdata, changed);
			}
>>>>>>> refs/remotes/origin/master
			break;
		case NL80211_IFTYPE_WDS:
			break;
		case NL80211_IFTYPE_AP_VLAN:
		case NL80211_IFTYPE_MONITOR:
			/* ignore virtual */
			break;
<<<<<<< HEAD
=======
		case NL80211_IFTYPE_P2P_DEVICE:
			changed = BSS_CHANGED_IDLE;
			break;
>>>>>>> refs/remotes/origin/master
		case NL80211_IFTYPE_UNSPECIFIED:
		case NUM_NL80211_IFTYPES:
		case NL80211_IFTYPE_P2P_CLIENT:
		case NL80211_IFTYPE_P2P_GO:
			WARN_ON(1);
			break;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	ieee80211_recalc_ps(local, -1);

	/*
	 * The sta might be in psm against the ap (e.g. because
	 * this was the state before a hw restart), so we
	 * explicitly send a null packet in order to make sure
	 * it'll sync against the ap (and get out of psm).
	 */
	if (!(local->hw.conf.flags & IEEE80211_CONF_PS)) {
		list_for_each_entry(sdata, &local->interfaces, list) {
			if (sdata->vif.type != NL80211_IFTYPE_STATION)
				continue;
			if (!sdata->u.mgd.associated)
				continue;

			ieee80211_send_nullfunc(local, sdata, 0);
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* APs are now beaconing, add back stations */
	mutex_lock(&local->sta_mtx);
	list_for_each_entry(sta, &local->sta_list, list) {
		enum ieee80211_sta_state state;

		if (!sta->uploaded)
			continue;

		if (sta->sdata->vif.type != NL80211_IFTYPE_AP)
			continue;

		for (state = IEEE80211_STA_NOTEXIST;
		     state < sta->sta_state; state++)
			WARN_ON(drv_sta_state(local, sta->sdata, sta, state,
					      state + 1));
	}
	mutex_unlock(&local->sta_mtx);

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/* add back keys */
	list_for_each_entry(sdata, &local->interfaces, list)
		if (ieee80211_sdata_running(sdata))
			ieee80211_enable_keys(sdata);

 wake_up:
<<<<<<< HEAD
<<<<<<< HEAD
=======
	local->in_reconfig = false;
	barrier();

	if (local->monitors == local->open_count && local->monitors > 0)
		ieee80211_add_virtual_monitor(local);

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/*
	 * Clear the WLAN_STA_BLOCK_BA flag so new aggregation
	 * sessions can be established after a resume.
	 *
	 * Also tear down aggregation sessions since reconfiguring
	 * them in a hardware restart scenario is not easily done
	 * right now, and the hardware will have lost information
	 * about the sessions, but we and the AP still think they
	 * are active. This is really a workaround though.
	 */
	if (hw->flags & IEEE80211_HW_AMPDU_AGGREGATION) {
		mutex_lock(&local->sta_mtx);

		list_for_each_entry(sta, &local->sta_list, list) {
<<<<<<< HEAD
			ieee80211_sta_tear_down_BA_sessions(sta, true);
<<<<<<< HEAD
			clear_sta_flags(sta, WLAN_STA_BLOCK_BA);
=======
			clear_sta_flag(sta, WLAN_STA_BLOCK_BA);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			ieee80211_sta_tear_down_BA_sessions(
					sta, AGG_STOP_LOCAL_REQUEST);
			clear_sta_flag(sta, WLAN_STA_BLOCK_BA);
>>>>>>> refs/remotes/origin/master
		}

		mutex_unlock(&local->sta_mtx);
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	ieee80211_wake_queues_by_reason(hw,
			IEEE80211_QUEUE_STOP_REASON_SUSPEND);
=======
	ieee80211_wake_queues_by_reason(hw, IEEE80211_MAX_QUEUE_MAP,
					IEEE80211_QUEUE_STOP_REASON_SUSPEND);
>>>>>>> refs/remotes/origin/master

	/*
	 * If this is for hw restart things are still running.
	 * We may want to change that later, however.
	 */
<<<<<<< HEAD
=======
	if (!local->suspended || reconfig_due_to_wowlan)
		drv_restart_complete(local);

>>>>>>> refs/remotes/origin/master
	if (!local->suspended)
		return 0;

#ifdef CONFIG_PM
	/* first set suspended false, then resuming */
	local->suspended = false;
	mb();
	local->resuming = false;

	list_for_each_entry(sdata, &local->interfaces, list) {
<<<<<<< HEAD
		switch(sdata->vif.type) {
		case NL80211_IFTYPE_STATION:
			ieee80211_sta_restart(sdata);
			break;
		case NL80211_IFTYPE_ADHOC:
			ieee80211_ibss_restart(sdata);
			break;
		case NL80211_IFTYPE_MESH_POINT:
			ieee80211_mesh_restart(sdata);
			break;
		default:
			break;
		}
	}

	mod_timer(&local->sta_cleanup, jiffies + 1);

	mutex_lock(&local->sta_mtx);
	list_for_each_entry(sta, &local->sta_list, list)
		mesh_plink_restart(sta);
	mutex_unlock(&local->sta_mtx);
=======
		if (!ieee80211_sdata_running(sdata))
			continue;
		if (sdata->vif.type == NL80211_IFTYPE_STATION)
			ieee80211_sta_restart(sdata);
	}

	mod_timer(&local->sta_cleanup, jiffies + 1);
>>>>>>> refs/remotes/origin/master
#else
	WARN_ON(1);
#endif
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
void ieee80211_resume_disconnect(struct ieee80211_vif *vif)
{
	struct ieee80211_sub_if_data *sdata;
	struct ieee80211_local *local;
	struct ieee80211_key *key;

	if (WARN_ON(!vif))
		return;

	sdata = vif_to_sdata(vif);
	local = sdata->local;

	if (WARN_ON(!local->resuming))
		return;

	if (WARN_ON(vif->type != NL80211_IFTYPE_STATION))
		return;

	sdata->flags |= IEEE80211_SDATA_DISCONNECT_RESUME;

	mutex_lock(&local->key_mtx);
	list_for_each_entry(key, &sdata->key_list, list)
		key->flags |= KEY_FLAG_TAINTED;
	mutex_unlock(&local->key_mtx);
}
EXPORT_SYMBOL_GPL(ieee80211_resume_disconnect);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static int check_mgd_smps(struct ieee80211_if_managed *ifmgd,
			  enum ieee80211_smps_mode *smps_mode)
{
	if (ifmgd->associated) {
		*smps_mode = ifmgd->ap_smps;

		if (*smps_mode == IEEE80211_SMPS_AUTOMATIC) {
			if (ifmgd->powersave)
				*smps_mode = IEEE80211_SMPS_DYNAMIC;
			else
				*smps_mode = IEEE80211_SMPS_OFF;
		}

		return 1;
	}

	return 0;
}

/* must hold iflist_mtx */
void ieee80211_recalc_smps(struct ieee80211_local *local)
{
	struct ieee80211_sub_if_data *sdata;
	enum ieee80211_smps_mode smps_mode = IEEE80211_SMPS_OFF;
	int count = 0;

	lockdep_assert_held(&local->iflist_mtx);

	/*
	 * This function could be improved to handle multiple
	 * interfaces better, but right now it makes any
	 * non-station interfaces force SM PS to be turned
	 * off. If there are multiple station interfaces it
	 * could also use the best possible mode, e.g. if
	 * one is in static and the other in dynamic then
	 * dynamic is ok.
	 */

	list_for_each_entry(sdata, &local->interfaces, list) {
		if (!ieee80211_sdata_running(sdata))
			continue;
		if (sdata->vif.type != NL80211_IFTYPE_STATION)
			goto set;

		count += check_mgd_smps(&sdata->u.mgd, &smps_mode);

		if (count > 1) {
			smps_mode = IEEE80211_SMPS_OFF;
			break;
		}
	}

	if (smps_mode == local->smps_mode)
		return;

 set:
	local->smps_mode = smps_mode;
	/* changed flag is auto-detected for this */
	ieee80211_hw_config(local, 0);
=======
void ieee80211_recalc_smps(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_chanctx_conf *chanctx_conf;
	struct ieee80211_chanctx *chanctx;

	mutex_lock(&local->chanctx_mtx);

	chanctx_conf = rcu_dereference_protected(sdata->vif.chanctx_conf,
					lockdep_is_held(&local->chanctx_mtx));

	if (WARN_ON_ONCE(!chanctx_conf))
		goto unlock;

	chanctx = container_of(chanctx_conf, struct ieee80211_chanctx, conf);
	ieee80211_recalc_smps_chanctx(local, chanctx);
 unlock:
	mutex_unlock(&local->chanctx_mtx);
>>>>>>> refs/remotes/origin/master
}

static bool ieee80211_id_in_list(const u8 *ids, int n_ids, u8 id)
{
	int i;

	for (i = 0; i < n_ids; i++)
		if (ids[i] == id)
			return true;
	return false;
}

/**
 * ieee80211_ie_split - split an IE buffer according to ordering
 *
 * @ies: the IE buffer
 * @ielen: the length of the IE buffer
 * @ids: an array with element IDs that are allowed before
 *	the split
 * @n_ids: the size of the element ID array
 * @offset: offset where to start splitting in the buffer
 *
 * This function splits an IE buffer by updating the @offset
 * variable to point to the location where the buffer should be
 * split.
 *
 * It assumes that the given IE buffer is well-formed, this
 * has to be guaranteed by the caller!
 *
 * It also assumes that the IEs in the buffer are ordered
 * correctly, if not the result of using this function will not
 * be ordered correctly either, i.e. it does no reordering.
 *
 * The function returns the offset where the next part of the
 * buffer starts, which may be @ielen if the entire (remainder)
 * of the buffer should be used.
 */
size_t ieee80211_ie_split(const u8 *ies, size_t ielen,
			  const u8 *ids, int n_ids, size_t offset)
{
	size_t pos = offset;

	while (pos < ielen && ieee80211_id_in_list(ids, n_ids, ies[pos]))
		pos += 2 + ies[pos + 1];

	return pos;
}

size_t ieee80211_ie_split_vendor(const u8 *ies, size_t ielen, size_t offset)
{
	size_t pos = offset;

	while (pos < ielen && ies[pos] != WLAN_EID_VENDOR_SPECIFIC)
		pos += 2 + ies[pos + 1];

	return pos;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master

static void _ieee80211_enable_rssi_reports(struct ieee80211_sub_if_data *sdata,
					    int rssi_min_thold,
					    int rssi_max_thold)
{
	trace_api_enable_rssi_reports(sdata, rssi_min_thold, rssi_max_thold);

	if (WARN_ON(sdata->vif.type != NL80211_IFTYPE_STATION))
		return;

	/*
	 * Scale up threshold values before storing it, as the RSSI averaging
	 * algorithm uses a scaled up value as well. Change this scaling
	 * factor if the RSSI averaging algorithm changes.
	 */
	sdata->u.mgd.rssi_min_thold = rssi_min_thold*16;
	sdata->u.mgd.rssi_max_thold = rssi_max_thold*16;
}

void ieee80211_enable_rssi_reports(struct ieee80211_vif *vif,
				    int rssi_min_thold,
				    int rssi_max_thold)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);

	WARN_ON(rssi_min_thold == rssi_max_thold ||
		rssi_min_thold > rssi_max_thold);

	_ieee80211_enable_rssi_reports(sdata, rssi_min_thold,
				       rssi_max_thold);
}
EXPORT_SYMBOL(ieee80211_enable_rssi_reports);

void ieee80211_disable_rssi_reports(struct ieee80211_vif *vif)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);

	_ieee80211_enable_rssi_reports(sdata, 0, 0);
}
EXPORT_SYMBOL(ieee80211_disable_rssi_reports);

u8 *ieee80211_ie_build_ht_cap(u8 *pos, struct ieee80211_sta_ht_cap *ht_cap,
			      u16 cap)
{
	__le16 tmp;

	*pos++ = WLAN_EID_HT_CAPABILITY;
	*pos++ = sizeof(struct ieee80211_ht_cap);
	memset(pos, 0, sizeof(struct ieee80211_ht_cap));

	/* capability flags */
	tmp = cpu_to_le16(cap);
	memcpy(pos, &tmp, sizeof(u16));
	pos += sizeof(u16);

	/* AMPDU parameters */
	*pos++ = ht_cap->ampdu_factor |
		 (ht_cap->ampdu_density <<
			IEEE80211_HT_AMPDU_PARM_DENSITY_SHIFT);

	/* MCS set */
	memcpy(pos, &ht_cap->mcs, sizeof(ht_cap->mcs));
	pos += sizeof(ht_cap->mcs);

	/* extended capabilities */
	pos += sizeof(__le16);

	/* BF capabilities */
	pos += sizeof(__le32);

	/* antenna selection */
	pos += sizeof(u8);

	return pos;
}

<<<<<<< HEAD
u8 *ieee80211_ie_build_ht_info(u8 *pos,
			       struct ieee80211_sta_ht_cap *ht_cap,
			       struct ieee80211_channel *channel,
			       enum nl80211_channel_type channel_type)
{
	struct ieee80211_ht_info *ht_info;
	/* Build HT Information */
	*pos++ = WLAN_EID_HT_INFORMATION;
	*pos++ = sizeof(struct ieee80211_ht_info);
	ht_info = (struct ieee80211_ht_info *)pos;
	ht_info->control_chan =
			ieee80211_frequency_to_channel(channel->center_freq);
	switch (channel_type) {
	case NL80211_CHAN_HT40MINUS:
		ht_info->ht_param = IEEE80211_HT_PARAM_CHA_SEC_BELOW;
		break;
	case NL80211_CHAN_HT40PLUS:
		ht_info->ht_param = IEEE80211_HT_PARAM_CHA_SEC_ABOVE;
		break;
	case NL80211_CHAN_HT20:
	default:
		ht_info->ht_param = IEEE80211_HT_PARAM_CHA_SEC_NONE;
		break;
	}
	if (ht_cap->cap & IEEE80211_HT_CAP_SUP_WIDTH_20_40)
		ht_info->ht_param |= IEEE80211_HT_PARAM_CHAN_WIDTH_ANY;

	/*
	 * Note: According to 802.11n-2009 9.13.3.1, HT Protection field and
	 * RIFS Mode are reserved in IBSS mode, therefore keep them at 0
	 */
	ht_info->operation_mode = 0x0000;
	ht_info->stbc_param = 0x0000;

	/* It seems that Basic MCS set and Supported MCS set
	   are identical for the first 10 bytes */
	memset(&ht_info->basic_set, 0, 16);
	memcpy(&ht_info->basic_set, &ht_cap->mcs, 10);

	return pos + sizeof(struct ieee80211_ht_info);
}

enum nl80211_channel_type
ieee80211_ht_info_to_channel_type(struct ieee80211_ht_info *ht_info)
{
	enum nl80211_channel_type channel_type;

	if (!ht_info)
		return NL80211_CHAN_NO_HT;

	switch (ht_info->ht_param & IEEE80211_HT_PARAM_CHA_SEC_OFFSET) {
=======
u8 *ieee80211_ie_build_vht_cap(u8 *pos, struct ieee80211_sta_vht_cap *vht_cap,
			       u32 cap)
{
	__le32 tmp;

	*pos++ = WLAN_EID_VHT_CAPABILITY;
	*pos++ = sizeof(struct ieee80211_vht_cap);
	memset(pos, 0, sizeof(struct ieee80211_vht_cap));

	/* capability flags */
	tmp = cpu_to_le32(cap);
	memcpy(pos, &tmp, sizeof(u32));
	pos += sizeof(u32);

	/* VHT MCS set */
	memcpy(pos, &vht_cap->vht_mcs, sizeof(vht_cap->vht_mcs));
	pos += sizeof(vht_cap->vht_mcs);

	return pos;
}

u8 *ieee80211_ie_build_ht_oper(u8 *pos, struct ieee80211_sta_ht_cap *ht_cap,
			       const struct cfg80211_chan_def *chandef,
			       u16 prot_mode)
{
	struct ieee80211_ht_operation *ht_oper;
	/* Build HT Information */
	*pos++ = WLAN_EID_HT_OPERATION;
	*pos++ = sizeof(struct ieee80211_ht_operation);
	ht_oper = (struct ieee80211_ht_operation *)pos;
	ht_oper->primary_chan = ieee80211_frequency_to_channel(
					chandef->chan->center_freq);
	switch (chandef->width) {
	case NL80211_CHAN_WIDTH_160:
	case NL80211_CHAN_WIDTH_80P80:
	case NL80211_CHAN_WIDTH_80:
	case NL80211_CHAN_WIDTH_40:
		if (chandef->center_freq1 > chandef->chan->center_freq)
			ht_oper->ht_param = IEEE80211_HT_PARAM_CHA_SEC_ABOVE;
		else
			ht_oper->ht_param = IEEE80211_HT_PARAM_CHA_SEC_BELOW;
		break;
	default:
		ht_oper->ht_param = IEEE80211_HT_PARAM_CHA_SEC_NONE;
		break;
	}
	if (ht_cap->cap & IEEE80211_HT_CAP_SUP_WIDTH_20_40 &&
	    chandef->width != NL80211_CHAN_WIDTH_20_NOHT &&
	    chandef->width != NL80211_CHAN_WIDTH_20)
		ht_oper->ht_param |= IEEE80211_HT_PARAM_CHAN_WIDTH_ANY;

	ht_oper->operation_mode = cpu_to_le16(prot_mode);
	ht_oper->stbc_param = 0x0000;

	/* It seems that Basic MCS set and Supported MCS set
	   are identical for the first 10 bytes */
	memset(&ht_oper->basic_set, 0, 16);
	memcpy(&ht_oper->basic_set, &ht_cap->mcs, 10);

	return pos + sizeof(struct ieee80211_ht_operation);
}

void ieee80211_ht_oper_to_chandef(struct ieee80211_channel *control_chan,
				  const struct ieee80211_ht_operation *ht_oper,
				  struct cfg80211_chan_def *chandef)
{
	enum nl80211_channel_type channel_type;

	if (!ht_oper) {
		cfg80211_chandef_create(chandef, control_chan,
					NL80211_CHAN_NO_HT);
		return;
	}

	switch (ht_oper->ht_param & IEEE80211_HT_PARAM_CHA_SEC_OFFSET) {
>>>>>>> refs/remotes/origin/master
	case IEEE80211_HT_PARAM_CHA_SEC_NONE:
		channel_type = NL80211_CHAN_HT20;
		break;
	case IEEE80211_HT_PARAM_CHA_SEC_ABOVE:
		channel_type = NL80211_CHAN_HT40PLUS;
		break;
	case IEEE80211_HT_PARAM_CHA_SEC_BELOW:
		channel_type = NL80211_CHAN_HT40MINUS;
		break;
	default:
		channel_type = NL80211_CHAN_NO_HT;
	}

<<<<<<< HEAD
	return channel_type;
}

int ieee80211_add_srates_ie(struct ieee80211_vif *vif, struct sk_buff *skb)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_supported_band *sband;
	int rate;
	u8 i, rates, *pos;

	sband = local->hw.wiphy->bands[local->hw.conf.channel->band];
	rates = sband->n_bitrates;
=======
	cfg80211_chandef_create(chandef, control_chan, channel_type);
}

int ieee80211_parse_bitrates(struct cfg80211_chan_def *chandef,
			     const struct ieee80211_supported_band *sband,
			     const u8 *srates, int srates_len, u32 *rates)
{
	u32 rate_flags = ieee80211_chandef_rate_flags(chandef);
	int shift = ieee80211_chandef_get_shift(chandef);
	struct ieee80211_rate *br;
	int brate, rate, i, j, count = 0;

	*rates = 0;

	for (i = 0; i < srates_len; i++) {
		rate = srates[i] & 0x7f;

		for (j = 0; j < sband->n_bitrates; j++) {
			br = &sband->bitrates[j];
			if ((rate_flags & br->flags) != rate_flags)
				continue;

			brate = DIV_ROUND_UP(br->bitrate, (1 << shift) * 5);
			if (brate == rate) {
				*rates |= BIT(j);
				count++;
				break;
			}
		}
	}
	return count;
}

int ieee80211_add_srates_ie(struct ieee80211_sub_if_data *sdata,
			    struct sk_buff *skb, bool need_basic,
			    enum ieee80211_band band)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_supported_band *sband;
	int rate, shift;
	u8 i, rates, *pos;
	u32 basic_rates = sdata->vif.bss_conf.basic_rates;
	u32 rate_flags;

	shift = ieee80211_vif_get_shift(&sdata->vif);
	rate_flags = ieee80211_chandef_rate_flags(&sdata->vif.bss_conf.chandef);
	sband = local->hw.wiphy->bands[band];
	rates = 0;
	for (i = 0; i < sband->n_bitrates; i++) {
		if ((rate_flags & sband->bitrates[i].flags) != rate_flags)
			continue;
		rates++;
	}
>>>>>>> refs/remotes/origin/master
	if (rates > 8)
		rates = 8;

	if (skb_tailroom(skb) < rates + 2)
		return -ENOMEM;

	pos = skb_put(skb, rates + 2);
	*pos++ = WLAN_EID_SUPP_RATES;
	*pos++ = rates;
	for (i = 0; i < rates; i++) {
<<<<<<< HEAD
		rate = sband->bitrates[i].bitrate;
		*pos++ = (u8) (rate / 5);
=======
		u8 basic = 0;
		if ((rate_flags & sband->bitrates[i].flags) != rate_flags)
			continue;

		if (need_basic && basic_rates & BIT(i))
			basic = 0x80;
		rate = sband->bitrates[i].bitrate;
		rate = DIV_ROUND_UP(sband->bitrates[i].bitrate,
				    5 * (1 << shift));
		*pos++ = basic | (u8) rate;
>>>>>>> refs/remotes/origin/master
	}

	return 0;
}

<<<<<<< HEAD
int ieee80211_add_ext_srates_ie(struct ieee80211_vif *vif, struct sk_buff *skb)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_supported_band *sband;
	int rate;
	u8 i, exrates, *pos;

	sband = local->hw.wiphy->bands[local->hw.conf.channel->band];
	exrates = sband->n_bitrates;
=======
int ieee80211_add_ext_srates_ie(struct ieee80211_sub_if_data *sdata,
				struct sk_buff *skb, bool need_basic,
				enum ieee80211_band band)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_supported_band *sband;
	int rate, shift;
	u8 i, exrates, *pos;
	u32 basic_rates = sdata->vif.bss_conf.basic_rates;
	u32 rate_flags;

	rate_flags = ieee80211_chandef_rate_flags(&sdata->vif.bss_conf.chandef);
	shift = ieee80211_vif_get_shift(&sdata->vif);

	sband = local->hw.wiphy->bands[band];
	exrates = 0;
	for (i = 0; i < sband->n_bitrates; i++) {
		if ((rate_flags & sband->bitrates[i].flags) != rate_flags)
			continue;
		exrates++;
	}

>>>>>>> refs/remotes/origin/master
	if (exrates > 8)
		exrates -= 8;
	else
		exrates = 0;

	if (skb_tailroom(skb) < exrates + 2)
		return -ENOMEM;

	if (exrates) {
		pos = skb_put(skb, exrates + 2);
		*pos++ = WLAN_EID_EXT_SUPP_RATES;
		*pos++ = exrates;
		for (i = 8; i < sband->n_bitrates; i++) {
<<<<<<< HEAD
			rate = sband->bitrates[i].bitrate;
			*pos++ = (u8) (rate / 5);
=======
			u8 basic = 0;
			if ((rate_flags & sband->bitrates[i].flags)
			    != rate_flags)
				continue;
			if (need_basic && basic_rates & BIT(i))
				basic = 0x80;
			rate = DIV_ROUND_UP(sband->bitrates[i].bitrate,
					    5 * (1 << shift));
			*pos++ = basic | (u8) rate;
>>>>>>> refs/remotes/origin/master
		}
	}
	return 0;
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======

int ieee80211_ave_rssi(struct ieee80211_vif *vif)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;

	if (WARN_ON_ONCE(sdata->vif.type != NL80211_IFTYPE_STATION)) {
		/* non-managed type inferfaces */
		return 0;
	}
	return ifmgd->ave_beacon_signal / 16;
}
EXPORT_SYMBOL_GPL(ieee80211_ave_rssi);

u8 ieee80211_mcs_to_chains(const struct ieee80211_mcs_info *mcs)
{
	if (!mcs)
		return 1;

	/* TODO: consider rx_highest */

	if (mcs->rx_mask[3])
		return 4;
	if (mcs->rx_mask[2])
		return 3;
	if (mcs->rx_mask[1])
		return 2;
	return 1;
}

/**
 * ieee80211_calculate_rx_timestamp - calculate timestamp in frame
 * @local: mac80211 hw info struct
 * @status: RX status
 * @mpdu_len: total MPDU length (including FCS)
 * @mpdu_offset: offset into MPDU to calculate timestamp at
 *
 * This function calculates the RX timestamp at the given MPDU offset, taking
 * into account what the RX timestamp was. An offset of 0 will just normalize
 * the timestamp to TSF at beginning of MPDU reception.
 */
u64 ieee80211_calculate_rx_timestamp(struct ieee80211_local *local,
				     struct ieee80211_rx_status *status,
				     unsigned int mpdu_len,
				     unsigned int mpdu_offset)
{
	u64 ts = status->mactime;
	struct rate_info ri;
	u16 rate;

	if (WARN_ON(!ieee80211_have_rx_timestamp(status)))
		return 0;

	memset(&ri, 0, sizeof(ri));

	/* Fill cfg80211 rate info */
	if (status->flag & RX_FLAG_HT) {
		ri.mcs = status->rate_idx;
		ri.flags |= RATE_INFO_FLAGS_MCS;
		if (status->flag & RX_FLAG_40MHZ)
			ri.flags |= RATE_INFO_FLAGS_40_MHZ_WIDTH;
		if (status->flag & RX_FLAG_SHORT_GI)
			ri.flags |= RATE_INFO_FLAGS_SHORT_GI;
	} else if (status->flag & RX_FLAG_VHT) {
		ri.flags |= RATE_INFO_FLAGS_VHT_MCS;
		ri.mcs = status->rate_idx;
		ri.nss = status->vht_nss;
		if (status->flag & RX_FLAG_40MHZ)
			ri.flags |= RATE_INFO_FLAGS_40_MHZ_WIDTH;
		if (status->flag & RX_FLAG_80MHZ)
			ri.flags |= RATE_INFO_FLAGS_80_MHZ_WIDTH;
		if (status->flag & RX_FLAG_80P80MHZ)
			ri.flags |= RATE_INFO_FLAGS_80P80_MHZ_WIDTH;
		if (status->flag & RX_FLAG_160MHZ)
			ri.flags |= RATE_INFO_FLAGS_160_MHZ_WIDTH;
		if (status->flag & RX_FLAG_SHORT_GI)
			ri.flags |= RATE_INFO_FLAGS_SHORT_GI;
	} else {
		struct ieee80211_supported_band *sband;
		int shift = 0;
		int bitrate;

		if (status->flag & RX_FLAG_10MHZ)
			shift = 1;
		if (status->flag & RX_FLAG_5MHZ)
			shift = 2;

		sband = local->hw.wiphy->bands[status->band];
		bitrate = sband->bitrates[status->rate_idx].bitrate;
		ri.legacy = DIV_ROUND_UP(bitrate, (1 << shift));
	}

	rate = cfg80211_calculate_bitrate(&ri);
	if (WARN_ONCE(!rate,
		      "Invalid bitrate: flags=0x%x, idx=%d, vht_nss=%d\n",
		      status->flag, status->rate_idx, status->vht_nss))
		return 0;

	/* rewind from end of MPDU */
	if (status->flag & RX_FLAG_MACTIME_END)
		ts -= mpdu_len * 8 * 10 / rate;

	ts += mpdu_offset * 8 * 10 / rate;

	return ts;
}

void ieee80211_dfs_cac_cancel(struct ieee80211_local *local)
{
	struct ieee80211_sub_if_data *sdata;

	mutex_lock(&local->iflist_mtx);
	list_for_each_entry(sdata, &local->interfaces, list) {
		cancel_delayed_work_sync(&sdata->dfs_cac_timer_work);

		if (sdata->wdev.cac_started) {
			ieee80211_vif_release_channel(sdata);
			cfg80211_cac_event(sdata->dev,
					   NL80211_RADAR_CAC_ABORTED,
					   GFP_KERNEL);
		}
	}
	mutex_unlock(&local->iflist_mtx);
}

void ieee80211_dfs_radar_detected_work(struct work_struct *work)
{
	struct ieee80211_local *local =
		container_of(work, struct ieee80211_local, radar_detected_work);
	struct cfg80211_chan_def chandef = local->hw.conf.chandef;

	ieee80211_dfs_cac_cancel(local);

	if (local->use_chanctx)
		/* currently not handled */
		WARN_ON(1);
	else
		cfg80211_radar_event(local->hw.wiphy, &chandef, GFP_KERNEL);
}

void ieee80211_radar_detected(struct ieee80211_hw *hw)
{
	struct ieee80211_local *local = hw_to_local(hw);

	trace_api_radar_detected(local);

	ieee80211_queue_work(hw, &local->radar_detected_work);
}
EXPORT_SYMBOL(ieee80211_radar_detected);

u32 ieee80211_chandef_downgrade(struct cfg80211_chan_def *c)
{
	u32 ret;
	int tmp;

	switch (c->width) {
	case NL80211_CHAN_WIDTH_20:
		c->width = NL80211_CHAN_WIDTH_20_NOHT;
		ret = IEEE80211_STA_DISABLE_HT | IEEE80211_STA_DISABLE_VHT;
		break;
	case NL80211_CHAN_WIDTH_40:
		c->width = NL80211_CHAN_WIDTH_20;
		c->center_freq1 = c->chan->center_freq;
		ret = IEEE80211_STA_DISABLE_40MHZ |
		      IEEE80211_STA_DISABLE_VHT;
		break;
	case NL80211_CHAN_WIDTH_80:
		tmp = (30 + c->chan->center_freq - c->center_freq1)/20;
		/* n_P40 */
		tmp /= 2;
		/* freq_P40 */
		c->center_freq1 = c->center_freq1 - 20 + 40 * tmp;
		c->width = NL80211_CHAN_WIDTH_40;
		ret = IEEE80211_STA_DISABLE_VHT;
		break;
	case NL80211_CHAN_WIDTH_80P80:
		c->center_freq2 = 0;
		c->width = NL80211_CHAN_WIDTH_80;
		ret = IEEE80211_STA_DISABLE_80P80MHZ |
		      IEEE80211_STA_DISABLE_160MHZ;
		break;
	case NL80211_CHAN_WIDTH_160:
		/* n_P20 */
		tmp = (70 + c->chan->center_freq - c->center_freq1)/20;
		/* n_P80 */
		tmp /= 4;
		c->center_freq1 = c->center_freq1 - 40 + 80 * tmp;
		c->width = NL80211_CHAN_WIDTH_80;
		ret = IEEE80211_STA_DISABLE_80P80MHZ |
		      IEEE80211_STA_DISABLE_160MHZ;
		break;
	default:
	case NL80211_CHAN_WIDTH_20_NOHT:
		WARN_ON_ONCE(1);
		c->width = NL80211_CHAN_WIDTH_20_NOHT;
		ret = IEEE80211_STA_DISABLE_HT | IEEE80211_STA_DISABLE_VHT;
		break;
	case NL80211_CHAN_WIDTH_5:
	case NL80211_CHAN_WIDTH_10:
		WARN_ON_ONCE(1);
		/* keep c->width */
		ret = IEEE80211_STA_DISABLE_HT | IEEE80211_STA_DISABLE_VHT;
		break;
	}

	WARN_ON_ONCE(!cfg80211_chandef_valid(c));

	return ret;
}

/*
 * Returns true if smps_mode_new is strictly more restrictive than
 * smps_mode_old.
 */
bool ieee80211_smps_is_restrictive(enum ieee80211_smps_mode smps_mode_old,
				   enum ieee80211_smps_mode smps_mode_new)
{
	if (WARN_ON_ONCE(smps_mode_old == IEEE80211_SMPS_AUTOMATIC ||
			 smps_mode_new == IEEE80211_SMPS_AUTOMATIC))
		return false;

	switch (smps_mode_old) {
	case IEEE80211_SMPS_STATIC:
		return false;
	case IEEE80211_SMPS_DYNAMIC:
		return smps_mode_new == IEEE80211_SMPS_STATIC;
	case IEEE80211_SMPS_OFF:
		return smps_mode_new != IEEE80211_SMPS_OFF;
	default:
		WARN_ON(1);
	}

	return false;
}

int ieee80211_send_action_csa(struct ieee80211_sub_if_data *sdata,
			      struct cfg80211_csa_settings *csa_settings)
{
	struct sk_buff *skb;
	struct ieee80211_mgmt *mgmt;
	struct ieee80211_local *local = sdata->local;
	int freq;
	int hdr_len = offsetof(struct ieee80211_mgmt, u.action.u.chan_switch) +
			       sizeof(mgmt->u.action.u.chan_switch);
	u8 *pos;

	if (sdata->vif.type != NL80211_IFTYPE_ADHOC &&
	    sdata->vif.type != NL80211_IFTYPE_MESH_POINT)
		return -EOPNOTSUPP;

	skb = dev_alloc_skb(local->tx_headroom + hdr_len +
			    5 + /* channel switch announcement element */
			    3 + /* secondary channel offset element */
			    8); /* mesh channel switch parameters element */
	if (!skb)
		return -ENOMEM;

	skb_reserve(skb, local->tx_headroom);
	mgmt = (struct ieee80211_mgmt *)skb_put(skb, hdr_len);
	memset(mgmt, 0, hdr_len);
	mgmt->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT |
					  IEEE80211_STYPE_ACTION);

	eth_broadcast_addr(mgmt->da);
	memcpy(mgmt->sa, sdata->vif.addr, ETH_ALEN);
	if (ieee80211_vif_is_mesh(&sdata->vif)) {
		memcpy(mgmt->bssid, sdata->vif.addr, ETH_ALEN);
	} else {
		struct ieee80211_if_ibss *ifibss = &sdata->u.ibss;
		memcpy(mgmt->bssid, ifibss->bssid, ETH_ALEN);
	}
	mgmt->u.action.category = WLAN_CATEGORY_SPECTRUM_MGMT;
	mgmt->u.action.u.chan_switch.action_code = WLAN_ACTION_SPCT_CHL_SWITCH;
	pos = skb_put(skb, 5);
	*pos++ = WLAN_EID_CHANNEL_SWITCH;			/* EID */
	*pos++ = 3;						/* IE length */
	*pos++ = csa_settings->block_tx ? 1 : 0;		/* CSA mode */
	freq = csa_settings->chandef.chan->center_freq;
	*pos++ = ieee80211_frequency_to_channel(freq);		/* channel */
	*pos++ = csa_settings->count;				/* count */

	if (csa_settings->chandef.width == NL80211_CHAN_WIDTH_40) {
		enum nl80211_channel_type ch_type;

		skb_put(skb, 3);
		*pos++ = WLAN_EID_SECONDARY_CHANNEL_OFFSET;	/* EID */
		*pos++ = 1;					/* IE length */
		ch_type = cfg80211_get_chandef_type(&csa_settings->chandef);
		if (ch_type == NL80211_CHAN_HT40PLUS)
			*pos++ = IEEE80211_HT_PARAM_CHA_SEC_ABOVE;
		else
			*pos++ = IEEE80211_HT_PARAM_CHA_SEC_BELOW;
	}

	if (ieee80211_vif_is_mesh(&sdata->vif)) {
		struct ieee80211_if_mesh *ifmsh = &sdata->u.mesh;
		__le16 pre_value;

		skb_put(skb, 8);
		*pos++ = WLAN_EID_CHAN_SWITCH_PARAM;		/* EID */
		*pos++ = 6;					/* IE length */
		*pos++ = sdata->u.mesh.mshcfg.dot11MeshTTL;	/* Mesh TTL */
		*pos = 0x00;	/* Mesh Flag: Tx Restrict, Initiator, Reason */
		*pos |= WLAN_EID_CHAN_SWITCH_PARAM_INITIATOR;
		*pos++ |= csa_settings->block_tx ?
			  WLAN_EID_CHAN_SWITCH_PARAM_TX_RESTRICT : 0x00;
		put_unaligned_le16(WLAN_REASON_MESH_CHAN, pos); /* Reason Cd */
		pos += 2;
		pre_value = cpu_to_le16(ifmsh->pre_value);
		memcpy(pos, &pre_value, 2);		/* Precedence Value */
		pos += 2;
	}

	ieee80211_tx_skb(sdata, skb);
	return 0;
}
>>>>>>> refs/remotes/origin/master
