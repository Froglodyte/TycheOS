# The Build

-   The best way to know your way around a C/C++ codebase is by analyzing its _Makefile_ (atleast I conjecture so)
-   Whilst writing an OS, one cannot simply use the standard gcc compiler settings to compile the code
-   We gotta strip away the stdlib, the default startup files. We accomplish by using the following compiler flags
    ```shell
    CFLAGS = -ffreestanding -O2 -nostdlib -Wall -Wextra -I$(INC_DIR) -mgeneral-regs-only
    LDFLAGS = -T linker.ld -nostdlib
    ```
    -   The option `-ffreestanding` directs the compiler to not assume that standard functions have their usual definition.
    -   The option `-nostdlib` strips away the stdlibs
-   The Operating Systems we are writing is meant for AArch64. Hence we gotta use cross compilers and system emulation to run stuff
    -   AS: aarch64-linux-gnu-as
    -   CC: aarch64-linux-gnu-gcc
    -   LD: aarch64-linux-gnu-ld
    -   OBJCOPY: aarch64-linux-gnu-objcopy
    -   qemu: `qemu-system-aarch64 -M raspi3b -device loader,file=$(KERNEL_IMG) -serial stdio -display none`
        -   It is important to load the KERNEL_IMG as a -device laoder
-   The compilation works by taking all the .c/.S files from the src directory

    -   Compiling .c files:
        ```shell
        $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
        @mkdir -p $(@D)
        $(CC) $(CFLAGS) -c $< -o $@
        ```
        It picks up all the c files from the src dir and compiles them into object files which are placed in the build directory
    -   Compiling .S files:
        ```shell
        $(BUILD_DIR)/%_s.o: $(SRC_DIR)/%.S
        @mkdir -p $(@D)
        $(CC) $(CFLAGS) -c $< -o $@
        ```
        All the assembly files are first pre-processed by C processors and then compiled into object files. Note the `_s` in the output object files. This is to prevent collision between C files and assembly files with the same name!
    -   Linker
        ```shell
        $(KERNEL_ELF): $(OBJECTS)
        @mkdir -p $(@D)
        $(LD) $(LDFLAGS) $(OBJECTS) -o $@
        ```
        The linker links all the object files into a .elf file.