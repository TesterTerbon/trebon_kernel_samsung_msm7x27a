#
#  Copyright 2000, 2008 MontaVista Software Inc.
#  Author: MontaVista Software, Inc. <source@mvista.com>
#
# Makefile for the Alchemy Au1xx0 CPUs, generic files.
#

obj-y += prom.o time.o clocks.o platform.o power.o setup.o \
<<<<<<< HEAD
<<<<<<< HEAD
	sleeper.o dma.o dbdma.o

obj-$(CONFIG_ALCHEMY_GPIOINT_AU1000) += irq.o

# optional gpiolib support
ifeq ($(CONFIG_ALCHEMY_GPIO_INDIRECT),)
 ifeq ($(CONFIG_GPIOLIB),y)
  obj-$(CONFIG_ALCHEMY_GPIOINT_AU1000) += gpiolib-au1000.o
 endif
endif

obj-$(CONFIG_PCI)		+= pci.o
=======
	sleeper.o dma.o dbdma.o vss.o irq.o
=======
	sleeper.o dma.o dbdma.o vss.o irq.o usb.o
>>>>>>> refs/remotes/origin/master

# optional gpiolib support
ifeq ($(CONFIG_ALCHEMY_GPIO_INDIRECT),)
 obj-$(CONFIG_GPIOLIB) += gpiolib.o
endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
