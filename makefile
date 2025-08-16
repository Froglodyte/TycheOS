# Tools from the cross-compiler toolchain
AS = aarch64-linux-gnu-as
CC = aarch64-linux-gnu-gcc
LD = aarch64-linux-gnu-ld
OBJCOPY = aarch64-linux-gnu-objcopy

# Compiler and Linker flags
# -ffreestanding: We're not using a standard library
# -nostdlib: Don't link against a standard library
CFLAGS = -ffreestanding -O2 -nostdlib -Wall -Wextra
LDFLAGS = -T linker.ld -nostdlib

# Final output files
KERNEL_ELF = kernel.elf
KERNEL_IMG = kernel8.img

# All object files
OBJECTS = boot.o kernel.o

all: $(KERNEL_IMG)

# Rule to create the final binary image from the ELF file
$(KERNEL_IMG): $(KERNEL_ELF)
	$(OBJCOPY) -O binary $(KERNEL_ELF) $(KERNEL_IMG)

# Rule to link the object files into an ELF file
$(KERNEL_ELF): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $(KERNEL_ELF) $(OBJECTS)

# Rule to compile C code
kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

# Rule to assemble assembly code
boot.o: boot.s
	$(AS) boot.s -o boot.o

# Clean up build files
clean:
	rm -f *.o $(KERNEL_ELF) $(KERNEL_IMG)

# Build and run in QEMU
run: all
	qemu-system-aarch64 -M raspi3b -kernel $(KERNEL_IMG) -serial stdio