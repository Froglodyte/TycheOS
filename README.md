# TycheOS

A lightweight kernel and OS developed in C

To use, download the cross compiler and QEMU version for your device, update the makefile with your system-specific resources, and use (Do WSL if you are on Windows)

```
make
```

to generate the object files, then

```
make run
```

to boot up the kernel in QEMU
