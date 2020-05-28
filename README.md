# diablo-prdemo-patches

## !!! WARNING: This repo uses submodules, please clone recursively!

Download the game: https://diablo-evolution.net/index.php?pageid=files

After downloading, look at [00_dplay](00_dplay) and [01_ddraw](01_ddraw) to get it up and running on modern Windows.

To get all missing graphics, look at [02_missing_gfx](02_missing_gfx).

After that I'd recommend (in order):

  * [fullgame](fullgame)
  * [infraring_fix](infraring_fix)
  * [always_load_flare](always_load_flare)
  * [no_tp_light](no_tp_light)
  * [hell_automap](hell_automap)
  * [undead_crown](undead_crown)
  * [tp_setlvl_fix](tp_setlvl_fix)
  * [too_much_hp_crash](too_much_hp_crash)
  * [thunder-demon-missile-fix](thunder-demon-missile-fix)
  * [snake_frame_fix](snake_frame_fix)
  * [mega_fix](mega_fix)

> Some patches have dependencies on other patches. If they are not applied in the correct order then some may be overridden or not applied at all. Refer to the README in each folder for dependency resolution.

## Philosophy

"If it ain't broke, don't fix it" is our motto. The goal is to get the most benefit from the fewest changes.

Things that we will do:

* Any crash. The game should not crash.
* The Skeleton King is supposed to drop the Undead Crown but doesn't because it's missing from the unique table. This is probably an explicit ommision from the PR demo (along with the Infravision Ring) so should be fixed to be closer to the intent at the time.

Things that we won't do, these are part of the "charm of the PR Demo" (these use the `prority::low` tag on the Issue Tracker):

* Gameplay bugs that otherwise don't crash the game or prevent game progression, e.g. Griswold showing affixes while selling unidentified magic items.
* A lot of later monsters don't have their own sound files, e.g. succubus. We will not add the missing files because that was not the state of the game at the time.

Deviating from this philosophy needs to be evaluated on a case-by-case basis. Discussion will happen on the [Issue Tracker](https://gitlab.com/moralbacteria/diablo-prdemo-patches/-/issues).

## Making patches

VCDIFF patches are made with [open-vcdiff](https://github.com/google/open-vcdiff). A precompiled version can be found in [!Tools/vcdiff.exe](!Tools/vcdiff.exe).

To make creation and application of patches easier, I've made a Python UI with Gooey. Checkout the [!Tools/vcdiff-gooey](!Tools/vcdiff-gooey) submodule.

## Adding Missing Files to MPQ

(For the original MPQ listfile, see [!Tools/listfile.txt](!Tools/listfile.txt))

For modifying/creating compatible MPQs, use [!Tools/mpqediten32_3.5.1.795](!Tools/mpqediten32_3.5.1.795) with the Game Compatibility set to _Diablo I_. This is an old version of Ladik's MPQ Editor that works for some reason. If you use the latest then files added to the MPQ will be ignored (the game will pretend that they don't exist).