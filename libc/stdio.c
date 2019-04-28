#include <stdio.h>

#include <stdarg.h>
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

int printf(const char * restrict format, ...) {
  va_list parameters;
  va_start(parameters, format);

  // TODO: Proper printf implementation
  size_t len = strlen(format);
  int result = write(format, len);

  va_end(parameters);

  if (result == EOF) return EOF;
  return len;
}

int putchar(int c) {
  return write((const char *)(&c), 1);
}

int puts(const char *str) {
  return write(str, SIZE_UNSPECIFIED) && putchar('\n');
}