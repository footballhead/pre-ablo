# mpqadd

A CLI to add files to an MPQ.

Designed specifically for the Diablo Pre-Release Demo.

## Running

The MPQ to modify is an argument. Files are fed in over stdin. They must be relative to the current directory. The provided path is used as the filename in the MPQ.

    find plrgfx/ -type f | mpqadd DIABDAT.MPQ
