#include "palette.h"

//
// uninitialized vars (.data:0061A94C)
//

DWORD dword_61A94C;
UINT delayed_Msg;
DWORD dword_61A954;
PALETTEENTRY system_palette[256];
DWORD palette_buffer[256];
DWORD dword_61B160[32];
DWORD dword_61B1E0;
PALETTEENTRY menu_palette[256];
DWORD fade_state;

//
// code
//

// LoadPalette	0000000000481FB0	
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
