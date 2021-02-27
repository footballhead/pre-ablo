#pragma once

#include "structs.hpp"

extern MonsterData* const monsterdata;
extern MonsterStruct* const monster;
extern ObjDataStruct* const AllObjects;
extern ObjectStruct* const object; // array size == 127
extern QuestStruct* const quests;

extern int* const myplr;

extern int* const currlevel;

extern int* const setpc_x;
extern int* const setpc_y;
extern int* const setpc_w;
extern int* const setpc_h;

// TODO: Eventually the player struct will be fully defined and part of structs.hpp
extern DWORD* const plr_px;
extern DWORD* const plr_py;
extern DWORD* const plr_pSeedTbl; // array size == 17
extern BOOL* const plr_pLvlVisited; // array size == 17
