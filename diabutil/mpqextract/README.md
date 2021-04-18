# mpqextract

A CLI to extract files from an MPQ.

Designed specifically for the Diablo Pre-Release Demo.

Used by https://hub.docker.com/repository/docker/moralbacteria/diablo-prdemo-patches. Rebuild that container when changed.

## Prerequisites

* C++17 compiler
* CMake 3.10+
* [StormLib 9.20](https://github.com/ladislav-zezula/StormLib/tree/v9.20) (yes, 9.20 specifically)

## Building

After building StormLib, it must be installed. If it's installed to an unconvential location, set `STORM_ROOT` environment variable before running cmake

    mkdir build
    cd build
    # export STORM_ROOT=/stormlib/install/prefix
    cmake ..
    make

## Running

The MPQ to modify is an argument. Files are fed in over stdin. They wil be extracted relative to the current directory.

    cat listfile.txt | mpqextract DIABDAT.MPQ
