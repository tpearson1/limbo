#include <stdio.h>

void arch_initialize();

// Early entry point. Called before global constructors
void kernel_early() {
  arch_initialize();
}

// The main entry point. Called after global constructors
void kernel_main() {
  printf("Hello, kernel world!\n");
}