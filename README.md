![Pre-ablo Banner](docs/banner.png)

**Download latest release:** [v0.4.2](https://gitlab.com/moralbacteria/diablo-prdemo-patches/-/releases)  
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

## Developing

> WARNING: This repo uses submodules, please clone recursively!

_Pre-ablo_ is a project that consists of many parts:

  * A custom DPLAY.DLL which contains the fixes (this repo)
  * A custom MPQ CLI (based on StormLib) for packaging files in the final MPQ (this repo)
  * A custom ddraw.dll for better graphics support and other goodies (https://github.com/footballhead/diablo-ddrawwrapper/tree/prde)
  * A CI/CD pipeline for producing builds for release (see [.gitlab-ci.yml](.gitlab-ci.yml))
  * A simple website on the [pages](https://gitlab.com/moralbacteria/diablo-prdemo-patches/-/tree/pages) branch. Hosted with GitLab pages at https://moralbacteria.gitlab.io/diablo-prdemo-patches/ (accessible via https://pre-ablo.com)

### dplay.dll

Patches are compiled into a phony `dplay.dll` and are applied when the DLL is loaded by DIABLO.EXE

Each patch is stored under [dplay/src/patches](dplay/src/patches) in its own `patch_name.cpp.`. It has a `PATCH_MAIN` (`void patch_name_main(void)`) which is designed to run on DLL load. This is accomplished by calling it from `DllMain()`.

A master list of all patches is in [dplay/src/patches.cpp](dplay/src/patches.cpp). The patch must be defined and registered there.

Users toggle patches through the Options menu on the main menu. Selections are saved in `last_patch.txt`. If no `last_patch.txt` is present then recommendations are applied instead.

This replaces the old method of binary patching with VCDIFF. The old patches are stored (for now) in [old_patches](old_patches). The README's will migrate into the patch C++ code at some point.

The DLL is cross-compiled in Docker with mingw. See [dplay/README.md](dplay/README.md) for more details.

### mpqadd

We have a custom MPQ packer: [diabutil/mpqadd](diabutil/mpqadd)

This CLI is designed to add/replace files in the PR Demo MPQ. It's used by CI to pack the final MPQ.

### ddrawwrapper

We have a custom ddraw.dll: https://github.com/footballhead/diablo-ddrawwrapper/tree/prde

Initially created by Strange Bytes, we forked it to add our own modifications.

This implements a subset of DirectDraw (the parts used by Diablo) to use a DirectX 9 backend. This works better on modern Windows than the default DirectDraw implementation. It also allows for window mode and other graphics related customizations.

It runs on its own release schedule. It must be manually compiled (due to needing DirectX SDK and latest MSVC)

### CI/CD Pipeline

The CI/CD Pipeline is hosted by GitLab on this repo: https://gitlab.com/moralbacteria/diablo-prdemo-patches/-/pipelines

This is responsible for producing releasable ZIP files. It's a script run on every commit and tag, stored in [.gitlab-ci.yml](.gitlab-ci.yml). It has several steps:

  * Build mpqadd, used by packaging
  * Cross-compile DPLAY.DLL
  * Package everything up
      * Downloads a base pre-release demo, treat as release folder
      * Downloads a release of ddraw.dll and puts it into the release folder
      * Copies our custom dplay.dll with the patches
      * Adds all missing graphics to the MPQ
      * Exposes the release as a downloadable artifact

All releases must come from the CI/CD pipeline. Not only is it convenient and it's reproducible, but it also acts as a file host.
