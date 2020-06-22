Snake Frame No. Fix
===============================================================================

The `monsterdata` table contains the number of frames it wants for each animation type. It's stored as `monsterdata[x].Frames`, a 6 element array, in this order:

* neutral
* walk
* attack
* get hit
* death
* special (only used if has_special is TRUE)

However, this can differ from how many frames the file actually contains. The game will crash trying to draw or apply a TRN in this case.

The Snake is one of these monsters with this mismatch. Since we're using the retail graphics, we need to use the retail frame amounts:

* walk: 12 -> 11
* attack: 14 --> 13

Or

```
monsterdata[70].Frames[1] = 11
monsterdata[70].Frames[2] = 13
monsterdata[71].Frames[1] = 11
monsterdata[71].Frames[2] = 13
monsterdata[72].Frames[1] = 11
monsterdata[72].Frames[2] = 13
monsterdata[73].Frames[1] = 11
monsterdata[73].Frames[2] = 13
```

Or

* 0xAEAD4: `0B 00 00 00  0D 00 00 00`
* 0xAEB44: `0B 00 00 00  0D 00 00 00`
* 0xAEBB4: `0B 00 00 00  0D 00 00 00`
* 0xAEC24: `0B 00 00 00  0D 00 00 00`
