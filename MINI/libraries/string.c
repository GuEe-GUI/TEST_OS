// libraries/string.c
#include <string.h>

void* memcpy(void* dst, const void* src, uint8_t size) {
    char *d;
    const char *s;

    if (dst == NULL || src == NULL) return NULL;

    if ((char*)dst > ((char*)src + sizeof(src)) || ((char*)dst < (char*)src)) {
        d = (char*)dst;
        s = (char*)src;
        while (size--) *d++ = *s++;
    } else {
        d = ((char*)dst + size - 1);
        s = ((char*)src + size -1);
        while (size --) *d-- = *s--;
    }

    return dst;
}

void* memset(void* dst, uint32_t val, uint32_t size) {
    for (; 0 < size; size--) {
        *(char*)dst = val;
        dst++;
    }

    return dst;
}

uint32_t memcmp(void* buf1,void* buf2, uint32_t size) {
    while (size --> 0) {
        if (*(uint32_t*)buf1++ != *(uint32_t*)buf2++) {
            return 0;
        }
    }
    return 1;
}

void* memmove(void *dst, const void *src, size_t n) {
    char *tmp;
    const char *s;

    if (dst <= src) {
        tmp = dst;
        s = src;
        while (n--)
            *tmp++ = *s++;
    } else {
        tmp = dst;
        tmp += n;
        s = src;
        s += n;
        while (n--)
            *--tmp = *--s;
    }
    return dst;
}
