void gdt_install();

void arch_initialize() {
  gdt_install();
}