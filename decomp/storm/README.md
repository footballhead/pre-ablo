# STORM

Tools and stuff related to using demo storm.dll.

WARNING: storm.def is incomplete! It still requires manual adjustment!

## Header

storm.h is the header of functions used by the demo decomp.

Most of it is copy-pasted from Devilution.

## Library

We don't have an original storm.lib (and probably never will) so decomp can't
link against storm.dll...

... Except we can make our own .lib based on .dll contents! The demo Storm.dll
includes both names and oridinals.

Based on https://stackoverflow.com/questions/9946322/how-to-generate-an-import-library-lib-file-from-a-dll:

1. `dumpbin /exports storm.dll`
2. Massage the output into a storm.def file
3. `lib /def:storm.def /out:storm.lib /machine:x86`

(.DEF reference: https://docs.microsoft.com/en-us/cpp/build/reference/module-definition-dot-def-files?view=msvc-170)

I have tools for 1 and 2. (See next section) 3 must be run manually.

Unfortunately, step 2 is slighty harder than it appears. Storm functions are
declared `__stdcall` which means name decoration happens (reference:
https://docs.microsoft.com/en-us/cpp/cpp/stdcall?view=msvc-170). In a perfect
world, we'd just rearrange the dumpbin output. However, now we have to postfix
names with `@XYZ` where XYZ is the number of bytes that the caller pushes onto
the stack. (The prefix is handled by the compiler for ??? reasons).

(I'm unsure why Devilution doesn't have this problem.)

## Tools

### make_storm_def.py

This runs dumpbin on storm.dll and massages the output to produce a base .DEF.
It does not include name-decoration!

You shouldn't need to run this. The .DEF included in this repo has manual
modifications and should be good enough to make storm.lib. Rerunning this script
destroys those modifications!

### compile_test.bat

Compiles test.c using storm.h and storm.lib.
