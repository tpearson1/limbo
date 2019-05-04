#ifndef _ARCH_I386_REGISTERS_H
#define _ARCH_I386_REGISTERS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct __attribute__((__packed__)) registers {
  uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // From pusha
	uint32_t int_no, err_code;
	uint32_t eip, cs, eflags, user_esp, ss;
};

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _ARCH_I386_REGISTERS_H
