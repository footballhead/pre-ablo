#ifndef __MISSILES_H__
#define __MISSILES_H__

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
    MIS_RNDTELEPORT = 3,
    MIS_LIGHTBALL = 4,
    MIS_FIREWALL = 5,
    MIS_FIREBALL = 6,
    MIS_LIGHTCTRL = 7,
    MIS_TOWN = 10,
    MIS_FLASH = 11,
    MIS_FLASH2 = 12,
    MIS_MANASHIELD = 13,
    MIS_FIREMOVE = 14,
    MIS_SENTINAL = 15,
    MIS_CHAIN = 0x10,
    MIS_CHAIN2 = 0x11, // this is demo exclusive (check beta??)
    MIS_TELEPORT = 0x1B,
    MIS_FARROW = 0x1C,
    MIS_DOOMSERP = 0x1D,
    MIS_STONE = 0x1F,
    MIS_INVISIBL = 0x21,
    MIS_GOLEM = 0x22,
    MIS_ETHEREALIZE = 0x23,
    MIS_BLODBUR = 0x24,
    MIS_BOOM = 0x25,
};

//
// structs
//

#pragma pack(push, 8)
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
    int _miFlags;
    int _miAnimData;
    int anonymous_7;
    int anonymous_8;
    int _miAnimLen;
    int anonymous_9;
    int _miAnimFrame;
    int _miAnimWidth;
    int _miAnimWidth2;
    int _miLightFlag;
    int _miPreFlag;
    int _miUniqTrans;
    int _mirange;
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
#pragma pack(pop)

//
// variables
//

extern int nummissiles;
extern int missileactive[MAXMISSILES];
extern MissileStruct missile[MAXMISSILES];

//
// functions
//

int AddMissile(int sx, int sy, int dx, int dy, int midir, int mitype, int micaster, int id, int midam);

#endif