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

struct MonsterStruct {
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
    int monster_mAnimDelay;
    int _mAnimCnt;
    int _mAnimLen;
    int _mAnimFrame;
    int anonymous_4;
    int anonymous_5;
    int monster_mVar1;
    int monster_mVar2;
    int monster_mVar3;
    int monster_mVar4;
    int monster_mVar5;
    int monster_mVar6;
    int monster_mVar7;
    int monster_mVar8;
    int anonymous_6;
    int anonymous_7;
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
    char anonymous_18;
    char anonymous_19;
    WORD anonymous_20;
    char anonymous_21;
    char anonymous_22;
    char anonymous_23;
    char anonymous_24;
    char anonymous_25;
    char anonymous_26;
    char anonymous_27;
    char anonymous_28;
    WORD anonymous_29;
    char anonymous_30;
    char anonymous_31;
    char anonymous_32;
    char anonymous_33;
    int anonymous_34;
    void *_mtype;
    int _MData;
};
static_assert(sizeof(MonsterStruct) == 0xC4, "MonsterStruct compiled to wrong size");

struct ObjDataStruct {
    char oload;
    char ofindex;
    char ominlvl;
    char omaxlvl;
    char olvltype;
    char otheme;
    int oAnimFlag;
    int oAnimDelay;
    int oAnimLen;
    int oAnimWidth;
    int oSolidFlag;
    int oMissFlag;
    int oLightFlag;
    char oBreak;
    char oSelFlag;
    int oTrapFlag;
};
static_assert(sizeof(ObjDataStruct) == 0x2C, "MonsterStruct compiled to wrong size");

struct QuestStruct {
    BYTE _qlevel;
    BYTE _qtype;
    BYTE _qactive;
    BYTE unknown_03;
    DWORD _qtx;
    DWORD _qty;
    BYTE unknown_0C;
    BYTE unknown_0D;
    BYTE unknown_0E;
    BYTE unknown_0F;
};
static_assert(sizeof(QuestStruct) == 0x10, "QuestStruct compiled to wrong size");

// Copied from DevilutionX
struct ObjectStruct {
	int _otype;
	int _ox;
	int _oy;
	int _oLight;
	int _oAnimFlag;
	unsigned char *_oAnimData;
	int _oAnimDelay; // Tick length of each frame in the current animation
	int _oAnimCnt;   // Increases by one each game tick, counting how close we are to _pAnimDelay
	int _oAnimLen;   // Number of frames in current animation
	int _oAnimFrame; // Current frame of animation.
	int _oAnimWidth;
	int _oAnimWidth2;
	BOOL _oDelFlag;
	char _oBreak; // check
	BOOL _oSolidFlag;
	BOOL _oMissFlag;
	char _oSelFlag; // check
	BOOL _oPreFlag;
	BOOL _oTrapFlag;
	BOOL _oDoorFlag;
	int _olid;
	// int _oRndSeed; // Not in PR Demo
	int _oVar1;
	int _oVar2;
	int _oVar3;
	int _oVar4;
	int _oVar5;
	int _oVar6;
	int _oVar7;
	int _oVar8;
};
static_assert(sizeof(ObjectStruct) == 0x74, "ObjectStruct compiled to wrong size");

struct ItemDataStruct
{
  int iRnd;
  char iClass;
  char iLoc;
  //char __padding_0[2];
  int iCurs; // This is typically read as a byte (e.g. loaded into `al`)
  //char __padding_1[3];
  char itype;
  char iItemId;
  //char __padding_2[2];
  const char *iName;
  const char *iSName;
  char iMinMLvl;
  //char __padding_3[3];
  int iDurability;
  int iMinDam;
  int iMaxDam;
  int iMinAC;
  int iMaxAC;
  char iMinStr;
  char iMinMag;
  char iMinDex;
  //char __padding_4;
  int iFlags;
  int iMiscId;
  int iSpell;
  BOOL iUsable;
  int iValue;
};
static_assert(sizeof(ItemDataStruct) == 0x48, "ItemDataStruct compiled to wrong size");
