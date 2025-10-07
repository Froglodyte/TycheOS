#include <stdint.h>
#ifndef STRING_H
#define STRING_H

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, int n);
void itoa(uint64_t n, char *s);
int strlen(const char *s);
char* strncpy(char *dest, const char *src, int n);
const char* strstr(const char* haystack, const char* needle);
const char* strrchr(const char* s, int c);
char* strchr(const char* s, int c);
char* strtok(char* str, const char* delim);
char* strcpy(char* dest, const char* src);

#endif
