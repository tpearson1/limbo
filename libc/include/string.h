#ifndef _STRING_H
#define _STRING_H

#include <sys/cdefs.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int memcmp(const void *, const void *, size_t size);
void *memcpy(void * __restrict dst, const void * __restrict src, size_t size);
void *memmove(void *dst, const void *src, size_t size);
void *memset(void *mem, int value, size_t size);
size_t strlen(const char *);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _STRING_H
