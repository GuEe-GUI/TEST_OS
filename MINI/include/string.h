// include/string.h
#ifndef _STRING_H
#define _STRING_H

#include <types.h>

void* memcpy(void* dst, const void* src, uint8_t size);
void* memset(void* dst, uint32_t val, uint32_t size);
uint32_t memcmp(void* buf1,void* buf2, uint32_t size);
void* memmove(void *dest, const void *src, size_t n);

#endif
