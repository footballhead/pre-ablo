Mega Demon Fix
===============================================================================

There's a couple of problems with the Mega Demon:

1. The mImgSize is 3380, but the game won't load anything that pushes the total monster loaded gfx byte count over 3000 (see .text:00401366)
2. The assets changed between PR Demo and retail. Retail has fewer frames.

Since we only have converted graphics from retail, we'll unfortunately need to overwrite the existing animation data with the retail data.

Make the following changes:

```c
// Mega Demon
monsterdata[83].mImgSize = 2220;
monsterdata[83].Frames = {6, 7, 14, 1, 24, 5};
monsterdata[83].Frames = {3, 0, 0, 0, 2, 0};
// Guardian
monsterdata[84].mImgSize = 2220;
monsterdata[84].Frames = {6, 7, 14, 1, 24, 5};
monsterdata[84].Frames = {3, 0, 0, 0, 2, 0};
// Vortex Lord
monsterdata[85].mImgSize = 2220;
monsterdata[85].Frames = {6, 7, 14, 1, 24, 5};
monsterdata[85].Frames = {3, 0, 0, 0, 2, 0};
// Balrog
monsterdata[86].mImgSize = 2220;
monsterdata[86].Frames = {6, 7, 14, 1, 24, 5};
monsterdata[86].Frames = {3, 0, 0, 0, 2, 0};
```

The corrresponds to the following binary changes:

* 0xAF064: AC 08 00 00
* 0xAF080: 06 00 00 00  07 00 00 00  0E 00 00 00  01 00 00 00  18 00 00 00  05 00 00 00  03 00 00 00  00 00 00 00 00  00 00 00 00  00 00 00 00  02 00 00 00  00 00 00 00
* 0xAF0D4: AC 08 00 00
* 0xAF0F0: 06 00 00 00  07 00 00 00  0E 00 00 00  01 00 00 00  18 00 00 00  05 00 00 00  03 00 00 00  00 00 00 00 00  00 00 00 00  00 00 00 00  02 00 00 00  00 00 00 00
* 0xAF144: AC 08 00 00
* 0xAF160: 06 00 00 00  07 00 00 00  0E 00 00 00  01 00 00 00  18 00 00 00  05 00 00 00  03 00 00 00  00 00 00 00 00  00 00 00 00  00 00 00 00  02 00 00 00  00 00 00 00
* 0xAF1B4: AC 08 00 00
* 0xAF1D0: 06 00 00 00  07 00 00 00  0E 00 00 00  01 00 00 00  18 00 00 00  05 00 00 00  03 00 00 00  00 00 00 00 00  00 00 00 00  00 00 00 00  02 00 00 00  00 00 00 00
