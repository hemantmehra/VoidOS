# Makefile for building and running initramfs

CC = gcc
CFLAGS = -Os
TARGET = echo_init
INITRAMFS = initramfs.cpio.gz
ROOTFS_DIR = rootfs
KERNEL = linux-6.14.4/arch/x86/boot/bzImage

QEMU = qemu-system-x86_64
QEMU_FLAGS = -kernel $(KERNEL) -initrd $(INITRAMFS) -nographic -append "console=ttyS0"

all: $(INITRAMFS)

$(TARGET): echo_init.c
	$(CC) $(CFLAGS) -o $(TARGET) echo_init.c

$(ROOTFS_DIR)/init: $(TARGET)
	mkdir -p $(ROOTFS_DIR)/dev
	cp $(TARGET) $(ROOTFS_DIR)/init
	sudo mknod -m 600 $(ROOTFS_DIR)/dev/console c 5 1 || true
	$(MAKE) copy_libs

copy_libs:
	@echo "Copying shared library dependencies..."
	mkdir -p $(ROOTFS_DIR)/lib $(ROOTFS_DIR)/lib64
	ldd $(TARGET) | awk '{ if ($$3 ~ /^\//) print $$3; else if ($$1 ~ /^\//) print $$1 }' | while read lib; do \
		dest=$(ROOTFS_DIR)`dirname $$lib`; \
		mkdir -p $$dest; \
		cp -u $$lib $$dest/; \
	done


$(INITRAMFS): $(ROOTFS_DIR)/init
	cd $(ROOTFS_DIR) && find . | cpio -o -H newc | gzip > ../$(INITRAMFS)

run: all
	$(QEMU) $(QEMU_FLAGS)

clean:
	rm -f $(TARGET) $(INITRAMFS)
	sudo rm -rf $(ROOTFS_DIR)

.PHONY: all run clean copy_libs
