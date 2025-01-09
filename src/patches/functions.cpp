#include "functions.hpp"

BYTE* (__fastcall * LoadFileInMem)(const char* pszName) = reinterpret_cast<BYTE* (__fastcall *)(const char* pszName)>(0x00482790);

void (__fastcall * AddL1Objs)(int, int, int, int) = reinterpret_cast<void (__fastcall *)(int, int, int, int)>(0x00459015);
void (__fastcall * AddMazeBook)(int, int, int, int, int, int, int, int, int) = reinterpret_cast<void (__fastcall *)(int, int, int, int, int, int, int, int, int)>(0x00458D47);
void (__fastcall * SpawnUnique)(int, int, int) = reinterpret_cast<void (__fastcall *)(int, int, int)>(0x00421CDC);
void (__fastcall * NetSendCmd)(int, BYTE, BYTE, int, int) = reinterpret_cast<void (__fastcall *)(int, BYTE, BYTE, int, int)>(0x0046ED1E);
void (__fastcall * SetPlrAnims)(int) = reinterpret_cast<void (__fastcall *)(int)>(0x0046652F);
void (__fastcall * SyncMonsterAnim)(int) = reinterpret_cast<void (__fastcall *)(int)>(0x0040BFA3);
