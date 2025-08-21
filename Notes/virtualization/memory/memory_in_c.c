#include <stdio.h>
#include <stdlib.h>

int main()
{
    int c = 10;
    /**
     * Malloc definition:
     * void *malloc(size_t size);
     * size_t unsigned long long int on 64-bit systems
     * The max possible word size in some sense supported by the ISA
     */
    // ALLOCATING MEMORY
    int *x = (int *)malloc(10 * sizeof(int));
    printf("%d", sizeof(x)); // prints 8, not 40. Think how sizeof works!
    char *s = "Hello World";
    char *str = (char *)malloc(strlen(s) + 1); // be sure for the +1

    // DE-ALLOCATING MEMORY (filled with problems) (garbage collector does this for you in higher level languages)
    //  Just cause it worked once, doesn't mean it would work again, these bugs require exhaustive testing

    /**
     * Common Errors:  (code examples below) (lead to UB behaviour, program crash being one of them)
     * 1) Forgetting to allocate memory (segfault)
     * 2) Not Allocating Enough Memory (buffer overflow)
     * 3) Forgetting to Initialize Allocated Memory (uninitialized read)
     * 4) Forgetting to Free memory (memory leak) (be in the habit of explicitly deallocating memory)
     * 5) Freeing memory before you are done with it (danling pointer)
     * 6) Freeing Memory repeatedly (double free)
     * 7) Calling free() incorrectly (just pass a pointer dude)
     * Extra Note on point 4): Short running processes don't need require you to explicitly free memory, OS will do when the process exits. However it is not the wisest thing todo
     * form good habits, EXPLICITLY FREE YOUR MEMORY)
     *
     */
    // Additional methods: calloc(intializes stuff to zero, helps with), realloc(take original memory, gives a new bigger chunk according to the size you provided, and copies memory)

    return 0;
}