GetSpellLevel with a monster ID will try to index the player spell level which can crash.

This happens for lightning demons

Bad code in sub_4371BD (bin offset 0x3673b):

.text:0043733B E8 9B C9 01 00  call    GetSpellLevel

Replace with

33 C0 xor eax, eax
90 nop
90 nop
90 nop