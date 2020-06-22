Medium/Heavy Armor on Warrior Hangs
===============================================================================

The medium and heavy armor graphics have been converted, see [02_missing_gfx](../02_missing_gfx). You should only need this patch if you encounter issues using them.

This patch will disable medium and heavy armor graphics, treating them all as light.

Replace (at binary offset 0xB7EF0)

```
.data:004B96F0 4C 4D 48 00                   plrgfxlmh       db 'LMH',0
```

with

```
.data:004B96F0 4C 4C 4C 00                   plrgfxlmh       db 'LLL',0
```
