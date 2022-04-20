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

typedef enum shrine_type {
    SHRINE_MYSTERIOUS,
    SHRINE_IMPOSING,
    SHRINE_HIDDEN,
    SHRINE_MAGICAL,
    SHRINE_MYSTIC,
    SHRINE_ENCHANTED,
    SHRINE_THAUMATURGIC,
    SHRINE_FASCINATING,
    SHRINE_CRYPTIC,
    SHRINE_SUPERNATURAL,
    SHRINE_EERIE,
    SHRINE_HOLY,
    SHRINE_SPIRITUAL,
    NUM_SHRINETYPE
} shrine_type;

typedef enum inv_body_loc {
    INVLOC_HEAD,
    INVLOC_CHEST,
    INVLOC_RING_LEFT,
    INVLOC_RING_RIGHT,
    INVLOC_HAND_LEFT,
    INVLOC_HAND_RIGHT
} inv_body_loc;

typedef enum spell_type {
    RSPLTYPE_SKILL   = 0x0,
    RSPLTYPE_SPELL   = 0x1,
    RSPLTYPE_SCROLL  = 0x2,
    RSPLTYPE_CHARGES = 0x3,
    RSPLTYPE_INVALID = 0x4,
} spell_type;

// TODO make sure these line up
typedef enum _object_id {
    OBJ_L1LIGHT      = 0x0,
    OBJ_L1LDOOR      = 0x1,
    OBJ_L1RDOOR      = 0x2,
    OBJ_SKFIRE       = 0x3,
    OBJ_LEVER        = 0x4,
    OBJ_CHEST1       = 0x5,
    OBJ_CHEST2       = 0x6,
    OBJ_CHEST3       = 0x7,
    OBJ_CANDLE1      = 0x8,
    OBJ_CANDLE2      = 0x9,
    OBJ_CANDLEO      = 0xA,
    OBJ_BANNERL      = 0xB,
    OBJ_BANNERM      = 0xC,
    OBJ_BANNERR      = 0xD,
    OBJ_SKPILE       = 0xE,
    OBJ_SKSTICK1     = 0xF,
    OBJ_SKSTICK2     = 0x10,
    OBJ_SKSTICK3     = 0x11,
    OBJ_SKSTICK4     = 0x12,
    OBJ_SKSTICK5     = 0x13,
    OBJ_CRUX1        = 0x14,
    OBJ_CRUX2        = 0x15,
    OBJ_CRUX3        = 0x16,
    OBJ_STAND        = 0x17,
    OBJ_ANGEL        = 0x18,
    OBJ_BOOK2L       = 0x19,
    OBJ_BCROSS       = 0x1A,
    OBJ_NUDEW2R      = 0x1B,
    OBJ_SWITCHSKL    = 0x1C,
    OBJ_TNUDEM1      = 0x1D,
    OBJ_TNUDEM2      = 0x1E,
    OBJ_TNUDEM3      = 0x1F,
    OBJ_TNUDEM4      = 0x20,
    OBJ_TNUDEW1      = 0x21,
    OBJ_TNUDEW2      = 0x22,
    OBJ_TNUDEW3      = 0x23,
    OBJ_TORTURE1     = 0x24,
    OBJ_TORTURE2     = 0x25,
    OBJ_TORTURE3     = 0x26,
    OBJ_TORTURE4     = 0x27,
    OBJ_TORTURE5     = 0x28,
    OBJ_BOOK2R       = 0x29,
    OBJ_L2LDOOR      = 0x2A,
    OBJ_L2RDOOR      = 0x2B,
    OBJ_TORCHL       = 0x2C,
    OBJ_TORCHR       = 0x2D,
    OBJ_TORCHL2      = 0x2E,
    OBJ_TORCHR2      = 0x2F,
    OBJ_SARC         = 0x30,
    OBJ_FLAMEHOLE    = 0x31,
    OBJ_FLAMELVR     = 0x32,
    OBJ_WATER        = 0x33,
    OBJ_BOOKLVR      = 0x34,
    OBJ_TRAPL        = 0x35,
    OBJ_TRAPR        = 0x36,
    OBJ_BOOKSHELF    = 0x37,
    OBJ_WEAPRACK     = 0x38,
    OBJ_BARREL       = 0x39,
    OBJ_BARRELEX     = 0x3A,
    OBJ_SHRINEL      = 0x3B,
    OBJ_SHRINER      = 0x3C,
    OBJ_SKELBOOK     = 0x3D,
    OBJ_BOOKCASEL    = 0x3E,
    OBJ_BOOKCASER    = 0x3F,
    OBJ_BOOKSTAND    = 0x40,
    OBJ_BOOKCANDLE   = 0x41,
    OBJ_BLOODFTN     = 0x42,
    OBJ_DECAP        = 0x43,
    OBJ_NULL_68      = 0x44,
} _object_id;

typedef enum quest_id {
    Q_SKELKING = 0,
    Q_BUTCHER  = 1,
    Q_ROCK     = 2,
    Q_SCHAMB   = 3,
    Q_MAZE     = 4,
    Q_INVALID  = -1
} quest_id;

typedef enum quest_state {
    QUEST_NOTAVAIL = 0,
    QUEST_INIT     = 1,
    QUEST_ACTIVE   = 2,
    QUEST_DONE     = 3
} quest_state;

enum spell_id
{
  SPL_NULL = 0x0,
  SPL_FIREBOLT = 0x1,
  SPL_HEAL = 0x2,
  SPL_LIGHTNING = 0x3,
  SPL_FLASH = 0x4,
  SPL_IDENTIFY = 0x5,
  SPL_FIREWALL = 0x6,
  SPL_TOWN = 0x7,
  SPL_STONE = 0x8,
  SPL_INFRA = 0x9,
  SPL_RNDTELEPORT = 0xA,
  SPL_MANASHIELD = 0xB,
  SPL_FIREBALL = 0xC,
  SPL_GUARDIAN = 0xD,
  SPL_CHAIN = 0xE,
  SPL_WAVE = 0xF,
  SPL_DOOMSERP = 0x10,
  SPL_BLODRIT = 0x11,
  SPL_NOVA = 0x12,
  SPL_INVISIBIL = 0x13,
  SPL_SENTINEL = 0x14,
  SPL_GOLEM = 0x15,
  SPL_BLODBOIL = 0x16,
  SPL_TELEPORT = 0x17,
  SPL_APOCA = 0x18,
  SPL_ETHEREALIZE = 0x19,
  SPL_REPAIR = 0x1A,
  SPL_RECHARGE = 0x1B,
  SPL_DISARM = 0x1C,
};

enum monster_resistance
{
  RESIST_MAGIC = 0x1,
  RESIST_FIRE = 0x2,
  RESIST_LIGHTNING = 0x4,
  IMMUNE_MAGIC = 0x8,
  IMMUNE_FIRE = 0x10,
  IMMUNE_LIGHTNING = 0x20,
  IMMUNE_NULL_40 = 0x40, // :)
  IMMUNE_ACID = 0x80,
};

#endif