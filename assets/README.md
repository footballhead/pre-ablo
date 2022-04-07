# Assets

## Contents

The directory contains:

### Missing Files

These files are required past dlvl 1 but are missing from the MPQ

* `levels/l2data`: catacombs graphics, from Beta
* `levels/l3data`: caves graphics, from Beta
* `plrgfx/rogue`: rogue graphics, from Beta
* `plrfgx/warrrior`: warrior graphics for Medium/Heavy armor, from Beta
* `monsters`: missing monster graphics, from Alpha4 (unknown provenance)

TODO: Scripts to convert these from retail

### Fixes

* Beta only has rln/whn/wmn death. It's been copied to other weapons (see #117)
* scbsd.cel was reconverted from retail (see #124)
* wlnlm.cel + wludt.cel from prdemo needs header fixing (see #78)
* l2.cel and l3.cel required backporting (see #11)
* l3.amp required fixing (see #7)
* fireman attack is broken in Beta, was fixed in alpha4

### Known Issues

* wludt has wrong number of frames (see #115)
* firemh.cel (see #119)
* warrior fire cast no weapon is weird (see #79)
* our magew.cel is actually retail mages.cel. We need 20 frames for pre-release demo but in retail magew.cel is only 1 frame.
* some retail monster graphics don't match pre-release demo required frames (see #91)
* some objects are broken (see #127)
* goatld.cel levitates on certain death angles
* unrava.cel needs to come from hellfire because retail is broken
    * This has a shadow compared to the others! which makes it out of place
* In a similar vein, unravw.cel needs to come from hellfire because it's missing in retail, so see note about shadow
* Worm graphics come from hellfire antworm with a palette swap and width reduction (192 -> 160) because the real worm graphics have not been found yet :(

Thanks to Diablo Evolution for a lot of these, check them out: https://diablo-evolution.net/index.php?pageid=alpha4

## Installing

You have two options for using these files:

### Local Files

If DIABDAT.MPQ is missing (e.g. if you delete it) then the game will load the files from subdirectories instead. This is useful for development or debugging where you are constantly modifying and inspecting the 

1. Extract DIABDAT.MPQ into the same directory as DIABLO.EXE. Any version of Ladik's MPQ Editor will work for extraction, but I've included a specific known working version in [../tools/mpqediten32_3.5.1.795](../tools/mpqediten32_3.5.1.795)
2. Delete DIABDAT.MPQ
3. Merge these directories with the ones you just extracted.

This will break music. To get music working, use `music_nompq_fix` patch

### Generate pre-ablo.mpq

We provide `package.sh` to generate our own `pre-ablo.mpq`. This works with the `load_mpq_customizations` patch.

You can modify `package.sh` to include your files alongside the other Pre-ablo assets.

If you are modifying the MPQ manually, **The latest Ladik's MPQ Editor will not work.**
Prefer the known working version I've included in [../tools/mpqediten32_3.5.1.795](../tools/mpqediten32_3.5.1.795)
