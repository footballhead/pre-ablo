// This file is for pre-release demo DIABLO.EXE structs. Currently this is
// bootstrapped from IDA exports.

#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <windows.h>

_STATIC_ASSERT(sizeof(void*) == 4);

// IDA export quirk: gaps of undefined data use this type, which is 8-bits
typedef char _BYTE;

#pragma pack(push, 8)
typedef struct _ItemStruct
{
  int _itype;
  int _ix;
  int _iy;
  int field_C;
  int _iAnimData;
  int field_14;
  int _iAnimFrame;
  int _iAnimWidth;
  int field_20;
  int field_24;
  char field_28;
  int field_2C;
  int _iIdentified;
  char _iMagical;
  char _iName[64];
  char _iIName[64];
  char _iLoc;
  char _iClass;
  char _iCurs;
  int _ivalue;
  int _iIvalue;
  int _iMinDam;
  int _iMaxDam;
  int _iAC;
  int _iFlags;
  int _iMiscID;
  int _iSpell;
  int _iCharges;
  int _iMaxCharges;
  int _iDurability;
  int _iMaxDur;
  _BYTE gapE8[66];
  char _iMinMag;
  int _iStatFlag;
  int IDidx;
  int field_134;
} ItemStruct;
#pragma pack(pop)

// export from IDA
// Some ints should be void*
#pragma pack(push, 8)
typedef struct _PlayerStruct
{
  int plr_pmode;
  BOOL plractive;
  char to_send_message_id;
  char anonymous_0[3];
  int anonymous_1;
  char anonymous_2;
  char anonymous_3[3];
  int plr_to_send_ready;
  char plr_to_send_action;
  char anonymous_4[3];
  int plr_to_send_v1;
  int plr_to_send_v2;
  int plr_to_send_v3;
  int plr_recv_ready;
  char plr_recv_action;
  char anonymous_5[3];
  int plr_recv_v1;
  int plr_recv_v2;
  int plr_recv_v3;
  int plr_recv_buffer_idx;
  int plr_send_buffer_idx;
  char plr_mp_buffer[256];
  char plr_send_array_1[256];
  char plr_send_array_2[256];
  char plr_send_array_3[256];
  char plr_walkpath[25];
  char anonymous_6[3];
  int plr_walkPathRelated;
  int _px;
  int _py;
  int anonymous_7;
  int anonymous_8;
  int _pxoff;
  int _pyoff;
  int anonymous_9;
  int anonymous_10;
  int _pdir;
  int anonymous_11;
  int plr_pgfxnum;
  int plr_pAnimData;
  int plr_pAnimDelay;
  int plr_pAnimCnt;
  int plr_pAnimLen;
  int plr_pAnimFrame;
  int plr_pAnimWidth;
  int plr_pAimWidth2;
  int plr_peflag;
  int anonymous_12;
  int anonymous_13;
  int _pRSpell;
  char _pRSplType;
  char _pSplLvl[35];
  int _pMemSpells;
  int _pAblSpells;
  int _pScrlSpells;
  int anonymous_14;
  char anonymous_15[8];
  char anonymous_16;
  char anonymous_17[3];
  int plr_pInvincible;
  int plr_pLightRad;
  int plr_pLvlChanging;
  char anonymous_18;
  _BYTE gap509[61];
  char anonymous_19;
  char _pName[32];
  _BYTE gap567[30];
  char _pClass;
  char anonymous_20[2];
  int _pStrength;
  int _pBaseStr;
  int _pMagic;
  int _pBaseMag;
  int _pDexterity;
  int _pBaseDex;
  int _pVitality;
  int _pBaseVit;
  int _pStatPts;
  int anonymous_21;
  int anonymous_22;
  int anonymous_23;
  int _pHPBase;
  int _pMaxHPBase;
  int _pHitPoints;
  int _pMaxHP;
  int _pHPPer;
  int _pManaBase;
  int _pMaxManaBase;
  int _pMana;
  int _pMaxMana;
  int _pManaPer;
  char _pLevel;
  char _pMaxLvl;
  char anonymous_24[2];
  int plr_pExperience;
  int plr_pMaxExp;
  int plr_pNextExper;
  char plr_pArmorClass;
  char plr_pMagResist;
  char plr_pFireResist;
  char plr_pLghtResist;
  int _pGold;
  int plr_pInfraFlag;
  int plr_pVar1;
  int plr_pVar2;
  int plr_pVar3;
  int plr_pVar4;
  int plr_pVar5;
  int plr_pVar6;
  int plr_pVar7;
  int plr_pVar8;
  int _pSeedTbl[17]; 
  int _pLevelTypeTbl[17];
  int _pLvlVisited[17];
  int _pSLvlVisited[17];
  int plr_pGFXLoad;
  int plr_pNAnim[8];
  int plr_pNFrames;
  int plr_pNWidth;
  int plr_pWAnim[8];
  int plr_pWFrames;
  int plr_pWWidth;
  int plr_pAAnim[8];
  int plr_pAFrames;
  int plr_pAWidth;
  int plr_pAFNum;
  int plr_pLAnim[8];
  int plr_pFAnim[8];
  int plr_pTAnim[8];
  int plr_pSFrames;
  int plr_pSWidth;
  int plr_pSFNum;
  int plr_pHAnim[8];
  int plr_pHFrames;
  int plr_pHWidth;
  int plr_pDAnim[8];
  int plr_pDFrames;
  int plr_pDWidth;
  int plr_pBAnim[8];
  int plr_pBFrames;
  int plr_pBWidth;
  ItemStruct InvBody[6];
  int plr_potbox_item;
  ItemStruct InvList[40];
  int _pNumInv;
  char InvGrid[40];
  int anonymous_25;
  _BYTE gap40D4[44];
  int anonymous_26;
  char anonymous_27;
  _BYTE gap4105[128];
  char anonymous_28;
  char anonymous_29;
  char anonymous_30;
  int anonymous_31;
  int anonymous_32;
  _BYTE gap4190[105];
  char anonymous_33;
  char anonymous_34;
  char anonymous_35;
  int anonymous_36;
  char anonymous_37[8];
  int anonymous_38;
  int anonymous_39;
  int anonymous_40;
  int anonymous_41;
  int anonymous_42;
  int anonymous_43;
  int anonymous_44;
  int anonymous_45;
  int plr_pIFlags;
  int anonymous_46;
  char _pISplLvlAdd;
  char _pISplCost;
  char anonymous_47;
  char anonymous_48;
  int plr_WarpActive;
  int plr_WarpLevel;
  int plr_WarpLvlType;
  int plr_WarpSet;
  int plr_WarpX;
  int plr_WarpY;
  int plr_pNData;
  int plr_pWData;
  int plr_pAData;
  int plr_pLData;
  int plr_pFData;
  int plr_pTData;
  int plr__pHData;
  int plr_pDData;
  int plr_pBData;
} PlayerStruct;
#pragma pack(pop)
_STATIC_ASSERT(sizeof(PlayerStruct) == 0x4270);

#pragma pack(push, 8)
typedef struct _QuestStruct
{
  char _qlevel;
  char _qtype;
  char _qactive;
  char anonymous_2;
  int _qtx;
  int _qty;
  char _qslvl;
  char _qidx;
  char field_E;
  char field_F;
} QuestStruct;
#pragma pack(pop)
_STATIC_ASSERT(sizeof(QuestStruct) == 0x10);

#pragma pack(push, 8)
struct ObjectStruct
{
  int _otype;
  int _ox;
  int _oy;
  int _oLight;
  BOOL _oAnimFlag;
  BYTE *_oAnimData;
  int _oAnimDelay;
  int _oAnimCnt;
  int _oAnimLen;
  int _oAnimFrame;
  int _oAnimWidth;
  int _oAnimWidth2;
  int _oDelFlag;
  char _oBreak;
  char anonymous_0[3];
  int _oSolidFlag;
  int _oMissFlag;
  char _oSelFlag;
  char anonymous_1[3];
  int _oPreFlag;
  int _oTrapFlag;
  int _oDoorFlag;
  int _olid;
  int _oVar1; // if shrine then shrine type (see OperateShrine)
  int _oVar2;
  int _oVar3;
  int _oVar4;
  int _oVar5;
  int _oVar6;
  int _oVar7;
  int _oVar8;
};
#pragma pack(pop)
_STATIC_ASSERT(sizeof(QuestStruct) == 0x74);

#pragma pack(push, 8)
struct MonsterStruct
{
  int _mMTidx;
  int _mmode;
  char anonymous_0;
  char anonymous_1[3];
  int anonymous_2;
  int anonymous_3;
  _BYTE gap14[8];
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
  _BYTE gapA0[4];
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
_STATIC_ASSERT(sizeof(MonsterStruct) == 0xC4);

#pragma pack(push, 8)
struct ObjDataStruct
{
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
#pragma pack(pop)
_STATIC_ASSERT(sizeof(ObjDataStruct) == 0x2C);

#pragma pack(push, 8)
struct SpellStruct
{
  int type;
  int x;
  int y;
  int anonymous_2;
  int anonymous_3;
  int animdata;
  int anonymous_5;
  int anonymous_6;
  int anonymous_7;
  int anonymous_8;
  int animWidth;
  int animWidth2;
  int anonymous_11;
  int anonymous_12;
  int anonymous_13;
  int range;
  int anonymous_15;
  int delFlag;
  int source;
  int oldx;
  int oldy;
  int dir;
  int nextx;
  int nexty;
  int anonymous_23;
  int anonymous_24;
  int anonymous_25;
};
#pragma pack(pop)
_STATIC_ASSERT(sizeof(SpellStruct) == 0x6C);

#endif