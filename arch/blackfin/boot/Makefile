#
# arch/blackfin/boot/Makefile
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#

<<<<<<< HEAD
<<<<<<< HEAD
MKIMAGE := $(srctree)/scripts/mkuboot.sh

targets := vmImage vmImage.bin vmImage.bz2 vmImage.gz vmImage.lzma vmImage.lzo vmImage.xip
extra-y += vmlinux.bin vmlinux.bin.gz vmlinux.bin.bz2 vmlinux.bin.lzma vmlinux.bin.lzo vmlinux.bin.xip

UIMAGE_OPTS-y :=
UIMAGE_OPTS-$(CONFIG_RAMKERNEL) += -a $(CONFIG_BOOT_LOAD)
UIMAGE_OPTS-$(CONFIG_ROMKERNEL) += -a $(CONFIG_ROM_BASE) -x

quiet_cmd_uimage = UIMAGE  $@
      cmd_uimage = $(CONFIG_SHELL) $(MKIMAGE) -A $(ARCH) -O linux -T kernel \
                   -C $(2) -n '$(CPU_REV)-$(KERNELRELEASE)' \
                   -e $(shell $(NM) vmlinux | awk '$$NF == "__start" {print $$1}') \
                   $(UIMAGE_OPTS-y) -d $< $@
=======
targets := vmImage vmImage.bin vmImage.bz2 vmImage.gz vmImage.lzma vmImage.lzo vmImage.xip
=======
targets := uImage uImage.bin uImage.bz2 uImage.gz uImage.lzma uImage.lzo uImage.xip
>>>>>>> refs/remotes/origin/master
extra-y += vmlinux.bin vmlinux.bin.gz vmlinux.bin.bz2 vmlinux.bin.lzma vmlinux.bin.lzo vmlinux.bin.xip

ifeq ($(CONFIG_RAMKERNEL),y)
UIMAGE_LOADADDR = $(CONFIG_BOOT_LOAD)
else # CONFIG_ROMKERNEL must be set
UIMAGE_LOADADDR = $(CONFIG_ROM_BASE)
endif
UIMAGE_ENTRYADDR = $(shell $(NM) vmlinux | awk '$$NF == "__start" {print $$1}')
UIMAGE_NAME = '$(CPU_REV)-$(KERNELRELEASE)'
UIMAGE_OPTS-$(CONFIG_ROMKERNEL) += -x
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

$(obj)/vmlinux.bin: vmlinux FORCE
	$(call if_changed,objcopy)

$(obj)/vmlinux.bin.gz: $(obj)/vmlinux.bin FORCE
	$(call if_changed,gzip)

$(obj)/vmlinux.bin.bz2: $(obj)/vmlinux.bin FORCE
	$(call if_changed,bzip2)

$(obj)/vmlinux.bin.lzma: $(obj)/vmlinux.bin FORCE
	$(call if_changed,lzma)

$(obj)/vmlinux.bin.lzo: $(obj)/vmlinux.bin FORCE
	$(call if_changed,lzo)

# The mkimage tool wants 64bytes prepended to the image
quiet_cmd_mk_bin_xip = BIN     $@
      cmd_mk_bin_xip = ( printf '%64s' | tr ' ' '\377' ; cat $< ) > $@
$(obj)/vmlinux.bin.xip: $(obj)/vmlinux.bin FORCE
	$(call if_changed,mk_bin_xip)

<<<<<<< HEAD
$(obj)/vmImage.bin: $(obj)/vmlinux.bin
	$(call if_changed,uimage,none)

$(obj)/vmImage.bz2: $(obj)/vmlinux.bin.bz2
	$(call if_changed,uimage,bzip2)

$(obj)/vmImage.gz: $(obj)/vmlinux.bin.gz
	$(call if_changed,uimage,gzip)

$(obj)/vmImage.lzma: $(obj)/vmlinux.bin.lzma
	$(call if_changed,uimage,lzma)

$(obj)/vmImage.lzo: $(obj)/vmlinux.bin.lzo
	$(call if_changed,uimage,lzo)

$(obj)/vmImage.xip: $(obj)/vmlinux.bin.xip
=======
$(obj)/uImage.bin: $(obj)/vmlinux.bin
	$(call if_changed,uimage,none)

$(obj)/uImage.bz2: $(obj)/vmlinux.bin.bz2
	$(call if_changed,uimage,bzip2)

$(obj)/uImage.gz: $(obj)/vmlinux.bin.gz
	$(call if_changed,uimage,gzip)

$(obj)/uImage.lzma: $(obj)/vmlinux.bin.lzma
	$(call if_changed,uimage,lzma)

$(obj)/uImage.lzo: $(obj)/vmlinux.bin.lzo
	$(call if_changed,uimage,lzo)

$(obj)/uImage.xip: $(obj)/vmlinux.bin.xip
>>>>>>> refs/remotes/origin/master
	$(call if_changed,uimage,none)

suffix-y                      := bin
suffix-$(CONFIG_KERNEL_GZIP)  := gz
suffix-$(CONFIG_KERNEL_BZIP2) := bz2
suffix-$(CONFIG_KERNEL_LZMA)  := lzma
suffix-$(CONFIG_KERNEL_LZO)   := lzo
suffix-$(CONFIG_ROMKERNEL)    := xip

<<<<<<< HEAD
$(obj)/vmImage: $(obj)/vmImage.$(suffix-y)
=======
$(obj)/uImage: $(obj)/uImage.$(suffix-y)
>>>>>>> refs/remotes/origin/master
	@ln -sf $(notdir $<) $@

install:
	sh $(srctree)/$(src)/install.sh $(KERNELRELEASE) $(BOOTIMAGE) System.map "$(INSTALL_PATH)"
