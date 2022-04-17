// These are all based on devilution with some modifications

#ifndef __ENUMS_H__
#define __ENUMS_H__

typedef enum quest_state {
    QUEST_NOTAVAIL = 0,
    QUEST_INIT     = 1,
    QUEST_ACTIVE   = 2,
    QUEST_DONE     = 3
} quest_state;

typedef enum dungeon_type {
    DTYPE_TOWN          = 0x0,
    DTYPE_OLD_CATHEDRAL = 0x1,
    DTYPE_CATACOMBS     = 0x2,
    DTYPE_CAVES         = 0x3,
    DTYPE_HELL          = 0x4,
    DTYPE_CATHEDRAL       = 0x5
} dungeon_type;

typedef enum lvl_entry {
    ENTRY_MAIN    = 0,
    ENTRY_PREV    = 1,
    ENTRY_SETLVL  = 2,
    ENTRY_RTNLVL  = 3,
    ENTRY_LOAD    = 4,
    ENTRY_WARPLVL = 5,
    ENTRY_TWARPDN = 6,
    ENTRY_TWARPUP = 7,
} lvl_entry;

#endif