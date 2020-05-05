# diablo-prdemo-patches

## !!! WARNING: This repo uses submodules, please clone recursively!

Download the game: https://diablo-evolution.net/index.php?pageid=files

After downloading, look at [00_dplay](00_dplay) and [01_ddraw](01_ddraw) to get it up and running on modern Windows.

To get all missing graphics, look at [02_missing_gfx](02_missing_gfx).

After that I'd recommend (in order):

  * [fullgame](fullgame)
  * [skip_intros](skip_intros)
  * [music-nompq-fix](music-nompq-fix)
  * [infraring_fix](infraring_fix)
  * [always_load_flare](always_load_flare)
  * [thunder-demon-missile-fix](thunder-demon-missile-fix)

## Philosophy

The goal is to take a light handed "if it ain't broke, don't fix it approach" to making the Pre-Release Demo:

1. as enjoyable as possible
2. with as few crashes as possible
3. by making as few changes as possible.

Things that we will do:

* The skeleton king is supposed to drop the Undead Crown but doesn't because it's missing from the unique table. This is probably an explicit ommision from the PR demo (along with the infravision ring) so should be fixed to be closer to the intent at the time.
* Any crash. The game should not crash.

Things that we won't do, these are part of the "charm of the PR Demo":

* A lot of later monsters don't have their own sound files, e.g. succubus. We will not add the missing files because that was not the state of the game at the time.
* Gameplay bugs that otherwise don't crash the game, e.g. Griswold showing affixes while selling unidentified magic items.

Deviating from this philosophy needs to be evaluated on a case-by-case basis.

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
