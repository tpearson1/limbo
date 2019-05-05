#ifndef _ARCH_I386_IRQ_H
#define _ARCH_I386_IRQ_H

#include <stddef.h>

#include "registers.h"

#ifdef __cplusplus
extern "C" {
#endif

void irq_install();

void irq_install_handler(size_t i, void (*handler)(struct registers));
void irq_uninstall_handler(size_t i);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _ARCH_I386_IRQ_H
