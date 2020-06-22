# Full Game

There's a single BOOL (4 byte) flag that is checked around the code. If it's TRUE (1) then restrict to level 1, etc. If it's FALSE (0) then no restrictions.

## Changes

diablo.exe, Binary offset 0xBA95C: 1 -> 0
