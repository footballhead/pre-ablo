#ifndef __MONSTER_H__
#define __MONSTER_H__

#include <windows.h>

//
// defines
//

#define MAXMONSTERS 200

//
// structs
//

#pragma pack(push, 8)
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
    int monster_mVar1;
    int monster_mVar2;
    int monster_mVar3;
    int monster_mVar4;
    int monster_mVar5;
    int monster_mVar6;
    int monster_mVar7;
    int monster_mVar8;
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
#pragma pack(pop)

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

#endif