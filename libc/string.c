#include <string.h>

#include <stdint.h>

int memcmp(const void *a, const void *b, size_t size) {
  const uint8_t *p1 = (const uint8_t *)a;
  const uint8_t *p2 = (const uint8_t *)b;
  for (size_t i = 0; i < size; i++) {
    if (p1[i] < p2[i])
      return -1;
    else if (p1[i] > p2[i])
      return 1;
  }
  return 0;
}

void *memcpy(void * restrict dst, const void * restrict src, size_t size) {
  uint8_t *d = (uint8_t *)dst;
  const uint8_t *s = (const uint8_t *)src;
  for (size_t i = 0; i < size; i++)
    d[i] = s[i];
  return dst;
}

void *memmove(void *dst, const void *src, size_t n) {
  if (n == 0 || dst == src) return dst;

  uint8_t *d = (uint8_t *)dst;
  const uint8_t *s = (const uint8_t *)src;

  // Case 1: str1 < str2, can simply copy index 0 from str2 to str1, and then
  // index 1...
  if ((uintptr_t)dst < (uintptr_t)src) {
    for (size_t i = 0; i < n; i++)
      d[i] = s[i];
    return dst;
  }

  // Case 2: str1 > str2, can copy backwards
  for (size_t i = 0; i < n; i--)
    d[n - i - 1] = s[n - i - 1];
  return dst;
}

void *memset(void *mem, int value, size_t size) {
  uint8_t *ptr = (uint8_t *)mem;
  for (size_t i = 0; i < size; i++)
    ptr[i] = (uint8_t)value;
  return ptr;
}

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len]) len++;
  return len;
}