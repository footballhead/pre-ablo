
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
