#
# Makefile for the Zorro bus specific drivers.
#

<<<<<<< HEAD
obj-$(CONFIG_ZORRO)	+= zorro.o zorro-driver.o zorro-sysfs.o names.o
obj-$(CONFIG_PROC_FS)	+= proc.o
=======
obj-$(CONFIG_ZORRO)	+= zorro.o zorro-driver.o zorro-sysfs.o
obj-$(CONFIG_PROC_FS)	+= proc.o
obj-$(CONFIG_ZORRO_NAMES) +=  names.o
>>>>>>> refs/remotes/origin/master

hostprogs-y 		:= gen-devlist

# Files generated that shall be removed upon make clean
clean-files := devlist.h

# Dependencies on generated files need to be listed explicitly
$(obj)/names.o: $(obj)/devlist.h

# And that's how to generate them
quiet_cmd_devlist = DEVLIST $@
      cmd_devlist = ( cd $(obj); ./gen-devlist ) < $<
$(obj)/devlist.h: $(src)/zorro.ids $(obj)/gen-devlist
	$(call cmd,devlist)

