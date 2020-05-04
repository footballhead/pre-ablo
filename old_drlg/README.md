`InitLevelType` will return the level type based on the dungeon level. It's called at program start and stored in the character file.

It returns 5 for catedral. If it returns 1 then it will use the old cathedral level generator.

Change (binary offset 0x87873)

```
.text:00488473 B8 05 00 00 00                                mov     eax, 5
```

to

```
.text:00488473 B8 01 00 00 00                                mov     eax, 1
```

New characters, when they enter the cathedral, will use the old generator.

WARNING: This is very crash prone
