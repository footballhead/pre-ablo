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

struct AnimStruct
{
    BYTE *CMem;    // Pointer to beginning of CEL data
    BYTE *Data[8]; // CEL animation data for each direction
    int Frames;    // # of frames for each direction
    int Rate;      // Delay between each frame in game ticks. The game runs at 20 FPS so the delay is 50ms * Rate between each frame.
};
static_assert(sizeof(AnimStruct) == 0x2C, "AnimStruct compiled to wrong size");

struct TSnd
{
    DWORD pcm_size;
    void* pFormat; // LPWAVEFORMATEX
    int dword_8;
    int dword_c;
    int dword_10;
    int dword_14;
    int dword_18;
    BOOL uses_hardware_mixing;
    void* DSB; // LPDIRECTSOUNDBUFFER
    int dword_24;
    const char *sound_path;
};
static_assert(sizeof(AnimStruct) == 0x2C, "TSnd compiled to wrong size");

struct CMonster
{
    // `enum _monster_id`. Index into `monsterdata`
    char mtype;
    // Loaded CEL animation data.
    // There are 6 distinct animations: idle, walk, attack, hit, death, and (optional) special.
    AnimStruct Anims[6];
    DWORD field_10C; // padding?
    DWORD field_110; // padding?
    DWORD field_114; // padding?
    // Monster sounds. There's 4 sound types with 2 variants for each sound.
    TSnd Snds[4][2];
    BYTE gap278[36]; // padding???
    // Width in pixels for all CELs in `Anims`.
    int width;
    // `(width - 64) / 2`. Used to center the monster inside a tile (see scrollrt.cpp)
    int width2;
    // Lower bound on randomly generated HP
    unsigned char mMinHP;
    // Upper bound on randomly generated HP
    unsigned char mMaxHP;
    // If true then need to load the special attack graphics
    BOOL has_special;
    // "Attack Frame Number". Damage is not done until this attack animation frame.
    char mAFNum;
    // Index into `dead`. Used for creating corpses.
    char mdeadval;
    // Pointer to an entry in `monsterdata`
    MonsterData *MData;
    // Pointer to TRN file loaded into memory. Must be 256 bytes.
    BYTE *trans_file;
    // Difference from Devilution: no mFlags
};
static_assert(sizeof(CMonster) == 0x2B8, "CMonster compiled to wrong size");

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
    BYTE* _mAnimData;
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
    CMonster *MType;
    MonsterData *MData;
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

struct MissileStruct
{
    int _mitype; // enum missile_id
    int _mix;
    int _miy;
    int _mixoff;
    int _miyoff;
    int _mixvel;
    int _miyvel;
    int _misx;
    int _misy;
    int _mitxoff;
    int _mityoff;
    int _mimfnum;
    int _miDelFlag;
    int _miAnimFlags;
    BYTE *_miAnimData;
    int _miAnimDelay;
    int _miAnimCnt;
    int _miAnimLen;
    int _miAnimAdd;
    int _miAnimFrame;
    int _miAnimWidth;
    int _miAnimWidth2;
    int _miLightFlag;
    BOOL _miPreFlag; // TRUE if the missile should be drawn behind everything else on the tile (e.g. players, monsters, etc). Used by Flash and acid puddles
    int _miUniqTrans;
    int _mirange; // Can either mean "how far the missile can travel" or "how long the effect lasts"; depends on the spell specifically
    int _misource;
    int _micaster;
    int _midam;
    int _midist;
    int _mirnd;
    int _mlid;
    int _miVar1;
    int _miVar2;
    int _miVar3;
    int _miVar4;
    int _miVar5;
    int _miVar6;
    int _miVar7;
    int _miVar8;
    // NOTE: _miDrawFlag is missing!
};
static_assert(sizeof(MissileStruct) == 0xA0, "MissileStruct compiled to wrong size");

struct SpellStruct {
  int type;  // enum spell_id
  int x;
  int y;
  BOOL animFlag;
  int field_10;    // Basically unused
  BYTE *animdata;  // Basically unused
  int animDelay;   // How many ticks between frames
  int animCnt;     // Current progress towards a new frame
  int animLen;     // How many frames in animdata
  int animFrame;   // Current animation frame
  int animWidth;   // Width of animdata
  int animWidth2;  // animWidth / 8
  int field_30;    // Basically unused
  int field_34;    // Basically unused
  int field_38;    // Basically unused
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
static_assert(sizeof(SpellStruct) == 0x6C,
              "SpellStruct compiled to the wrong size");

struct ItemStruct
{
    int _itype;
    int _ix;
    int _iy;
    BOOL _iAnimFlag;
    BYTE *_iAnimData;
    int _iAnimLen;
    int _iAnimFrame;
    int _iAnimWidth;
    int _iAnimWidth2;
    BOOL _iDelFlag;
    char _iSelFlag;
    BOOL _iPostDraw;
    BOOL _iIdentified;
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
    int _iMiscId;
    int _iSpell;
    int _iCharges;
    int _iMaxCharges;
    int _iDurability;
    int _iMaxDur;
    int _iPLDam;
    int _iPLToHit;
    int _iPLAC;
    int _iPLStr;
    int _iPLMag;
    int _iPLDex;
    int _iPLVit;
    int _iPLFR;
    int _iPLLR;
    int _iPLMR;
    int _iPLMana;
    int _iPLHP;
    int _iPLDamMod;
    int _iPLGetHit;
    int _iPLLight;
    char _iSplLvlAdd;
    char _iPLSplCost; // Demo specific
    char _iPLSplDur;  // Demo specific
    char _iPrePower; // IPL_*
    char _iSufPower; // IPL_*
    char _iMinStr;
    char _iMinMag;
    char _iMinDex;
    BOOL _iStatFlag;
    int IDidx; // Index into AllItemsList
    int field_134; // unused?
};
static_assert(sizeof(ItemStruct) == 0x138,
              "ItemStruct compiled to the wrong size");

#define __udec
#define __tabform(x)
#define __bin
#define _BYTE char

#pragma pack(push, 8)
struct PlayerStruct
{
  int _pmode;
  int plractive;
  char to_send_message_id;
  _BYTE anonymous_0[3];
  int anonymous_1;
  char anonymous_2;
  _BYTE anonymous_3[3];
  int to_send_ready;
  char to_send_action;
  _BYTE anonymous_4[3];
  int to_send_v1;
  int to_send_v2;
  int to_send_v3;
  int recv_ready;
  char recv_action;
  _BYTE anonymous_5[3];
  int recv_v1;
  int recv_v2 __bin;
  int recv_v3;
  int recv_buffer_idx;
  int send_buffer_idx;
  char mp_buffer[1024] __tabform(DEC);
  char walkpath[25] __tabform(NODUPS|DEC);
  _BYTE anonymous_6[3];
  int plr_walkPathRelated;
  int _px __udec;
  int _py __udec;
  int _pfutx;
  int _pfuty;
  int _pxoff __udec;
  int _pyoff;
  int _pxvel;
  int _pyvel;
  int _pdir;
  int anonymous_11;
  int _pgfxnum;
  int _pAnimData;
  int _pAnimDelay;
  int _pAnimCnt;
  int _pAnimLen;
  int _pAnimFrame;
  int _pAnimWidth __udec;
  int _pAnimWidth2;
  int _peflag;
  int anonymous_12;
  int anonymous_13;
  int _pRSpell;
  char _pRSplType;
  char _pSplLvl[35] __tabform(NODUPS|DEC);
  int _pMemSpells;
  int _pAblSpells;
  int _pScrlSpells;
  int _pSplHotKey[3];
  char anonymous_16;
  _BYTE anonymous_17[3];
  int _pInvincible;
  int unknown_var_name;
  int _pBlockFlag;
  char _pLightRad;
  _BYTE gap509[61];
  char anonymous_19;
  char _pName[32] __tabform(DEC);
  _BYTE gap567[30];
  char _pClass;
  _BYTE anonymous_20[2];
  int _pStrength;
  int _pBaseStr;
  int _pMagic;
  int _pBaseMag;
  int _pDexterity;
  int _pBaseDex;
  int _pVitality;
  int _pBaseVit;
  int _pStatPts;
  int _pBaseToHit;                      ///< This is not in Devilution!
  int _pDamageMod;
  int _pBaseToBlk;
  int _pHPBase;
  int _pMaxHPBase;
  int _pHitPoints;
  int _pMaxHP;
  int _pHPPer;
  int _pManaBase;
  int _pMaxManaBase;
  int _pMana;
  int _pMaxMana;
  int plr_pManaPer;
  char _pLevel;
  char _pMaxLvl;
  _BYTE anonymous_24[2];
  int _pExperience;
  int _pMaxExp;
  int _pNextExper;
  char _pArmorClass;
  char _pMagResist;
  char _pFireResist;
  char _pLghtResist;
  int _pGold;
  int _pInfraFlag;
  int _pVar1;
  int _pVar2;
  int _pVar3;
  int _pVar4;
  int _pVar5;
  int _pVar6;
  int _pVar7;
  int _pVar8;
  int _pSeedTbl[17] __udec __tabform(NODUPS|DEC);
  int _pLevelTypeTbl[17] __tabform(NODUPS|DEC);
  int _pLvlVisited[17] __tabform(NODUPS|DEC);
  int _pSLvlVisited[17] __tabform(NODUPS|DEC);
  int _pGFXLoad;
  int _pNAnim[8] __tabform(DEC);
  int _pNFrames;
  int _pNWidth;
  int _pWAnim[8] __tabform(NODUPS|DEC);
  int _pWFrames;
  int _pWWidth;
  int _pAAnim[8] __tabform(DEC);
  int _pAFrames;
  int _pAWidth;
  int _pAFNum;
  int _pLAnim[8] __tabform(DEC);
  int _pFAnim[8] __tabform(DEC);
  int _pTAnim[8] __tabform(DEC);
  int _pSFrames;
  int _pSWidth;
  int _pSFNum;
  int _pHAnim[8] __tabform(DEC);
  int _pHFrames;
  int _pHWidth;
  int _pDAnim[8] __tabform(DEC);
  int _pDFrames;
  int _pDWidth;
  int _pBAnim[8] __tabform(DEC);
  int _pBFrames;
  int _pBWidth;
  ItemStruct InvBody_HEAD __tabform(DEC);
  ItemStruct InvBody_CHEST;
  ItemStruct InvBody_RING_LEFT;
  ItemStruct InvBody_RING_RIGHT;
  ItemStruct InvBody_HAND_LEFT;
  ItemStruct InvBody_HAND_RIGHT;
  int potbox_item;
  ItemStruct InvList[40] __tabform(DEC);
  int _pNumInv;
  char InvGrid[40] __tabform(DEC);
  ItemStruct HoldItem;
  int anonymous_38;
  int anonymous_39;
  int anonymous_40;
  int anonymous_41;
  int anonymous_42;
  int anonymous_43;
  int anonymous_44;
  int _pISpells;
  int _pIFlags;
  int anonymous_46;
  char _pISplLvlAdd;
  char _pISplCost;
  char anonymous_47;
  _BYTE anonymous_48;
  int _WarpActive;
  int plr_WarpLevel;
  int plr_WarpLvlType;
  int plr_WarpSet;
  int plr_WarpX __udec;
  int plr_WarpY __udec;
  int _pNData;
  int _pWData;
  int _pAData;
  int _pLData;
  int _pFData;
  int _pTData;
  int _pHData;
  int _pDData;
  int _pBData;
};
#pragma pack(pop)
static_assert(sizeof(PlayerStruct) == 0x4270, "PlayerStruct compiled to the wrong size");
