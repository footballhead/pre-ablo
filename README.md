# Diablo Pre-Release Demo ENHANCED

![Banner](_DOCS/banner.png)

**Download latest release:** [v0.4.0](https://gitlab.com/moralbacteria/diablo-prdemo-patches/-/releases)  
**Join our Discord:** https://discord.gg/2btAntp

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

## Running

 1. Download the latest release from: https://gitlab.com/moralbacteria/diablo-prdemo-patches/-/releases
      * Look for the _Download Now!_ link
 2. Extract `prdemo_enhanced_XYZ.zip`. This should produce a folder called `DIABLO`.
 3. **This is important:** Copy the extracted `DIABLO` folder to `C:\DIABLO`. After this, you should find the launcher at `C:\DIABLO\launcher.exe`.
      * This is to work around an issue where the pre-release demo crashes on long pathes (i.e. your Downloads folder).
      * You can choose any folder in `C:\` (I'm partial to `C:\PRDEMO`) if you already have Diablo installed.
 4. Run `DIABLO.EXE`

Patches can be toggled in the Options menu on the main menu. The game will automatically restart after confirming chosen patches.

## Developing

> WARNING: This repo uses submodules, please clone recursively if you want the tools!

_Diablo Pre-release Demo ENHANCED_ (PRDE) is a project that consists of many parts:

  * Individual fixes, packaged in a custom dplay.dll (this repo)
  * A custom CLI for packaging files in the final MPQ (https://gitlab.com/moralbacteria/mpqadd)
  * A custom ddraw.dll for better graphics support and other goodies (https://github.com/footballhead/diablo-ddrawwrapper/tree/prde)
  * A CI/CD pipeline for producing builds for release (see [.gitlab-ci.yml](.gitlab-ci.yml))

### diablo-prdemo-patches / dplay.dll (this repo!)

Patches are compiled into a phony `dplay.dll` and are applied when the DLL is loaded by DIABLO.EXE

Each patch is stored under [00_dplay/src/patches](00_dplay/src/patches) in its own `patch_name.cpp.`. It has a `PATCH_MAIN` (`void patch_name_main(void)`) which is designed to run on DLL load. This is accomplished by calling it from `DllMain()`.

A master list of all patches is in [00_dplay/src/patches.cpp](00_dplay/src/patches.cpp). The patch must be defined and registered there.

Users toggle patches through the Options menu on the main menu. Selections are saved in `last_patch.txt`. If no `last_patch.txt` is present then recommendations are applied instead.

This replaces the old method of binary patching with VCDIFF. The old patches are stored (for now) in [old_patches](old_patches). The README's will migrate into the patch C++ code at some point.

The DLL is cross-compiled in Docker with mingw. See [00_dplay/README.md](00_dplay/README.md) for more details.

### mpqadd

We have a custom MPQ packer: https://gitlab.com/moralbacteria/mpqadd

This CLI is designed to add/replace files in the PR Demo MPQ. It is included in the Docker image (see [!Tools/docker-pipeline](!Tools/docker-pipeline)) used by the CI/CD pipeline. Any changes here require rebuilding the Docker image.

### ddrawwrapper

We have a custom ddraw.dll: https://github.com/footballhead/diablo-ddrawwrapper/tree/prde

Initially created by Strange Bytes, we forked it to add our own modifications.

This implements a subset of DirectDraw (the parts used by Diablo) to use a DirectX 9 backend. This works better on modern Windows than the default DirectDraw implementation. It also allows for window mode and other graphics related customizations.

### CI/CD Pipeline

The CI/CD Pipeline is hosted by GitLab on this repo: https://gitlab.com/moralbacteria/diablo-prdemo-patches/-/pipelines

This is responsible for producing releasable ZIP files.

It has several components:

  * A Docker image called `moralbacteria/diablo-prdemo-patches`. This is produced from [!Tools/docker-pipeline](!Tools/docker-pipeline) and is hosted at https://gitlab.com/moralbacteria/diablo-prdemo-patches/container_registry/. This Docker image contains anything needed by a CI agent to produce the final ZIP. Currently, it contains:
      * Extracted version of the base PR Demo
      * `mpqadd`
  * A script run on every commit and tag, stored in [.gitlab-ci.yml](.gitlab-ci.yml). This:
      * Cross-compiles DPLAY.DLL
      * Downloads a release of ddraw.dll  and puts it into the release folder
      * Copies our custom dplay.dll with the patches
      * Adds all missing graphics to the MPQ
      * Exposes the release as a downloadable artifact

All releases must come from the CI/CD pipeline. Not only is it convenient and it's reproducible, but it also acts as a file host.
