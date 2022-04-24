#include "palette.h"

#include "diablo.h"
#include "storm.h"

#include <stdio.h>

//
// uninitialized vars (.data:0061A94C)
//

DWORD dword_61A94C;
UINT delayed_Msg;
DWORD dword_61A954;
PALETTEENTRY system_palette[256];
BYTE *palette_buffer[256 * 4]; // Array of tuples: (r, g, b, unused)
DWORD dword_61B160[32];
DWORD dword_61B1E0;
PALETTEENTRY menu_palette[256];
DWORD fade_state;

//
// code (text:00481FB0)
//

// .text:00481FB0
void LoadPalette(char *pszFilename, BYTE *dest)
{
    HANDLE hFile;
    int i;
    char path[256];

    sprintf(path, "%s%s", fileLoadPrefix, pszFilename);
    while (SFileOpenFile(path, &hFile) == FALSE)
    {
        // do nothing... keep trying forever until we can open!
    }

    for (i = 0; i < 256; i++)
    {
        SFileReadFile(hFile, i * 4 + dest, 1, NULL, NULL);
        SFileReadFile(hFile, i * 4 + dest + 1, 1, NULL, NULL);
        SFileReadFile(hFile, i * 4 + dest + 2, 1, NULL, NULL);
        dest[i * 4 + 3] = 0; // The third byte is never used... maybe just for alignment?
    }

    SFileCloseFile(hFile);
}

// DoFadeIn	000000000048209F
// DoFadeOut	00000000004821E1
// PaletteFadeIn	000000000048233A
// PaletteFadeOut	000000000048237B
// CopyPalette	00000000004823BC
// ApplyGamma	0000000000482430
// LoadRndLvlPal	00000000004825A6
// palette_482623_cycle	0000000000482623
// palette_482672_cycle_caves	0000000000482672
// palette_update	00000000004826F0
