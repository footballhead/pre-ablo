![Pre-ablo Banner](docs/banner.png)

**Download latest release:** [v0.5.1](https://gitlab.com/moralbacteria/diablo-prdemo-patches/-/releases)  
**Join our Discord:** https://discord.gg/2btAntp

<a href="https://www.moddb.com/mods/pre-ablo" title="View Pre-ablo on Mod DB" target="_blank"><img src="https://button.moddb.com/rating/medium/mods/46670.png" alt="View Pre-ablo on Mod DB" /></a>

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

## Running

 1. Download the latest release from [GitLab Releases](https://gitlab.com/moralbacteria/diablo-prdemo-patches/-/releases) or [ModDB](https://www.moddb.com/mods/pre-ablo/downloads)
 2. Extract `pre-ablo_XYZ.zip`. This should produce a folder called `PRE-ABLO`.
 3. **This is important:** Copy the extracted `PRE-ABLO` folder to `C:\PRE-ABLO`. After this, you should find the game at `C:\PRE-ABLO\DIABLO.exe`.
      * This is to work around an issue where the pre-release demo crashes on long pathes (i.e. your Downloads folder).
 4. Run `DIABLO.EXE`

Patches can be toggled in the Options submenu on the main menu. The game will automatically restart after confirming chosen patches.

## Building

See [docs/BUILDING.md](docs/BUILDING.md)
