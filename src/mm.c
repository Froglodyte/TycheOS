#include "mm.h"
#include "uart.h"

// size and number of memory blocks
#define BLOCK_SIZE 64
#define NUM_BLOCKS 1024 // 64KB of heap memory

// memory is treated like a linked list.
// each block of memory is a node
typedef struct header {
    struct header* next;
} header_t;

static header_t memory_map[NUM_BLOCKS];
static header_t* free_list_head = NULL;

void memory_init() {
    for (int i = 0; i < NUM_BLOCKS - 1; i++) {
        memory_map[i].next = &memory_map[i + 1];
    }
    memory_map[NUM_BLOCKS - 1].next = NULL;

    free_list_head = &memory_map[0];
    uart_puts("memory_init: initialized memory\r\n");
}

void* malloc(size_t size) {
    if (size > BLOCK_SIZE || size == 0) {
        // can't allocate more than a block or zero bytes
        return NULL;
    }

    if (free_list_head == NULL) {
        // no free blocks left
        uart_puts("malloc: out of memory\r\n");
        return NULL;
    }

    // get a block from the front of the free list
    header_t* block_to_allocate = free_list_head;
    
    // move the head of the list to the next free block
    free_list_head = block_to_allocate->next;

    // return a pointer to the usable memory area (after the header)
    return (void*)block_to_allocate;
}

void free(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    // cast the pointer back to our block header type
    header_t* block_to_free = (header_t*)ptr;

    // add the freed block to the front of the free list
    block_to_free->next = free_list_head;
    free_list_head = block_to_free;
}
