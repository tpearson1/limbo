#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir/boot/grub

cp sysroot/boot/limbo.kernel isodir/boot
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "myos" {
  multiboot /boot/limbo.kernel
}
EOF
grub-mkrescue -o limbo.iso isodir
