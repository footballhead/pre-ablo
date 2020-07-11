#!/bin/bash

set -eux

if [ $# -ne 1 ]
then
	echo Usage: $0 file.mpq
	exit 1
fi

mpqadd="../tools/mpqadd/build/mpqadd"

find levels/ -type f | "${mpqadd}" "$1"
find monsters/ -type f | "${mpqadd}" "$1"
find plrgfx/ -type f | "${mpqadd}" "$1"
