#ifndef __DIABLO_H__
#define __DIABLO_H__

#include <windows.h>

//
// defines
//

#define WM_DIABPAINT WM_USER

//
// variables
//

extern BYTE* gpBuffer;
extern char fileLoadPrefix[64];
extern BOOL cheat_mode;
extern BOOL gbActive;
extern BOOL shouldStopPaintTimer;
extern BOOL demo_mode;
extern BOOL debug_mode;
extern char savedir_abspath[64];
extern int force_redraw;

//
// functions
//

void FreeGameMem();
void LoadGameLevel(BOOL firstflag, int lvldir, BOOL first_flag);

#endif