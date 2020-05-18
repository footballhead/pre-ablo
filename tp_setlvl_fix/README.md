Town Portals Combined with Set Levels are No Bueno
===============================================================================

If your town portal is in a set level (e.g. sking lair, bone chamber) and you return from town then the game still thinks you are in town. This is a problem because the game will eventually crash. Also you can't use spells.

This is because the TP code doesn't save or restore `currlevel` in the case of a `setlevel`. We can add code with hardcoded numbers to restore `currlevel` based on the `setlvlnum`. (Alternatives are discussed in #2).

```
.text:0040DFBD                               restore_setlvlnum:                     ; BINOFF 0xD3BD
.text:0040DFBD A1 B8 BF 4F 00                                mov     eax, portal_currlevel_or_setlvlnum ; same
.text:0040DFC2 E9 C2 31 FF FF                                jmp     repurposed_dead_code ; DIFFERENT! 
.text:0040DFC2                                                                            ; need more space. jump to .text:00401189
.text:0040DFC7
.text:0040DFC7                               end_of_function:
.text:0040DFC7 E9 00 00 00 00                                jmp     $+5
                                                             ; etc, the rest of the function


.text:00401189                               repurposed_dead_code:                  ; BINOFF 0x589
.text:00401189                                                                      ; portal_currlevel_or_setlvlnum still in eax
.text:00401189 A3 AC 95 4E 00                                mov     setlvlnum, eax ; old code, keep
.text:0040118E 3C 01                                         cmp     al, 1          ; SL_SKELKING
.text:0040118E                                                                      ; 3C (CMP AL, imm8) is kinda cheating here but it's easy to write
.text:00401190 0F 85 0C 00 00 00                             jnz     case_2
.text:00401196 C6 05 DC B0 4C 00 03                          mov     currlevel, 3   ; quests[Q_SKELKING]._qlevel
.text:00401196                                                                      ; This is a byte move so is also kinda cheating but lol never gonna be > 255
.text:0040119D E9 25 CE 00 00                                jmp     end_of_function ; .text:0040DFC7
.text:0040119D                               case2:
.text:004011A2 3C 02                                         cmp     al, 2           ; SL_BONECHAMB
.text:004011A4 0F 85 1D CE 00 00                             jnz     end_of_function ; .text:0040DFC7
.text:004011AA C6 05 DC B0 4C 00 07                          mov     currlevel, 7    ; quests[Q_SCHAMB]._qlevel
.text:004011B1 E9 11 CE 00 00                                jmp     end_of_function ; .text:0040DFC7
```