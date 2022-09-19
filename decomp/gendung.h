#ifndef __GENDUNG_H__
#define __GENDUNG_H__

#include <windows.h>

//
// defines
//

#define MAXTILES 2048

// Size of level in each direction in tiles (1x1 tiles)
#define MAXDUNX 112
#define MAXDUNY 112

// Size of level in each direction in megatiles (2x2 tiles)
#define DMAXX 40
#define DMAXY 40

// I think MAXDUNX = DMAXX * 2 + 16 + 16. Based on InitRndLocObj, there's a 16
// minitile buffer around the perimeter of the level

//
// enums
//

// TODO: Copied from Devilution, verify
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

//
// structs
//

// A micro tile is made up of 16 smaller CEL pieces (defined by the .MIN file).
// (For reference, the player occupies the space of a micro tile.)
// Tiles are then grouped into 2x2 to form megatiles (defined by the .TIL file).
// Only the first 10 can be used in the dungeon. All 16 can be used in town.
struct MICROS
{
    WORD mt[16];
};

struct ScrollStruct
{
    int _sxoff;
    int _syoff;
    int _sdx;
    int _sdy;
    int _sdir;
};

//
// variables
//

extern BYTE dungeon[DMAXX][DMAXY];
extern BYTE pdungeon[DMAXX][DMAXY];
extern char dObject[MAXDUNX][MAXDUNY];
extern int dPiece[MAXDUNX][MAXDUNY];
extern char dFlags[MAXDUNX][MAXDUNY];
extern int dMonster[MAXDUNX][MAXDUNY];
extern char dSpecial[MAXDUNX][MAXDUNY];
extern char dPlayer[MAXDUNX][MAXDUNY];
extern char dMissile[MAXDUNX][MAXDUNY];
extern BOOL nSolidTable[MAXTILES + 1];
extern BOOL nTrapTable[MAXTILES + 1];
extern BYTE *pMegaTiles;
extern int currlevel;
extern int leveltype;
extern MICROS dpiece_defs_map_1[MAXDUNX * MAXDUNY];
extern MICROS dpiece_defs_map_2[MAXDUNX][MAXDUNY];
extern int setpc_x;
extern int setpc_y;
extern int setpc_w;
extern int setpc_h;
extern int setlvlnum;
extern BOOL setlevel;
extern BYTE *pDungeonCels;
extern BYTE *pMegaTiles;
extern BYTE *pLevelPieces;
extern BYTE *pSpecialCels;
extern BYTE *pSpeedCels;
extern int ViewX;
extern int ViewY;
extern int MouseOffX; // This value is used but never initalized :X
extern int MouseOffY; // This value is used but never initalized :X
extern ScrollStruct ScrollInfo;

//
// functions
//

int IsometricCoord(int x, int y);
void DRLG_ListTrans(int num, BYTE *List);
void DRLG_AreaTrans(int num, BYTE *List);

#endif