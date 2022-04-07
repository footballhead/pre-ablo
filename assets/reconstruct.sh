#!/bin/sh

# Re-populate this directory by taking original resources and applying fixes

set -eu

if [ $# -ne 4 ]
then
	echo "Usage: $0 prdemo.mpq beta.mpq retail.mpq hellfire.mpq"
	exit 1
fi

prdemo=$1
beta=$2
retail=$3
hellfire=$4

fixamp=$(realpath ../diabutil/build/fixamp/fixamp)
joincel=$(realpath ../diabutil/build/joincel/joincel)
mpqextract=$(realpath ../diabutil/build/mpqextract/mpqextract)
retail2prdemo=$(realpath ../diabutil/build/retail2prdemo/retail2prdemo)
splitcel=$(realpath ../diabutil/build/splitcel/splitcel)

#
# EXTRACT
#

cat ./listfile_prdemo.txt | ${mpqextract} "${prdemo}"
cat ./listfile_beta.txt | ${mpqextract} "${beta}"
cat ./listfile_retail.txt | ${mpqextract} "${retail}"
cat ./listfile_hellfire.txt | ${mpqextract} "${hellfire}"

#
# FIX
#

# l2.cel
tmp=$(mktemp -d)
cp levels/l2data/* "${tmp}"

old_pwd=$(pwd -P)
cd "${tmp}"
${splitcel} l2.cel .
${retail2prdemo} l2.min
${joincel} 1471 l2_fix.cel
cp l2_fix.cel "${old_pwd}/levels/l2data/l2.cel"
cd "${old_pwd}"
rm -rf "${tmp}"

# l3.cel
tmp=$(mktemp -d)
cp levels/l3data/* "${tmp}"

old_pwd=$(pwd -P)
cd "${tmp}"
${splitcel} l3.cel .
${retail2prdemo} l3.min
${joincel} 1771 l3_fix.cel
cp l3_fix.cel "${old_pwd}/levels/l3data/l3.cel"
cd "${old_pwd}"
rm -rf "${tmp}"

# l3.amp
${fixamp} levels/l3data/l3.amp

# copy death animations to cover for missing files
for i in a b d h m s t u
do
	cp plrgfx/warrior/wmn/wmndt.cel plrgfx/warrior/wm${i}/wm${i}dt.cel
	cp plrgfx/warrior/whn/whndt.cel plrgfx/warrior/wh${i}/wh${i}dt.cel
	cp plrgfx/rogue/rln/rlndt.cel plrgfx/rogue/rl${i}/rl${i}dt.cel
done

# firema.cel
python fix_firema.py monsters/fireman/firema.cel monsters/fireman/firema.cel

# wlnlm.cel
python fix_wlnlm.py plrgfx/warrior/wln/wlnlm.cel plrgfx/warrior/wln/wlnlm.cel

# wludt.cel
python fix_wludt.py plrgfx/warrior/wlu/wludt.cel plrgfx/warrior/wlu/wludt.cel

# For magee, use mages instead because of # of frames
cp monsters/mage/mages.cl2 monsters/mage/magew.cl2

# Convert monster GFX
for i in a d h n w
do
	python cl2_to_cel.py --input monsters/black/black${i}.cl2 --width 160 --groups 8 --output monsters/black/black${i}.cel
	python cl2_to_cel.py --input monsters/diablo/diablo${i}.cl2 --width 160 --groups 8 --output monsters/diablo/diablo${i}.cel
	python cl2_to_cel.py --input monsters/goatlord/goatl${i}.cl2 --width 160 --groups 8 --output monsters/goatlord/goatl${i}.cel
	python cl2_to_cel.py --input monsters/mage/mage${i}.cl2 --width 128 --groups 8 --output monsters/mage/mage${i}.cel
	python cl2_to_cel.py --input monsters/mega/mega${i}.cl2 --width 160 --groups 8 --output monsters/mega/mega${i}.cel
	python cl2_to_cel.py --input monsters/snake/snake${i}.cl2 --width 160 --groups 8 --output monsters/snake/snake${i}.cel
	python cl2_to_cel.py --input monsters/succ/scbs${i}.cl2 --width 128 --groups 8 --output monsters/succ/scbs${i}.cel
	python cl2_to_cel.py --input monsters/thin/thin${i}.cl2 --width 160 --groups 8 --output monsters/thin/thin${i}.cel
	python cl2_to_cel.py --input monsters/tsneak/tsneak${i}.cl2 --width 128 --groups 8 --output monsters/tsneak/tsneak${i}.cel
	python cl2_to_cel.py --input monsters/unrav/unrav${i}.cl2 --width 128 --groups 8 --output monsters/unrav/unrav${i}.cel
	#python cl2_to_cel.py --input monsters/antworm/worm${i}.cl2 --width 128 --groups 8 --output monsters/worm/worm${i}.cel
done

python cl2_to_cel.py --input monsters/snake/snakes.cl2 --width 160 --groups 8 --output monsters/snake/snakes.cel
python cl2_to_cel.py --input monsters/thin/thins.cl2 --width 160 --groups 8 --output monsters/thin/thins.cel

#
# CLEANUP
#

find monsters -type f -name "*.cl2" -delete