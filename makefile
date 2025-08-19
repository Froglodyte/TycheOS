# Tools from the cross-compiler toolchain
AS = aarch64-linux-gnu-as
CC = aarch64-linux-gnu-gcc
LD = aarch64-linux-gnu-ld
OBJCOPY = aarch64-linux-gnu-objcopy

# --- MODIFIED SECTION START ---

# Define project directories using relative paths
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Find all C source files in the src directory
C_SOURCES = $(wildcard $(SRC_DIR)/*.c)
# boot.s is in the root directory
ASM_SOURCES = boot.s

# Generate object file names and place them in the build directory
C_OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
ASM_OBJS = $(patsubst %.s,$(BUILD_DIR)/%.o,$(ASM_SOURCES))
OBJECTS = $(C_OBJS) $(ASM_OBJS)

# --- MODIFIED SECTION END ---

# Compiler and Linker flags
# We add -I$(INC_DIR) to tell GCC where to find headers.
CFLAGS = -ffreestanding -O2 -nostdlib -Wall -Wextra -I$(INC_DIR)
LDFLAGS = -T linker.ld -nostdlib

# Final output files
KERNEL_ELF = $(BUILD_DIR)/kernel.elf
KERNEL_IMG = kernel8.img

# The first rule is the default goal
all: $(KERNEL_IMG)

# Rule to create the final binary image from the ELF file
$(KERNEL_IMG): $(KERNEL_ELF)
	$(OBJCOPY) -O binary $(KERNEL_ELF) $(KERNEL_IMG)

# Rule to link the object files into an ELF file
$(KERNEL_ELF): $(OBJECTS)
	@mkdir -p $(@D) # Create build directory if it doesn't exist
	$(LD) $(LDFLAGS) -o $(KERNEL_ELF) $(OBJECTS)

# --- MODIFIED COMPILATION RULES ---

# Generic rule to compile C files from src into the build directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D) # Create build directory if it doesn't exist
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to assemble boot.s into the build directory
$(BUILD_DIR)/boot.o: boot.s
	@mkdir -p $(@D)
	$(AS) $< -o $@

# --- END MODIFIED COMPILATION RULES ---

# Clean up build files
clean:
	rm -rf $(BUILD_DIR) $(KERNEL_IMG)

# Build and run in QEMU
run: all
	qemu-system-aarch64 -M raspi3b -kernel $(KERNEL_IMG) -serial stdio