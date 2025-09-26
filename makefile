# Tools from the cross-compiler toolchain ("sudo apt install gcc-aarch64-linux-gnu")
AS = aarch64-linux-gnu-as
CC = aarch64-linux-gnu-gcc
LD = aarch64-linux-gnu-ld
OBJCOPY = aarch64-linux-gnu-objcopy


# Define project directories using relative paths
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Object files
C_SOURCES = $(wildcard $(SRC_DIR)/*.c)
ASM_SOURCES = $(wildcard $(SRC_DIR)/*.S)
C_OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
ASM_OBJS = $(patsubst $(SRC_DIR)/%.S,$(BUILD_DIR)/%_s.o,$(ASM_SOURCES))
OBJECTS = $(C_OBJS) $(ASM_OBJS)

# Compiler and Linker flags
CFLAGS = -ffreestanding -O2 -nostdlib -Wall -Wextra -I$(INC_DIR) -mgeneral-regs-only
LDFLAGS = -T linker.ld -nostdlib

# Final output files
KERNEL_ELF = $(BUILD_DIR)/kernel.elf
KERNEL_IMG = $(BUILD_DIR)/kernel8.img

# The first rule is the default goal
all: $(KERNEL_IMG)

# Rule to create the final binary image from the ELF file
$(KERNEL_IMG): $(KERNEL_ELF)
	$(OBJCOPY) -O binary $(KERNEL_ELF) $(KERNEL_IMG)

# Rule to link the object files into an ELF file
$(KERNEL_ELF): $(OBJECTS)
	@mkdir -p $(@D) 
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@ 

# Generic rule to compile C files from src into the build directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D) 
	$(CC) $(CFLAGS) -c $< -o $@ 

# Rule to assemble boot.s into the build directory
$(BUILD_DIR)/%_s.o: $(SRC_DIR)/%.S
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ 


# Clean up build files
clean:
	rm -rf $(BUILD_DIR) $(KERNEL_IMG)

# Build and run in QEMU (apt install qemu-system)
run: all
	qemu-system-aarch64 -M raspi3b -kernel $(KERNEL_IMG) -serial stdio