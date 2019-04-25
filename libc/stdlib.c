#include <stdlib.h>

#include <stdio.h>

__attribute__((__noreturn__))
void abort() {
#ifdef __IS_LIBK
  // TODO: Add kernel panic
  printf("[kernel] panic: abort()\n");
#else
  // TODO: Abnormally terminate process
  printf("abort()\n");
#endif

  while (1) ;
  __builtin_unreachable();
}