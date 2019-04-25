# Declare constants for the multiboot header.
.set ALIGN, 1<<0 # Align loaded modules on page boundaries
.set MEMINFO, 1<<1 # Provide memory map
.set FLAGS, ALIGN | MEMINFO # Multiboot 'flag' field
.set MAGIC, 0x1BADB002 # Lets the bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # Checksum for above, proves we are multiboot

/*
Declare a multiboot header that marks the program as a kernel. These are magic
values that are documented in the multiboot standard. The bootloader will
search for this signature in the first 8 KiB of the kernel file, aligned at a
32bit boundary. The signature is in its own section so the header can be forced
to be within the first 8 KiB of the kernel file.
*/
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/*
The multiboot standard does not define the value of the stack poiner register
(esp) and so the kernel needs to provide a stack. The stack grows
downwards on x86. The stack is in its own section so it can be marked nobits,
which means the kernel file is smaller because it does not contain an
uninitialized stack. The stack on x86 must be 16-byte aligned according to the
System V ABI standard and de-facto extensions.
*/
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

# The kernel entry point, as specified in linker.ld
.section .text
.global _start
.type _start, @function
_start:
  /*
  The bootloader has loaded us into 32 bit protected mode on a x86 machine.
  Interrupts are disabled. Paging is disabled. The processor state is as
  defined in the multiboot standard.
  */

  # Set up the stack
  mov $stack_top, %esp

  # Call global constructors
  call _init

  /*
  We now transfer control to kernel_main. The ABI requires the stack is now very
  16-byte aligned at the time of the call instruction (which afterwards pushes
  the return pointer of size 4 bytes). The stack was originally 16-byte aligned
  above and since we have not since pushed any bytes, the alignment is preserved
  and the call is well defined.
  */
  call kernel_main

  /*
  If the system has nothing more to do, put the computer into an infinite loop.
  To do that:
  1) Disable interrupts with cli (clear interrupt enable in eflags).
     They are already disabled by the bootloader, so this is not needed.
     However, if interrupts are later enabled, this prevents any problems.
  2) Wait for the next interrupt to arrive with hlt. Since they are disabled,
     this will lock up the computer.
  3) Jump to the hlt instruction if it ever wakes up due to a non-maskable
     interrupt occurring or due to system management mode.
  */
  cli
1:
  hlt
  jmp 1b

/*
Set the size of the _start symbol to the current location minus its start.
This is useful when debugging or when call tracing is enabled.
*/
.size _start, . - _start
