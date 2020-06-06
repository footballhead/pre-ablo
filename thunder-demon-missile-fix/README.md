Thunder Demon Missile Fix
===============================================================================

The Thunder Demon missile calls GetSpellLevel (.text:00453CDB) with a monster ID. This is supposed to be a player ID though, so it can crash trying to access an invalid entry in the `plr` table.

Retail added ID validation, we can try the same. Return 1 if `id != myplr`

```
.text:00453CEA 8B 45 FC                                      mov     eax, [ebp+id]       ; BINOFF 0x530EA
.text:00453CED E9 DF D4 FA FF                                jmp     reclaimed_dead_code ; displaces 2 instructions
.text:00453CF2                               rest_of_function:
.text:00453CF2 03 C1                                         add     eax, ecx
.text:00453CF4 8D 04 81                                      lea     eax, [ecx+eax*4]
.text:00453CF7 C1 E0 03                                      shl     eax, 3
.text:00453CFA 2B C1                                         sub     eax, ecx
.text:00453CFC C1 E0 04                                      shl     eax, 4
.text:00453CFF 8B 4D F8                                      mov     ecx, [ebp+sn]
.text:00453D02 0F BE 84 08 35 59 60 00                       movsx   eax, plr_pSplLvl[eax+ecx]
...
.text:00453D2D                               end_of_function:
.text:00453D2D 5F                                            pop     edi
.text:00453D2E 5E                                            pop     esi
.text:00453D2F 5B                                            pop     ebx
.text:00453D30 C9                                            leave
.text:00453D31 C3                                            retn



.text:004011D1                               reclaimed_dead_code:                   ; BINOFF 0x5D1
.text:004011D1 39 05 38 5E 61 00                             cmp     myplr, eax
.text:004011D7 0F 84 0A 00 00 00                             jz      is_player
.text:004011DD B8 01 00 00 00                                mov     eax, 1
.text:004011E2 E9 46 2B 05 00                                jmp     end_of_function
.text:004011E7                               is_player:
.text:004011E7 8B C8                                         mov     ecx, eax ; displaced code
.text:004011E9 C1 E0 05                                      shl     eax, 5   ; displcaed code
.text:004011EC E9 01 2B 05 00                                jmp     rest_of_function
```