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

//
// variables
//

extern BYTE dungeon[DMAXX][DMAXY];
extern BYTE pdungeon[DMAXX][DMAXY];
extern char dObject[MAXDUNX][MAXDUNY];
extern char dPiece[MAXDUNX][MAXDUNY];
extern char dFlags[MAXDUNX][MAXDUNY];
extern char dMonster[MAXDUNX][MAXDUNY];
extern char dSpecial[MAXDUNX][MAXDUNY];
extern char dPlayer[MAXDUNX][MAXDUNY];
extern BOOLEAN nSolidTable[MAXTILES + 1];
extern BOOLEAN nTrapTable[MAXTILES + 1];
extern BYTE *pMegaTiles;
extern int currlevel;
extern int leveltype;
extern MICROS dpiece_defs_map_1[MAXDUNX * MAXDUNY];
extern int setpc_x;
extern int setpc_y;
extern int setpc_w;
extern int setpc_h;

//
// functions
//

int IsometricCoord(int x, int y);

#endif