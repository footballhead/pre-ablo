#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern BYTE* (__fastcall * LoadFileInMem)(const char* pszName);

extern void (__fastcall * AddL1Objs)(int, int, int, int);
extern void (__fastcall * AddMazeBook)(int, int, int, int, int, int, int, int, int);
