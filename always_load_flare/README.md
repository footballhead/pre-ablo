Always Load Flare Graphics
===============================================================================

> Depends on [infraring_fix](../infraring_fix) being applied first

Loading the Blood Star/"flare" CELs is conditional on a Succubus being present on the level. However, some monsters other than the Succubus want these graphics (e.g. certain boss packs.) If they aren't loaded then the game will crash.

The solution is to always load the Blood Star CELs.

We'll need to modify InitMonsterGFX to load it if we haven't already. Change (starting at binary offset 0xD69):

```
.text:00401969 83 BD F8 FE FF FF 4F                          cmp     [ebp+mtype], 4Fh
.text:00401970 0F 8C 2B 00 00 00                             jl      loc_4019A1
.text:00401976 83 BD F8 FE FF FF 52                          cmp     [ebp+mtype], 52h
.text:0040197D 0F 8F 1E 00 00 00                             jg      loc_4019A1
.text:00401983 B9 C8 22 4A 00                                mov     ecx, offset aMonstersSuccFl ; "Monsters\\Succ\\Flare.CEL"
.text:00401988 E8 03 0E 08 00                                call    LoadFileInMem
.text:0040198D A3 E0 71 5F 00                                mov     pFlareCel, eax
.text:00401992 B9 E0 22 4A 00                                mov     ecx, offset aMonstersSuccFl_0 ; "Monsters\\Succ\\Flarexp.CEL"
.text:00401997 E8 F4 0D 08 00                                call    LoadFileInMem
.text:0040199C A3 9C C8 5F 00                                mov     pFlarexpCel, eax
```

into:

```
.text:00401969 83 3D E0 71 5F 00 00                          cmp     pFlareCel, 0
.text:00401970 0F 85 2B 00 00 00                             jnz     loc_4019A1
.text:00401976 90 90 90 90 90 90 90                          nop (x7)
.text:0040197D 90 90 90 90 90 90                             nop (x6)
.text:00401983 B9 C8 22 4A 00                                mov     ecx, offset aMonstersSuccFl ; "Monsters\\Succ\\Flare.CEL"
.text:00401988 E8 03 0E 08 00                                call    LoadFileInMem
.text:0040198D A3 E0 71 5F 00                                mov     pFlareCel, eax
.text:00401992 B9 E0 22 4A 00                                mov     ecx, offset aMonstersSuccFl_0 ; "Monsters\\Succ\\Flarexp.CEL"
.text:00401997 E8 F4 0D 08 00                                call    LoadFileInMem
.text:0040199C A3 9C C8 5F 00                                mov     pFlarexpCel, eax
...
```

A similar fix needs to happen to FreeMonsters (starting at binary offset 0xAC07). Change this:

```
.text:0040B807 83 7D FC 4F                                   cmp     [ebp+var_4], 4Fh
.text:0040B80B 0F 8C 56 00 00 00                             jl      loc_40B867
.text:0040B811 83 7D FC 52                                   cmp     [ebp+var_4], 52h
.text:0040B815 0F 8F 4C 00 00 00                             jg      loc_40B867
.text:0040B81B A1 E0 71 5F 00                                mov     eax, pFlareCel
.text:0040B820 50                                            push    eax             ; pMem
.text:0040B821 FF 15 C8 03 62 00                             call    ds:GlobalHandle
.text:0040B827 50                                            push    eax             ; hMem
.text:0040B828 FF 15 CC 03 62 00                             call    ds:GlobalUnlock
.text:0040B82E A1 E0 71 5F 00                                mov     eax, pFlareCel
.text:0040B833 50                                            push    eax             ; pMem
.text:0040B834 FF 15 C8 03 62 00                             call    ds:GlobalHandle
.text:0040B83A 50                                            push    eax             ; hMem
.text:0040B83B FF 15 A0 04 62 00                             call    ds:GlobalFree
.text:0040B841 A1 9C C8 5F 00                                mov     eax, pFlarexpCel
.text:0040B846 50                                            push    eax             ; pMem
.text:0040B847 FF 15 C8 03 62 00                             call    ds:GlobalHandle
.text:0040B84D 50                                            push    eax             ; hMem
.text:0040B84E FF 15 CC 03 62 00                             call    ds:GlobalUnlock
.text:0040B854 A1 9C C8 5F 00                                mov     eax, pFlarexpCel
.text:0040B859 50                                            push    eax             ; pMem
.text:0040B85A FF 15 C8 03 62 00                             call    ds:GlobalHandle
.text:0040B860 50                                            push    eax             ; hMem
.text:0040B861 FF 15 A0 04 62 00                             call    ds:GlobalFree
.text:0040B867 E9 23 FE FF FF                                jmp     loc_40B68F
```

To this (.text:00401129 is binary offset 0x529):

```
.text:00401129 C7 05 E0 71 5F 00 00 00 00 00                 mov     pFlareCel, 0
.text:00401133 C7 05 9C C8 5F 00 00 00 00 00                 mov     pFlarexpCel, 0
.text:0040113D E9 4D A5 00 00                                jmp     loc_40B68F
...
.text:0040B807 83 3D E0 71 5F 00 00                          cmp     pFlareCel, 0
.text:0040B80E 0F 84 53 00 00 00                             jz      loc_40B867
.text:0040B814 90                                            nop (x1)
.text:0040B815 90 90 90 90 90 90                             nop (x6)
.text:0040B81B A1 E0 71 5F 00                                mov     eax, pFlareCel
.text:0040B820 50                                            push    eax             ; pMem
.text:0040B821 FF 15 C8 03 62 00                             call    ds:GlobalHandle
.text:0040B827 50                                            push    eax             ; hMem
.text:0040B828 FF 15 CC 03 62 00                             call    ds:GlobalUnlock
.text:0040B82E A1 E0 71 5F 00                                mov     eax, pFlareCel
.text:0040B833 50                                            push    eax             ; pMem
.text:0040B834 FF 15 C8 03 62 00                             call    ds:GlobalHandle
.text:0040B83A 50                                            push    eax             ; hMem
.text:0040B83B FF 15 A0 04 62 00                             call    ds:GlobalFree
.text:0040B841 A1 9C C8 5F 00                                mov     eax, pFlarexpCel
.text:0040B846 50                                            push    eax             ; pMem
.text:0040B847 FF 15 C8 03 62 00                             call    ds:GlobalHandle
.text:0040B84D 50                                            push    eax             ; hMem
.text:0040B84E FF 15 CC 03 62 00                             call    ds:GlobalUnlock
.text:0040B854 A1 9C C8 5F 00                                mov     eax, pFlarexpCel
.text:0040B859 50                                            push    eax             ; pMem
.text:0040B85A FF 15 C8 03 62 00                             call    ds:GlobalHandle
.text:0040B860 50                                            push    eax             ; hMem
.text:0040B861 FF 15 A0 04 62 00                             call    ds:GlobalFree
.text:0040B867 E9 BD 58 FF FF                                jmp     loc_00401129
```


