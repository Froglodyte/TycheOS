#include "string.h"

#include <stdint.h>

void* memcpy(void* dest, const void* src, unsigned int n) {
    char* d = dest;
    const char* s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

void* memset(void* s, int c, unsigned int n) {
    char* p = s;
    while (n--) {
        *p++ = (char)c;
    }
    return s;
}

// string compare standard
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

// string compare but for n characters
int strncmp(const char *s1, const char *s2, int n) {
    while (n-- > 0) {
        if (*s1 != *s2) {
            return *(unsigned char *)s1 - *(unsigned char *)s2;
        }
        if (*s1 == '\0') {
            break;
        }
        s1++;
        s2++;
    }
    return 0;
}

// int to ascii, gotta reverse the string at the end
void itoa(uint64_t n, char *s) {
    int i = 0;
    do {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);
    s[i] = '\0';

    // reverse
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = s[j];
        s[j] = s[k];
        s[k] = temp;
    }
}

int strlen(const char *s) {
    int len = 0;
    while (s[len]) {
        len++;
    }
    return len;
}

char* strncpy(char *dest, const char *src, int n) {
    int i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for ( ; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

char* strcpy(char* dest, const char* src) {
    char* saved_dest = dest;
    while ((*dest++ = *src++));
    return saved_dest;
}

char* strcat(char* dest, const char* src) {
    char* ptr = dest + strlen(dest);
    while (*src != '\0') {
        *ptr++ = *src++;
    }
    *ptr = '\0';
    return dest;
}

char* strtok(char* str, const char* delim) {
    static char* last;
    if (str)
        last = str;
    if (!last || *last == '\0')
        return 0;
    char* token = last;
    while (*last != '\0') {
        const char* d = delim;
        while (*d != '\0') {
            if (*last == *d++) {
                *last++ = '\0';
                return token;
            }
        }
        last++;
    }
    return token;
}

const char* strrchr(const char* s, int c) {
    const char* last = 0;
    while (*s) {
        if (*s == c) {
            last = s;
        }
        s++;
    }
    return last;
}

char* strchr(const char* s, int c) {
    while (*s != (char)c) {
        if (!*s++) {
            return 0;
        }
    }
    return (char*)s;
}