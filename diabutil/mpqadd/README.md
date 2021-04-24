# mpqadd

A CLI to add files to an MPQ.

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

The MPQ to modify is an argument. Files are fed in over stdin. They must be relative to the current directory. The provided path is used as the filename in the MPQ.

    find plrgfx/ -type f | mpqadd DIABDAT.MPQ
