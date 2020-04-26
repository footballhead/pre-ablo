# diablo-prdemo-patches

## Missing graphics

Player graphics follow this formula:

    plrgfx/CLASS/XYZ/XYZ*.CEL

Where:

 * `class` is either `warrior` or `rogue`
 * `X` is either `w` (warrior) or `r` (rogue)
 * `Y` is either `l` (light), `m` (medium) or `h` (heavy)
 * `z` is either `a`, `b`, `d`, `h`, `m`, `n`, `s`, `t`, `u`
 * `*` is `as`, `at`, `aw`, `dt`, `fm`, `ht`, `lm`, `qm`, `st`, `wl`

So we're missing:

* All rogue graphics
* All heavy/medium warrior graphics

These can be grabbed from retail, but need a CL2 -> CEL conversion. (Alpha4 has already done this, just use theirs).

NOTE: The game doesn't try to load medium and heavy variants :( 

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
