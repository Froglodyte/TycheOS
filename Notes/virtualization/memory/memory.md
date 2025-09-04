# Memory ramblings

Three main goals:

-   Transparency
-   Safety
-   Efficiency

Keywords:

-   Multiprogramming: The ability of a computer to run more than one task simultaneously by optimizing CPU and IO load times
    efficiently
-   Time-sharing: Giving everybody a fair share of CPU time, aims at less starvation time for processe

Approaches:

-   Static Reallocation: Software (loader) decides the addresses. Not the most safe or efficient
-   Dynamic Reallocation: runtime mapping of virtual address to physical address. Pretty safe and transparent but has massive drawbacks in efficiency. Fixed chunk of memory is allocated which requires that much portion to be free. Free memory (holes) can be created which need to be maintained in a data-list
