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
// structs
//

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
    int monster_myoff;
    int monster_mxvel;
    int monster_myvel;
    int _mdir;
    int monster_menemy;
    int _mAnimData;
    int _mAnimDelay;
    int _mAnimCnt;
    int _mAnimLen;
    int _mAnimFrame;
    int anonymous_4;
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
    char anonymous_8;
    char anonymous_9;
    char anonymous_10[2];
    int _mflags;
    char anonymous_11;
    char anonymous_12[3];
    int anonymous_13;
    int anonymous_14;
    int anonymous_15;
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
    void *MType;
    int _MData;
};

struct AnimStruct
{
    BYTE *CMem; // Pointer to beginng of CEL data
    BYTE *Cels[8]; // CEL animation data for each direction
    int Frames; // # of frames for each direction
    int Rate; // delay between each frame
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

//
// variables
//

extern int nummonsters;
extern int monstactive[MAXMONSTERS];
extern MonsterStruct monster[MAXMONSTERS];

//
// functions
//

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

#endif