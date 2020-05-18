Game Crash Drawing HP > MaxHP
===============================================================================

> Depends on [tp_setlvl_fix](../tp_setlvl_fix) being applied first

The game takes perhaps an unintuitive approach to drawing the health globes: the main control panel graphic has full HP and MP orbs, then the missing HP/MP is drawn on top. If just the control panel is drawn then it would look like your health is always full. If your HP isn't full then it draws the missing HP on top of the control panel.

If you have more HP than max then the game trys to draw "negative missing health". Since that doesn't exist then it crashes.

We can remove the crash by preventing health globe from overflowing. This is controlled by the `_pHPPer` member of `PlayerStruct`. The player will gain health over max but the game won't try to draw it.

(The same should probably be applied to mana but I haven't found a crash for that case)

```
.text:0041034B 8B 45 FC                                      mov     eax, [ebp+dv]       ; BINOFF 0xF74B
.text:0041034E E9 63 0E FF FF                                jmp     reclaimed_dead_code ; displace mov ecx, myplr 
.text:00410353 90                                            nop                         ; padding
.text:00410354                               store_hp_per:
.text:00410354 8B D1                                         mov     edx, ecx
.text:00410356 C1 E1 05                                      shl     ecx, 5
.text:00410359 03 CA                                         add     ecx, edx
.text:0041035B 8D 0C 8A                                      lea     ecx, [edx+ecx*4]
.text:0041035E C1 E1 03                                      shl     ecx, 3
.text:00410361 2B CA                                         sub     ecx, edx
.text:00410363 C1 E1 04                                      shl     ecx, 4
.text:00410366 89 81 40 5A 60 00                             mov     plr_pHPPer[ecx], eax


.text:004011B6                               reclaimed_dead_code:                   ; BINOFF 0x5B6
.text:004011B6                                                                      ; eax is dv
.text:004011B6 8B 0D 38 5E 61 00                             mov     ecx, myplr     ; This is displaced code to make room for jmp
.text:004011BC 3D 50 00 00 00                                cmp     eax, 80
.text:004011C1 0F 8E 8D F1 00 00                             jle     store_hp_per
.text:004011C7 B8 50 00 00 00                                mov     eax, 80
.text:004011CC E9 83 F1 00 00                                jmp     store_hp_per
```