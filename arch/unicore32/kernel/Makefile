#
# Makefile for the linux kernel.
#

# Object file lists.
obj-y				:= dma.o elf.o entry.o process.o ptrace.o
obj-y				+= setup.o signal.o sys.o stacktrace.o traps.o

obj-$(CONFIG_MODULES)		+= ksyms.o module.o
obj-$(CONFIG_EARLY_PRINTK)	+= early_printk.o

<<<<<<< HEAD
obj-$(CONFIG_CPU_FREQ)		+= cpu-ucv2.o
=======
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_UNICORE_FPU_F64)	+= fpu-ucf64.o

# obj-y for architecture PKUnity v3
obj-$(CONFIG_ARCH_PUV3)		+= clock.o irq.o time.o

obj-$(CONFIG_PUV3_GPIO)		+= gpio.o
<<<<<<< HEAD
obj-$(CONFIG_PUV3_PWM)		+= pwm.o
=======
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_PUV3_PM)		+= pm.o sleep.o
obj-$(CONFIG_HIBERNATION)	+= hibernate.o hibernate_asm.o

obj-$(CONFIG_PCI)		+= pci.o

# obj-y for specific machines
obj-$(CONFIG_ARCH_PUV3)		+= puv3-core.o
obj-$(CONFIG_PUV3_NB0916)	+= puv3-nb0916.o

head-y				:= head.o
obj-$(CONFIG_DEBUG_LL)		+= debug.o

<<<<<<< HEAD
extra-y				:= $(head-y) init_task.o vmlinux.lds
=======
extra-y				:= $(head-y) vmlinux.lds
>>>>>>> refs/remotes/origin/master
