#ifndef __DIABLO_H__
#define __DIABLO_H__

#include <windows.h>
#include "directx/DDRAW.H"

//
// defines
//

// Causes a repaint
#define WM_DIABPAINT WM_USER
// Triggers a transition to the next screen (e.g. blizzard logo to title screen,
// etc)
#define WM_DIABNEXTMODE WM_USER + 8
// ... Or just trigger initialization of that screen???
#define WM_DIABMODEINIT WM_USER + 8

//
// variables
//

extern BYTE *gpBuffer;
extern char fileLoadPrefix[64];
extern BOOL cheat_mode;
extern BOOL gbActive;
extern BOOL shouldStopPaintTimer;
extern BOOL demo_mode;
extern BOOL debug_mode;
extern char savedir_abspath[64];
extern int force_redraw;
extern BOOL debugMusicOn;
extern HANDLE sghMusic;
extern LPDIRECTDRAWSURFACE lpDDSBackBuf;
extern LPDIRECTDRAWPALETTE lpDDPalette;

//
// functions
//

void FreeGameMem();
void LoadGameLevel(BOOL firstflag, int lvldir, BOOL first_flag);

#endif