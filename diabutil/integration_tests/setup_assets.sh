#!/bin/bash

# Usage: setup_assets.sh predmo.mpq retail.mpq

# TODO: Can we test with our own files? devilution files?

set -eu

if [ $# -ne 2 ]
then
    echo "Usage: $0 prdemo.mpq retail.mpq"
    echo "See the integration tests readme"
    exit 1
fi

# You can get the prdemo mpq from diablo-evolution
prdemo_mpq=$1
# You can buy Diablo on GoG and get the MPQ from there
retail_mpq=$2

# mpqextract puts files in cwd
mkdir -p assets
cd assets

# mpqextract binary must be refereced from assets subdir
# TODO: If we're testing mpqextract... should we be using mpqextract here?
# (which could be broken because we haven't tested it)
mpqextract=../../build/mpqextract/mpqextract

echo data/bigtgold.cel | ${mpqextract} "${prdemo_mpq}"
echo gendata/mainmenu.pal | ${mpqextract} "${prdemo_mpq}"
echo data/inv/objcurs.cel | ${mpqextract} "${prdemo_mpq}"

echo ui_art/font42.bin | ${mpqextract} "${retail_mpq}"
echo ui_art/font42g.pcx | ${mpqextract} "${retail_mpq}"
echo levels/l3data/l3.amp | ${mpqextract} "${retail_mpq}"
