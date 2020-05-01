Medium/Heavy Armor on Warrior Hangs
===============================================================================

The medium and heavy armor gfx for Warrior are missing. Trying to load them causes a hang.

The long term fix is to convert all retail graphics from CL2 to CEL (started, see `medium-gfx`).

The short term fix is to not try to load medium or heavy.

Replace (at binary offset 0xB7EF0)

```
.data:004B96F0 4C 4D 48 00                   plrgfxlmh       db 'LMH',0
```

with

```
.data:004B96F0 4C 4C 4C 00                   plrgfxlmh       db 'LLL',0
```
