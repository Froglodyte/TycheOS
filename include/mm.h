#ifndef MM_H
#define MM_H
#include <stddef.h>

void memory_init();
void* malloc(size_t size);
void free(void* ptr);

#endif
