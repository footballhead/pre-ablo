Skeleton King Does Not Drop Undead Crown
===============================================================================

> Depends on [always_load_flare](../always_load_flare) being applied first

> Also depennds on [infraring_fix](../infraring_fix) to prevent a crash when hovering the item in the inventory.

The Skeleton King reward is stored in `monsterdata[48]` under the `mTreasure` column like this:

    0x8001 = 0x8000 + 0x1 = U_T + UID_SKCROWN

Where `UID_SKCROWN` is an index into `UnqiueItemList`. Problem is that there is no unique data for the Undead Crown so it will just drop whatever is at `UniqueItemList[1]`, which happens to be the Rift Bow.

But the base item with the health steal exists in `AllItemsLst[8]` with the life steal flag (`IAF_SKING` or `2`). We can generate items from this list with `CreateItem` (which is what the Magic Rock uses for it's reward), we just need code space to call that function.

    CreateItem(8, monster[m]._mx, monster[m]._my)

Starting at binary offset 0x70DF2:

```
.text:004719F2 83 F9 30                                      cmp     ecx, MT_KING
.text:004719F5 0F 84 47 F7 F8 FF                             jz      loc_00401142
.text:004719FB 90 90 90 90 90 90 90                          nop (x7)
.text:00471A02 90 90 90 90 90 90 90 90 90 90                 nop (x10)
.text:00471A0C 90 90 90 90 90 90 90 90 90 90                 nop (x10)
```

Also, starting at binary offset 0x542:

```
.text:00401142 8B 45 FC                                      mov     eax, [ebp+m]
.text:00401145 8B C8                                         mov     ecx, eax
.text:00401147 C1 E0 02                                      shl     eax, 2
.text:0040114A 8D 04 40                                      lea     eax, [eax+eax*2]
.text:0040114D 8D 04 81                                      lea     eax, [ecx+eax*4]
.text:00401150 8B 0C 85 08 E5 4B 00                          mov     ecx, monster_my[eax*4]
.text:00401157 51                                            push    ecx
.text:00401158 8B 14 85 04 E5 4B 00                          mov     edx, monster_mx[eax*4]
.text:0040115F B9 08 00 00 00                                mov     ecx, 8
.text:00401164 E8 ED 0E 02 00                                call    CreateItem ; .text:00422056
.text:00401169 C6 05 42 5F 61 00 03                          mov     quests_Q_SKELKING._qactive, QUEST_DONE
.text:00401170 C7 05 5C 53 60 00 1E 00 00 00                 mov     sfxdelay, 30
.text:0040117A C7 05 08 45 60 00 38 00 00 00                 mov     sfxdnum, 56
.text:00401184 E9 8D 08 07 00                                jmp     loc_471A16
```

Disabling SKing Drop
-------------------------------------------------------------------------------

Since we explicitly create the crown, we need the sking `mTreasure` to not be set.

Change: `monsterdata[48].mTreasure` from `0x8001` to `0`.

So at binary offset 0xAE178: `01 80` => `00 00`
