#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include <vga.h>
#include <io.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t * const tty_buffer = (uint16_t *)0xB8000;

static size_t tty_row;
static size_t tty_column;
static uint8_t tty_color;
static bool cursor_enabled = true;

bool tty_get_cursor_enabled() {
  return cursor_enabled;
}

void tty_disable_cursor() {
  outb(0x3D4, 0x0A);
  outb(0x3D5, 0x20);
  cursor_enabled = false;
}

void tty_enable_cursor(uint8_t start, uint8_t end) {
  if (start > end) return;

  outb(0x3D4, 0x0A);
  outb(0x3D5, (inb(0x3D5) & 0xC0) | start);
  outb(0x3D4, 0x0B);
  outb(0x3D5, (inb(0x3D5) & 0xE0) | end);
  cursor_enabled = true;
}

static void update_cursor() {
  if (!cursor_enabled) return;

  uint16_t temp = tty_row * VGA_WIDTH + tty_column;
  outb(0x3D4, 14);
  outb(0x3D5, temp >> 8);
  outb(0x3D4, 15);
  outb(0x3D5, temp);
}

size_t tty_get_cursor_x() {
  return tty_column;
}

size_t tty_get_cursor_y() {
  return tty_row;
}

void tty_set_cursor_location(size_t x, size_t y) {
  tty_column = x;
  tty_row = y;
  update_cursor();
}

uint8_t tty_get_color() {
  return tty_color;
}

void tty_set_color(uint8_t color) {
  tty_color = color;
}

void tty_put_entry_at(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  tty_buffer[index] = vga_entry(c, color);
}

void tty_clear(uint8_t color) {
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++)
      tty_put_entry_at(' ', color, x, y);
  }

  tty_set_cursor_location(0, 0);
}

void tty_initialize() {
  tty_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  tty_clear(tty_color);

  tty_enable_cursor(15, 15);
}

static void scroll_up() {
  tty_row -= 1;
  update_cursor();

  // Move the terminal memory, starting at the second line, to the beginning
  // of the buffer.
  memmove(
    tty_buffer,
    tty_buffer + VGA_WIDTH,
    VGA_WIDTH * (VGA_HEIGHT - 1) * sizeof(uint16_t));

  // 'Clear' the last line, using the current terminal color
  for (size_t i = 0; i < VGA_WIDTH; i++)
    tty_put_entry_at(' ', tty_color, i, VGA_HEIGHT - 1);
}

static void put_newline() {
  // Fill the remaining characters on the line with the current background color
  for (size_t i = tty_column; i < VGA_WIDTH; i++)
    tty_put_entry_at(' ', tty_color, i, tty_row);

  tty_column = 0;
  if (++tty_row == VGA_HEIGHT)
    scroll_up();
  update_cursor();
}

void tty_putchar(char c) {
  if (c == '\n') {
    put_newline();
    return;
  }

  if (c == '\r') {
    tty_column = 0;
    update_cursor();
    return;
  }

  tty_put_entry_at(c, tty_color, tty_column, tty_row);
  if (++tty_column == VGA_WIDTH) {
    tty_column = 0;

    if (++tty_row == VGA_HEIGHT)
      scroll_up();
  }

  update_cursor();
}

void tty_write(const char *data, size_t size) {
  for (size_t i = 0; i < size; i++)
    tty_putchar(data[i]);
}

void tty_writestring(const char *data) {
  for (size_t i = 0; data[i] != '\0'; i++)
    tty_putchar(data[i]);
}