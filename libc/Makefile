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
INCLUDE_DIR?=$(PREFIX)/include
LIB_DIR?=$(EXEC_PREFIX)/lib

CFLAGS:=$(CFLAGS) -ffreestanding
CXXFLAGS:=$(CXXFLAGS) -ffreestanding
CPPFLAGS:=$(CPPFLAGS) -D__IS_LIBC -Iinclude
LIBK_CFLAGS:=$(CFLAGS)
LIBK_CXXFLAGS:=$(CXXFLAGS)
LIBK_CPPFLAGS:=$(CPPFLAGS) -D__IS_LIBK

ARCH_DIR:=arch/$(HOST_ARCH)

include $(ARCH_DIR)/make.config

CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
CXXFLAGS:=$(CXXFLAGS) $(ARCH_CXXFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)
LIBK_CFLAGS:=$(LIBK_CFLAGS) $(KERNEL_ARCH_CFLAGS)
LIBK_CXXFLAGS:=$(LIBK_CXXFLAGS) $(KERNEL_ARCH_CXXFLAGS)
LIBK_CPPFLAGS:=$(LIBK_CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)

FREE_SOURCES:=\
$(ARCH_FREE_SOURCES) \
stdio.c \
stdlib.c \
string.c \

HOSTED_SOURCES:=\
$(ARCH_HOSTED_SOURCES) \

SOURCES:=\
$(FREE_SOURCES) \
$(HOSTED_SOURCES) \

$(info $$SOURCES is [${SOURCES}])

FREE_OBJS:=$(FREE_SOURCES:%=%.o)

OBJS:=$(SOURCES:%=%.o)

$(info $$OBJS is [${OBJS}])

LIBK_OBJS:=$(FREE_OBJS:.o=.libk.o)

# TODO: Enable libc
# BINARIES:=libc.a libk.a
BINARIES:=libk.a

.PHONY: all clean install install-headers install-libs

all: $(BINARIES)

libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)

%.c.o: %.c
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

%.cpp.o: %.cpp
	$(CC) -MD -c $< -o $@ -std=c++17 $(CXXFLAGS) $(CPPFLAGS)

%.S.o: %.S
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

%.c.libk.o: %.c
	$(CC) -MD -c $< -o $@ -std=gnu11 $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

%.cpp.libk.o: %.cpp
	$(CC) -MD -c $< -o $@ -std=c++17 $(LIBK_CXXFLAGS) $(LIBK_CPPFLAGS)

%.S.libk.o: %.S
	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

clean:
	rm -f $(BINARIES) *.a
	rm -f $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-libs

install-headers:
	mkdir -p $(DEST_DIR)$(INCLUDE_DIR)
	cp -R --preserve=timestamps include/. $(DEST_DIR)$(INCLUDE_DIR)/.

install-libs: $(BINARIES)
	mkdir -p $(DEST_DIR)$(LIB_DIR)
	cp $(BINARIES) $(DEST_DIR)$(LIB_DIR)

-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)
