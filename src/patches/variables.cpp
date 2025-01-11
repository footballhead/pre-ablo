#include "variables.hpp"

MonsterData* const monsterdata = reinterpret_cast<MonsterData*>(0x004AE410);
char* const MonstAvailTbl = reinterpret_cast<char*>(0x004B0D20);
MonsterStruct* const monster = reinterpret_cast<MonsterStruct*>(0x004BE4E8);
ObjDataStruct* const AllObjects =
    reinterpret_cast<ObjDataStruct* const>(0x004AB578);
ObjectStruct* const object = reinterpret_cast<ObjectStruct* const>(0x006002B0);
QuestStruct* const quests = reinterpret_cast<QuestStruct* const>(0x00615F40);
ItemDataStruct* const AllItemsList =
    reinterpret_cast<ItemDataStruct* const>(0x004A3B00);
MissileStruct* const missile =
    reinterpret_cast<MissileStruct* const>(0x005F7268);

int& currlevel = *reinterpret_cast<int*>(0x004CB0DC);
int* const leveltype = reinterpret_cast<int* const>(0x004E3648);

int* const setpc_x = reinterpret_cast<int* const>(0x005030EC);
int* const setpc_y = reinterpret_cast<int* const>(0x005030F0);
int* const setpc_w = reinterpret_cast<int* const>(0x005030E8);
int* const setpc_h = reinterpret_cast<int* const>(0x005030E0);

BYTE** const pShatter1Cel = reinterpret_cast<BYTE** const>(0x005F71D8);
dMonster_& dMonster = *reinterpret_cast<dMonster_*>(0x005670F8);
dPiece_& dPiece = *reinterpret_cast<dPiece_*>(0x004ECAB8);
SpellStruct* const spells = reinterpret_cast<SpellStruct* const>(0x005FF2E8);
char* const CrawlTable = reinterpret_cast<char* const>(0x004ACC00);
PlayerStruct* const plr = reinterpret_cast<PlayerStruct* const>(0x00605478);
int* const L2UpList = reinterpret_cast<int* const>(0x004B9C48);
int* const L2DownList = reinterpret_cast<int* const>(0x004B9C58);
int& cursmx = *reinterpret_cast<int*>(0x00619500);
int& cursmy = *reinterpret_cast<int*>(0x00619504);
int& numtrigs = *reinterpret_cast<int*>(0x00616028);
char* const infostr = reinterpret_cast<char* const>(0x005DDDC0);
TriggerStruct* const trigs = reinterpret_cast<TriggerStruct* const>(0x00616030);
