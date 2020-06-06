Old Cathedral DRLG
===============================================================================

> This conflicts with [hell_automap](hell_automap)

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

Automap Not Working
-------------------------------------------------------------------------------

Now that we're `leveltype` of `1`, the game thinks we're not in the cathedral. We need to change `InitAutomap` to load and use `l1.amp` in a few places:

At binary offset 0x51251, change:

```
.text:00451E51 83 3D 48 36 4E 00 05                          cmp     leveltype, 5
.text:00451E58 0F 84 1F 00 00 00                             jz      loc_451E7D
```

to

```
.text:00451E51 83 3D 48 36 4E 00 01                          cmp     leveltype, 1
.text:00451E58 0F 84 1F 00 00 00                             jz      loc_451E7D
```

And at binary offset 0x51306, change:

```
.text:00451F06 83 7D D8 05                                   cmp     [ebp+__switch__], 5
.text:00451F0A 0F 84 7A FF FF FF                             jz      loc_451E8A
```

to

```
.text:00451F06 83 7D D8 01                                   cmp     [ebp+__switch__], 1
.text:00451F0A 0F 84 7A FF FF FF                             jz      loc_451E8A
```

This will load the AMP but the game has additional checks in `DrawAutomap` based on level type.

At binary offset 0x526D2, change this:

```
.text:004532D2 83 3D 48 36 4E 00 05                          cmp     leveltype, 5
.text:004532D9 0F 84 1F 00 00 00                             jz      loc_4532FE
```

to this:

```
.text:004532D2 83 3D 48 36 4E 00 01                          cmp     leveltype, 1
.text:004532D9 0F 84 1F 00 00 00                             jz      loc_4532FE
```

Dev Mode Spawns in Newer Dungeons
-------------------------------------------------------------------------------

Dev mode explicitly sets `leveltype` for new characters to `5` when it should be `1`.

At binary offset 0x888AF, change:

```
.text:004894AF C7 05 48 36 4E 00 05 00 00 00                 mov     leveltype, 5
``

to

```
.text:004894AF C7 05 48 36 4E 00 01 00 00 00                 mov     leveltype, 1
```
