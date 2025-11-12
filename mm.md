# Memory Management

## Overview

We're using a Fixed-Size Block Allocator for our dynamic memory management. Its a simple and easy way to manage the memory pool/heap. This is good enough for an early stage kernel, although we'll need to use more advanced systems if the kernel grows.

> The allocator is built on three fundamental concepts: a static memory pool, fixed-size blocks, and a free list to track available memory.

### 1. The Heap (Memory Pool)
A large, contiguous block of memory is reserved at compile time in the .bss section of the kernel. This serves as the heap from which all dynamic allocations are made.

    memory_map: A static array that represents the entire memory pool.

    NUM_BLOCKS: Defines how many blocks the heap is divided into.

### 2. Fixed-Size Blocks
The heap is partitioned into a predefined number of equally sized blocks. Regardless of whether a request is for 1 byte or 60 bytes, the allocator will always reserve one full 64-byte block.

### 3. The Free List
The core of the allocator is a singly linked list that chains together all currently available (free) memory blocks.

    free_list_head: A pointer that always points to the first available block in the free list.

    Instead of using extra memory for list pointers, the pointer to the next free block is stored directly inside the free block itself. This is achieved by overlaying a header_t struct on the block's memory.

## Advantages

    Speed: Both allocation and deallocation are O(1) operations.

    Simplicity: The implementation is straightforward and easy to debug.

    No External Fragmentation: Because all blocks are the same size, the heap can never become fragmented into small, unusable pieces.

## Disadvantages

    Internal Fragmentation: A ton of memory can still be wasted if many small allocations are made. For example, allocating 8 bytes still consumes a 64-byte block, wasting 56 bytes.

    Inflexibility: The system cannot handle requests for memory larger than BLOCK_SIZE.