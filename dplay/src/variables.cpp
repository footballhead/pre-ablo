#include "variables.hpp"

MonsterData* const monsterdata = reinterpret_cast<MonsterData*>(0x004AE410);
MonsterStruct* const monster = reinterpret_cast<MonsterStruct*>(0x004BE4E8);
ObjDataStruct* const AllObjects = reinterpret_cast<ObjDataStruct* const>(0x004AB578);
QuestStruct* const quests = reinterpret_cast<QuestStruct* const>(0x00615F40);

int* const myplr = reinterpret_cast<int* const>(0x00615E38);

int* const currlevel = reinterpret_cast<int* const>(0x004CB0DC);

int* const setpc_x = reinterpret_cast<int* const>(0x005030EC);
int* const setpc_y = reinterpret_cast<int* const>(0x005030F0);
int* const setpc_w = reinterpret_cast<int* const>(0x005030E8);
int* const setpc_h = reinterpret_cast<int* const>(0x005030E0);
