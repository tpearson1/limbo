#include <kernel/tty.h>
#include <idt.h>
#include <irq.h>
#include <stdio.h>

void gdt_install();
void isrs_install();

void arch_initialize() {
  gdt_install();
  idt_install();
  isrs_install();
  irq_install();
  asm volatile ("sti");
  tty_initialize();
}