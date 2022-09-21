#ifndef __PALETTE_H__
#define __PALETTE_H__

#include <windows.h>

#include "directx/DDRAW.H"

//
// variables
//

extern PALETTEENTRY system_palette[256];
extern PALETTEENTRY orig_palette[256];
extern PALETTEENTRY logical_palette[256];
extern DWORD fade_state;

//
// functions
//

void LoadPalette(char *pszFilename, PALETTEENTRY *dest);
void CopyPalette(PALETTEENTRY* dest, PALETTEENTRY* src);
void PaletteFadeIn(int delta);
void ResetPal();

#endif