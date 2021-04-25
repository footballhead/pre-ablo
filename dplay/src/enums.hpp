#pragma once

enum monster_availability {
    MAT_NO = 0,
    MAT_SW = 1,
    MAT_YES = 2
};

// Index into monsterdata, MonstConvTbl, MonstAvailTbl
enum _monster_id
{
  MT_NZOMBIE = 0x0,
  MT_BZOMBIE = 0x1,
  MT_GZOMBIE = 0x2,
  MT_YZOMBIE = 0x3,
  MT_RFALLSP = 0x4,
  MT_DFALLSP = 0x5,
  MT_YFALLSP = 0x6,
  MT_BFALLSP = 0x7,
  MT_WSKELAX = 0x8,
  MT_TSKELAX = 0x9,
  MT_RSKELAX = 0xA,
  MT_XSKELAX = 0xB,
  MT_RFALLSD = 0xC,
  MT_DFALLSD = 0xD,
  MT_YFALLSD = 0xE,
  MT_BFALLSD = 0xF,
  MT_NSCAV = 0x10,
  MT_BSCAV = 0x11,
  MT_WSCAV = 0x12,
  MT_YSCAV = 0x13,
  MT_WSKELBW = 0x14,
  MT_TSKELBW = 0x15,
  MT_RSKELBW = 0x16,
  MT_XSKELBW = 0x17,
  MT_WSKELSD = 0x18,
  MT_TSKELSD = 0x19,
  MT_RSKELSD = 0x1A,
  MT_XSKELSD = 0x1B,
  MT_SNEAK = 0x1C,
  MT_STALKER = 0x1D,
  MT_UNSEEN = 0x1E,
  MT_ILLWEAV = 0x1F,
  MT_NGOATMC = 0x20,
  MT_BGOATMC = 0x21,
  MT_RGOATMC = 0x22,
  MT_GGOATMC = 0x23,
  MT_FIEND = 0x24,
  MT_BLINK = 0x25,
  MT_GLOOM = 0x26,
  MT_FAMILIAR = 0x27,
  MT_NGOATBW = 0x28,
  MT_BGOATBW = 0x29,
  MT_RGOATBW = 0x2A,
  MT_GGOATBW = 0x2B,
  MT_NACID = 0x2C,
  MT_RACID = 0x2D,
  MT_BACID = 0x2E,
  MT_XACID = 0x2F,
  MT_SKING = 0x30,
  MT_FAT = 0x31,
  MT_MUDMAN = 0x32,
  MT_TOAD = 0x33,
  MT_FLAYED = 0x34,
  MT_WYRM = 0x35,
  MT_NMAGMA = 0x36,
  MT_YMAGMA = 0x37,
  MT_BMAGMA = 0x38,
  MT_WMAGMA = 0x39,
  MT_HORNED = 0x3A,
  MT_MUDRUN = 0x3B,
  MT_FROSTC = 0x3C,
  MT_OBLORD = 0x3D,
  MT_STORM = 0x3E,
  MT_RSTORM = 0x3F,
  MT_STORML = 0x40,
  MT_MAEL = 0x41,
  MT_WINGED = 0x42,
  MT_GARGOYLE = 0x43,
  MT_BLOODCLW = 0x44,
  MT_DEATHW = 0x45,
  MT_NSNAKE = 0x46,
  MT_RSNAKE = 0x47,
  MT_BSNAKE = 0x48,
  MT_GSNAKE = 0x49,
  MT_NBLACK = 0x4A,
  MT_RTBLACK = 0x4B,
  MT_BTBLACK = 0x4C,
  MT_RBLACK = 0x4D,
  MT_CLEAVER = 0x4E,
  MT_SUCCUBUS = 0x4F,
  MT_SNOWWICH = 0x50,
  MT_HLSPWN = 0x51,
  MT_SOLBRNR = 0x52,
  MT_MEGA = 0x53,
  MT_GUARD = 0x54,
  MT_VTEXLRD = 0x55,
  MT_BALROG = 0x56,
  MT_COUNSLR = 0x57,
  MT_INCIN = 0x58,
  MT_UNRAV = 0x59,
  MT_LRDSAYTR = 0x5A,
  MT_INVILORD = 0x5B,
  MT_DIABLO = 0x5C,
  NUM_MTYPES,
};

typedef enum item_type {
	ITYPE_MISC   = 0x0,
	ITYPE_SWORD  = 0x1,
	ITYPE_AXE    = 0x2,
	ITYPE_BOW    = 0x3,
	ITYPE_MACE   = 0x4,
	ITYPE_SHIELD = 0x5,
	ITYPE_LARMOR = 0x6,
	ITYPE_HELM   = 0x7,
	ITYPE_MARMOR = 0x8,
	ITYPE_HARMOR = 0x9,
	ITYPE_STAFF  = 0xA,
	ITYPE_GOLD   = 0xB,
	ITYPE_RING   = 0xC,
	ITYPE_FOOD   = 0xD,
	ITYPE_NONE   = -1,
} item_type;