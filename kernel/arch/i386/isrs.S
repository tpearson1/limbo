.macro isr_no_error_code num
.global isr\num
.type isr\num, @function
isr\num:
  cli
  push $0 # This isr does not provide error code, so push dummy 0 error code
  push $\num # Push the interrupt number
  jmp isr_common_stub
.endm

.macro isr_error_code num
.global isr\num
.type isr\num, @function
isr\num:
  cli
  push $\num # Push the interrupt number
  jmp isr_common_stub
.endm

isr_no_error_code 0
isr_no_error_code 1
isr_no_error_code 2
isr_no_error_code 3
isr_no_error_code 4
isr_no_error_code 5
isr_no_error_code 6
isr_no_error_code 7
isr_error_code 8
isr_no_error_code 9
isr_error_code 10
isr_error_code 11
isr_error_code 12
isr_error_code 13
isr_error_code 14
isr_no_error_code 15
isr_no_error_code 16
isr_no_error_code 17
isr_no_error_code 18
isr_no_error_code 19
isr_no_error_code 20
isr_no_error_code 21
isr_no_error_code 22
isr_no_error_code 23
isr_no_error_code 24
isr_no_error_code 25
isr_no_error_code 26
isr_no_error_code 27
isr_no_error_code 28
isr_no_error_code 29
isr_no_error_code 30
isr_no_error_code 31

isr_common_stub:
  pusha # Push edi, esi, ebp, esp, ebx, edx, ecx, and eax

  # Push onto stack to match registers structure, and so that we can revert back
  # to them once we've handled the isr
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

  call isr_handler

  # Reload original data segment descriptors
  pop %gs
  pop %fs
  pop %es
  pop %ds

  popa # Undo pusha
  add $8, %esp # Undo the two pushes made in each isr

  sti
  iret