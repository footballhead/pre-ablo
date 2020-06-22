# Fix Automap for Unsupported Level Types

The tileset to load is dictated by the `leveltype` variable. It has the following possible values:

* `0`: Town. No automap
* `1`: Old Alpha DRLG. No automap but we want the cathedral automap
* `2`: Catacombs (dlvl 5-8)
* `3`: Caves (dlvl 9-12)
* `4`: Hell (dlvl 13-16). No automap but we want the catacombs automap
* `5`: Cathedral (dlvl 1-4)

Notice that 0/1/4 has no automap. We don't care about 0, but we want the automap for 1/4.

## InitAutomapOnce (.text:00451D87)

This function loads the right AMP file for the `leveltype`. It needs to be modified to support leveltype 1 (old DRLG) and 4 (Hell).

Before

```
                             LAB_00451e51                                    XREF[1]:     00451e39(j)  
        00451e51 83 3d 48        CMP        dword ptr [leveltype],0x5                        = ??
                 36 4e 00 05
        00451e58 0f 84 1f        JZ         LAB_00451e7d
                 00 00 00
        00451e5e 83 3d 48        CMP        dword ptr [leveltype],0x2                        = ??
                 36 4e 00 02
        00451e65 0f 84 12        JZ         LAB_00451e7d
                 00 00 00
        00451e6b 83 3d 48        CMP        dword ptr [leveltype],0x3                        = ??
                 36 4e 00 03
        00451e72 0f 84 05        JZ         LAB_00451e7d
                 00 00 00
        00451e78 e9 d5 01        JMP        LAB_00452052
                 00 00

```

After:

```
                             LAB_00451e51                                    XREF[1]:     00451e39(j)  
        00451e51 a1 48 36        MOV        EAX,[leveltype]                                  = ??
                 4e 00
        00451e56 83 f8 01        CMP        EAX,0x1
        00451e59 74 2f           JZ         cathedral
        00451e5b 83 f8 02        CMP        EAX,0x2
        00451e5e 74 4b           JZ         catacombs
        00451e60 83 f8 03        CMP        EAX,0x3
        00451e63 74 67           JZ         caves
        00451e65 83 f8 04        CMP        EAX,0x4
        00451e68 74 41           JZ         catacombs
        00451e6a 83 f8 05        CMP        EAX,0x5
        00451e6d 74 1b           JZ         cathedral
        00451e6f 66 e9 df 01     JMP        end_of_function
```

I condensed the existing code and added support for 1 and 4.

## DrawAutomap (.text:004532C9)

`DrawAutomap` has checks for whether to even consider drawing, based on `leveltype`.

Before:

```
        004532d2 83 3d 48        CMP        dword ptr [leveltype],0x5                        = ??
                 36 4e 00 05
        004532d9 0f 84 1f        JZ         do_draw
                 00 00 00
        004532df 83 3d 48        CMP        dword ptr [leveltype],0x2                        = ??
                 36 4e 00 02
        004532e6 0f 84 12        JZ         do_draw
                 00 00 00
        004532ec 83 3d 48        CMP        dword ptr [leveltype],0x3                        = ??
                 36 4e 00 03
        004532f3 0f 84 05        JZ         do_draw
                 00 00 00
        004532f9 e9 29 03        JMP        end_of_function
                 00 00
```

After:

```
        004532d2 a1 48 36        MOV        EAX,[leveltype]                                  = ??
                 4e 00
        004532d7 83 f8 01        CMP        EAX,0x1
        004532da 0f 8c 47        JL         end_of_function
                 03 00 00
        004532e0 83 f8 05        CMP        EAX,0x5
        004532e3 0f 8f 3e        JG         end_of_function
                 03 00 00
        004532e9 eb 13           JMP        DrawAutomap::do_draw
```

This means all leveltypes between 1-5 (inclusive) will be drawn.
