#!/bin/sh
set -e

. ./headers.sh

for PROJECT in $PROJECTS; do
  (cd $PROJECT && DEST_DIR="$SYSROOT" $MAKE install)
done
