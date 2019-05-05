#include <irq.h>

#include <idt.h>
#include <io.h>

// Master PIC
#define PIC1_CMD 0x20
#define PIC1_DATA 0x21
// Slave PIC
#define PIC2_CMD 0xA0
#define PIC2_DATA 0xA1

#define ICW1_INIT 0x10
#define ICW1_ICW4_NEEDED 0x1 // Fourth initilization word not needed

#define ICW4_8086_MODE 0x01 // 8086/88 (MCS-80/85) mode

#define PIC_EOI 0x20 // End of interrupt

// Offsetting IRQs from PIC, preventing overlap
#define IRQ_OFFSET 32

// The CPU has two PICs, which will currently send interrupts that overlap with
// the first 32 ISRs already reserved. Thus we remap the IRQs to interrupts 32
// to 47
void pic_remap() {
  // Start initializing PIC, specify we are providing a fourth initialization
  // word
  outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4_NEEDED);
  io_wait();
  outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4_NEEDED);
  io_wait();

  // Master PIC will use interrupts 32-39
  outb(PIC1_DATA, IRQ_OFFSET);
  io_wait();
  // Slave PIC will use interrupts 40-47
  outb(PIC2_DATA, IRQ_OFFSET + 8);
  io_wait();

  // Tells the master PIC there is a slave PIC at IRQ2
  outb(PIC1_DATA, 4);
  io_wait();
  // Tells the slave PIC its cascade identity
  outb(PIC2_DATA, 2);
  io_wait();

  // Operate in use 8086 mode
  outb(PIC1_DATA, ICW4_8086_MODE);
  io_wait();
  outb(PIC2_DATA, ICW4_8086_MODE);
  io_wait();
}

void irq0();
void irq1();
void irq2();
void irq3();
void irq4();
void irq5();
void irq6();
void irq7();
void irq8();
void irq9();
void irq10();
void irq11();
void irq12();
void irq13();
void irq14();
void irq15();

void irq_install() {
  pic_remap();

  set_idt_entry(IRQ_OFFSET, (uint32_t)irq0, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(IRQ_OFFSET + 1, (uint32_t)irq1, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(IRQ_OFFSET + 2, (uint32_t)irq2, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(IRQ_OFFSET + 3, (uint32_t)irq3, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(IRQ_OFFSET + 4, (uint32_t)irq4, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(IRQ_OFFSET + 5, (uint32_t)irq5, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(IRQ_OFFSET + 6, (uint32_t)irq6, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(IRQ_OFFSET + 7, (uint32_t)irq7, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(IRQ_OFFSET + 8, (uint32_t)irq8, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(IRQ_OFFSET + 9, (uint32_t)irq9, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(IRQ_OFFSET + 10, (uint32_t)irq10, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(IRQ_OFFSET + 11, (uint32_t)irq11, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(IRQ_OFFSET + 12, (uint32_t)irq12, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(IRQ_OFFSET + 13, (uint32_t)irq13, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(IRQ_OFFSET + 14, (uint32_t)irq14, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(IRQ_OFFSET + 15, (uint32_t)irq15, SELECTOR, TYPE_AND_ATTRS);
}

void *irq_routines[16] = {
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

void irq_install_handler(size_t i, void (*handler)(struct registers)) {
  if (i > 15) return;
  irq_routines[i] = handler;
}

void irq_uninstall_handler(size_t i) {
  irq_install_handler(i, NULL);
}

void irq_handler(struct registers r) {
  uint32_t irq_no = r.int_no - IRQ_OFFSET;
  if (irq_no > 15) return;

  // Send end of interrupt signals to the master (and slave when necessary) PICs
  if (irq_no > 7) outb(PIC2_CMD, PIC_EOI);
  outb(PIC1_CMD, PIC_EOI);

  // Call the handler if present
  void (*handler)(struct registers r) = irq_routines[irq_no];
  if (handler) handler(r);
}