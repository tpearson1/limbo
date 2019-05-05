.set IRQ_OFFSET, 32 # Equal to macro of same name in irq.c

.macro irq_stub num, int
.global irq\num
.type irq\num, @function
irq\num:
  cli
  push $0 # Pushed to match format of registers structure (dummy error code)
  push $\int # Push the interrupt number
  jmp irq_common_stub
.endm

irq_stub 0, IRQ_OFFSET
irq_stub 1, IRQ_OFFSET + 1
irq_stub 2, IRQ_OFFSET + 2
irq_stub 3, IRQ_OFFSET + 3
irq_stub 4, IRQ_OFFSET + 4
irq_stub 5, IRQ_OFFSET + 5
irq_stub 6, IRQ_OFFSET + 6
irq_stub 7, IRQ_OFFSET + 7
irq_stub 8, IRQ_OFFSET + 8
irq_stub 9, IRQ_OFFSET + 9
irq_stub 10, IRQ_OFFSET + 10
irq_stub 11, IRQ_OFFSET + 11
irq_stub 12, IRQ_OFFSET + 12
irq_stub 13, IRQ_OFFSET + 13
irq_stub 14, IRQ_OFFSET + 14
irq_stub 15, IRQ_OFFSET + 15

irq_common_stub:
  pusha # Push edi, esi, ebp, esp, ebx, edx, ecx, and eax

  # Push onto stack to match registers structure, and so that we can revert back
  # to them once we've handled the irq
  push %ds
  push %es
  push %fs
  push %gs

  # Loads kernel data segment descriptors (ring 0)
  mov $0x10, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs

  call irq_handler

  # Reload original data segment descriptors
  pop %gs
  pop %fs
  pop %es
  pop %ds

  popa # Undo pusha
  add $8, %esp # Undo the two pushes made in each irq

  sti
  iret