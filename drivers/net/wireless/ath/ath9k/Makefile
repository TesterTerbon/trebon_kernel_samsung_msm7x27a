ath9k-y +=	beacon.o \
		gpio.o \
		init.o \
		main.o \
		recv.o \
<<<<<<< HEAD
<<<<<<< HEAD
		xmit.o \

ath9k-$(CONFIG_ATH9K_RATE_CONTROL) += rc.o
ath9k-$(CONFIG_ATH9K_PCI) += pci.o
ath9k-$(CONFIG_ATH9K_AHB) += ahb.o
ath9k-$(CONFIG_ATH9K_DEBUGFS) += debug.o
=======
		xmit.o
=======
		xmit.o \
		link.o \
		antenna.o
>>>>>>> refs/remotes/origin/master

ath9k-$(CONFIG_ATH9K_BTCOEX_SUPPORT) += mci.o
ath9k-$(CONFIG_ATH9K_LEGACY_RATE_CONTROL) += rc.o
ath9k-$(CONFIG_ATH9K_PCI) += pci.o
ath9k-$(CONFIG_ATH9K_AHB) += ahb.o
ath9k-$(CONFIG_ATH9K_DEBUGFS) += debug.o
ath9k-$(CONFIG_ATH9K_DFS_DEBUGFS) += dfs_debug.o
<<<<<<< HEAD
ath9k-$(CONFIG_ATH9K_DFS_CERTIFIED) += dfs.o
>>>>>>> refs/remotes/origin/cm-10.0
=======
ath9k-$(CONFIG_ATH9K_DFS_CERTIFIED) += \
		dfs.o
ath9k-$(CONFIG_PM_SLEEP) += wow.o
>>>>>>> refs/remotes/origin/master

obj-$(CONFIG_ATH9K) += ath9k.o

ath9k_hw-y:=	\
		ar9002_hw.o \
		ar9003_hw.o \
		hw.o \
		ar9003_phy.o \
		ar9002_phy.o \
		ar5008_phy.o \
		ar9002_calib.o \
		ar9003_calib.o \
<<<<<<< HEAD
<<<<<<< HEAD
=======
		ar9003_rtt.o \
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ar9003_rtt.o \
>>>>>>> refs/remotes/origin/master
		calib.o \
		eeprom.o \
		eeprom_def.o \
		eeprom_4k.o \
		eeprom_9287.o \
		ani.o \
<<<<<<< HEAD
<<<<<<< HEAD
		btcoex.o \
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		mac.o \
		ar9002_mac.o \
		ar9003_mac.o \
		ar9003_eeprom.o \
		ar9003_paprd.o

<<<<<<< HEAD
<<<<<<< HEAD
=======
ath9k_hw-$(CONFIG_ATH9K_BTCOEX_SUPPORT) += btcoex.o \
					   ar9003_mci.o
>>>>>>> refs/remotes/origin/cm-10.0
=======
ath9k_hw-$(CONFIG_ATH9K_BTCOEX_SUPPORT) += btcoex.o \
					   ar9003_mci.o
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_ATH9K_HW) += ath9k_hw.o

obj-$(CONFIG_ATH9K_COMMON) += ath9k_common.o
ath9k_common-y:=	common.o

ath9k_htc-y +=	htc_hst.o \
		hif_usb.o \
		wmi.o \
		htc_drv_txrx.o \
		htc_drv_main.o \
		htc_drv_beacon.o \
		htc_drv_init.o \
		htc_drv_gpio.o

ath9k_htc-$(CONFIG_ATH9K_HTC_DEBUGFS) += htc_drv_debug.o

obj-$(CONFIG_ATH9K_HTC) += ath9k_htc.o
