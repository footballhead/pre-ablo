# mpqextract

A CLI to extract files from an MPQ.

Designed specifically for the Diablo Pre-Release Demo.

## Prerequisites

  * C++17 compiler
  * CMake 3.10+
  * [StormLib 9.20](https://github.com/ladislav-zezula/StormLib/tree/v9.20) (yes, 9.20 specifically)
      * This is checked out as a submodule

## Building

    mkdir build
    cd build
    cmake ..
    make

## Running

The MPQ to modify is an argument. Files are fed in over stdin. They wil be extracted relative to the current directory.

    cat listfile.txt | mpqextract DIABDAT.MPQ
