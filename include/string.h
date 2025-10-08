#include <stdint.h>
#ifndef _STRING_H
#define _STRING_H

void* memcpy(void* dest, const void* src, unsigned int n);
void* memset(void* s, int c, unsigned int n);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, int n);
int strlen(const char* s);
const char* strrchr(const char* s, int c);
int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, int n);
char* strcat(char* dest, const char* src);

#endif

