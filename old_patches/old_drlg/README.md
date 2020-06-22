# Old Cathedral DRLG

`InitLevelType` will return the leveltype based on the dungeon level. It's called at program start, kept in `gnLevelTypeTbl`, and stored in the character file.

It returns `5` for cathedral. If it returns `1` then it will use the old cathedral level generator.

Change (binary offset 0x87873):

```
.text:00488473 B8 05 00 00 00                                mov     eax, 5
```

to

```
.text:00488473 B8 01 00 00 00                                mov     eax, 1
```

New characters, when they enter the cathedral, will use the old generator.

Loading object graphics
-------------------------------------------------------------------------------

Some objects will try to spawn without their graphics. E.g. `AllObjects[3]` (.data:004AB578) wants `ObjMasterLoadList[7]`, the skull pile. However, the object is conditional on a themed room (look at `AllObjects[3].oload` and `AllObjects[3].otheme`) that the level generator doesn't provide (maybe it was added the DRLG was replaced)

The upshot is that we need to make the skulpile a must-load. We need to change oload to 1 and otheme to -1. Change:

 * .data:004ab5fC: 3 -> 1
 * .data:004ab601: 3 -> 0xFF (-1)

Dev Mode Spawns in Newer Dungeons
-------------------------------------------------------------------------------

Dev mode explicitly sets `leveltype` for new characters to `5` when it should be `1`.

Before:

```
.text:004894AF C7 05 48 36 4E 00 05 00 00 00                 mov     leveltype, 5
``

After:

```
.text:004894AF C7 05 48 36 4E 00 01 00 00 00                 mov     leveltype, 1
```
