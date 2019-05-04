#include <idt.h>
#include <string.h>

// Packed struct which contains the IDT entry data in the format required by an
// x86 CPU.
struct __attribute__((__packed__)) idt_entry {
  uint16_t base_low;
  uint16_t selector;
  uint8_t zero; // Unused, must be 0
  /* Type and attributes byte:
   * - Bits 0-3: gate type. See gate type definitions below.
   * - Bit 4: storage segment. Should be 0 for interrupt and trap gates.
   * - Bits 5-6: descriptor privilege level.
   * - Bit 7: present bit. Is 0 for unused interrupts.
   */
  uint8_t type_and_attrs;
  uint16_t base_high;
};

struct __attribute__((__packed__)) idt_ptr {
  uint16_t limit;
  uint32_t base;
};

struct idt_entry idt[256];
struct idt_ptr idt_ptr;

void idt_load();

void set_idt_entry(size_t i, uint32_t base, uint16_t selector, uint8_t type_and_attrs) {
  idt[i].base_low = base & 0xFFFF;
  idt[i].base_high = base >> 16;
  idt[i].selector = selector;
  idt[i].zero = 0;
  idt[i].type_and_attrs = type_and_attrs;
}

void idt_install() {
  // Limit should be one less than the size of the idt
  idt_ptr.limit = sizeof(struct idt_entry) * 256 - 1;
  idt_ptr.base = (uint32_t)&idt;

  // Fill with null entries, which can be set later
  memset(&idt, 0, sizeof(struct idt_entry) * 256);

  // Load the idt
  idt_load();
}
