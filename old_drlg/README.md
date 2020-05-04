Old Cathedral DRLG
===============================================================================

WARNING: This is very crash prone

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

Some objects will try to spawn without their graphics. E.g. `AllObjects[3]` wants `ObjMasterLoadList[7]`, the skull pile. However, the object is conditional on a themed room (look at `AllObjects[3].oload` and `AllObjects[3].otheme`) that the level generator doesn't provide (maybe it was added the DRLG was replaced)

The upshot is that we need to make the skulpile a must-load. We need to change oload to 1 and otheme to -1. Change:

 * binary offset 0xA9DFC: 3 -> 1
 * binary offset 0xA9E01: 3 -> 0xFF (-1)
