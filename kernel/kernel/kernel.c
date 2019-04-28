#include <stdio.h>

#include <kernel/tty.h>

// The C kernel entry point. Called after global constructors
void kernel_main() {
  tty_initialize();
  printf("Hello, kernel world!\n");
}