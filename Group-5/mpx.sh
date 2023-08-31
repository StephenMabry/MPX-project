#!/bin/sh

set -e

termsettings=$(stty -g)
qemu-system-i386 -nographic -kernel kernel.bin -s "$@"
stty "${termsettings}"
