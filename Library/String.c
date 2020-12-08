/*
    File:       String.c

    Contains:   String funcition for std c

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#include <lib/string.h>

size_t strlen(const char *str){
    const char* p = str;
    while (*p++);

    return (p - str - 1);
}

char *strcpy(char *dest, const char *src){
    char* r = dest;
    while (*dest++ = *src++);

    return r;
}

void *memcpy(void *dest, const void *src, size_t n){
    char *d;
    const char *s;

    if (dest == NULL || src == NULL) return NULL;

    if ((char*)dest > ((char*)src + sizeof(src)) || ((char*)dest < (char*)src)) {
        d = (char*)dest;
        s = (char*)src;
        while (n--) *d++ = *s++;
    } else {
        d = ((char*)dest + n - 1);
        s = ((char*)src + n -1);
        while (n --) *d-- = *s--;
    }

    return dest;
}

void *memset(void *str, int c, size_t n){
    const unsigned char dst_tmp = c;
    char *dst = (char*)str;
    for (; 0 < n; n--) {
        *dst = dst_tmp;
        dst++;
    }

    return str;
}

void *memmove(void *dest, const void *src, size_t n) {
    char *tmp;
    const char *s;

    if (dest <= src) {
        tmp = dest;
        s = src;
        while (n--)
            *tmp++ = *s++;
    } else {
        tmp = dest;
        tmp += n;
        s = src;
        s += n;
        while (n--)
            *--tmp = *--s;
    }
    return dest;
}
