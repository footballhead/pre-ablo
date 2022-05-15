#ifndef __ITEMDAT_H__
#define __ITEMDAT_H__

#include <windows.h>

//
// enums
//

enum item_class
{
    ICLASS_NONE = 0,
    ICLASS_WEAPON = 1,
    ICLASS_ARMOR = 2,
    ICLASS_MISC = 3,
    ICLASS_GOLD = 4,
    ICLASS_QUEST = 5,
};

enum item_equip_type
{
    ILOC_NONE = 0x0,
    ILOC_ONEHAND = 0x1,
    ILOC_TWOHAND = 0x2,
    ILOC_ARMOR = 0x3,
    ILOC_HELM = 0x4,
    ILOC_RING = 0x5,
    ILOC_UNEQUIPABLE = 0x6,
    ILOC_INVALID = -1,
};

// TODO: ICURS

enum item_type
{
    ITYPE_MISC = 0x0,
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
    ITYPE_FOOD = 0xD,
    ITYPE_NONE = -1,
};

enum item_misc_id
{
    IMISC_NONE = 0,
    IMISC_HEAL = 1,
    // ???
    IMISC_MANA = 4,
    IMISC_FULLMANA = 5,
    IMISC_POTEXP = 6,
    // ???
    IMISC_ELIXSTR = 8,
    IMISC_ELIXMAG = 9,
    IMISC_ELIXDEX = 10,
    IMISC_ELIXVIT = 11,
    IMISC_ELIXWEAK = 12,
    IMISC_ELIXDIS = 13,
    IMISC_ELIXCLUM = 14,
    IMISC_ELIXSICK = 15,
    IMISC_FULLHEAL = 16,
    IMISC_SCROLL = 17,
    IMISC_BOOK = 18,
    IMISC_STAFF = 19,
    IMISC_RING = 20,
    IMISC_UNIQUE = 21,
    IMISC_FOOD = 22,
};

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
    IPL_INVALID = -1,
};

enum item_special_effect
{
    ISPL_NONE = 0x00000000,
    ISPL_INFRAVISION = 0x00000001,
    ISPL_RNDSTEALLIFE = 0x00000002,
};

// TODO UITYPE

// TODO: This is copied from Devilution, verify this is correct
enum _item_indexes
{
    IDI_GOLD,
    IDI_WARRIOR,
    IDI_WARRSHLD,
    IDI_WARRCLUB,
    IDI_ROGUE,
    IDI_SORCEROR,
    IDI_CLEAVER,
    IDI_FIRSTQUEST = IDI_CLEAVER,
    IDI_SKCROWN,
    IDI_INFRARING,
    IDI_ROCK,
    IDI_OPTAMULET,
    IDI_TRING,
    IDI_BANNER,
    IDI_HARCREST,
    IDI_STEELVEIL,
    IDI_GLDNELIX,
    IDI_ANVIL,
    IDI_MUSHROOM,
    IDI_BRAIN,
    IDI_FUNGALTM,
    IDI_SPECELIX,
    IDI_BLDSTONE,
    IDI_MAPOFDOOM,
    IDI_LASTQUEST = IDI_MAPOFDOOM,
    IDI_EAR,
    IDI_HEAL,
    IDI_MANA,
    IDI_IDENTIFY,
    IDI_PORTAL,
    IDI_ARMOFVAL,
    IDI_FULLHEAL,
    IDI_FULLMANA,
    IDI_GRISWOLD,
    IDI_LGTFORGE,
    IDI_LAZSTAFF,
    IDI_RESURRECT,
};

// I've copied this comment from the Devilution source because I thought it was
// funny:

/*
Looks like someone treated hex values as binary, so 0x10 came after 0x01, that's why we have 1 and 16, they did the same thing with affix_item_type
*/

enum goodorevil
{
    GOE_ANY = 0x00,
    GOE_EVIL = 0x01,
    GOE_GOOD = 0x10,
};

enum affix_item_type
{
    PLT_MISC = 0x1,
    PLT_BOW = 0x10,
    PLT_STAFF = 0x100,
    PLT_WEAP = 0x1000,
    PLT_SHLD = 0x10000,
    PLT_ARMO = 0x100000,
};

//
// structs
//

struct ItemDataStruct
{
    BOOL iRnd;   // I only see 0 and 1 here so assuming it's a BOOL
    char iClass; // enum item_class
    char iLoc;   // enum item_equip_type
    int iCurs;
    char iType;   // enum item_type
    char iItemId; // enum unique_base_item
    const char *iName;
    const char *iSName;
    char iMinMLvl;
    int iDurability;
    int iMinDam;
    int iMaxDam;
    int iMinAC;
    int iMaxAC;
    char iMinStr;
    char iMinMag;
    char iMinDex;
    int iFlags;  // enum item_special_effect
    int iMiscId; // enum item_misc_id
    int iSpell;  // enum spell_id
    BOOL iUsable;
    int iValue;
};

struct PLStruct
{
    BOOL isWorking; // Demo specific
    const char *PLName;
    int PLPower;
    int PLParam1;
    int PLParam2;
    char PLMinLvl;
    int PLIType;
    char PLGOE;
    int PLDouble;
    int PLOk;
    int PLMinVal;
    int PLMaxVal;
    int PLMultVal;
};

struct UItemStruct
{
    const char *UIName;
    char UIItemId; // Key constraint for joining with AllItemsList, see iItemId
    char UIMinLvl;
    char UINumPL;
    int UIValue;
    int field_C; // Demo specific? Looks like a BOOL
    char UIPower1;
    int UIParam1;
    int UIParam2;
    char UIPower2;
    int UIParam3;
    int UIParam4;
    char UIPower3;
    int UIParam5;
    int UIParam6;
    char UIPower4;
    int UIParam7;
    int UIParam8;
    char UIPower5;
    int UIParam9;
    int UIParam10;
    char UIPower6;
    int UIParam11;
    int UIParam12;
};

//
// variables
//

extern ItemDataStruct AllItemsList[]; // TODO
extern PLStruct PL_Prefix[];
extern PLStruct PL_Suffix[];
extern UItemStruct UniqueItemList[]; // TODO

#endif