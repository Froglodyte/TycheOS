# TycheOS

A lightweight, bare-metal microkernel and operating system developed in C and AArch64 Assembly for the Raspberry Pi 3B.

TycheOS is designed to run on the emulated **Raspberry Pi 3 Model B** (utilizing a Broadcom BCM2837 SoC with ARM Cortex-A53 cores) in QEMU. It provides a clean, instructional codebase showcasing core operating systems concepts, from boot and exception routing to scheduling and custom file systems.

---

## Architectural Features

### 1. Bare-Metal Boot & Privilege Transitions
* **EL3 Entry to EL1 Execution:** Starts execution at Exception Level 3 (EL3), configures essential system registers (SCTLR, HCR, SCR, SPSR), and performs a controlled switch down to EL1 (Kernel Mode) using ARMv8 exception returns (`eret`).
* **Hardware Initialization:** Configures stacks, zeroes out the BSS segment, and initializes low-level peripherals before jumping to the C entrypoint ([main](file:///home/mudit/Desktop/TycheOS/src/main.c#L23)).
* **Implementation:** Written in [boot.S](file:///home/mudit/Desktop/TycheOS/src/boot.S).

### 2. Precise Interrupt Routing & Exception Vectors
* **ARMv8 Vector Table:** Implements a fully-aligned ($2^{11}$-byte) vector table handling synchronous exceptions, IRQs, FIQs, and system errors across different privilege states (EL1h, EL1t, EL0 64-bit, and EL0 32-bit).
* **Preemptive & Cooperative Timer Interrupts:** Integrates the Raspberry Pi System Timer interrupt to drive the scheduler's quantum tick.
* **Implementation:** Defined in [entry.S](file:///home/mudit/Desktop/TycheOS/src/entry.S) and managed in [irq.c](file:///home/mudit/Desktop/TycheOS/src/irq.c#L23) and [timer.c](file:///home/mudit/Desktop/TycheOS/src/timer.c#L12).

### 3. Multi-Level Feedback Queue (MLFQ) Scheduler
* **State Management:** Fully supports thread creation via [copy_process](file:///home/mudit/Desktop/TycheOS/src/fork.c#L9) (cloning registers and stack state).
* **MLFQ Scheduling:** Features a 3-level Multi-Level Feedback Queue scheduler ([_schedule](file:///home/mudit/Desktop/TycheOS/src/scheduler.c#L4)) supporting priority boosting, timeslice demotion (quantum sizes: 2, 4, 8 ticks), and preemption.
* **Context Switching:** Performs low-level context switches via assembly register saves (x19-x29, SP, and LR).
* **Implementation:** Managed in [scheduler.c](file:///home/mudit/Desktop/TycheOS/src/scheduler.c) and [fork.c](file:///home/mudit/Desktop/TycheOS/src/fork.c).

### 4. Custom Virtual File System (VFS) & TmpFS
* **VFS Abstraction:** Clean separation of concerns using generic `filesystem`, `vnode`, and `file` structures.
* **In-Memory File System (TmpFS):** Supports creation and navigation of directory trees, file writing, reading, and lookup resolution.
* **Implementation:** Initialized via [vfs_init](file:///home/mudit/Desktop/TycheOS/src/fs.c#L10) and implemented across [fs.c](file:///home/mudit/Desktop/TycheOS/src/fs.c) and [tmpfs.c](file:///home/mudit/Desktop/TycheOS/src/tmpfs.c).

### 5. System Call Interface (SVC)
* **Kernel Transitions:** Userspace processes request kernel services via the `SVC` instruction.
* **Syscall Table:** Implements syscall routing for critical operations, currently supporting:
  * `sys_write` (Console writing)
  * `sys_malloc` (Dynamic memory page allocation)
  * `sys_clone` (Process cloning)
  * `sys_exit` (Process termination stub)
* **Implementation:** Table defined in [sys.c](file:///home/mudit/Desktop/TycheOS/src/sys.c) and handled in [entry.S](file:///home/mudit/Desktop/TycheOS/src/entry.S#L171).

### 6. Interactive UART-Driven Shell
* **Interactive CLI:** Runs an interactive shell ([shell](file:///home/mudit/Desktop/TycheOS/src/shell.c#L138)) over the bare-metal PL011 UART driver.
* **Text Editing & Execution:** Implements standard input buffering, character echoing, backspace handling, and command dispatching.
* **Built-in Commands:**
  * `help` - Show instructions
  * `clear` - Clear terminal screen
  * `echo [msg]` - Echo text back to console
  * `ls` - List folder contents
  * `pwd` - Print working directory
  * `mkdir [name]` - Create subdirectory
  * `touch [name]` - Create blank file
  * `cat [name]` - Print file content
  * `cd [path]` - Navigate directories
* **Implementation:** Driven by [shell.c](file:///home/mudit/Desktop/TycheOS/src/shell.c) and [uart.c](file:///home/mudit/Desktop/TycheOS/src/uart.c).

---

## Getting Started

### Prerequisites

You need a cross-compiler toolchain targeting `aarch64` and the `qemu-system-aarch64` emulator.

#### On Ubuntu / Debian:
```bash
sudo apt update
sudo apt install gcc-aarch64-linux-gnu qemu-system-arm make
```

#### On macOS (using Homebrew):
```bash
brew tap mac-ports/arm-none-eabi
brew install aarch64-elf-gcc qemu
```
*(Note: If utilizing a non-Linux target cross-compiler on macOS, update the compiler commands in the [makefile](file:///home/mudit/Desktop/TycheOS/makefile) to match your toolchain prefixes, e.g., `aarch64-elf-gcc`).*

### Compilation & Running

1. **Clean previous builds:**
   ```bash
   make clean
   ```

2. **Compile the kernel image:**
   ```bash
   make
   ```
   This generates the raw binary image `build/kernel8.img` and ELF debugging symbol file `build/kernel.elf`.

3. **Launch the OS in QEMU:**
   ```bash
   make run
   ```
   This executes the kernel on a simulated Raspberry Pi 3 Model B, establishing a direct interactive terminal connection via stdout/stdin.

