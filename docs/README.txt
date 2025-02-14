On Windows, open me in WordPad!

PRE-ABLO v0.5.2
================================================================================

Website: https://www.pre-ablo.com/  
Source:  https://moralbacteria.gitlab.io/diablo-prdemo-patches/  
Discord: https://discord.gg/2btAntp  
ModDB:   https://www.moddb.com/mods/pre-ablo

In August 1996, Blizzard was hard at work developing Diablo. For promotional
purposes, they produced a build to be distributed as shareware called the
Pre-Release Demo. Blizzard intended this build to be locked down: players would
be restricted to the Warrior class and the first level of the cathedral.

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

Installing
--------------------------------------------------------------------------------

First, install the Visual Studio 2019 redistributable. See this page:
https://support.microsoft.com/en-ca/help/2977003/the-latest-supported-visual-c-downloads

Next, extract the zip to C:\PRE-ABLO. It can be any folder, really, as long as
it's shorter than 48 letters. Otherwise, the game might crash on startup.

Running
--------------------------------------------------------------------------------

Run DIABLO.EXE.

Patches can be customized on the main menu by selecting Options. Hovering over
a patch on the left-hand side will display a description on the right-hand side.
Clicking a patch will toggle it on/off and save your selection. Clicking apply
will relaunch the game in order to apply your selected patches.

If you encounter an issue, delete last_patch.txt to reset your selections.

Running (Linux)
--------------------------------------------------------------------------------

While not extensively tested, Pre-ablo has been confirmed to run with Wine 5.0
on Ubuntu 20.04. For the custom DLLs to load properly, set the WINEDLLOVERRIDES
variable to specify dplay and ddraw as native:

  WINEDLLOVERRIDES="dplay,ddraw=n" wine DIABLO.EXE

It is also recommended to turn off Virtual Desktop in winecfg.

If you encounter any issues, please reach out via Discord (link at the top).

Graphics Options
--------------------------------------------------------------------------------

After starting a new game, press the ~ key to open the graphics options. This
allows you to toggle fullscreen or change window resolution.

You must use the Cursor Keys and Enter to navigate this menu; the mouse will not
work.

(This functionality is thanks to a fork of Strange Bytes's Diablo 1 Patch.)

New Game with Same Character
--------------------------------------------------------------------------------

reset_dungeon.exe will regenerate all dungeon levels so that they can be
explored again with the same character. To use:

 1. Go to town.
 2. Save and exit.
 3. Run reset_dungeon.exe.
 4. Load game.
 5. Use the stairs to the cathedral.
 6. Enjoy!

Known Bugs
--------------------------------------------------------------------------------

For a full list: https://gitlab.com/moralbacteria/diablo-prdemo-patches/-/issues

Here are some bugs worth mentioning:

  * Loading can crash depending on where the game is installed (#33)
      * WORKAROUND: Install to short path like C:\PRE-ABLO
  * Hell dungeons look different on first load (monsters in walls, etc) (#39)
      * WORKAROUND: Start a new game, then load
  * reset_dungeon.exe launches Program Compatibility Assistant (#150)
      * WORKAROUND: This program ran correctly

If you encounter other issues, please reach out via Discord.

Changelog
--------------------------------------------------------------------------------

v0.5.2

  * (New!) unimplemented_spells_fix: prevent bugs with unimplemented spells
  * (New!) blood_boil_fix: fix a bug preventing Blood Boil from working
  * (New!) stone_curse_target_fix: fix a bug preventing Stone Curse from working
  * (New!) performance_improvements: load faster, use less CPU
  * (New!) tp_load_fix: town portals won't crash loading the game now
  * (New!) remove_promo: skip promotional slide show on quit
  * (New!) old_guardian: replace Guardian with a prototype found in code
  * (New!) bone_chamber_stairs_fix: allow clicking on the Bone Chamber stairs
  * Remove no_tp_light since tp_load_fix supersedes it
  * old_drlg: allow the player to get the Magic Rock quest
  * load_mpq_customizations: exit the game when DIABDAT.MPQ is missing

v0.5.1

  * Introduce stone_curse_load_fix: prevents crash on load involving Stone Curse

v0.5.0

  * Introduce reset_dungeon.exe so you can enjoy Pre-ablo a second time
  * Fixed a bunch of graphics:
      * wludt.cel: Alter to add missing frames
      * whmaw.cel: Convert from retail to include missing frames
      * whtat.cel: Convert from retail to include missing frames
      * wmtat.cel: Convert from retail to include missing frames
      * whtlm.cel: Convert from retail to fix bad animation
      * snakea.cel: Alter to add missing frames
      * snakew.cel: Alter to add missing frames
      * megah.cel: Alter to add missing frames
      * megan.cel: Alter to add missing frames
      * megaw.cel: Alter to add missing frames
  * plrgfx_frame_fix: Fix crashes from death animations and large graphics
  * Remove snake_frame_fix since it's no longer needed
  * mega_fix: Remove frame table alterations; still required to spawn Mega Demon
  * Pre-ablo assets are now packaged in a separate MPQ called pre-ablo.mpq
      * load_mpq_customizations: This (required) patch loads pre-ablo.mpq
  * Established provenance of all assets, traceable to official MPQs
      * All missing monster graphics now come from beta, retail, or Hellfire

v0.4.6

  * enable_cut_monsters: Unraveler (cut monster) now shows on dlvl 12-14
      * This used to be called enable_fireman
  * Prefer beta assets for Catacombs (specifically automap, bone chamber)
  * Fix out-of-order succubus death animation frames
  * Replace unimplemented caves automap tiles with dirt for better automap
  * Add internal listfile to MPQ for easier inspection and iteration
  * Remove file compression within MPQ, saves ~7MB in the final ZIP

v0.4.5

  * enable_fireman: Incinerator (cut monster) now shows up on dlvl 8-10

v0.4.4

  * Introduce plrgfx_frame_fix
  * Fix wludt.cel header

v0.4.3

  * old_drlg: Always generate the Tome on dlvl 4 to prevent getting stuck
      * Still looking into how this affects catacombs quests

v0.4.2

  * Add basic Linux/Wine instructions to the README

v0.4.1

  * Fix some missed Pre-ablo rebranding
  * Reformat some patch descriptions to be slightly more legible
  * Remove compression for new files in MPQ, makes ZIP smaller

v0.4.0

  * Migrate all binary patches to C++ code in custom dplay.dll
  * Replace external launcher with in-game Options menu (on main menu)

v0.3.0

  * Introduce modified ddraw patch based on diablo-ddrawwrapper by Strange Bytes
      * Upstream: https://github.com/strangebytes/diablo-ddrawwrapper
      * Fork: https://github.com/footballhead/diablo-ddrawwrapper
      * This release bundles v0.1.0
  * Modify ddraw patch to lock the mouse to the window (reduces panic mistakes)
      * Configurable via the "Capture Mouse" option in the ~ menu
  * Remove lmh_nocrash since we have missing warrior graphics
  * Remove music-nompq-fix because we bundle a working MPQ

v0.2.3

  * Fix a crash when a missile overlaps a Stone Cursed enemy (#75)

v0.2.2

  * Update launcher. Here are some highlights:
      * Qt 5.15 (built with VS 2019)
      * Remember last applied patches in last_patch.txt
      * Add buttons to easily check recommend/all/none
      * Improve high DPI support by allowing window to resize
      * Full changelog: https://gitlab.com/moralbacteria/prde_patcher/-/releases/0.3.0
  * Add more info to devmode/cheat patch descriptions

v0.2.1

  * Improve hell_automap fix to not conflict with old_drlg
  * Rename hell_automap to automap_fix
  * Update old_drlg to work with automap_fix
  * Add known issues to old_drlg description
  * Changed V key behaviour in-game to print our version

v0.2.0

  * Include v0.2.0 of prde_patcher, a custom launcher
    (https://gitlab.com/moralbacteria/prde_patcher)
  * Create meta.xml for each patch. This provides information for the launcher
  * Bundle all patches in the release. By default, select recommended patches
      * Specifically, I want to point out the old_drlg patch. Try it out!

v0.1.0

  * Remove demo restrictions
  * Display our version on main menu
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
  * Fix crash when encountering Blightfire (#23)
  * Prevent overlapping missiles from drawing (#75 workaround)
  * Fix crash when HP > max or < 0 (#40, #76)
  * Fix town portals in Skeleton King Lair and Bone Chamber (#2)
  * Fix Thunder Demon Lightning missile-related crash (#9)

Acknowledgements
--------------------------------------------------------------------------------

Pre-ablo includes a fork of Strange Bytes's Diablo 1 Patch. Strange Bytes has
generously published the source at
https://github.com/strangebytes/diablo-ddrawwrapper

My fork can be found at https://github.com/footballhead/diablo-ddrawwrapper

Pre-ablo borrows heavily from Devilution, which is made available under the
Sustainable Use License: https://github.com/diasurgical/devilution

This project could not happen without the help and efforts of many people:

  * Mystery
  * qndel
  * mewmew
  * nitekat
  * xavier_sb
  * catalyst1995
  * chris24xd
  * All contributors to diablo-evolution.net:
    https://diablo-evolution.net
  * All contributors to Alpha4:
    https://diablo-evolution.net/index.php?pageid=alpha4
  * Strange Bytes for their patch:
    https://github.com/strangebytes/diablo-ddrawwrapper
