#ifndef __PALETTE_H__
#define __PALETTE_H__

#include <windows.h>

#include "directx/DDRAW.H"

//
// defines
//

#define PALETTE_NO_FADE 0
#define PALETTE_FADE_IN 1
#define PALETTE_FADE_OUT 2

//
// variables
//

extern PALETTEENTRY system_palette[256];
extern PALETTEENTRY orig_palette[256];
extern PALETTEENTRY logical_palette[256];
extern DWORD fade_state;
extern UINT delayed_Msg;

//
// functions
//

void LoadPalette(char *pszFilename, PALETTEENTRY *dest);
void DoFadeIn();
void DoFadeOut();
void PaletteFadeIn(int delta);
void PaletteFadeOut(int delta);
void CopyPalette(PALETTEENTRY* dest, PALETTEENTRY* src);
void ApplyGamma(PALETTEENTRY *pal, int start, int end);
void LoadRndLvlPal(int l);
void palette_init_caves();
void palette_update_caves();
void ResetPal();

#endif