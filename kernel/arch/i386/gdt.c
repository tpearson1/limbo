#include <stddef.h>
#include <stdint.h>

// Packed struct which contains the GDT entry data in the format required by an
// x86 CPU.
struct __attribute__((__packed__)) gdt_entry {
  // The lowest 16 bits of the limit value
  uint16_t limit_low;
  // The lowest 16 bits of the base value
  uint16_t base_low;
  // The middle 8 bits of the base value
  uint8_t base_middle;
  /* Access byte:
   * - Bit 0: accessed bit. Set to 0; the CPU will set it to 1 when the segment
   *   is accessed.
   * - Bit 1: RW bit. For code selectors, specifies whether read access to the
   *   segment is allowed. For data selectors, specifies whether write access to
   *   the segment is allowed. Code segments can never be written to, and it is
   *   not possible to restrict read access to data segments.
   * - Bit 2: direction/conforming bit. For data selectors, determines the
   *   selector direction (0 if the segment grows up, 1 if it grows down). For
   *   code selectors, 0 specifies that the code in the segment can only be
   *   executed from the ring specified in bits 5-6, whereas if 1, specifies
   *   that code in the segment can be executed from an equal or lower
   *   privilege level.
   * - Bit 3: executable bit. Is 1 if code in the segment can be executed (code
   *   selector), 0 otherwise (data selector).
   * - Bit 4: descriptor type. Should be 1 for code and data segments, 0 for
   *   system segments.
   * - Bits 5-6: privilege bits. Specifies the ring level for the segment.
   * - Bit 7: present bit. Is 1 for all valid selectors.
   */
  uint8_t access;
  /* Bits 0-3 (lowest 4 bits) are the higher 4 bits of the limit value.
   * Bits 4-7 store flags:
   * - Bit 4: always 0.
   * - Bit 5: zero on x86. Different for x86_64.
   * - Bit 6: size bit. A value of 0 means the selector is for 16 bit protected
   *   mode, while 1 specifies 32 bit protected mode.
   * - Bit 7: granularity bit. If 0 the limit value is in 1-byte blocks, and if
   *   1 the limit value is in 4 KiB blocks.
   */
  uint8_t limit_and_flags;
  // The higher 8 bits of the base value
  uint8_t base_high;
};

struct __attribute__((__packed__)) gdt_ptr {
  uint16_t limit;
  uint32_t base;
};

struct gdt_entry gdt[5];
struct gdt_ptr gdt_ptr;

extern void gdt_load();

#define ACCESS_CODE_READ (0x1 << 1)
#define ACCESS_DATA_WRITE (0x1 << 1)
#define ACCESS_DIRECTION_CONFORMING (0x1 << 2)
#define ACCESS_EXECUTABLE (0x1 << 3)
#define ACCESS_DESCRIPTOR_TYPE_CODE_DATA (0x1 << 4)
#define ACCESS_DESCRIPTOR_TYPE_SYSTEM 0x0
#define ACCESS_DESCRIPTOR_PRIVILEGE(n) (n << 5)
#define ACCESS_PRESENT (0x1 << 7)

#define FLAGS_SIZE_16_BIT_PROTECTED 0x0
#define FLAGS_SIZE_32_BIT_PROTECTED (0x1 << 6)
#define FLAGS_GRANULARITY_1_BYTE 0x0
#define FLAGS_GRANULARITY_4_KIB (0x1 << 7)

static void set_gdt_entry(size_t i, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
  gdt[i].limit_low = limit & 0xFFFF;
  gdt[i].limit_and_flags = (limit >> 16) & 0xF;

  gdt[i].base_low = base & 0xFFFF;
  gdt[i].base_middle = (base >> 16) & 0xFF;
  gdt[i].base_high = base >> 24;

  gdt[i].access = access;
  gdt[i].limit_and_flags |= flags & 0xF0;
}

#define ACCESS_CODE_FLAGS ( \
  ACCESS_PRESENT | ACCESS_CODE_READ | \
  ACCESS_EXECUTABLE | ACCESS_DESCRIPTOR_TYPE_CODE_DATA)
#define ACCESS_CODE_FLAGS_PL0 (ACCESS_CODE_FLAGS | ACCESS_DESCRIPTOR_PRIVILEGE(0))
#define ACCESS_CODE_FLAGS_PL3 (ACCESS_CODE_FLAGS | ACCESS_DESCRIPTOR_PRIVILEGE(3))

#define ACCESS_DATA_FLAGS ( \
  ACCESS_PRESENT | ACCESS_DATA_WRITE | ACCESS_DESCRIPTOR_TYPE_CODE_DATA)
#define ACCESS_DATA_FLAGS_PL0 (ACCESS_DATA_FLAGS | ACCESS_DESCRIPTOR_PRIVILEGE(0))
#define ACCESS_DATA_FLAGS_PL3 (ACCESS_DATA_FLAGS | ACCESS_DESCRIPTOR_PRIVILEGE(3))

#define GDT_FLAGS (FLAGS_SIZE_32_BIT_PROTECTED | FLAGS_GRANULARITY_4_KIB)

void gdt_install() {
  // Limit should be one less than the size of the gdt
  gdt_ptr.limit = sizeof(struct gdt_entry) * 5 - 1;
  gdt_ptr.base = (uint32_t)&gdt;

  // Dummy NULL descriptor.
  set_gdt_entry(0, 0, 0, 0, 0);

  // Code segment, ring 0
  set_gdt_entry(1, 0, 0xFFFFF, ACCESS_CODE_FLAGS_PL0, GDT_FLAGS);
  // Data segment, ring 0
  set_gdt_entry(2, 0, 0xFFFFF, ACCESS_DATA_FLAGS_PL0, GDT_FLAGS);
  // Code segment, ring 3
  set_gdt_entry(3, 0, 0xFFFFF, ACCESS_CODE_FLAGS_PL3, GDT_FLAGS);
  // Data segment, ring 3
  set_gdt_entry(4, 0, 0xFFFFF, ACCESS_DATA_FLAGS_PL3, GDT_FLAGS);

  // Load the gdt and update the segment registers
  gdt_load();
}