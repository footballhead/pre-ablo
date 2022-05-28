# MIS_STONE

Created by casting Stone Curse. Targets an enemy and lingers until they die or
the spell wears off.

## Missile Add

Find a monster up to 5 tiles away (using CrawlTable) and turn it to stone:

* `mmode` becomes `MM_STONE`
* `_mirange` is set to number of ticks of duration based on clvl and Stone Curse slvl
* M_DoStone doesn't update the enemy AI so it stays frozen
* ProcessMonsters will not animate the monster
* TODO: Something changes the color palette
* TODO: What shatters the monster  in one hit?

## Missile Update

While the spell (missile) is active, monitor for monster death. If the monster
dies, create the monster shatter death animtion (shatter1.cel).

When the spell (missile) wears off, restore `mmode` to the previous mode (stored
in missile `var1`).

## Variables

* `var1`: monster's previous mode, used to unfreeze the monster when the spell wears off
* `var2`: the index into the `monster` array of the frozen monster
* `_mirange`: duration of spell in ticks