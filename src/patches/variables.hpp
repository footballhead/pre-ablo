#pragma once

#include "structs.hpp"

extern MonsterData* const monsterdata;
extern char* const MonstAvailTbl;
extern MonsterStruct* const monster;
extern ObjDataStruct* const AllObjects;
extern ObjectStruct* const object; // array size == 127
extern QuestStruct* const quests;
extern ItemDataStruct* const AllItemsList;
extern MissileStruct* const missile; // array size == 125

extern int* const currlevel;
extern int* const leveltype;

extern int* const setpc_x;
extern int* const setpc_y;
extern int* const setpc_w;
extern int* const setpc_h;

extern BYTE** const pShatter1Cel;
using dMonster_ = int[112][112];
extern dMonster_& dMonster;
extern SpellStruct* const spells;
extern char* const CrawlTable;
extern PlayerStruct* const plr;