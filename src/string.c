#include "string.h"

#include <stdint.h>

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