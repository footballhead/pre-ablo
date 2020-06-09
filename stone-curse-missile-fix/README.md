# Stone Curse Missile Fix

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

## Enter Stone Curse

Casting Stone Curse creates a missile at the location of the monster, but `missile.mAnimData` is `0` (`NULL`).

So how does the game not crash when casting Stone Curse on one enemy? Well it doesn't update `dFlags` to tell the game to draw it. As soon as another missile occupies that tile though, `dFlags` gets set and the game will try to draw all missiles on that tile. So it finds the `MIT_STONE` missle and tries to deference `missile.mAnimData` which is NULL and the game crashes.

## Fix

Bail from both `CDrawMissile` and `DrawMissile` when `missile._mAnimData` is `NULL` in the `-1` case. This required rewriting the existing checks to compress then and create code space

`DrawMissile` before:

```
        0047c002 8b 45 fc        MOV        EAX,dword ptr [EBP + i]
        0047c005 8b 04 85        MOV        EAX,dword ptr [EAX*0x4 + missileactive]          = ??
                 a8 c6 5f 00
        0047c00c 89 45 f0        MOV        dword ptr [EBP + gnMI],EAX
        0047c00f 8b 45 f0        MOV        EAX,dword ptr [EBP + gnMI]
        0047c012 8d 04 80        LEA        EAX,[EAX + EAX*0x4]
        0047c015 c1 e0 05        SHL        EAX,0x5
        0047c018 8b 4d ec        MOV        ECX,dword ptr [EBP + sx]
        0047c01b 39 88 6c        CMP        dword ptr [EAX + missile_mix],ECX                = ??
                 72 5f 00
        0047c021 0f 85 7b        JNZ        loop_continue
                 01 00 00
        0047c027 8b 45 f0        MOV        EAX,dword ptr [EBP + gnMI]
        0047c02a 8d 04 80        LEA        EAX,[EAX + EAX*0x4]
        0047c02d c1 e0 05        SHL        EAX,0x5
        0047c030 8b 4d e8        MOV        ECX,dword ptr [EBP + sy]
        0047c033 39 88 70        CMP        dword ptr [EAX + missile_miy],ECX                = ??
                 72 5f 00
        0047c039 0f 85 63        JNZ        loop_continue
                 01 00 00
        0047c03f 8b 45 f0        MOV        EAX,dword ptr [EBP + gnMI]
        0047c042 8d 04 80        LEA        EAX,[EAX + EAX*0x4]
        0047c045 c1 e0 05        SHL        EAX,0x5
        0047c048 8b 4d 18        MOV        ECX,dword ptr [EBP + pre]
        0047c04b 39 88 c4        CMP        dword ptr [EAX + missile_miPreFlag],ECX          = ??
                 72 5f 00
        0047c051 0f 85 4b        JNZ        loop_continue
                 01 00 00
```

`DrawMissile` after:

```
        0047c002 8b 45 fc        MOV        EAX,dword ptr [EBP + i]
        0047c005 8b 04 85        MOV        EAX,dword ptr [EAX*0x4 + missileactive]
                 a8 c6 5f 00
        0047c00c 89 45 f0        MOV        dword ptr [EBP + gnMI],EAX
        0047c00f 8b 45 f0        MOV        EAX,dword ptr [EBP + gnMI]
        0047c012 8d 04 80        LEA        EAX,[EAX + EAX*0x4]
        0047c015 c1 e0 05        SHL        EAX,0x5
        0047c018 8b 4d ec        MOV        ECX,dword ptr [EBP + sx]
        0047c01b 39 88 6c        CMP        dword ptr [EAX + missile_mix],ECX
                 72 5f 00
        0047c021 0f 85 7b        JNZ        loop_continue
                 01 00 00
        0047c027 8b 4d e8        MOV        ECX,dword ptr [EBP + -0x18]
        0047c02a 39 88 70        CMP        dword ptr [EAX + missile_miy],ECX
                 72 5f 00
        0047c030 0f 85 6c        JNZ        loop_continue
                 01 00 00
        0047c036 8b 4d 18        MOV        ECX,dword ptr [EBP + 0x18]
        0047c039 39 88 c4        CMP        dword ptr [EAX + 0x5f72c4],ECX
                 72 5f 00
        0047c03f 0f 85 5d        JNZ        loop_continue
                 01 00 00
        0047c045 81 bc 20        CMP        dword ptr [EAX + 0x5f72a0],0x0
                 a0 72 5f 
                 00 00 00 
        0047c050 0f 84 4c        JZ         loop_continue
                 01 00 00
        0047c056 90              NOP
```

---

`CDrawMissile` before:

```
        0047c384 8b 45 fc        MOV        EAX,dword ptr [EBP + i]
        0047c387 8b 04 85        MOV        EAX,dword ptr [EAX*0x4 + missileactive]          = ??
                 a8 c6 5f 00
        0047c38e 89 45 f0        MOV        dword ptr [EBP + gnMI],EAX
        0047c391 8b 45 f0        MOV        EAX,dword ptr [EBP + gnMI]
        0047c394 8d 04 80        LEA        EAX,[EAX + EAX*0x4]
        0047c397 c1 e0 05        SHL        EAX,0x5
        0047c39a 8b 4d ec        MOV        ECX,dword ptr [EBP + sx]
        0047c39d 39 88 6c        CMP        dword ptr [EAX + missile_mix],ECX                = ??
                 72 5f 00
        0047c3a3 0f 85 7b        JNZ        loop_continue
                 01 00 00
        0047c3a9 8b 45 f0        MOV        EAX,dword ptr [EBP + gnMI]
        0047c3ac 8d 04 80        LEA        EAX,[EAX + EAX*0x4]
        0047c3af c1 e0 05        SHL        EAX,0x5
        0047c3b2 8b 4d e8        MOV        ECX,dword ptr [EBP + sy]
        0047c3b5 39 88 70        CMP        dword ptr [EAX + missile_miy],ECX                = ??
                 72 5f 00
        0047c3bb 0f 85 63        JNZ        loop_continue
                 01 00 00
        0047c3c1 8b 45 f0        MOV        EAX,dword ptr [EBP + gnMI]
        0047c3c4 8d 04 80        LEA        EAX,[EAX + EAX*0x4]
        0047c3c7 c1 e0 05        SHL        EAX,0x5
        0047c3ca 8b 4d 18        MOV        ECX,dword ptr [EBP + pre]
        0047c3cd 39 88 c4        CMP        dword ptr [EAX + missile_miPreFlag],ECX          = ??
                 72 5f 00
        0047c3d3 0f 85 4b        JNZ        loop_continue
                 01 00 00
```

`CDrawMissile` after:

```
        0047c384 8b 45 fc        MOV        EAX,dword ptr [EBP + i]
        0047c387 8b 04 85        MOV        EAX,dword ptr [EAX*0x4 + missileactive]          = ??
                 a8 c6 5f 00
        0047c38e 89 45 f0        MOV        dword ptr [EBP + gnMI],EAX
        0047c391 8b 45 f0        MOV        EAX,dword ptr [EBP + gnMI]
        0047c394 8d 04 80        LEA        EAX,[EAX + EAX*0x4]
        0047c397 c1 e0 05        SHL        EAX,0x5
        0047c39a 8b 4d ec        MOV        ECX,dword ptr [EBP + sx]
        0047c39d 39 88 6c        CMP        dword ptr [EAX + missile_mix],ECX                = ??
                 72 5f 00
        0047c3a3 0f 85 7b        JNZ        loop_continue
                 01 00 00
        0047c3a9 8b 4d e8        MOV        ECX,dword ptr [EBP + -0x18]
        0047c3ac 39 88 70        CMP        dword ptr [EAX + 0x5f7270],ECX
                 72 5f 00
        0047c3b2 0f 85 6c        JNZ        loop_continue
                 01 00 00
        0047c3b8 8b 4d 18        MOV        ECX,dword ptr [EBP + 0x18]
        0047c3bb 39 88 c4        CMP        dword ptr [EAX + 0x5f72c4],ECX
                 72 5f 00
        0047c3c1 0f 85 5d        JNZ        loop_continue
                 01 00 00
        0047c3c7 81 bc 20        CMP        dword ptr [EAX + 0x5f72a0],0x0
                 a0 72 5f 
                 00 00 00 
        0047c3d2 0f 84 4c        JZ         loop_continue
                 01 00 00
        0047c3d8 90              NOP

```