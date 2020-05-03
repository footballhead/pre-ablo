Snake Frame No. Fix
===============================================================================

The `monsterdata` table contains the number of frames it wants for each animation type in this order:

* neutral
* walk
* attack
* get hit
* death
* special (only used if has_special is TRUE)

However, this can differ from how many frames the file actually contains. The game will crash trying to draw or apply a TRN in this case.

The snake is one of these monsters with this mismatch. Since we're using the retail graphics, we need to use the retail frame amounts.

For each snake type (0xAEADO, 0xAEB40, 0xAEBB0, OxAEC20), change:

* neutral: 0Ch  NO CHANGE
* walk: 0Ch (12) --> 0Bh (11)
* attack: 0Eh (14) --> 0Dh (13)
* get hit: 5  NO CHANGE
* death: 12h (18)  NO CHANGE
* special: 10h (16)  NO CHANGE
