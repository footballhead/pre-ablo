#ifndef __DIABLO_H__
#define __DIABLO_H__

#include <windows.h>

extern BYTE* gpBuffer;

void FreeGameMem();
void LoadGameLevel(BOOL firstflag, int lvldir, BOOL first_flag);

#endif