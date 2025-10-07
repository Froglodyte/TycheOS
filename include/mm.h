#ifndef MM
#define MM
#include "peripherals/base.h"

/**
 * So our kernel loads at 0th position in the RAM
 * Then the kernel reserves 4 KBs for itself
 * This forms the low memory
 * PBASE forms the high memory cause MMIO stuff
 */

#define PAGE_SHIFT 12
#define TABLE_SHIFT 9
#define SECTION_SHIFT (PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE (1 << PAGE_SHIFT)  // 4kbs
#define SECTION_SIZE (1 << SECTION_SHIFT)

#define LOW_MEMORY (2 * SECTION_SIZE)  // 4mb kernel image + stack?
#define HIGH_MEMORY PBASE              // last 1mb goes to MMIO

#define PAGING_MEMORY (HIGH_MEMORY - LOW_MEMORY)
#define PAGING_PAGES (PAGING_MEMORY / PAGE_SIZE)

#ifndef __ASSEMBLER__
#include <stddef.h>

unsigned long get_free_page();
void free_page(unsigned long p);
void memzero(unsigned long src, unsigned long n);

#endif

#endif
