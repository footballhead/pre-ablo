// Things from the Pre-Release Demo decomp. They were exported from IDA. They
// are by no means perfect but they get the job done for now.

#pragma once

#include <windows.h>

#define MAXQUESTS 10
#define MAX_PLRS 4
#define NUMLEVELS 17

typedef enum quest_state {
  QUEST_NOTAVAIL = 0,
  QUEST_INIT = 1,
  QUEST_ACTIVE = 2,
  QUEST_DONE = 3
} quest_state;

typedef enum dungeon_type {
  DTYPE_TOWN = 0x0,
  DTYPE_OLD_CATHEDRAL = 0x1,
  DTYPE_CATACOMBS = 0x2,
  DTYPE_CAVES = 0x3,
  DTYPE_HELL = 0x4,
  DTYPE_CATHEDRAL = 0x5
} dungeon_type;

// This is a workaround to an IDA export quirk: gaps of undefined data use this
// type, which is 8-bits
typedef char _BYTE;

#pragma pack(push, 8)
typedef struct _ItemStruct {
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
// TODO static_assert(sizeof(ItemStruct) == ???)
#pragma pack(pop)

#pragma pack(push, 8)
typedef struct _PlayerStruct {
  int plr_pmode;
  BOOL plractive;
  char plr_to_send_message_id;
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
  int plr_px;
  int plr_py;
  int anonymous_7;
  int anonymous_8;
  int plr_pxoff;
  int plr_pyoff;
  int anonymous_9;
  int anonymous_10;
  int plr_pdir;
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
  int plr_pRSpell;
  char plr_pRSplType;
  char plr_pSplLvl[35];
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
  char plr_pClass;
  char anonymous_20[2];
  int plr_pStrength;
  int plr_pBaseStr;
  int plr_pMagic;
  int plr_pBaseMag;
  int plr_pDexterity;
  int plr_pBaseDex;
  int plr_pVitality;
  int plr_pBaseVit;
  int plr_pStatPts;
  int anonymous_21;
  int anonymous_22;
  int anonymous_23;
  int plr_pHPBase;
  int plr_pMaxHPBase;
  int plr_pHitPoints;
  int plr_pMaxHP;
  int plr_pHPPer;
  int plr_pManaBase;
  int plr_pMaxManaBase;
  int plr_pMana;
  int plr_pMaxMana;
  int plr_pManaPer;
  char plr_pLevel;
  char plr_pMaxLvl;
  char anonymous_24[2];
  int plr_pExperience;
  int plr_pMaxExp;
  int plr_pNextExper;
  char plr_pArmorClass;
  char plr_pMagResist;
  char plr_pFireResist;
  char plr_pLghtResist;
  int plr_pGold;
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
  ItemStruct plr_InvBodyHead;
  ItemStruct plr_InvBodyChest;
  ItemStruct plr_InvBodyRingLeft;
  ItemStruct plr_InvBodyRingRight;
  ItemStruct plr_InvBodyHandLeft;
  ItemStruct plr_InvBodyHandRight;
  int plr_potbox_item;
  ItemStruct plr_InvList[40];
  int plr_pNumInv;
  char plr_InvGrid[40];
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
  char plr_pISplLvlAdd;
  char plr_pISplCost;
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
static_assert(sizeof(PlayerStruct) == 0x4270,
              "PlayerStruct must be 0x4270 bytes");

#pragma pack(push, 8)
typedef struct _QuestStruct {
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
static_assert(sizeof(QuestStruct) == 0x10, "QuestStruct must be 0x10 bytes");
