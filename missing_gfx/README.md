# Missing Graphics

The directory contains:

* `levels/l2data`: catacombs graphics, from Alpha4 with modifications (see #11)
* `levels/l3data`: caves graphics, from Alpha4 with modifications (see #11)
* `monsters`: missing monster graphics, from Alpha4
* `plrgfx/rogue`: rogue graphics, from Alpha4
* `plrfgx/warrrior`: warrior graphics for Medium/Heavy armor, from Battle.net Beta

Thanks to Diablo Evolution for a lot of these, check them out: https://diablo-evolution.net/index.php?pageid=alpha4

## Installing

You have two options for using these files:

### Local Files

If DIABDAT.MPQ is missing (e.g. if you delete it) then the game will load the files from subdirectories instead. This is useful for development or debugging where you are constantly modifying and inspecting the 

1. Extract DIABDAT.MPQ into the same directory as DIABLO.EXE. Any version of Ladik's MPQ Editor will work for extraction, but I've included a specific known working version in [../tools/mpqediten32_3.5.1.795](../tools/mpqediten32_3.5.1.795)
2. Delete DIABDAT.MPQ
3. Merge these directories with the ones you just extracted.

This will break music. To get music working, check out [music-nompq-fix](../music-nompq-fix)

### Modify the MPQ

**The latest Ladik's MPQ Editor will not work.** Prefer the known working version I've included in [../tools/mpqediten32_3.5.1.795](../tools/mpqediten32_3.5.1.795)

Open the MPQ, then drag & drop these folders into the right place.

## Notes

* caves minimap doesn't 100% work, see https://gitlab.com/moralbacteria/diablo-prdemo-patches/-/issues/7
* monsters/mage/magew.cel is actually mages.cel (since it's the required 20 frames). In retail, magew.cel is only 1 frame.
* 