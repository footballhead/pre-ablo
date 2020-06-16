# DPLAY.DLL was not found

tl;dr: Download our dplay.dll from here: https://gitlab.com/moralbacteria/dplaywrapper/-/releases

Running DIABLO.EXE will most likely show this error dialog:

> The code execution cannot proceed because DPLAY.dll was not found. Reinstalling the program may fix this problem. 

The Pre-Release Demo contains early netcode (even though it's mostly unused) that is built on DirectPlay. Given the binary date of 8/16/1996, it's either DirectX 1 or 2.

We've built a custom dplay.dll that works. It can be downloaded from this GitLab repo: https://gitlab.com/moralbacteria/dplaywrapper/-/releases
