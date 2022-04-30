#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <windows.h>

#include "items.h"
#include "spells.h"

//
// defines
//

#define MAX_PLRS 4

//
// enums
//

enum inv_body_loc
{
    INVLOC_HEAD,
    INVLOC_CHEST,
    INVLOC_RING_LEFT,
    INVLOC_RING_RIGHT,
    INVLOC_HAND_LEFT,
    INVLOC_HAND_RIGHT
};

enum plr_class
{
    PC_WARRIOR = 0x0,
    PC_ROGUE = 0x1,
    PC_SORCERER = 0x2,
};

//
// structs
//

#pragma pack(push, 8)
struct PlayerStruct
{
    int _pmode;
    BOOL plractive;
    char to_send_message_id;
    // padding
    int anonymous_1;
    char anonymous_2;
    // padding
    int to_send_ready;
    char to_send_action;
    // padding
    int to_send_v1;
    int to_send_v2;
    int to_send_v3;
    int recv_ready;
    char plr_recv_action;
    // padding
    int recv_v1;
    int recv_v2;
    int recv_v3;
    int recv_buffer_idx;
    int send_buffer_idx;
    char mp_buffer[256];
    char send_array_1[256];
    char send_array_2[256];
    char send_array_3[256];
    char walkpath[25];
    char anonymous_6[3];
    int walkPathRelated;
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
    int _pgfxnum;
    int _pAnimData;
    int _pAnimDelay;
    int _pAnimCnt;
    int _pAnimLen;
    int _pAnimFrame;
    int _pAnimWidth;
    int _pAimWidth2;
    int _peflag;
    int anonymous_12;
    int anonymous_13;
    int _pRSpell;
    char _pRSplType;
    char _pSplLvl[MAX_SPELLS]; // There's 35 bytes here, wondering if the difference is due to padding/packing
    // padding? 6 bytes
    int _pMemSpells;
    int _pAblSpells;
    int _pScrlSpells;
    int anonymous_14;
    char anonymous_15[8];
    char anonymous_16;
    char anonymous_17[3];
    int _pInvincible;
    int _pLightRad;
    int _pLvlChanging;
    char anonymous_18;
    BYTE gap509[61];
    char anonymous_19;
    char _pName[32];
    BYTE gap567[30];
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
    int _pExperience;
    int _pMaxExp;
    int _pNextExper;
    char _pArmorClass;
    char _pMagResist;
    char _pFireResist;
    char _pLghtResist;
    int _pGold;
    BOOL _pInfraFlag;
    int _pVar1;
    int _pVar2;
    int _pVar3;
    int _pVar4;
    int _pVar5;
    int _pVar6;
    int _pVar7;
    int _pVar8;
    int _pSeedTbl[17];      // In retail this is stored outside the player!
    int _pLevelTypeTbl[17]; // In retail this is stored outside the player!
    BOOL _pLvlVisited[17];
    BOOL _pSLvlVisited[17];
    int _pGFXLoad;
    BYTE *_pNAnim[8];
    int _pNFrames;
    int _pNWidth;
    BYTE *_pWAnim[8];
    int _pWFrames;
    int _pWWidth;
    BYTE *_pAAnim[8];
    int _pAFrames;
    int _pAWidth;
    int _pAFNum;
    BYTE *_pLAnim[8];
    BYTE *_pFAnim[8];
    BYTE *_pTAnim[8];
    int _pSFrames;
    int _pSWidth;
    int _pSFNum;
    BYTE *_pHAnim[8];
    int _pHFrames;
    int _pHWidth;
    BYTE *_pDAnim[8];
    int _pDFrames;
    int _pDWidth;
    BYTE *_pBAnim[8];
    int _pBFrames;
    int _pBWidth;
    ItemStruct InvBody[6];
    int potbox_item;
    ItemStruct InvList[40];
    int _pNumInv;
    char InvGrid[40];
    int anonymous_25;
    BYTE gap40D4[44];
    int anonymous_26;
    char anonymous_27;
    BYTE gap4105[128];
    char anonymous_28;
    char anonymous_29;
    char anonymous_30;
    int anonymous_31;
    int anonymous_32;
    BYTE gap4190[105];
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
    int _pIFlags;
    int anonymous_46;
    char _pISplLvlAdd;
    char _pISplCost;
    char anonymous_47;
    char anonymous_48;
    int _WarpActive;
    int _WarpLevel;
    int _WarpLvlType;
    int _WarpSet;
    int _WarpX;
    int _WarpY;
    BYTE *_pNData;
    BYTE *_pWData;
    BYTE *_pAData;
    BYTE *_pLData;
    BYTE *_pFData;
    BYTE *_pTData;
    BYTE *_pHData;
    BYTE *_pDData;
    BYTE *_pBData;
};
#pragma pack(pop)

//
// variables
//

extern int gbActivePlayers;
extern PlayerStruct plr[MAX_PLRS];
extern int myplr;

//
// functions
//

void AddPlrExperience(int pnum, int lvl, int exp);
void SyncPlrKill(int pnum);
void StartPlrHit(int pnum);

#endif