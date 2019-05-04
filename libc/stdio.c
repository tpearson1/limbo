#include <stdio.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef __IS_LIBK
#include <kernel/tty.h>
#endif

// System call for write. Passing in a size of zero will use the null terminator
// to determine length.
#define SIZE_UNSPECIFIED 0
static int write(const char *str, size_t size) {
#ifdef __IS_LIBK
  if (size == SIZE_UNSPECIFIED)
    tty_writestring(str);
  else tty_write(str, size);
  return 0;
#else
  // TODO: Implement the write system call
  return EOF;
#endif
}

// Returns number of characters written
static int putint(int n) {
  if (n == 0) {
    putchar('0');
    return 1;
  }

  bool neg = false;
  if (n < 0) {
    putchar('-');
    n = -n;
    neg = true;
  }

  char temp[16]; // Do not make the buffer smaller than this
  size_t digits = 0;
  while (n != 0) {
    temp[digits] = n % 10 + '0';
    n /= 10;
    digits += 1;
  }

  for (size_t i = 0; i < digits; i++) putchar(temp[digits - i - 1]);
  return neg ? digits + 1 : digits;
}

// Returns number of characters written
static int puthex(uint32_t n) {
  char temp[8] = {'0', '0', '0', '0', '0','0', '0', '0'};
  size_t i;
  for (i = 7; n != 0; i--, n >>= 4) {
    int four_bits = n & 15;
    temp[i] = four_bits < 10 ? four_bits + '0' : four_bits - 10 + 'A';
  }

  i = 0;
  write("0x", SIZE_UNSPECIFIED);
  while (i < 8) putchar(temp[i++]);
  return 10;
}

int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;
	size_t amount;
	bool rejected_bad_specifier = false;

	while (*format) {
		if (*format != '%') {
		print_chars:
			amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			write(format, amount);
			format += amount;
			written += amount;
			continue;
		}

		// We are at a '%'
		const char* format_begun_at = format;
		if (*(++format) == '%')
			goto print_chars; // Print '%' and following text
		else if (*format == 'c') {
			format++;
			putchar((char)va_arg(parameters, int)); // Print single character
      written += 1;
		}
		else if (*format == 's') {
			format++;
			const char* s = va_arg(parameters, const char*);
      size_t len = strlen(s);
			write(s, len);
      written += len;
		}
		else if (*format == 'd') {
			format++;
			written += putint(va_arg(parameters, int));
		}
		else if (*format == 'x') {
			format++;
			written += puthex(va_arg(parameters, uint32_t));
		}
		else
			goto incomprehensible_conversion;

		if (rejected_bad_specifier) {
		incomprehensible_conversion:
			rejected_bad_specifier = true;
			format = format_begun_at;
			goto print_chars;
		}
	}

	va_end(parameters);
	return written;
}

int putchar(int c) {
  return write((const char *)(&c), 1);
}

int puts(const char *str) {
  int r1 = write(str, SIZE_UNSPECIFIED);
  int r2 = putchar('\n');
  return r1 || r2;
}