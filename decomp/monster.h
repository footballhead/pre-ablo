#ifndef __MONSTER_H__
#define __MONSTER_H__

#include <windows.h>

#include "monstdat.h"

//
// defines
//

#define MAXMONSTERS 200
#define MAX_LVLMTYPES 16

//
// enums
//

// Behaviors as part of the monster state machine. See ProcessMonsters
enum MON_MODE
{
    MM_STAND = 0,
    MM_WALK = 1,
    MM_WALK2 = 2,
    MM_WALK3 = 3,
    MM_ATTACK = 4,
    MM_GOTHIT = 5,
    MM_DEATH = 6,
    MM_SATTACK = 7,
    MM_FADEIN = 8,
    MM_FADEOUT = 9,
    MM_RATTACK = 10,
    MM_SPSTAND = 11,
    MM_RSPATTACK = 12,
    MM_DELAY = 13,
    MM_CHARGE = 14,
    MM_STONE = 15, // Monster will act like it was turned to stone
    MM_HEAL = 16,
};

// TODO: Copied from Devilution, verify
typedef enum monster_flag
{
    MFLAG_HIDDEN = 0x01,
    // If set then monster _mAnimFrame won't progress. This stops the animation
    // Used by gargoyles and hidden
    MFLAG_LOCK_ANIMATION = 0x02, // verified (see ProcessMonsters)
    MFLAG_ALLOW_SPECIAL = 0x04, // TODO
    MFLAG_NOHEAL = 0x08,
    MFLAG_TARGETS_MONSTER = 0x10,
    MFLAG_GOLEM = 0x20,
    MFLAG_QUEST_COMPLETE = 0x40,
    MFLAG_KNOCKBACK = 0x80,
    MFLAG_SEARCH = 0x100,
    MFLAG_CAN_OPEN_DOOR = 0x200,
    MFLAG_NO_ENEMY = 0x400,
    MFLAG_BERSERK = 0x800,
    MFLAG_NOLIFESTEAL = 0x1000
} monster_flag;

//
// structs
//

struct AnimStruct
{
    BYTE *CMem;    // Pointer to beginng of CEL data
    BYTE *Cels[8]; // CEL animation data for each direction
    int Frames;    // # of frames for each direction
    int Rate;      // delay between each frame
};

// TODO: TSnd struct and Snds var
struct CMonster
{
    char mtype;
    AnimStruct Anims[6];
    DWORD field_10C;
    DWORD field_110;
    DWORD field_114;
    char Snds;        // TODO: Define TSnd (size == 48?)
    BYTE gap119[387]; // TODO: Define TSnd
    int width;
    int width2;
    unsigned char mMinHP;
    unsigned char mMaxHP;
    int has_special;
    char mAFNum;
    char mdeadval;
    MonsterData *MData;
    BYTE *trans_file;
};

struct MonsterStruct
{
    int _mMTidx;
    int _mmode;
    char anonymous_0;
    char anonymous_1[3];
    int anonymous_2;
    int anonymous_3;
    BYTE gap14[8];
    int _mx;
    int _my;
    int _moldx;
    int _moldy;
    int _mxoff;
    int _myoff;
    int _mxvel;
    int _myvel;
    int _mdir;
    int _menemy;
    int _mAnimData;
    int _mAnimDelay;
    int _mAnimCnt;
    int _mAnimLen;
    int _mAnimFrame;
    // TRUE if monster is occluded by another tile (e.g. behind a wall)?
    BOOL _meflag;
    int _mDelFlag;
    int _mVar1;
    int _mVar2;
    int _mVar3;
    int _mVar4;
    int _mVar5;
    int _mVar6;
    int _mVar7;
    int _mVar8;
    int _mmaxhp;
    int _mhitpoints;
    char _mAi;
    char anonymous_9;
    char anonymous_10[2];
    int _mFlags;
    BYTE _msquelch;
    char anonymous_12[3];
    int anonymous_13;
    int _lastx;
    int _lasty;
    BYTE gapA0[4];
    char _uniqtype;
    char _uniqtrans;
    char _udeadval;
    char mLevel;
    __int16 mExp;
    char mHit;
    char mMinDamage;
    char mMaxDamage;
    char mHit2;
    char mMinDamage2;
    char mMaxDamage2;
    char mArmorClass;
    char anonymous_28;
    __int16 mMagicRes;
    char anonymous_30;
    char anonymous_31;
    char anonymous_32;
    char anonymous_33;
    int anonymous_34;
    CMonster *MType;
    int _MData;
};

//
// variables
//

extern int nummtypes;
extern int nummonsters;
extern int monstactive[MAXMONSTERS];
extern int monstkills[MAXMONSTERS];
extern MonsterStruct monster[MAXMONSTERS];
extern CMonster Monsters[MAX_LVLMTYPES];

//
// functions
//

void InitMonsterGFX();

void M_StartKill(int i, int pnum);

void MAI_Zombie(int i);
void MAI_Fat(int i);
void MAI_SkelSd(int i);
void MAI_SkelBow(int i);
void MAI_Scav(int i);
void MAI_Rhino(int i);
void MAI_GoatMc(int i);
void MAI_GoatBow(int i);
void MAI_Fallen(int i);
void MAI_Magma(int i);
void MAI_SkelKing(int i);
void MAI_Bat(int i);
void MAI_Garg(int i);
void MAI_Cleaver(int i);
void MAI_Succ(int i);
void MAI_Sneak(int i);
void MAI_Storm(int i);

void SpawnSkeleton(int x, int y);
void FreeMonsters();

#endif