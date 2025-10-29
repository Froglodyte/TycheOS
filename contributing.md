# Contributing to the Kernel Project

Thank you for your interest in contributing to the **Kernel** project!  
This document explains how to set up your environment, build the kernel, and follow our development standards.

---

## 🧭 Table of Contents

1. [Getting Started](#getting-started)
2. [Setting Up the Environment (Windows + WSL)](#setting-up-the-environment-windows--wsl)
3. [Building the Kernel](#building-the-kernel)
4. [Commit Message Convention](#commit-message-convention)
5. [Code Style](#code-style)
6. [Pull Request Workflow](#pull-request-workflow)
7. [Getting Help](#getting-help)

---

## 🚀 Getting Started

Our kernel targets the **AArch64 (ARM64)** architecture.  
You’ll need a cross-compiler toolchain to build it.  
We recommend building inside **WSL (Windows Subsystem for Linux)** to ensure compatibility with GNU tools.

---

## 🧰 Setting Up the Environment (Windows + WSL)

### Step 1: Install WSL

Open PowerShell as Administrator and run:

```powershell
wsl --install -d Ubuntu
```

Once installed, open Ubuntu via Start Menu or by typing `wsl` in PowerShell.

---

### Step 2: Install Required Tools

Inside your WSL terminal, install the cross-compiler toolchain and dependencies:

```bash
sudo apt update
sudo apt install build-essential gcc-aarch64-linux-gnu make qemu-system
```

- `gcc-aarch64-linux-gnu` — ARM64 cross-compiler  
- `make` — for build automation  
- `qemu-system` — to emulate and run the kernel

---

### Step 3: Clone the Repository

```bash
git clone https://github.com/<your-org>/<kernel-repo>.git
cd <kernel-repo>
```

---

## 🏗️ Building the Kernel

After setup, you can build and run the kernel directly:

```bash
make run
```

This will:
- Compile the kernel for **AArch64** using the cross-compiler.  
- Create a bootable image.  
- Launch the image in **QEMU** for testing.

To clean the build:

```bash
make clean
```

---

## ✏️ Commit Message Convention

We use a **structured commit message format** inspired by [Conventional Commits](https://www.conventionalcommits.org/):

```
type: short description
```

### Allowed Types

- `feat:` — new feature or addition  
- `fix:` — bug fix or patch  
- `style:` — formatting, code style, or non-functional change  

### Examples

```
feat: add context switch mechanism
fix: resolve page fault in memory allocator
style: reformat scheduler.c to match clang style
```

> Keep commit messages concise (under 80 chars) and in **present tense**.

---

## 🧱 Code Style

We use **4 spaces per indent** and **LLVM-based formatting** through the VS Code C/C++ extension.

Add this to your `.vscode/settings.json`:

```json
{
    // Enable C/C++ extension as default formatter
    "[cpp]": {
        "editor.defaultFormatter": "ms-vscode.cpptools"
    },
    "[c]": {
        "editor.defaultFormatter": "ms-vscode.cpptools"
    },
    "[arm]": {
        "editor.defaultFormatter": "Mikhail-Arkhipov.armassemblyeditor"
    },

    // ms-vscode uses LLVM formatter.
    "C_Cpp.clang_format_style": "{ BasedOnStyle: Google, AllowShortIfStatementsOnASingleLine: Never, AllowShortLoopsOnASingleLine: false, BreakBeforeBraces: Attach, IndentWidth: 4, PointerAlignment: Left, UseTab: Never, ColumnLimit: 100 }"
}
```

### General Guidelines

- Use **4 spaces**, never tabs.  
- Keep lines under **100 characters**.  
- Place braces on the same line as control statements (`if`, `while`, `for`, etc.).  
- Keep functions short and focused.  
- Add comments where hardware-level behavior or tricky logic appears.

---

## 🔁 Pull Request Workflow

1. **Fork** the repository on GitHub.  
2. **Create a branch** for your change:
   ```bash
   git checkout -b feat/my-feature
   ```
3. **Commit** your work following the commit convention.  
4. **Push** your branch:
   ```bash
   git push origin feat/my-feature
   ```
5. **Open a Pull Request (PR)** to the main repository.

Each PR should:
- Build successfully (`make run` passes)  
- Follow code and commit style  
- Include a brief explanation of the
bka-nfug-xao