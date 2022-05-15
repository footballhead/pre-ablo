#ifndef __ITEMS_H__
#define __ITEMS_H__

#include "itemdat.h"

#include <windows.h>

//
// defines
//

#define MAXITEMS 127
#define ITEMTYPES 28

//
// enums
//

enum item_quality
{
    ITEM_QUALITY_NORMAL = 0,
    ITEM_QUALITY_MAGIC = 1,
    ITEM_QUALITY_UNIQUE = 2,
};

//
// structs
//

#pragma pack(push, 8)
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
    int _iMiscID;
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
#pragma pack(pop)

//
// functions
//

void CalcPlrInv(int p);
void UseItem(int p, int Mid, int spl);

#endif