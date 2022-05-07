# OBJ_BOOKLVR

A book on a stand. When read, it plays a message and (acting like a lever)
transforms part of the level.

In the demo, this is used on dlvl4 to open the stairs to dlvl5 (see
InitObjects)

In retail, the object itself looks like it is not used. However, the logic
(`AddBookLever`) is used in Halls of the Blind, Arkaine's Valor, and Warlord of
Blood. (The actual objects are `OBJ_BLINDBOOK`, `OBJ_BLOODBOOK`, and 
`OBJ_STEELTOME` respectively)

## Lever Mechanics

TODO

## Variables

* `var1`: lever x1
* `var2`: lever y1
* `var3`: lever x2
* `var4`: lever y2
* `var7`: qtext message ID to play when interacted with
* `var8`: lever ID