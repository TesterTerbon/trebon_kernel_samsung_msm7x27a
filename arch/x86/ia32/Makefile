#
# Makefile for the ia32 kernel emulation subsystem.
#

obj-$(CONFIG_IA32_EMULATION) := ia32entry.o sys_ia32.o ia32_signal.o
<<<<<<< HEAD
<<<<<<< HEAD
=======
obj-$(CONFIG_IA32_EMULATION) += nosyscall.o syscall_ia32.o
>>>>>>> refs/remotes/origin/cm-10.0

sysv-$(CONFIG_SYSVIPC) := ipc32.o
obj-$(CONFIG_IA32_EMULATION) += $(sysv-y)
=======
obj-$(CONFIG_IA32_EMULATION) += nosyscall.o syscall_ia32.o
>>>>>>> refs/remotes/origin/master

obj-$(CONFIG_IA32_AOUT) += ia32_aout.o

audit-class-$(CONFIG_AUDIT) := audit.o
obj-$(CONFIG_IA32_EMULATION) += $(audit-class-y)
