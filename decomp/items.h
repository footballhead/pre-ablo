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

enum item_effect_type
{
    IPL_TOHIT = 0x0,
    IPL_TOHIT_CURSE = 0x1,
    IPL_DAMP = 0x2,
    IPL_DAMP_CURSE = 0x3,
    IPL_TOHIT_DAMP = 0x4,
    IPL_TOHIT_DAMP_CURSE = 0x5,
    IPL_ACP = 0x6,
    IPL_ACP_CURSE = 0x7,
    IPL_FIRERES = 0x8,
    IPL_LIGHTRES = 0x9,
    IPL_MAGICRES = 0xA,
    IPL_ALLRES = 0xB,
    IPL_SPLCOST = 0xC, // Demo only (beta too?)
    IPL_SPLDUR = 0xD,  // Demo only (beta too?)
    IPL_SPLLVLADD = 0xE,
    IPL_CHARGES = 0xF,
    IPL_FIREDAM = 0x10,  // NW
    IPL_LIGHTDAM = 0x11, // NW
    IPL_RANDSPL = 0x12,  // NW. Demo only (beta too?)
    IPL_STR = 0x13,
    IPL_STR_CURSE = 0x14,
    IPL_MAG = 0x15,
    IPL_MAG_CURSE = 0x16,
    IPL_DEX = 0x17,
    IPL_DEX_CURSE = 0x18,
    IPL_VIT = 0x19,
    IPL_VIT_CURSE = 0x1A,
    IPL_ATTRIBS = 0x1B,
    IPL_ATTRIBS_CURSE = 0x1C,
    IPL_GETHIT_CURSE = 0x1D,
    IPL_GETHIT = 0x1E,
    IPL_LIFE = 0x1F,
    IPL_LIFE_CURSE = 0x20,
    IPL_MANA = 0x21,
    IPL_MANA_CURSE = 0x22,
    IPL_DUR = 0x23,
    IPL_DUR_CURSE = 0x24,
    IPL_INDESTRUCTIBLE = 0x25,
    IPL_LIGHT = 0x26,
    IPL_LIGHT_CURSE = 0x27,
    IPL_INVIS = 0x28,       // NW. Demo only
    IPL_MULT_ARROWS = 0x29, // NW. Demo/hellfire only
    IPL_FIRE_ARROWS = 0x2A,
    IPL_LIGHT_ARROWS = 0x2B,  // NW
    IPL_HOMING_ARROWS = 0x2C, // NW. Demo only; in Beta+ this is IPL_INVCURS
    IPL_THORNS = 0x2D,        // NW
    IPL_NOMANA = 0x2E,        // NW
    IPL_NOHEALPLR = 0x2F,     // NW
    IPL_SCARE = 0x30,         // NW. Demo only
    IPL_DOUBLE_STRIKE = 0x31, // NW. Demo only
    IPL_EXP_DAM = 0x32,       // NW. Demo only; in Hellfire this is IPL_FIREBALL
    IPL_SEE_INVIS = 0x33,     // NW. Demo only
    IPL_ABSHALFTRAP = 0x34,   // NW
    IPL_KNOCKBACK = 0x35,     // NW
    IPL_NOHEALMON = 0x36,     // NW
    // Devilution has additional things here... we don't though
    IPL_DAMMOD = 55,
    IPL_RNDARROWVEL = 56,
    IPL_SETDAM = 57,
    IPL_SETDUR = 58,
    // TODO: What is here? The message is "Another ability (NW)"
    IPL_DRAINLIFE = 66,
    // TODO: Any more???
};

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