#ifndef _ARCH_I386_IDT_H
#define _ARCH_I386_IDT_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define GATE_TYPE_32_BIT_TASK 0x5
#define GATE_TYPE_16_BIT_INTERRUPT 0x6
#define GATE_TYPE_16_BIT_TRAP 0x7
#define GATE_TYPE_32_BIT_INTERRUPT 0xE
#define GATE_TYPE_32_BIT_TRAP 0xF

#define DPL0 0
#define DPL3 (0x3 << 5)

#define PRESENT (0x1 << 7)

#define TYPE_AND_ATTRS (GATE_TYPE_32_BIT_INTERRUPT | DPL0 | PRESENT)
#define SELECTOR 0x8

void set_idt_entry(size_t i, uint32_t base, uint16_t selector, uint8_t type_and_attrs);

void idt_install();

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _ARCH_I386_IDT_H
