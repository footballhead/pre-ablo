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

for dir in levels monsters plrgfx
do
	find "${dir}/" -type f | ${mpqadd} "${out_mpq}"
done
