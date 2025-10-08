#include "mm.h"

static unsigned short mem_map[PAGING_PAGES] = {0};

unsigned long get_free_page() {
    for (int i = 0; i < PAGING_PAGES; i++) {
        if (mem_map[i] == 0) {
            mem_map[i] = 1;
            return LOW_MEMORY + i * PAGE_SIZE;
        }
    }
}

// size and number of memory blocks
#define BLOCK_SIZE 64
#define NUM_BLOCKS 1024  // 64KB of heap memory

// memory is treated like a linked list.
// each block of memory is a node
typedef struct header {
    struct header *next;
} header_t;

static header_t memory_map[NUM_BLOCKS];
static header_t *free_list_head = NULL;

#define KERNEL_HEAP_SIZE (64 * 1024)
static char kernel_heap[KERNEL_HEAP_SIZE];
static char* heap_ptr = kernel_heap;


void memory_init() {
    for (int i = 0; i < NUM_BLOCKS - 1; i++) {
        memory_map[i].next = &memory_map[i + 1];
    }
}

void free_page(unsigned long p) {
    int i = (p - LOW_MEMORY) / PAGE_SIZE;
    mem_map[i] = 0;
}

void* kmalloc(size_t size) {
    // Align the size to a multiple of 8
    size = (size + 7) & ~7;

    if (heap_ptr + size > kernel_heap + KERNEL_HEAP_SIZE) {
        return NULL; // Out of memory
    }

    void* ptr = heap_ptr;
    heap_ptr += size;
    return ptr;
}

void kfree(void* ptr) {
    // With a bump allocator, we can't easily free individual blocks.
    // A simple implementation is to do nothing.
    // A more complex one would be to reset the heap_ptr, but that
    // would invalidate all previous allocations.
    (void)ptr; // Avoid unused parameter warning
}