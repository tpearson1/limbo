#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

bool tty_get_cursor_enabled();
void tty_disable_cursor();

/*
 * Makes the cursor visible. The values start and end specify how the cursor
 * appears. The start value is the first vertical scanline where the cursor
 * appears (>= 0). The end value is the last scanline where the cursor appears
 * (typically has a maximum of 15).
 */
void tty_enable_cursor(uint8_t start, uint8_t end);

size_t tty_get_cursor_x();
size_t tty_get_cursor_y();
void tty_set_cursor_location(size_t x, size_t y);

uint8_t tty_get_color();
void tty_set_color(uint8_t color);

void tty_put_entry_at(char c, uint8_t color, size_t x, size_t y);
void tty_clear(uint8_t color);

void tty_initialize();
void tty_putchar(char c);
void tty_write(const char *data, size_t size);
void tty_writestring(const char *data);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _KERNEL_TTY_H
