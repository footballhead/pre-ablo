#ifndef __MISSILES_H__
#define __MISSILES_H__

#include <windows.h>

//
// defines
//

#define MAXMISSILES 125

//
// enums
//

enum missile_id
{
    MIS_ARROW = 0,
    MIS_FIREBOLT = 1,
    MIS_GUARDIAN = 2,
    MIS_RNDTELEPORT = 3, // phasing
    MIS_LIGHTBALL = 4,   // nova
    MIS_FIREWALL = 5,
    MIS_FIREBALL = 6,
    MIS_LIGHTCTRL = 7,
    MIS_LIGHTNING = 8,
    MIS_MISEXP = 9,
    MIS_TOWN = 10,
    MIS_FLASH = 11,
    MIS_FLASH2 = 12,
    MIS_MANASHIELD = 13,
    MIS_FIREMOVE = 14,
    MIS_SENTINAL = 15, // same as guardian
    MIS_CHAIN = 16,
    MIS_CHAIN2 = 17,  // this is demo exclusive (check beta??)
    MIS_BLOOD = 18,   // TODO does anyone use?
    MIS_BONE = 19,    // TODO does anyone use?
    MIS_METLHIT = 20, // TODO does anyone use?
    MIS_RHINO = 21,
    MIS_MAGMABALL = 22,
    MIS_LIGHTCTRL2 = 23,
    MIS_LIGHTNING2 = 24,
    MIS_FLARE = 25,
    MIS_MISEXP2 = 26,
    MIS_TELEPORT = 27,
    MIS_FARROW = 28,
    MIS_DOOMSERP = 29,
    MIS_FIREWALLA = 30, // TODO: who makes this?
    MIS_STONE = 31,
    MIS_32 = 32,          // TODO: what is this?
    MIS_INVISIBL = 33,    // does nothing (broken MIS_STONE)
    MIS_GOLEM = 34,       // does nothing (broken MIS_STONE)
    MIS_ETHEREALIZE = 35, // does nothing (broken MIS_STONE)
    MIS_BLODBUR = 36,
    MIS_BOOM = 37, // Apocalypse
};

enum missile_resistance
{
    MISR_NONE = 0,
    MISR_FIRE = 1,
    MISR_LIGHTNING = 2,
    MISR_MAGIC = 3,
};

//
// structs
//

struct MissileStruct
{
    int _mitype;
    int _mix;
    int _miy;
    int _mixoff;
    int _miyoff;
    int _mixvel;
    int _miyvel;
    int _misx;
    int _misy;
    int _mitxoff;
    int _mityoff;
    int _mimfnum;
    int _miDelFlag;
    int _miAnimFlags;
    BYTE *_miAnimData;
    int _miAnimDelay;
    int _miAnimCnt;
    int _miAnimLen;
    int _miAnimAdd;
    int _miAnimFrame;
    int _miAnimWidth;
    int _miAnimWidth2;
    int _miLightFlag;
    int _miPreFlag;
    int _miUniqTrans;
    int _mirange; // Can either mean "how far the missile can travel" or "how long the effect lasts"; depends on the spell specifically
    int _misource;
    int _micaster;
    int _midam;
    int _midist;
    int _mirnd;
    int _mlid;
    int _miVar1;
    int _miVar2;
    int _miVar3;
    int _miVar4;
    int _miVar5;
    int _miVar6;
    int _miVar7;
    int _miVar8;
};

//
// variables
//

extern int nummissiles;
extern int missileactive[MAXMISSILES];
extern MissileStruct missile[MAXMISSILES];
extern BYTE *pMagballCels[8];
extern BYTE *pThinLghningCel;
extern BYTE *pFlareCel;
extern BYTE *pFlarexpCel;

//
// functions
//

int AddMissile(int sx, int sy, int dx, int dy, int midir, int mitype, int micaster, int id, int midam);
void FreeMissileGFX();
void InitMissileGFX();

#endif