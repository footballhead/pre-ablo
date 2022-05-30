#include "gendung.h"

#include "engine.h"
#include "enums.h"

//
// uninitialized variables (.data:004CB0D8)
//

BYTE *pLevelPieces;
int currlevel;
int dword_4CB0E0; // TODO
int dword_4CB0E4; // TODO
BOOL nTransTable[MAXTILES + 1];
char dItem[MAXDUNX][MAXDUNY];
BOOL nSolidTable[MAXTILES + 1];
int dmaxx;
int dmaxy;
// There's enough room for 3 ints here so it's probably not alignment...
int dword_4D21FC; // unused 1
int dword_4D2200; // unused 2
int dword_4D2204; // unused 3
WORD level_frame_types[MAXTILES];
int level_frame_count[MAXTILES];
char TransVal;
char dPlayer[MAXDUNX][MAXDUNY];
int dminx;
BYTE *pSpeedCels;
int dminy;
ScrollStruct ScrollInfo;
char dFlags[MAXDUNX][MAXDUNY]; // NOTE: dflags (note case!) exists in retail, I can't find it in demo...
int SpeedFrameTbl[128][16];
char dPlrPath[MAXDUNX][MAXDUNY]; // not in Devilution? something path related
char dPreLight[MAXDUNX][MAXDUNY];
BYTE *pDungeonCels;
int portal_leveltype;
int leveltype;
char dTransVal[MAXDUNX][MAXDUNY];
int ViewX;
int ViewY;
int portal_ViewX; // demo specific
int portal_ViewY; // demo specific
BYTE dungeon[DMAXX][DMAXY];
char block_lvid[MAXTILES + 1];
int tile_defs[MAXTILES];
BYTE *pSpecialCels;
int setlvlnum;
int dDead[MAXDUNX][MAXDUNY];
BOOL portal_setlevel; // demo specific
BOOL TransList[256];
int dPiece[MAXDUNX][MAXDUNY];
char dSpecial[MAXDUNX][MAXDUNY];
int portal_currlevel_or_setlvlnum; // demo specific
char dLight[MAXDUNX][MAXDUNY];
BOOL nMissileTable[MAXTILES + 1];
BOOL nTrapTable[MAXTILES + 1];
int MouseOffX; // demo specific
int MouseOffY; // demo specific
int ViewBX;
int ViewBY;
int ViewDX;
int setpc_h;
int ViewDY;
int setpc_w;
int setpc_x;
int setpc_y;
BOOL portal_save_load_level; // demo specific
MICROS dpiece_defs_map_1[MAXDUNX * MAXDUNY];
int level_frame_sizes[MAXTILES];
int dMonster[MAXDUNX][MAXDUNY];
char dObject[MAXDUNX][MAXDUNY];
BOOL setlevel;
BOOL nBlockTable[MAXTILES + 1];
char dMissile[MAXDUNX][MAXDUNY];
MICROS dpiece_defs_map_2[MAXDUNX][MAXDUNY];
BYTE pdungeon[DMAXX][DMAXY];
int nlevel_frames;
BYTE *pMegaTiles;

//
// code (.text:0040CF40)
//

// .text:0040CF40
void FillSolidBlockTbls()
{
    DWORD dwTiles;
    BYTE *pTmp;
    BYTE *pSBFile;
    int i;
    BYTE bv;

    for (i = 0; i < MAXTILES; i++)
    {
        nBlockTable[i] = FALSE;
        nSolidTable[i] = FALSE;
        nTransTable[i] = FALSE;
        nMissileTable[i] = FALSE;
        nTrapTable[i] = FALSE;
    }

    nBlockTable[0] = FALSE;
    nSolidTable[0] = FALSE;
    nMissileTable[0] = FALSE;

    switch (leveltype)
    {
    case DTYPE_TOWN:
        pSBFile = LoadFileInMem("Levels\\TownData\\Town.SOL");
        dwTiles = FileGetSize("Levels\\TownData\\Town.SOL");
        break;
    case DTYPE_OLD_CATHEDRAL:
        pSBFile = LoadFileInMem("Levels\\L1Data\\L1.SOL");
        dwTiles = FileGetSize("Levels\\L1Data\\L1.SOL");
        break;
    case DTYPE_CATACOMBS:
        pSBFile = LoadFileInMem("Levels\\L2Data\\L2.SOL");
        dwTiles = FileGetSize("Levels\\L2Data\\L2.SOL");
        break;
    case DTYPE_CAVES:
        pSBFile = LoadFileInMem("Levels\\L3Data\\L3.SOL");
        dwTiles = FileGetSize("Levels\\L3Data\\L3.SOL");
        break;
    case DTYPE_HELL:
        pSBFile = LoadFileInMem("Levels\\L2Data\\L2.SOL");
        dwTiles = FileGetSize("Levels\\L2Data\\L2.SOL");
        break;
    case DTYPE_CATHEDRAL:
        pSBFile = LoadFileInMem("Levels\\L1Data\\L1.SOL");
        dwTiles = FileGetSize("Levels\\L1Data\\L1.SOL");
        break;
    }

    pTmp = pSBFile;

    for (i = 1; dwTiles >= i; i++)
    {
        bv = *pTmp++;
        if (bv & 1)
            nSolidTable[i] = TRUE;
        if (bv & 2)
            nBlockTable[i] = TRUE;
        if (bv & 4)
            nMissileTable[i] = TRUE;
        if (bv & 8)
            nTransTable[i] = TRUE;
        if (bv & 0x80)
            nTrapTable[i] = TRUE;
        block_lvid[i] = (bv & 0x70) >> 4;
    }

    MemFreeDbg(pSBFile);
}

// .text:0040D1BE
// Same as Devilution
static void SwapTile(int f1, int f2)
{
    int swap;

    swap = level_frame_count[f1];
    level_frame_count[f1] = level_frame_count[f2];
    level_frame_count[f2] = swap;
    swap = tile_defs[f1];
    tile_defs[f1] = tile_defs[f2];
    tile_defs[f2] = swap;
    swap = level_frame_types[f1];
    level_frame_types[f1] = level_frame_types[f2];
    level_frame_types[f2] = swap;
    swap = level_frame_sizes[f1];
    level_frame_sizes[f1] = level_frame_sizes[f2];
    level_frame_sizes[f2] = swap;
}

// .text:0040D295
// Basically the same as Devilution
static void SortTiles(int in_frames)
{
    BOOL doneflag;
    int i;
    int frames;

    frames = in_frames;

    doneflag = FALSE;
    while (frames > 0 && !doneflag)
    {
        doneflag = TRUE;
        for (i = 0; i < frames; i++)
        {
            if (level_frame_count[i] < level_frame_count[i + 1])
            {
                SwapTile(i, i + 1);
                doneflag = FALSE;
            }
        }
        frames--;
    }
}

// .text:0040D328
void MakeSpeedCels()
{
    int i;
    for (i = 0; i < MAXTILES; i++)
    {
        tile_defs[i] = i;
        level_frame_count[i] = 0;
    }

    // TODO
}

// IsometricCoord	000000000040D804
int IsometricCoord(int x, int y)
{
    // TODO
    return 0;
}

// RotateMicros	000000000040D89C
// SetDungeonMicros	000000000040D926
// DRLG_InitTrans	000000000040DAAA
// DRLG_MRectTrans	000000000040DB47
// __dc_gendung_41264A	000000000040DBEE

// .text:0040DCE1
void DRLG_RectTrans(int x1, int y1, int x2, int y2)
{
    int i, j;

    for (j = y1; j <= y2; j++)
    {
        for (i = x1; i <= x2; i++)
        {
            dTransVal[i][j] = TransVal;
        }
    }
    TransVal++;
}

// DRLG_CopyTrans	000000000040DD5C

// .text:0040DDA5
void DRLG_ListTrans(int num, BYTE *List)
{
    BYTE x1, x2, y1, y2;
    int i;

    for (i = 0; i < num; i++)
    {
        x1 = *List++;
        y1 = *List++;
        x2 = *List++;
        y2 = *List++;
        DRLG_RectTrans(x1, y1, x2, y2);
    }
}

// .text:0040DE27
void DRLG_AreaTrans(int num, BYTE *List)
{
    BYTE x1, x2, y1, y2;
    int i;

    for (i = 0; i < num; i++)
    {
        x1 = *List++;
        y1 = *List++;
        x2 = *List++;
        y2 = *List++;
        DRLG_RectTrans(x1, y1, x2, y2);
        TransVal--;
    }
    TransVal++;
}

// DRLG_SetPC	000000000040DEB5
// gendung_GetPortalLevel	000000000040DF73
// gendung_GetPortalLvlPos	000000000040DFD1
