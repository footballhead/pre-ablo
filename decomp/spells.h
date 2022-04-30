#ifndef __SPELLS_H__
#define __SPELLS_H__

#include <windows.h>

//
// defines
//

#define MAX_SPELLS 29

//
// enums
//

enum magic_type
{
    STYPE_FIRE = 0x0,
    STYPE_LIGHTNING = 0x1,
    STYPE_MAGIC = 0x2,
};

enum spell_id
{
    SPL_NULL = 0x0,
    SPL_FIREBOLT = 0x1,
    SPL_HEAL = 0x2,
    SPL_LIGHTNING = 0x3,
    SPL_FLASH = 0x4,
    SPL_IDENTIFY = 0x5,
    SPL_FIREWALL = 0x6,
    SPL_TOWN = 0x7,
    SPL_STONE = 0x8,
    SPL_INFRA = 0x9,
    SPL_RNDTELEPORT = 0xA,
    SPL_MANASHIELD = 0xB,
    SPL_FIREBALL = 0xC,
    SPL_GUARDIAN = 0xD,
    SPL_CHAIN = 0xE,
    SPL_WAVE = 0xF,
    SPL_DOOMSERP = 0x10,
    SPL_BLODRIT = 0x11,
    SPL_NOVA = 0x12,
    SPL_INVISIBIL = 0x13,
    SPL_SENTINEL = 0x14,
    SPL_GOLEM = 0x15,
    SPL_BLODBOIL = 0x16,
    SPL_TELEPORT = 0x17,
    SPL_APOCA = 0x18,
    SPL_ETHEREALIZE = 0x19,
    SPL_REPAIR = 0x1A,
    SPL_RECHARGE = 0x1B,
    SPL_DISARM = 0x1C,
};

//
// structs
//

#pragma pack(push, 8)
struct SpellStruct
{
    int type;
    int x;
    int y;
    BOOL animFlag;
    int field_10;   // Basically unused
    BYTE *animdata; // Basically unused
    int animDelay;  // How many ticks between frames
    int animCnt;    // Current progress towards a new frame
    int animLen;    // How many frames in animdata
    int animFrame;  // Current animation frame
    int animWidth;  // Width of animdata
    int animWidth2; // animWidth / 8
    int field_30;   // Basically unused
    int field_34;   // Basically unused
    int field_38;   // Basically unused
    int range;
    int dam;
    BOOL delFlag;
    int source;
    int var1;
    int var2;
    int var3;
    int var4;
    int var5;
    int var6;
    int var7;
    int var8;
};
#pragma pack(pop)

//
// variables
//

extern SpellStruct spells[MAX_SPELLS];
extern int spellsactive[MAX_SPELLS];
extern int numspells;
extern int availspells[MAX_SPELLS];

//
// functions
//

void InitSpellGFX();
void InitSpells();
int GetSpellLevel(int id, int sn);
int GetManaAmount(int id, int sn);
BOOL CheckSpell(int id, int sn, BOOL manaonly);
void CastSpell(int id, int sn, int sx, int sy, int dx, int dy);
void ProcessSpells();
void FreeSpells();

#endif