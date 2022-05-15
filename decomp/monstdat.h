#ifndef __MONSTDAT_H__
#define __MONSTDAT_H__

#include <windows.h>

//
// enums
//

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
    MT_SNEAK,
    MT_STALKER,
    MT_UNSEEN,
    MT_ILLWEAV,
    MT_NGOATMC,
    MT_BGOATMC,
    MT_RGOATMC,
    MT_GGOATMC,
    MT_FIEND,
    MT_BLINK,
    MT_GLOOM,
    MT_FAMILIAR,
    MT_NGOATBW,
    MT_BGOATBW,
    MT_RGOATBW,
    MT_GGOATBW,
    MT_NACID,
    MT_RACID,
    MT_BACID,
    MT_XACID,
    MT_SKING,
    MT_FAT,
    MT_MUDMAN,
    MT_TOAD,
    MT_FLAYED,
    MT_WYRM,
    MT_NMAGMA,
    MT_YMAGMA,
    MT_BMAGMA,
    MT_WMAGMA,
    MT_HORNED,
    MT_MUDRUN,
    MT_FROSTC,
    MT_OBLORD,
    MT_STORM,
    MT_RSTORM,
    MT_STORML,
    MT_MAEL,
    MT_WINGED,
    MT_GARGOYLE,
    MT_BLOODCLW,
    MT_DEATHW,
    MT_NSNAKE,
    MT_RSNAKE,
    MT_BSNAKE,
    MT_GSNAKE,
    MT_NBLACK,
    MT_RTBLACK,
    MT_BTBLACK,
    MT_RBLACK,
    MT_CLEAVER,
    MT_SUCCUBUS,
    MT_SNOWWICH,
    MT_HLSPWN,
    MT_SOLBRNR,
    MT_MEGA,
    MT_GUARD,
    MT_VTEXLRD,
    MT_BALROG,
    MT_COUNSLR,
    MT_INCIN,
    MT_UNRAV,
    MT_LRDSAYTR,
    MT_INVILORD,
    MT_DIABLO, // 92
    MT_NULL = -1,
};

enum _mai_id
{
    AI_ZOMBIE = 0,
    AI_FAT = 1,
    AI_SKELSD = 2,
    AI_SKELBOW = 3,
    AI_SCAV = 4,
    AI_RHINO = 5,
    AI_GOATMC = 6,
    AI_GOATBOW = 7,
    AI_FALLEN = 8,
    AI_MAGMA = 9,
    AI_SKELKING = 10,
    AI_BAT = 11,
    AI_GARG = 12,
    AI_CLEAVER = 13,
    AI_SUCC = 14,
    AI_SNEAK = 15,
    AI_STORM = 16
};

enum _mc_id
{
    MC_UNDEAD = 0,
    MC_DEMON = 1,
    MC_ANIMAL = 2,
};

enum monster_resistance
{
    RESIST_MAGIC = 0x01,
    RESIST_FIRE = 0x02,
    RESIST_LIGHTNING = 0x04,
    IMMUNE_MAGIC = 0x08,
    IMMUNE_FIRE = 0x10,
    IMMUNE_LIGHTNING = 0x20,
    IMMUNE_NULL_40 = 0x40,
};

//
// structs
//

struct MonsterData
{
    int mAnimWidth;
    int mImgSize;
    const char *GraphicType;
    BOOL has_special;
    const char *sndfile;
    BOOL snd_special;
    BOOL has_trans;
    const char *TransFile;
    int Frames[6];
    int Rate[6];
    const char *mName;
    unsigned char mMinDLvl;
    unsigned char mMaxDLvl;
    unsigned char mLevel;
    unsigned char mMinHP;
    unsigned char mMaxHP;
    char mAi; // enum _mai_id
    unsigned char mInt;
    unsigned char mHit;
    unsigned char mAFNum;
    unsigned char mMinDamage;
    unsigned char mMaxDamage;
    unsigned char mHit2;
    unsigned char mAFNum2;
    unsigned char mMinDamage2;
    unsigned char mMaxDamage2;
    unsigned char mArmorClass;
    char mMonstClass; // enum _mc_id
    unsigned short mMagicRes; // enum monster_resistance
    unsigned short mTreasure; // probably an enum?
    char mSelFlag; // probably an enum?
    int mExp; // TODO: unsigned short?
};

struct UniqMonstStruct
{
    char mType; // enum _monster_id
    const char *mName;
    const char *mTrnName;
    unsigned char mlevel;
    unsigned short maxhp;
    unsigned char mAi; // enum _mai_id
    unsigned char mint;
    unsigned char mMinDamage;
    unsigned char mMaxDamage;
    unsigned short mMagicRes; // enum monster_resistance
    unsigned short mUniqAttr; // probably an enum?
    unsigned char mUnqVar1;
    unsigned char mUnqVar2;
};

//
// variables
//

extern MonsterData monsterdata[];
extern char MonstConvTbl[]; // Unused
extern char MonstAvailTbl[];
extern UniqMonstStruct UniqMonst[];

#endif