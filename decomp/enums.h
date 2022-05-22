// These are all based on devilution with some modifications

#ifndef __ENUMS_H__
#define __ENUMS_H__

 enum dungeon_type
{
    DTYPE_TOWN = 0x0,
    DTYPE_OLD_CATHEDRAL = 0x1,
    DTYPE_CATACOMBS = 0x2,
    DTYPE_CAVES = 0x3,
    DTYPE_HELL = 0x4,
    DTYPE_CATHEDRAL = 0x5
};

enum lvl_entry
{
    ENTRY_MAIN = 0,
    ENTRY_PREV = 1,
    ENTRY_SETLVL = 2,
    ENTRY_RTNLVL = 3,
    ENTRY_LOAD = 4,
    ENTRY_WARPLVL = 5,
    ENTRY_TWARPDN = 6,
    ENTRY_TWARPUP = 7,
};

enum spell_type
{
    RSPLTYPE_SKILL = 0x0,
    RSPLTYPE_SPELL = 0x1,
    RSPLTYPE_SCROLL = 0x2,
    RSPLTYPE_CHARGES = 0x3,
    RSPLTYPE_INVALID = 0x4,
};

enum game_mode
{
    MODE_BLIZ_LOGO = 0x0,
    MODE_MAINMENU = 0x1,
    MODE_NEWGAME = 0x2,
    MODE_GAME = 0x7,
    MODE_INTRO_VID = 0xC,
    MODE_DEMO_END = 0xD,
    MODE_PROGRESS = 0xE,
};

enum dflag
{
    BFLAG_MISSILE = 0x01,
    BFLAG_VISIBLE = 0x02,
    BFLAG_DEAD_PLAYER = 0x04,
    BFLAG_POPULATED = 0x08,
    BFLAG_MONSTLR = 0x10,
    BFLAG_PLAYERLR = 0x20,
    BFLAG_LIT = 0x40,
    BFLAG_EXPLORED = 0x80,
};

#endif