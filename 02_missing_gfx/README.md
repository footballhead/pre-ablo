# Missing Graphics

The directory contains:

* `levels/l2data`: catacombs graphics, subset of Alpha4
* `levels/l3data`: caves graphics, subset of Alpha4
* `plrgfx/rogue`: rogue graphics from Alpha4

Thanks to Diablo Evolution for a lot of these, check them out: https://diablo-evolution.net/index.php?pageid=alpha4

## Installing

The game doesn't like:

  * new files in the MPQ (not recognized for some reason)
  * files on filesystem alongside the MPQ

So:

1. Extract DIABDAT.MPQ into the same directory as DIABLO.EXE
2. Delete DIABDAT.MPQ
3. Merge these directories with the ones you just extracted.

To get music working, check out [music-nompq-fix](../music-nompq-fix)

## Known Problems:

* caves minimap doesn't 100% work, see https://gitlab.com/moralbacteria/diablo-prdemo-patches/-/issues/7
