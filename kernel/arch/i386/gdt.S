.extern gdt_ptr

.global gdt_load
.type gdt_load, @function
gdt_load:
  cli
  lgdt (gdt_ptr)

	# Enter protected mode
	mov %cr0, %eax
	or $1, %al # Set PE (Protection Enabled) bit in cr0
	mov %eax, %cr0

  # Reloads the cs register with the ring 0 code selector.
  # The value 0x08 has its last 3 bits set to 0, specifying privilege level 0.
  # The other bits specify index 0 into the gdt, which corresponds to the
  # ring 0 code selector entry.
  ljmp $0x08, $reload_cs
reload_cs:
  # Reloads the data segment registers with the ring 0 data selector.
  # The value 0x10 has its last 3 bits set to 0, specifying privilige level 0,
  # just as above. The other bits specify index 2 into the gdt, which
  # corresponds to the ring 0 data selector entry.
  mov $0x10, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs
  mov %ax, %ss
  ret


