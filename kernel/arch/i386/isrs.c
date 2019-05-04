#include <idt.h>
#include <stdio.h>
#include <stdlib.h>

#include <registers.h>

void isr0();
void isr1();
void isr2();
void isr3();
void isr4();
void isr5();
void isr6();
void isr7();
void isr8();
void isr9();
void isr10();
void isr11();
void isr12();
void isr13();
void isr14();
void isr15();
void isr16();
void isr17();
void isr18();
void isr19();
void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();

void isrs_install() {
  set_idt_entry(0, (uint32_t)isr0, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(1, (uint32_t)isr1, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(2, (uint32_t)isr2, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(3, (uint32_t)isr3, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(4, (uint32_t)isr4, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(5, (uint32_t)isr5, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(6, (uint32_t)isr6, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(7, (uint32_t)isr7, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(8, (uint32_t)isr8, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(9, (uint32_t)isr9, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(10, (uint32_t)isr10, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(11, (uint32_t)isr11, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(12, (uint32_t)isr12, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(13, (uint32_t)isr13, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(14, (uint32_t)isr14, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(15, (uint32_t)isr15, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(16, (uint32_t)isr16, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(17, (uint32_t)isr17, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(18, (uint32_t)isr18, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(19, (uint32_t)isr19, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(20, (uint32_t)isr20, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(21, (uint32_t)isr21, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(22, (uint32_t)isr22, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(23, (uint32_t)isr23, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(24, (uint32_t)isr24, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(25, (uint32_t)isr25, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(26, (uint32_t)isr26, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(27, (uint32_t)isr27, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(28, (uint32_t)isr28, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(29, (uint32_t)isr29, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(30, (uint32_t)isr30, SELECTOR, TYPE_AND_ATTRS);
  set_idt_entry(31, (uint32_t)isr31, SELECTOR, TYPE_AND_ATTRS);
}

static char *isr_messages[] = {
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt Exception",
	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

void isr_handler(struct registers r) {
  puts("");
  puts("");
  const char *isr_message = r.int_no > 31 ? "?" : isr_messages[r.int_no];
  printf("[ISR] number: %d (%s), error code: %d\n", r.int_no, isr_message, r.err_code);

  puts("Registers contents:");
  printf(" DS:  %x ES:  %x FS:  %x  GS: %x\n"
         " EDI: %x ESI: %x EBP: %x\n"
         " EAX: %x EBX: %x ECX: %x EDX: %x\n"
         " EIP: %x\n"
         " CS:  %x SS:  %x\n"
         " ESP: %x USER ESP: %x\n"
         " EFLAGS: %x\n\n",
         r.ds, r.es, r.fs, r.gs,
         r.edi, r.esi, r.ebp,
         r.eax, r.ebx, r.ecx, r.edx,
         r.eip,
         r.cs, r.ss,
         r.esp, r.user_esp,
         r.eflags);

  puts("System halted.");
  abort();
}