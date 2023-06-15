# Release ZIP

The final zip consists of:

- Pre-release demo, as Blizzard shipped it
- ddraw.dll
- dplay.dll
- reset_dungeon.exe
- pre-ablo.mpq

## Pre-release demo

This comes from a file from Diablo Evolution. We have it cached in conan.

## ddraw.dll

This comes from a separate repo. We have cached it in conan.

## dplay.dll

This is built from dplay/ using `PREABLO_TARGET=win32`

## reset_dungeon.exe

This is built from reset_dungeon/ `PREABLO_TARGET=win32`

## pre-ablo.mpq

Our custom MPQ is packaged every build with files from assets/ using tools from diabutil/

### mpqadd

This is a CLI mpq tool that uses a specific known-working version of StormLib. This is built with `PREABLO_TARGET=host`

### assets

This is a folder of .CELs, etc. These are extracted from various sources (Beta, retail conversion, etc) and cached as files in the repo.

This entire directory can be reconstructed from all MPQs using `reconstruct.py`. This is expensive and unnecessary so we don't use this.

## What does CI do

- host build for mpqadd
- win32 build for dplay.dll and reset_dungeon.exe
- package build that collects and zips all artifacts
    - Run conan to get pre-release demo and ddraw.dll
    - generate pre-ablo.mpq using mpqadd and assets/
    - add dplay.dll and reset_dungeon.exe
    - zip it all up
