.extern idt_ptr

.global idt_load
.type idt_load, @function
idt_load:
  lidt (idt_ptr)
  ret