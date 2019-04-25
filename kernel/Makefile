DEFAULT_HOST!=../default-host.sh
HOST?=DEFAULT_HOST
HOST_ARCH!=../target-triplet-to-arch.sh $(HOST)

CFLAGS?=-O2 -g -Wall -Wextra -Werror
CXXFLAGS?=$(CFLAGS)
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DEST_DIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOT_DIR?=$(EXEC_PREFIX)/boot
INCLUDE_DIR?=$(PREFIX)/include
KERNEL_OUTPUT?=limbo.kernel

ARCH_DIR:=arch/$(HOST_ARCH)

CFLAGS:=$(CFLAGS) -ffreestanding
CXXFLAGS:=$(CXXFLAGS) -ffreestanding
CPPFLAGS:=$(CPPFLAGS) -D__IS_KERNEL -Iinclude -I$(ARCH_DIR)/include
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc

include $(ARCH_DIR)/make.config

CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CXXFLAGS:=$(CXXFLAGS) $(KERNEL_ARCH_CXXFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)

KERNEL_SOURCES:=\
$(KERNEL_ARCH_SOURCES) \
kernel/kernel.c \

KERNEL_OBJS:=$(KERNEL_SOURCES:%=%.o)

OBJS:=\
$(ARCH_DIR)/crti.S.o \
$(ARCH_DIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(ARCH_DIR)/crtend.o \
$(ARCH_DIR)/crtn.S.o \

LINK_LIST:=\
$(LDFLAGS) \
$(ARCH_DIR)/crti.S.o \
$(ARCH_DIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(LIBS) \
$(ARCH_DIR)/crtend.o \
$(ARCH_DIR)/crtn.S.o \

.PHONY: all clean install install-headers install-kernel

all: $(KERNEL_OUTPUT)

$(KERNEL_OUTPUT): $(OBJS) $(ARCH_DIR)/linker.ld
	$(CC) -T $(ARCH_DIR)/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot $@

$(ARCH_DIR)/crtbegin.o $(ARCH_DIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

%.c.o: %.c
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

%.cpp.o: %.cpp
	$(CXX) -MD -c $< -o $@ -std=c++17 $(CXXFLAGS) $(CPPFLAGS)

%.S.o: %.S
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f $(KERNEL_OUTPUT)
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-kernel

install-headers:
	mkdir -p $(DEST_DIR)$(INCLUDE_DIR)
	cp -R --preserve=timestamps include/. $(DEST_DIR)$(INCLUDE_DIR)/.

install-kernel: $(KERNEL_OUTPUT)
	mkdir -p $(DEST_DIR)$(BOOT_DIR)
	cp $(KERNEL_OUTPUT) $(DEST_DIR)$(BOOT_DIR)

-include $(OBJS:.o=.d)