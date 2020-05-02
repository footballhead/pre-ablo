Always Load Flare Graphics
===============================================================================

Loading the Blood Star/"flare" CELs is conditional on a Succubus being present on the level. However, some monsters other than the Succubus want these graphics (e.g. certain boss packs.) If they aren't loaded then the game will crash.

The solution is to always load the Blood Star CELs.

We'll need to modify InitMonsterGFX to remove the condition in front of the load. Change (starting at binary offset 0xD69):

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
.text:00401969 90 90 90 90 90 90 90                          nop (x7)
.text:00401970 90 90 90 90 90 90                             nop (x6)
.text:00401976 90 90 90 90 90 90 90                          nop (x7)
.text:0040197D 90 90 90 90 90 90                             nop (x6)
.text:00401983 B9 C8 22 4A 00                                mov     ecx, offset aMonstersSuccFl ; "Monsters\\Succ\\Flare.CEL"
.text:00401988 E8 03 0E 08 00                                call    LoadFileInMem
.text:0040198D A3 E0 71 5F 00                                mov     pFlareCel, eax
.text:00401992 B9 E0 22 4A 00                                mov     ecx, offset aMonstersSuccFl_0 ; "Monsters\\Succ\\Flarexp.CEL"
.text:00401997 E8 F4 0D 08 00                                call    LoadFileInMem
.text:0040199C A3 9C C8 5F 00                                mov     pFlarexpCel, eax
```
