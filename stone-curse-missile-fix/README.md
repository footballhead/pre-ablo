Stone Curse Missile Fix
===============================================================================

The game uses a few variables to know when to draw a missile:

 1. `dFlags[112][112]`. One byte per (x,y) tile. This is a bitfield of all possible things that could occupy a tile (i.e. player, missile, object, monster). If `dFlags[x][y] & 1` then proceed to drawing missiles for this tile.
 2. `dMissile[112][112]`. One byte per (x,y) tile. Values fall into one of three cases:
    1. `0`: There is no missile here
    2. `X`: There is one missile here, consult `missileactive[X-1]`
    3. `-1`: There are two+ missiles here, scan through `missileactive` for IDs to draw
 3. `missileactive[125]`. List of IDs into `missile`. These are all missiles that are candidates for being updated and drawn.
 4. `missile[125]`: The actual missile data (location, animations, etc)
    1. Specifically, `missile.mAnimData` a pointer to the CEL buffer

Our problem resides when `dMissile[x][y] == -1`. When this is the case, the game scans through `missileactive` based on three things:

 1. x coordinate
 2. y coordinate
 3. preflag

If those 3 things match the draw expectation for the given tile then it will draw the missile using `missile.mAnimData`.

Enter Stone Curse
-------------------------------------------------------------------------------

Casting Stone Curse creates a missile at the location of the monster, but `missile.mAnimData` is `0` (`NULL`).

So how does the game not crash when casting Stone Curse on one enemy? Well it doesn't update `dFlags` to tell the game to draw it. As soon as another missile occupies that tile though, `dFlags` gets set and the game will try to draw all missiles on that tile. So it finds the `MIT_STONE` missle and tries to deference `missile.mAnimData` which is NULL and the game crashes.

Quick Fix
-------------------------------------------------------------------------------

Never set `dMissile` to `-1`. This is handled by `PutMissile`:

```
.text:00436796 8B 45 FC                                      mov     eax, [ebp+mx] ; BINOFF 0x35B96
.text:00436799 8B C8                                         mov     ecx, eax
.text:0043679B C1 E0 03                                      shl     eax, 3
.text:0043679E 2B C1                                         sub     eax, ecx
.text:004367A0 C1 E0 04                                      shl     eax, 4
.text:004367A3 8B 4D F8                                      mov     ecx, [ebp+my]
.text:004367A6 0F BE 84 08 08 86 57 00                       movsx   eax, dMissile[eax+ecx]
.text:004367AE 85 C0                                         test    eax, eax
.text:004367B0 0F 85 21 00 00 00                             jnz     more_than_one_missile
```

Replace this with nops.

This might have adverse effects on missile logic. It definitely creates graphical glitches with firewall. But those might be better than crashing!

Proper Fix
-------------------------------------------------------------------------------

Go through each draw function and gracefully exit if `pCelBuff` is `NULL`.
