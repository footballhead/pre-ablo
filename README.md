# diablo-prdemo-patches

## !!! WARNING: This repo uses submodules, please clone recursively!

Download the game: https://diablo-evolution.net/index.php?pageid=files

After downloading, look at [00_dplay](00_dplay) and [01_ddraw](01_ddraw) to get it up and running on modern Windows.

After that I'd recommend:

  * [fullgame](fullgame)
  * [skip_intros](skip_intros)
  * [rogue-fix](rogue-fix)
  * [catacombs-fix](catacombs-fix)
  * [caves-fix](caves-fix)

## Philosophy

The goal is to make the Pre-Release Demo as enjoyable as possible with as few crashes as possible by making as few changes as possible.

Things that we will do:

* The skeleton king is supposed to drop the Undead Crown but doesn't because it's missing from the unique table. This should be fixed to be closer to the intent

Things that we won't do:

* A lot of later monsters don't have their own sound files, e.g. succubus. We will not add the missing files because that was not the state of the game at the time.

## Making patches

VCDIFF patches are made with [open-vcdiff](https://github.com/google/open-vcdiff).

To make creation and application of patches easier, I've made a Python UI with Gooey. Checkout the [vcdiff-gooey](vcdiff-gooey) submodule.

## Adding Missing Files

The game doesn't like:

  * new files in the MPQ (not recognized for some reason)
  * files on filesystem alongside the MPQ
  
So:

 1. Extract the MPQ to wherever diablo.exe is
 2. Copy your files into the right place on FS
 3. Delete diabdat.mpq

## MPQ File List

For all files in Pre-Release Demo, see [listfile.txt]
