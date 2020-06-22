Game Crash Drawing HP > MaxHP or HP < 0
===============================================================================

The game takes perhaps an unintuitive approach to drawing the health globes: the main control panel graphic has full HP and MP orbs, then the missing HP/MP is drawn on top. If just the control panel is drawn then it would look like your health is always full. If your HP isn't full then it draws the missing HP on top of the control panel.

If you have more HP than max then the game trys to draw "negative missing health". Since that doesn't exist then it crashes.

If you have less HP than 0, the game tries to draw beyond the missing health graphic bounds and the game crashes.

We can remove the crash by preventing health globe from overflowing or underglowing. This is controlled by the `_pHPPer` member of `PlayerStruct`. The player will gain health over max or lose under min but the game won't try to draw it.

(The same should probably be applied to mana but I haven't found a crash for that case)

```
.text:00410348                                                                           ; BINOFF 0xF748
.text:00410348 E9 37 7A FF FF                                jmp     reclaimed_dead_code ; displace mov     [ebp+dv], eax
.text:00410348                                                                           ;          mov     eax, [ebp+dv]
.text:0041034D 90                                            nop                         ; padding
.text:0041034E                               store_dv:
.text:0041034E 89 45 FC                                      mov     [ebp+dv], eax       ; used to be `mov ecx, myplr` but ecx is already myplr from before
.text:00410351 8B 45 FC                                      mov     eax, [ebp+dv]       ; (ditto)
.text:00410354 8B D1                                         mov     edx, ecx
.text:00410356 C1 E1 05                                      shl     ecx, 5
.text:00410359 03 CA                                         add     ecx, edx
.text:0041035B 8D 0C 8A                                      lea     ecx, [edx+ecx*4]
.text:0041035E C1 E1 03                                      shl     ecx, 3
.text:00410361 2B CA                                         sub     ecx, edx
.text:00410363 C1 E1 04                                      shl     ecx, 4
.text:00410366 89 81 40 5A 60 00                             mov     plr_pHPPer[ecx], eax


.text:00407D84                               reclaimed_dead_code:                   ; BINOFF 0x7184
.text:00407D84                                                                      ; eax is value to go into dv
.text:00407D84                                                                      ; ecx is myplr
.text:00407D84 3D 50 00 00 00                                cmp     eax, 80
.text:00407D89 0F 8F 0D 00 00 00                             jg      over_max_fix
.text:00407D8F 85 C0                                         test    eax, eax
.text:00407D91 0F 8C 0F 00 00 00                             jl      under_min_fix
.text:00407D97 E9 B2 85 00 00                                jmp     store_dv
.text:00407D9C                               over_max_fix:
.text:00407D9C B8 50 00 00 00                                mov     eax, 80
.text:00407DA1 E9 A8 85 00 00                                jmp     store_dv
.text:00407DA6                               under_min_fix:
.text:00407DA6 33 C0                                         xor     eax, eax
.text:00407DA8 E9 A1 85 00 00                                jmp     store_dv
``` 
