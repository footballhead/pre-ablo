On Windows, open me in WordPad!

DIABLO PR DEMO ENHANCED v0.1.0
================================================================================

Website: https://gitlab.com/moralbacteria/diablo-prdemo-patches

This project is a conservation attempt.

In August 1996, Blizzard was hard at work developing Diablo. For promotional
purposes, they produced a build to be distributed as shareware called the Pre-
Release Demo. Blizzard intended this build to be locked down: players would be
restricted to the Warrior class and the first level of the cathedral.

It is well known that these restrictions are easy to bypass. Doing so gives
access to a snapshot of the entire game as it existed four months before launch.

However, there are two problems playing with the limitations removed:

 1. Many graphics are missing from the MPQ ranging from player animations to
    monster animations and entire level tilesets.
 2. Crashes abound at every turn.

This project aims to tackle both these issues while maintaining the spirit of
the PR Demo. If it ain't broke then we won't fix it. If something stops the
player from getting to level 16 then we will analyze and address.

Bugs are expected since, in August 1996, the game was actively under
development. These bugs are left in to be faithful to the state of the game as
it was being developed.

Changelog
-------------------------------------------------------------------------------

v0.1.0

  * Remove demo restrictions
  * Package additional file to support modern Windows:
      * dplay.dll from DirectX 2.0
      * ddraw.dll from Strange Bytes's wrapper
  * Add all missing graphics
      * Warrior medium armor and heavy armor from the Beta MPQ (#4)
      * Rogue light armor from the Beta MPQ
      * Monster graphics from Alpha4
      * l2data and l3data from Alpha4
  * Modify wlnlm.cel CEL group header to avoid crash (#78)
  * Modify l2.cel and l3.cel to render properly (#11)
  * Modify Snake frame table to match Alpha4 graphics (#24)
  * Modify Mega Demon frame table to match Alpha4 graphics (#56)
  * Modify Mega Demon mImageSize so that it appears in the dungeon (#56)
  * Fix Skeleton King not dropping the Undead Crown (#13)
  * Fix crash on inspecting Infravision Ring/Undead Crown (#3)
  * Remove town portal light source to avoid crash on load (#5 workaround)
  * Enable minimap in Hell (#8)
  * Fix crash when encountering <<FLARE GOAT BOW BOSS>> (#23)
  * Prevent overlapping missiles from drawing (#75 workaround)
  * Fix crash when HP > max or < 0 (#40, #76)
  * Fix town portals in Skeleton King Lair and Bone Chamber (#2)
  * Fix Thunder Demon Lightning missile-related crash (#9)

Known Bugs
-------------------------------------------------------------------------------

For a full list: https://gitlab.com/moralbacteria/diablo-prdemo-patches/-/issues

Here are some bugs worth mentioning:

  * File related functions don't work for paths > 64 characters (#33)
      * WORKAROUND: Install to short path like C:\prdemo
  * Stairs to the bone chamber say "up to level 6", can't click (#17)
      * WORKAROUND: Walk through stairs trigger, cast a spell when on same tile
  * Dungeon corruption in Hell on first load (#39)
      * WORKAROUND: Start a new game until you're in Tristram, then load

Thanks
-------------------------------------------------------------------------------

This project could not happen without the help and efforts of many people:

 * Mystery
 * All contributors to diablo-evolution.net:
   https://diablo-evolution.net
 * All contributors to Alpha4:
   https://diablo-evolution.net/index.php?pageid=alpha4
 * Strange Bytes for their ddraw wrapper:
   https://github.com/strangebytes/diablo-ddrawwrapper
