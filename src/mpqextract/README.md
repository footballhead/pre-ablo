# mpqextract

A CLI to extract files from an MPQ.

Designed specifically for the Diablo Pre-Release Demo.

## Running

The MPQ to modify is an argument. Files are fed in over stdin. They wil be extracted relative to the current directory.

    cat listfile.txt | mpqextract DIABDAT.MPQ
