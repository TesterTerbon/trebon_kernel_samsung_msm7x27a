<<<<<<< HEAD
obj-y					:= clock.o highbank.o system.o
obj-$(CONFIG_DEBUG_HIGHBANK_UART)	+= lluart.o
obj-$(CONFIG_SMP)			+= platsmp.o
obj-$(CONFIG_HOTPLUG_CPU)		+= hotplug.o
=======
obj-y					:= highbank.o system.o smc.o

plus_sec := $(call as-instr,.arch_extension sec,+sec)
AFLAGS_smc.o				:=-Wa,-march=armv7-a$(plus_sec)

>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_PM_SLEEP)			+= pm.o
