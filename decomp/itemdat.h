#ifndef __ITEMDAT_H__
#define __ITEMDAT_H__

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

// TODO ISPL

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

//
// structs
//

struct ItemDataStruct
{
    BOOL iRnd;
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
    char UIItemId;
    char UIMinLvl;
    char UINumPL;
    int UIValue;
    int field_C;
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

extern ItemDataStruct AllItemsList[];
extern PLStruct PL_Prefix[];
extern PLStruct PL_Suffix[];
extern UItemStruct UniqueItemList[];

#endif