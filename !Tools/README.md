# Tools

This directory contains ready-made or hard-to-find tools that are useful for this project.

## External Tools

These tools were made by other people:

### The Dark Graphics (TDG)

**Folder:** [TDG23](TDG23)  
**Homepage:** (DEAD LINK, RIP) http://www.thedark5.com/forum/forum_posts.asp?TID=960&PN=1

_Run as Administrator!_ A slightly modified distribution of TDG to work on mondern Windows. Only difference is I've included `comdlg32.ocx` which is missing on most modern windows.

### Ladik's MPQ Editor v3.5.1.795

**Folder:** [mpqediten32_3.5.1.795](mpqediten32_3.5.1.795)  
**Homepage:** http://www.zezula.net/en/mpq/download.html

_Set Game Compatibility to_ Diablo I _!_ Only v3.5.1.795 (and maybe earlier) will be able to create/modify MPQs in a way that PR Demo can read.

### open-vcdiff

**File:** [vcdiff.exe](vcdiff.exe)  
**Homepage**: https://github.com/google/open-vcdiff

This is a binary diff creator/applicator. It's the backbone of the entire operation.

## Internal Tools

Tools made by me for this project:

* vcdiff-gooey: A Python Gooey front-end for open-vcdiff.
* [apply.bat](apply.bat): a sample script of using VCDIFF to apply patches in order.
* [splitcel](splitcel): A C++ program that explodes a CEL file into one file per frame ("celparts").
* [joincel](joincel): A C++ program that combines "celparts" back into a CEL file.
* [retail2prdemo](retail2prdemo): A C++ program for converting retail `l*data` into PR demo `l*data`.
* [storm-sample](storm-sample): An example of using PR Demo's STORM.DLL in a C++ program to extract files.
