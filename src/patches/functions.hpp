#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern BYTE* (__fastcall * LoadFileInMem)(const char* pszName);

extern void (__fastcall * AddL1Objs)(int, int, int, int);
extern void (__fastcall * AddMazeBook)(int, int, int, int, int, int, int, int, int);
extern void (__fastcall * SpawnUnique)(int, int, int);
// pnum (int), action (byte), v1 (byte), v2 (int), v3 (int)
extern void (__fastcall * NetSendCmd)(int, BYTE, BYTE, int, int);
extern void (__fastcall * SetPlrAnims)(int);
extern void (__fastcall * SyncMonsterAnim)(int);

using cast_bloodboil_ = void(__fastcall*)(int id, int si, int sx, int sy,
                                          int dx, int dy);
extern cast_bloodboil_ cast_bloodboil;
