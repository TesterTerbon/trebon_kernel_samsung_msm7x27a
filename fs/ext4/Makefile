#
# Makefile for the linux ext4-filesystem routines.
#

obj-$(CONFIG_EXT4_FS) += ext4.o

ext4-y	:= balloc.o bitmap.o dir.o file.o fsync.o ialloc.o inode.o page-io.o \
		ioctl.o namei.o super.o symlink.o hash.o resize.o extents.o \
		ext4_jbd2.o migrate.o mballoc.o block_validity.o move_extent.o \
<<<<<<< HEAD
<<<<<<< HEAD
		mmp.o
=======
		mmp.o indirect.o
>>>>>>> refs/remotes/origin/cm-10.0

ext4-$(CONFIG_EXT4_FS_XATTR)		+= xattr.o xattr_user.o xattr_trusted.o
=======
		mmp.o indirect.o extents_status.o xattr.o xattr_user.o \
		xattr_trusted.o inline.o

>>>>>>> refs/remotes/origin/master
ext4-$(CONFIG_EXT4_FS_POSIX_ACL)	+= acl.o
ext4-$(CONFIG_EXT4_FS_SECURITY)		+= xattr_security.o
