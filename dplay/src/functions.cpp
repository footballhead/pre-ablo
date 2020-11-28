#include "functions.hpp"

BYTE* (__fastcall * LoadFileInMem)(const char* pszName) = reinterpret_cast<BYTE* (__fastcall *)(const char* pszName)>(0x00482790);

void (__fastcall * AddL1Objs)(int, int, int, int) = reinterpret_cast<void (__fastcall *)(int, int, int, int)>(0x00459015);
void (__fastcall * AddMazeBook)(int, int, int, int, int, int, int, int, int) = reinterpret_cast<void (__fastcall *)(int, int, int, int, int, int, int, int, int)>(0x00458D47);
