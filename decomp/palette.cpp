#include "palette.h"

#include "diablo.h"
#include "engine.h"
#include "enums.h"
#include "storm/storm.h"

#include <stdio.h>
#include <math.h>

//
// initialized vars (.data:004BBF38)
//

// wtf is this? nobody references it
static char registration_block[128] = "REGISTRATION_BLOCK";
double gamma_correction = 1.0;
double gamma_backup = 1.0;

//
// uninitialized vars (.data:0061A548)
//

// Only used by ShowProgress
PALETTEENTRY progress_palette[256];
// can't tell if padding or unused
DWORD unused_61A948;
// Which caves_pal_red to treat as the "first" red, wraps at 32
DWORD caves_pal_offset;
// While unused here, this looks like a message that is posted after fade out
UINT delayed_Msg;
// How much to fade each redraw
DWORD fade_delta;
// Colors with effects e.g. fade (actually used)
PALETTEENTRY system_palette[256];
// From file (always given to LoadPalette)
PALETTEENTRY orig_palette[256];
// Copy of first 32 reds to cycle through (baed on caves_pal_index)
DWORD caves_pal_red[32];
// Current fade progress (0-256)
DWORD fade_val;
// Palette that always seems to be involved in fade operations
PALETTEENTRY logical_palette[256];
// 0 is no fade, 1 is fade in, 2 is fade out (use PALLETE_FADE defines)
DWORD fade_state;

//
// code (text:00481FB0)
//

// .text:00481FB0
// Load a .PAL file into memory. Hangs if the file doesn't exist.
//
// (A .PAL file encodes an array of 256 RGB tuples, 768 total bytes)
//
// pszFilename: file to load (from MPQ, or disk)
// dest: 256 element array of colors (always orig_palette)
void LoadPalette(char *pszFilename, PALETTEENTRY *dest)
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
        SFileReadFile(hFile, &dest[i].peRed, 1, NULL, NULL);
        SFileReadFile(hFile, &dest[i].peGreen, 1, NULL, NULL);
        SFileReadFile(hFile, &dest[i].peBlue, 1, NULL, NULL);
        dest[i].peFlags = 0; // Never used, thrown away by subsequent operations
    }

    SFileCloseFile(hFile);
}

// .text:0048209F
// Perform one step of fade in. Uses fade_val for progress, fade_delta for how
// fast to fade. Resets fade_state at the end.
void DoFadeIn()
{
    int i;
    DWORD temp; // this might be compiler generated idk

    if (fade_val != 256)
    {
        for (i = 0; i < 256; i++)
        {
            temp = ((logical_palette[i].peRed * fade_val) >> 8) & 0xFF;
            system_palette[i].peRed = temp;
            temp = ((logical_palette[i].peGreen * fade_val) >> 8) & 0xFF;
            system_palette[i].peGreen = temp;
            temp = ((logical_palette[i].peBlue * fade_val) >> 8) & 0xFF;
            system_palette[i].peBlue = temp;
        }
        fade_val += fade_delta;
    }
    else
    {
        for (i = 0; i < 256; i++)
        {
            system_palette[i].peRed = logical_palette[i].peRed;
            system_palette[i].peGreen = logical_palette[i].peGreen;
            system_palette[i].peBlue = logical_palette[i].peBlue;
        }
        fade_state = PALETTE_NO_FADE;
    }
}

// .text:004821E1
// Perform one step of fade out. Uses fade_val for progress, fade_delta for how
// fast to fade. Resets fade_state at the end.
void DoFadeOut()
{
    DWORD temp; // this might be compiler generated idk
    int i;

    if (fade_val != 256)
    {
        for (i = 0; i < 256; i++)
        {
            temp = ((logical_palette[i].peRed * fade_val) >> 8) & 0xFF;
            system_palette[i].peRed = temp;
            temp = ((logical_palette[i].peGreen * fade_val) >> 8) & 0xFF;
            system_palette[i].peGreen = temp;
            temp = ((logical_palette[i].peBlue * fade_val) >> 8) & 0xFF;
            system_palette[i].peBlue = temp;
        }
    }
    else
    {
        for (i = 0; i < 256; i++)
        {
            system_palette[i].peRed = logical_palette[i].peRed;
            system_palette[i].peGreen = logical_palette[i].peGreen;
            system_palette[i].peBlue = logical_palette[i].peBlue;
        }
    }
    if (fade_val)
    {
        fade_val -= -(0 - fade_delta);
    }
    else
    {
        fade_state = PALETTE_NO_FADE;
    }
}

// .text:0048233A
// Start fade from black. Progress by calling DoFadeIn
void PaletteFadeIn(int delta)
{
    fade_delta = delta;
    fade_state = PALETTE_FADE_IN;
    fade_val = 0;
    DoFadeIn();
    fade_val = 0;
}

// .text:0048237B
// Start fade to black. Progress by calling DoFadeOut
void PaletteFadeOut(int delta)
{
    fade_delta = delta;
    fade_state = PALETTE_FADE_OUT;
    fade_val = 256;
    DoFadeOut();
    fade_val = 256;
}

// .text:004823BC
// Copy src into dest. Keeps RGB values, throws away peFlags
//
// dest: destination, where to copy to
// src: data source, where to copy from
void CopyPalette(PALETTEENTRY *dest, PALETTEENTRY *src)
{
    int i;
    for (i = 0; i < 256; ++i)
    {
        dest[i].peRed = src[i].peRed;
        dest[i].peGreen = src[i].peGreen;
        dest[i].peBlue = src[i].peBlue;
    }
}

// .text:00482430
// Apply gamma correction of elements between start and end to the given palette.
void ApplyGamma(PALETTEENTRY *pal, int start, int end)
{
    int i;

    for (i = start; i < end; i++)
    {
        pal[i].peRed = pow(pal[i].peRed / 256.0, gamma_correction) * 256.0;
        pal[i].peGreen = pow(pal[i].peGreen / 256.0, gamma_correction) * 256.0;
        pal[i].peBlue = pow(pal[i].peBlue / 256.0, gamma_correction) * 256.0;
    }
}

// .text:004825A6
// Pick a random level palette and load it.
void LoadRndLvlPal(int l)
{
    int rv;
    char szFileName[256];

    if (l == DTYPE_TOWN)
    {
        strcpy(szFileName, "Levels\\TownData\\Town.pal");
    }
    else
    {
        rv = random_(4) + 1;
        sprintf(szFileName, "Levels\\L%iData\\L%i_%i.PAL", l, l, rv);
    }
    LoadPalette(szFileName, orig_palette);
    CopyPalette(logical_palette, orig_palette);
}

// .text:00482623
// Backup the red channel and reset offset index
void palette_init_caves()
{
    int i;

    caves_pal_offset = 1;
    for (i = 0; i < 32; i++)
    {
        caves_pal_red[i] = orig_palette[i].peRed;
    }
}

// .text:00482672
// Update reds based on current offset and advance to next red
void palette_update_caves()
{
    int src_i, dest_i;

    src_i = caves_pal_offset;
    for (dest_i = 1; dest_i < 32; dest_i++)
    {
        system_palette[dest_i].peRed = caves_pal_red[src_i];

        src_i++;
        if (src_i == 32)
        {
            src_i = 1;
        }
    }

    caves_pal_offset++;
    if (caves_pal_offset == 32)
    {
        caves_pal_offset = 1;
    }
}

// .text:004826F0
// Restore the lost surface and palette
void ResetPal()
{
    if (lpDDSBackBuf)
    {
        if (lpDDSBackBuf->IsLost() == DDERR_SURFACELOST)
        {
            lpDDSBackBuf->Restore();
        }

        if (lpDDPalette)
        {
            CopyPalette(system_palette, orig_palette);
            ApplyGamma(system_palette, 0, 256);
            lpDDPalette->SetEntries(0, 0, 256, system_palette);
        }
    }
}
