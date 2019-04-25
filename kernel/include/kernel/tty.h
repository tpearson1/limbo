#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void terminal_initialize();
void terminal_putchar(char c);
void terminal_write(const char *data, size_t size);
void terminal_writestring(const char *data);
void terminal_setcolor(uint8_t color);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _KERNEL_TTY_H
