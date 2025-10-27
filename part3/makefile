# Target platform
ARCH := i686
TARGET := $(ARCH)-elf
 
# Compilers
CC := $(TARGET)-gcc
AS := $(TARGET)-as
 
# Compiler flags
# -I./include tells GCC to look in the 'include' folder for headers
CFLAGS := -std=gnu99 -ffreestanding -O2 -Wall -Wextra -m32 -I./include
ASFLAGS := -f elf32
 
# Find all C and Assembly files
C_SOURCES := $(shell find . -name '*.c')
S_SOURCES := $(shell find . -name '*.S')
OBJS := $(C_SOURCES:.c=.o) $(S_SOURCES:.S=.o)
 
# Kernel binary
KERNEL := build/kernel.bin
ISO_FILE := build/my_os.iso
 
# ISO structure
ISO_DIR := build/iso
ISO_BOOT_DIR := $(ISO_DIR)/boot
ISO_GRUB_DIR := $(ISO_BOOT_DIR)/grub
 
.PHONY: all clean run iso

# Default target
all: $(KERNEL)

# Create the .iso
iso: $(KERNEL) grub.cfg
	@mkdir -p $(ISO_GRUB_DIR)
	@cp $(KERNEL) $(ISO_BOOT_DIR)/
	@cp grub.cfg $(ISO_GRUB_DIR)/
	@grub-mkrescue -o $(ISO_FILE) $(ISO_DIR)
	@echo "========================================"
	@echo " Bootable ISO created at $(ISO_FILE) "
	@echo "========================================"

# Link the kernel
$(KERNEL): $(OBJS) linker.ld
	@mkdir -p build/
	$(CC) -T linker.ld -o $(KERNEL) $(OBJS) -ffreestanding -O2 -nostdlib -lgcc -m32

# Compile C files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble Assembly files
%.o: %.S
	$(AS) $(ASFLAGS) $< -o $@

# Clean build files
clean:
	rm -rf build/ $(OBJS)