#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// This was painstakingly reconstructed with IDA and Devilution
struct MonsterData {
    int mAnimWidth;
    int mImgSize;
    void *GraphicType;
    int has_special;
    void *sndfile;
    int snd_special;
    int transflag;
    void *TransFile;
    int Frames[6];
    int Rate[6];
    void *mName;
    char mMinDLvl;
    char mMaxDLvl;
    char mLevel;
    char mMinHP;
    char mMaxHP;
    char mAi;
    char mInt;
    char mHit;
    char mAFNum;
    char mMinDamage;
    char mMaxDamage;
    char mHit2;
    char mAFNum2;
    char mMinDamage2;
    char mMaxDamage2;
    char mArmorClass;
    char mMonstClass;
    // padding
    WORD mMagicRes;
    WORD mTreasure;
    char mSelFlag;
    // padding
    int mExp;
};

// To make offset math work, we need the compiled MonsterData size to match what's already there.
// This will probably only work on 32 bit machines that use 4 byte alignment
static_assert(sizeof(MonsterData) == 0x70, "MonsterData compiled to wrong size");
