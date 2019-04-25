#!/bin/sh
set -e
. ./config.sh

echo $PROJECTS

for PROJECT in $PROJECTS; do
  (cd $PROJECT && $MAKE clean)
done

rm -rf sysroot
rm -rf isodir
rm -rf limbo.iso
