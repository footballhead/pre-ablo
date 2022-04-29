#ifndef __DIABLO_H__
#define __DIABLO_H__

#include <windows.h>

extern BYTE* gpBuffer;
extern char fileLoadPrefix[64];
extern BOOL cheat_mode;

void FreeGameMem();
void LoadGameLevel(BOOL firstflag, int lvldir, BOOL first_flag);

#endif