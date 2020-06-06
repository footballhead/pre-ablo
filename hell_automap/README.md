Enable Hell Automap
===============================================================================

> This conflicts with [old_drlg](old_drlg)

Levels 12-16 have `leveltype == 4`. `InitAutomap` (.text:) only supports leveltype `2/3/5`. We can trick it to care about leveltype `2-5` then divert the 4 case to load the catacombs automap information.

Change (binary offset 0x51251):

```
.text:00451E51 83 3D 48 36 4E 00 05                          cmp     leveltype, 5
.text:00451E58 0F 84 1F 00 00 00                             jz      loc_451E7D
.text:00451E5E 83 3D 48 36 4E 00 02                          cmp     leveltype, 2
.text:00451E65 0F 84 12 00 00 00                             jz      loc_451E7D
.text:00451E6B 83 3D 48 36 4E 00 03                          cmp     leveltype, 3
.text:00451E72 0F 84 05 00 00 00                             jz      loc_451E7D
.text:00451E78 E9 D5 01 00 00                                jmp     loc_452052
```

to

```
.text:00451E51 83 3D 48 36 4E 00 05                          cmp     leveltype, 5
.text:00451E58 0F 8F F4 01 00 00                             jg      loc_452052 ; end of function, bypasses a lot of array init
.text:00451E5E 83 3D 48 36 4E 00 02                          cmp     leveltype, 2
.text:00451E65 0F 8C E7 01 00 00                             jl      loc_452052 ; ditto, end of function
.text:00451E6B 83 3D 48 36 4E 00 04                          cmp     leveltype, 4
.text:00451E72 0F 84 33 00 00 00                             jz      loc_451EAB ; this is the catacombs automap file loder
.text:00451E78 90 90 90 90 90                                nop (x5) ; the switch immediately follows, which is where we want to go for 2/3/5, so this can be NOP
```

Additionally, `DrawAutomap` (binary offset 0x526D2) will need to change from:

```
.text:004532D2 83 3D 48 36 4E 00 05                          cmp     leveltype, 5
.text:004532D9 0F 84 1F 00 00 00                             jz      loc_4532FE
.text:004532DF 83 3D 48 36 4E 00 02                          cmp     leveltype, 2
.text:004532E6 0F 84 12 00 00 00                             jz      loc_4532FE
.text:004532EC 83 3D 48 36 4E 00 03                          cmp     leveltype, 3
.text:004532F3 0F 84 05 00 00 00                             jz      loc_4532FE
.text:004532F9 E9 29 03 00 00                                jmp     loc_453627
```

to

```
.text:004532D2 83 3D 48 36 4E 00 05                          cmp     leveltype, 5
.text:004532D9 0F 8F 48 03 00 00                             jg      loc_453627 ; end of function
.text:004532DF 83 3D 48 36 4E 00 02                          cmp     leveltype, 2
.text:004532E6 0F 8C 3B 03 00 00                             jl      loc_453627 ; end of function
.text:004532EC 90 90 90 90 90 90 90                          nop (x7) ; drawing routine immediately follows so nop the rest
.text:004532F3 90 90 90 90 90 90                             nop (x6)
.text:004532F9 90 90 90 90 90                                nop (x5)
```