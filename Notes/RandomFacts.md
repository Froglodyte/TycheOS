# A collection of random, irrevalant yet very useful facts

-   Understanding architectures vs microarchitectures: Architecture as defined by the ARM docs, is the **functional specification**
    of the processor. It is a contract that binds the hardware and software together. It includes things like ISA, registers provided, exception model etc. Microarchitecture tells how the architecture actually works. (EE engineers are more concerned with this stuff)

-   The specs of this project: RPI 3B that uses the BCM2837 SoC containing a ARM Cortex-A53 64 bit quad core (every core only has one thread). A53 is optimized for better power performance and does not do stuff like Out-of-Order execution. It supports ARMv8-A architecture which is of type AArch64 (64 bits)

-   (A/R/M) profiles of ARM architecture
    All the profiles, basically variants of the core common ARM architecture, are designed keeping in mind their respective use-cases

    -   A profile: Feature rich applications
    -   R profile: Real Time (networking equipment)
    -   M profile: Small highly power-efficient devices

-   x86 is the 32 bit ISA Intel had introduced. The ISA was cross-licensed to AMD (obviously involved a bloody legal battle). AMD went on to pioneer 64 bit extension of that architecture, calling it `AMD64`. Intel later supported this 64 bit version. This 64-bit architecture is now commonly referred to as `x86-64` or `x64`

-   Bus: It is the component that aids in transfer of data in the CPU. It has 3 lines: Control, Address and Data lines

    -   There are 2 kinds of BUSES:

        -   Internal Bus: Handles transfer of data within the processor
        -   System Bus: Handles the transfer of data between CPU, memory, IO-devices. (Remember Nikhil sir's Von-Neumann Architecture diagram). They support the connection of **peripherals**

    -   Back in the day memory and IO devices were wired to CPU directly without there being a system bus. This forced stuff like DMA. Now with system buses like PCI which support the concept of bus arbitration (choosing the BUS master), DMA is sort of encoded into our structure now.

    -   PCI is a popular system BUS. USB controllers are attached to the PCI bus to allow for USB connections. Similary for SCSI bus! (If you remember Nikhil sir's docs)

    -   SoC generally use a AMBA bus protocol (maintained by ARM) which is better suited by SoCs

    -   Note: PCI is a protocol too maintained by PCI-SIG.

-   WTF is SoC, MCU?

    -   SoC: System on Chip in _crude_ words means that your whole motherboard (recall the _black computer case_ that resides besides your monitor) is magically fit onto a single handheld chip! The downside is the lack of ability to choose parts (you won't be able to do a PC build so as to say)
    -   MCU: Microcontrollers are basically less powerful than SoC. SoC contain a CPU, ROM, GPU, RAM, Wifi, Bluetooth, USB, GPIO etc. It is an entire _system_. MCUs are smaller and ain't as elaborative as SoCs
    -   ARM specializes in building AArch (ARM Architecture) based SoCs. It is a fabless company i.e it provides design IPs for their architectures (possibly in something like verilog) that must be taken to a fab like TSMC.

-   What does it mean for the architecure to be 64 bits?
    It generally means that the architecture supports registers that are capable of storing 64 bits at once. Generally also defines the Word Size which guides alignment of data in the memory.
    -   Does this mean the data bus is 64 bit? It does, but CPU architects do some extremely fancy stuff under-the-hood so making such a statement won't be perfectly precise. Again EE engineer's concern, ain't mine!
    -   What about the System bus? It makes sense for the system bus to have the ability to give 64 bits at once. Not sure if this is a requirement though. EE engineering is outright dark arts!
