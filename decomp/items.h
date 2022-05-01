#ifndef __ITEMS_H__
#define __ITEMS_H__

#include <windows.h>

//
// enums
//

// TODO: This is copied from Devilution, verify this is correct
enum item_type
{
    ITYPE_MISC = 0x0, // Verified; see OperateSkelBook
    ITYPE_SWORD = 0x1,
    ITYPE_AXE = 0x2,
    ITYPE_BOW = 0x3,
    ITYPE_MACE = 0x4,
    ITYPE_SHIELD = 0x5,
    ITYPE_LARMOR = 0x6,
    ITYPE_HELM = 0x7,
    ITYPE_MARMOR = 0x8,
    ITYPE_HARMOR = 0x9,
    ITYPE_STAFF = 0xA,
    ITYPE_GOLD = 0xB,
    ITYPE_RING = 0xC,
    ITYPE_AMULET = 0xD,
    ITYPE_FOOD = 0xE, /* used in demo */
    ITYPE_NONE = -1,
};

// TODO: This is copied from Devilution, verify this is correct
enum item_misc_id
{
    IMISC_NONE = 0x0,
    IMISC_USEFIRST = 0x1,
    IMISC_FULLHEAL = 0x2,
    IMISC_HEAL = 0x3,
    IMISC_OLDHEAL = 0x4,
    IMISC_DEADHEAL = 0x5,
    IMISC_MANA = 0x6,
    IMISC_FULLMANA = 0x7,
    IMISC_POTEXP = 0x8,  /* add experience */
    IMISC_POTFORG = 0x9, /* remove experience */
    IMISC_ELIXSTR = 0xA,
    IMISC_ELIXMAG = 0xB,
    IMISC_ELIXDEX = 0xC,
    IMISC_ELIXVIT = 0xD,
    IMISC_ELIXWEAK = 0xE, /* double check with alpha */
    IMISC_ELIXDIS = 0xF,
    IMISC_ELIXCLUM = 0x10,
    IMISC_ELIXSICK = 0x11,
    IMISC_USELAST = 0x14,
    IMISC_SCROLL = 0x11, // Verified, see OperateSkelBook
    IMISC_STAFF = 0x12,  
    IMISC_BOOK = 0x12, // Verified, see OperateSkelBook
    IMISC_RING = 0x19,
    IMISC_AMULET = 0x1A,
    IMISC_UNIQUE = 0x1B,
    IMISC_FOOD = 0x1C, /* from demo/PSX */
    IMISC_OILFIRST = 0x1D,
    IMISC_OILOF = 0x1E, /* oils are beta or hellfire only */
    IMISC_OILACC = 0x1F,
    IMISC_OILMAST = 0x20,
    IMISC_OILSHARP = 0x21,
    IMISC_OILDEATH = 0x22,
    IMISC_OILSKILL = 0x23,
    IMISC_OILBSMTH = 0x24,
    IMISC_OILFORT = 0x25,
    IMISC_OILPERM = 0x26,
    IMISC_OILHARD = 0x27,
    IMISC_OILIMP = 0x28,
    IMISC_OILLAST = 0x29,
    IMISC_MAPOFDOOM = 0x2A,
    IMISC_EAR = 0x2B,
    IMISC_SPECELIX = 0x2C,
    IMISC_INVALID = -1,
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
    int field_C;
    int _iAnimData;
    int field_14;
    BYTE *_iAnimFrame;
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
    BYTE gapE8[66];
    char _iMinMag;
    int _iStatFlag;
    int IDidx;
    int field_134;
};
#pragma pack(pop)

//
// functions
//

void CalcPlrInv(int p);

#endif