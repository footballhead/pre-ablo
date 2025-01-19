#pragma once

#include "structs.hpp"

extern MonsterData* const monsterdata;
extern char* const MonstAvailTbl;
extern MonsterStruct* const monster;
extern ObjDataStruct* const AllObjects;
extern ObjectStruct* const object;  // array size == 127
extern QuestStruct* const quests;
extern ItemDataStruct* const AllItemsList;
extern MissileStruct* const missile;  // array size == 125

extern int& currlevel;
extern int* const leveltype;

extern int* const setpc_x;
extern int* const setpc_y;
extern int* const setpc_w;
extern int* const setpc_h;

extern BYTE** const pShatter1Cel;
using dMonster_ = int[112][112];
extern dMonster_& dMonster;
using dPiece_ = int[112][112];
extern dPiece_& dPiece;
extern SpellStruct* const spells;
extern char* const CrawlTable;
extern PlayerStruct* const plr;
extern int* const L2UpList;
extern int* const L2DownList;
extern int& cursmx;
extern int& cursmy;
extern int& numtrigs;
extern char* const infostr;
extern TriggerStruct* const trigs;

inline DWORD& prev_timer_PostMessage_time =
    *reinterpret_cast<DWORD*>(0x0061BFD4);
inline HWND& ghMainWnd = *reinterpret_cast<HWND*>(0x0061B72C);
inline DWORD& prevTime = *reinterpret_cast<DWORD*>(0x0061BF70);
inline DWORD& main_loop_iteration_count = *reinterpret_cast<DWORD*>(0x0061B704);
inline DWORD& frames = *reinterpret_cast<DWORD*>(0x0061B708);
inline DWORD& last_fps = *reinterpret_cast<DWORD*>(0x0061B70C);
inline DWORD& last_main_loop_iteration_count =
    *reinterpret_cast<DWORD*>(0x0061B710);