#ifndef __DIABLO_H__
#define __DIABLO_H__

#include <windows.h>
#include "directx/DDRAW.H"

//
// defines
//

// Causes a repaint. E.g. progress fade, draw screen, etc
#define WM_DIABPAINT WM_USER
#define WM_DIABMODEINIT WM_USER + 8
#define WM_DIABMODEEXIT WM_USER + 9
#define WM_40C WM_USER + 12
#define WM_DIABLOADGAME WM_USER + 13

// TODO need to figure out these events. I think it's something like:
// - 0x408 is like "mode enter", it does init and starts fade in
// - 0x409 is like "mode exit", it's typically delayed until after fade out
// - 0x40C is ??? posted at end of quotes

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
extern LPDIRECTDRAWSURFACE lpDDSPrimary;
extern LPDIRECTDRAWPALETTE lpDDPalette;
extern int gMode;
extern BOOL shouldStopPaintTimer;
extern UINT paint_event_timer_resolution;
extern BOOL paint_callback_mutex;
extern BOOL did_paint_PostMessage;
extern int MouseX;
extern int MouseY;

//
// functions
//

void FreeGameMem();
void LoadGameLevel(BOOL firstflag, int lvldir, BOOL first_flag);
void set_did_paint_PostMessage(BOOL b);

#endif