#
# Makefile for the MN10300-specific core kernel code
#
<<<<<<< HEAD
extra-y := head.o init_task.o vmlinux.lds
=======
extra-y := head.o vmlinux.lds
>>>>>>> refs/remotes/origin/master

fpu-obj-y := fpu-nofpu.o fpu-nofpu-low.o
fpu-obj-$(CONFIG_FPU) := fpu.o fpu-low.o

obj-y   := process.o signal.o entry.o traps.o irq.o \
<<<<<<< HEAD
	   ptrace.o setup.o time.o sys_mn10300.o io.o kthread.o \
	   switch_to.o mn10300_ksyms.o kernel_execve.o $(fpu-obj-y) \
=======
	   ptrace.o setup.o time.o sys_mn10300.o io.o \
	   switch_to.o mn10300_ksyms.o $(fpu-obj-y) \
>>>>>>> refs/remotes/origin/master
	   csrc-mn10300.o cevt-mn10300.o

obj-$(CONFIG_SMP) += smp.o smp-low.o

obj-$(CONFIG_MN10300_WD_TIMER) += mn10300-watchdog.o mn10300-watchdog-low.o

obj-$(CONFIG_MN10300_TTYSM) += mn10300-serial.o mn10300-serial-low.o \
			       mn10300-debug.o
obj-$(CONFIG_GDBSTUB) += gdb-stub.o gdb-low.o
obj-$(CONFIG_GDBSTUB_ON_TTYSx) += gdb-io-serial.o gdb-io-serial-low.o
obj-$(CONFIG_GDBSTUB_ON_TTYSMx) += gdb-io-ttysm.o gdb-io-ttysm-low.o

obj-$(CONFIG_MN10300_RTC) += rtc.o
obj-$(CONFIG_PROFILE) += profile.o profile-low.o
obj-$(CONFIG_MODULES) += module.o
obj-$(CONFIG_KPROBES) += kprobes.o
obj-$(CONFIG_KGDB) += kgdb.o
