#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include <vga.h>
#include <io.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t * const terminal_buffer = (uint16_t *)0xB8000;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;

void terminal_update_cursor() {
  uint16_t temp = terminal_row * VGA_WIDTH + terminal_column;
  outb(0x3D4, 14);
  outb(0x3D5, temp >> 8);
  outb(0x3D4, 15);
  outb(0x3D5, temp);
}

void terminal_put_entry_at(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(c, color);
  terminal_update_cursor();
}

void terminal_clear(uint8_t color) {
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++)
      terminal_put_entry_at(' ', color, x, y);
  }

  terminal_row = 0;
  terminal_column = 0;
  terminal_update_cursor();
}

void terminal_initialize() {
  terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  terminal_clear(terminal_color);
}

void terminal_setcolor(uint8_t color) {
  terminal_color = color;
}

void terminal_scroll_up() {
  terminal_row -= 1;
  terminal_update_cursor();

  // Move the terminal memory, starting at the second line, to the beginning
  // of the buffer.
  memmove(
    terminal_buffer,
    terminal_buffer + VGA_WIDTH,
    VGA_WIDTH * (VGA_HEIGHT - 1) * sizeof(uint16_t));

  // 'Clear' the last line, using the current terminal color
  for (size_t i = 0; i < VGA_WIDTH; i++)
    terminal_put_entry_at(' ', terminal_color, i, VGA_HEIGHT - 1);
}

void terminal_put_newline() {
  // Fill the remaining characters on the line with the current background color
  for (size_t i = terminal_column; i < VGA_WIDTH; i++)
    terminal_put_entry_at(' ', terminal_color, i, terminal_row);

  terminal_column = 0;
  if (++terminal_row == VGA_HEIGHT)
    terminal_scroll_up();
  terminal_update_cursor();
}

void terminal_putchar(char c) {
  if (c == '\n') {
    terminal_put_newline();
    return;
  }

  if (c == '\r') {
    terminal_column = 0;
    terminal_update_cursor();
    return;
  }

  terminal_put_entry_at(c, terminal_color, terminal_column, terminal_row);
  if (++terminal_column == VGA_WIDTH) {
    terminal_column = 0;

    if (++terminal_row == VGA_HEIGHT)
      terminal_scroll_up();

    terminal_update_cursor();
  }
}

void terminal_write(const char *data, size_t size) {
  for (size_t i = 0; i < size; i++)
    terminal_putchar(data[i]);
  terminal_update_cursor();
}

void terminal_writestring(const char *data) {
  for (size_t i = 0; data[i] != '\0'; i++)
    terminal_putchar(data[i]);
  terminal_update_cursor();
}