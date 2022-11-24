# Assets

The directory contains:

- missing files
- scripts for file conversions and fixes

## Missing Files

These files are required past dlvl 1 but are missing from the MPQ

* `levels/l2data`: catacombs graphics, from Beta
    * `l2.cel` required backporting (see #11)
* `levels/l3data`: caves graphics, from Beta
    * `l3.cel` required backporting (see #11)
    * `l3.amp` required fixing (see #7)
* `plrgfx/rogue`: rogue graphics, from Beta
    * `rlndt` is the only death animation. It's been copied to other weapons (see #117)
* `plrfgx/warrior`: warrior graphics for Medium/Heavy armor, from Beta
    * `wmndt`/`whndt` are the only death animtions. They've been copied to other weapons (see #117)
    * `wlnlm`/`wludt` is from pre-release demo but the header needs header fixing (see #78, #115)
* `monsters`: missing monster graphics, from retail/beta/hellfire/alpha4
    * `firema` required fixing (see fix_fireman.py)

## Known Issues

* `wludt` has wrong number of frames (see #115)
* `firemh` levitates (see #119)
* warrior fire cast no weapon is weird (see #79)
* `magew` is actually retail `mages`. We need 20 frames for pre-release demo but in retail magew.cl2 is only 1 frame.
* Some retail graphics don't match Pre-Release Demo required frames (see #91)
    * Notably `mega` (see #56) and `snake` (see #25)
* some objects are broken (see #127)
* `goatld` levitates on certain death angles
* `unrava` needs to come from hellfire because retail is garbled
    * This has a shadow compared to the others! which makes it out of place
* `unravw` needs to come from hellfire because it's broken in retail (and once you fix the header it's garbled still)
    * This has a shadow compared to the others! which makes it out of place
* `worm` graphics haven't been found yet so they come from Alpha4, which itself is derived form hellfire `antworm` with a palette swap and width reduction (192 -> 160)

Thanks to Diablo Evolution for some of these, check them out: https://diablo-evolution.net/index.php?pageid=alpha4

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

If you are modifying the MPQ manually, **the latest Ladik's MPQ Editor will not work.**
Prefer the known working version I've included in [../tools/mpqediten32_3.5.1.795](../tools/mpqediten32_3.5.1.795)
