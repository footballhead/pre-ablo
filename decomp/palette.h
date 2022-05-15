#ifndef __PALETTE_H__
#define __PALETTE_H__

#include <windows.h>

#include "directx/DDRAW.H"

//
// variables
//

extern PALETTEENTRY system_palette[256];
extern PALETTEENTRY menu_palette[256];
extern BYTE palette_buffer[256 * 4];

//
// functions
//

void LoadPalette(char *pszFilename, BYTE *dest);
void CopyPalette(PALETTEENTRY* palette, BYTE* buffer);

#endif