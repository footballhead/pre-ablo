# Diablo Pre-Release Demo ENHANCED

![Banner](_DOCS/banner.png)

**Download latest release:** [v0.3.0](https://gitlab.com/moralbacteria/diablo-prdemo-patches/-/releases)  
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
 4. Run `launcher.exe`. This is a custom launcher that allows you to mix-and-match patches. The items checked by default are what we recommend to get the best possible experience.
 5. Choose patches (again, we recommend _Select Recommended_), then click _Launch!_ This will produce a patched `DIABLO.EXE` and run it.

If you like your last configuration, you can bypass the launcher and run `DIABLO.EXE` directly.

## Developing

### !!! WARNING: This repo uses submodules, please clone recursively!

_Diablo Pre-release Demo ENHANCED_ (PRDE) is a project that consists of many parts:

  * Individual fixes (this repo)
  * A custom launcher for applying the fixes (https://gitlab.com/moralbacteria/prde_patcher)
  * A custom CLI for packaging files in the final MPQ (https://gitlab.com/moralbacteria/mpqadd)
  * A custom ddraw.dll for window mode and other goodies (https://github.com/footballhead/diablo-ddrawwrapper/tree/prde)
  * A CI/CD pipeline for producing builds for release (see [.gitlab-ci.yml](.gitlab-ci.yml))

### diablo-prdemo-patches (this repo!)

Each patch directory has the following structure:

  * `patch_name/`
      * `diablo.exe.vcdiff`: This is the actual patch, encoded in VCDIFF format
      * `meta.xml`: This file contains information used by the launcher
      * `README.md`: An in-depth developer explanation of the problem and fix

#### diablo.exe.vcdiff

This is the actual binary of the patch. It is in VCDIFF format produced by [open-vcdiff](https://github.com/google/open-vcdiff). A pre-built copy for Windows can be found in [!Tools/vcdiff.exe](!Tools/vcdiff.exe).

Patches are produced by:

 1. Starting with an intial DIABLO.EXE binary, which I like to call `before.exe`
 2. Making a copy of `before.exe` and modifying the machine code, which I like to call `DIABLO.EXE`. This can be made by Ghidra, IDA, manual hex editing, or any tool of your choice.
 3. Running `vcdiff encode --dictionary before.exe --target DIABLO.exe --delta diablo.exe.vcdiff`

This is the hard part of the process and requires knowledge of x86 assembly and Diablo internals. For reference on the latter, I'd recommend [Devilution](https://github.com/diasurgical/devilution).

#### meta.xml

This file describes the patch to our launcher.

It has the following structure:

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<patch>
    <order>(int)</order>
    <recommended>true/false</recommended>
    <description>(string)</description>.
</patch>
```

  * `order`: Some patches must be applied before others. This is a limitation of the binary patching approach. If not applied in the right order then certain patches won't work correctly. To combat this, we impose a global ordering on patches.
      * Patches that don't have ordering restrictions can get away with `0`.
  * `recommended`: If `true` then this patch will be checked by default. If `false` then this patch will be unchecked by default.
  * `description`: A plain-text explanation of the patch to the end-user. Shown on the right-hand textbox in the GUI.

The launcher will ignore files that don't follow this format.

#### README.md

This is a textual description of how the developer made the patch. This is useful for reproducing the patch or understanding the patch if changes need to be made.

### prde_patcher

We have a custom launcher: https://gitlab.com/moralbacteria/prde_patcher

The launcher has it's own versioning scheme. Binaries are hand-produced and uploaded to a file hosting site like Google Drive. Then, the CI/CD pipeline downloads it and include it in the release.

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

  * A Docker image called `moralbacteria/diablo-prdemo-patches`. This is produced from [!Tools/docker-pipeline](!Tools/docker-pipeline) and hosted at https://hub.docker.com/repository/docker/moralbacteria/diablo-prdemo-patches . This Docker image contains anything that's not expected to change frequently. Currently, it contains:
      * `mpqadd`
      * `vcdiff`
      * Extracted version of the base PR Demo
      * DirectDraw patch
  * A script run on every commit and tag, stored in [.gitlab-ci.yml](.gitlab-ci.yml). This
      * Downloads a release of the launcher and puts it into the release folder
      * Copys the missing DirectPlay DLL
      * Adds all missing graphics to the MPQ
      * Adds all patches to the release
      * Exposes the release as a downloadable artifact

All releases must come from the CI/CD pipeline. Not only is it convenient, but it's reproducible.
