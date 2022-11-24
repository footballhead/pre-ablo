#!/bin/sh

# Package all resources in this directory into a new MPQ

set -eu

if [ $# -ne 1 ]
then
	echo "Usage: $0 out.mpq"
	exit 1
fi

out_mpq=$1

mpqadd=../diabutil/build/mpqadd/mpqadd

find levels/ monsters/ plrgfx/ -type f | ${mpqadd} "${out_mpq}"
