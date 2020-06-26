#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern BYTE* (__fastcall * LoadFileInMem)(const char* pszName);
