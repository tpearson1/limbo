#ifndef _ARCH_I386_IO_H
#define _ARCH_I386_IO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline uint8_t inb(uint16_t port) {
  uint8_t rv;
  __asm__ __volatile__("inb %1, %0" : "=a" (rv) : "dN" (port));
  return rv;
}

static inline void outb(uint16_t port, uint8_t data) {
  __asm__ __volatile__("outb %1, %0" : : "dN" (port), "a" (data));
}

static inline uint16_t inw(uint16_t port) {
  uint16_t rv;
  __asm__ __volatile__("inw %1, %0" : "=a" (rv) : "dN" (port));
  return rv;
}

static inline void outw(uint16_t port, uint16_t data) {
  __asm__ __volatile__("outw %1, %0" : : "dN" (port), "a" (data));
}

static inline uint32_t inl(uint16_t port) {
  uint32_t rv;
  __asm__ __volatile__("inl %1, %0" : "=a" (rv) : "dN" (port));
  return rv;
}

static inline void outl(uint16_t port, uint32_t data) {
  __asm__ __volatile__("outl %1, %0" : : "dN" (port), "a" (data));
}


#ifdef __cplusplus
} // extern "C"
#endif

#endif // _ARCH_I386_IO_H
