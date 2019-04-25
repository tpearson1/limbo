#!/bin/sh

SYSTEM_HEADER_PROJECTS="libc kernel"
PROJECTS="libc kernel"

export MAKE=${MAKE:-make}
export HOST=${HOST:-$(./default-host.sh)}
export AR=${HOST}-ar
export AS=${HOST}-as
export CC=${HOST}-gcc
export CXX=${HOST}-g++

export PREFIX=/usr
export EXEC_PREFIX=$PREFIX
export BOOT_DIR=/boot
export LIB_DIR=$EXEC_PREFIX/lib
export INCLUDE_DIR=$PREFIX/include

export CFLAGS='-O2 -g -Wall -Wextra -Werror'
export CXXFLAGS='-O2 -g -Wall -Wextra -Werror'
export CPPFLAGS=''

# Configure the cross-compiler to use the desired system root
export SYSROOT="$(pwd)/sysroot"
export CC="$CC --sysroot=$SYSROOT"

# Work around the fact that the -elf gcc targets doesn't have a system include
# directory because it was configured with --without-headers rather than
# --with-sysroot
if echo "$HOST" | grep -Eq -- '-elf($|-)'; then
  export CC="$CC -isystem=$INCLUDE_DIR"
fi
