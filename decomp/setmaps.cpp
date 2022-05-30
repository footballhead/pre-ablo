#include "drlg_l1.h"
#include "drlg_l2.h"
#include "gendung.h"
#include "objects.h"
#include "palette.h"
#include "trigs.h"

//
// Initialized variables (.data:004ACA58)
//

BYTE SkelKingTrans1[] = {
    19, 47, 26, 55,
    26, 49, 30, 53,
    18, 49, 18, 49};
BYTE SkelKingTrans2[] = {
    33, 19, 47, 29,
    37, 29, 43, 39};
BYTE SkelKingTrans3[] = {
    27, 53, 35, 61,
    27, 35, 34, 42,
    45, 35, 53, 43,
    45, 53, 53, 61,
    31, 39, 49, 57};
BYTE SkelKingTrans4[] = {
    49, 45, 58, 51,
    57, 31, 62, 37,
    63, 31, 69, 40,
    59, 41, 73, 55,
    63, 55, 69, 65,
    73, 45, 78, 51,
    79, 43, 89, 53};
BYTE SkelChamTrans1[] = {
    43, 19, 50, 26,
    51, 19, 59, 26,
    35, 27, 42, 34,
    43, 27, 49, 34,
    50, 27, 59, 34};
BYTE SkelChamTrans2[] = {
    19, 31, 34, 47,
    34, 35, 42, 42};
BYTE SkelChamTrans3[] = {
    43, 35, 50, 42,
    51, 35, 62, 42,
    63, 31, 66, 46,
    67, 31, 78, 34,
    67, 35, 78, 42,
    67, 43, 78, 46,
    35, 43, 42, 51,
    43, 43, 49, 51,
    50, 43, 59, 51};

//
// Code (.text:0042E080)
//

// ObjIndex	000000000042E080

// AddSKingObjs	000000000042E110
void AddSKingObjs()
{
    // TODO
}

// AddSChamObjs	000000000042E20D
void AddSChamObjs()
{
    // TODO
}

// DRLG_SetMapTrans	000000000042E26E
void DRLG_SetMapTrans(const char *sFileName)
{
    // TODO
}

// LoadSetMap	000000000042E35C
void LoadSetMap()
{
    switch (setlvlnum)
    {
    case 1:
        LoadPreL1Dungeon("Levels\\L1Data\\SklKng1.DUN", 84, 46);
        LoadL1Dungeon("Levels\\L1Data\\SklKng2.DUN", 84, 46);
        LoadPalette("Levels\\L1Data\\L1_2.pal", palette_buffer);
        CopyPalette(menu_palette, palette_buffer);
        DRLG_AreaTrans(sizeof(SkelKingTrans1) / 4, &SkelKingTrans1[0]);
        DRLG_ListTrans(sizeof(SkelKingTrans2) / 4, &SkelKingTrans2[0]);
        DRLG_AreaTrans(sizeof(SkelKingTrans3) / 4, &SkelKingTrans3[0]);
        DRLG_ListTrans(sizeof(SkelKingTrans4) / 4, &SkelKingTrans4[0]);
        AddL1Objs(0, 0, MAXDUNX, MAXDUNY);
        AddSKingObjs();
        InitSKingTriggers();
        break;
    case 2:
        LoadPreL2Dungeon("Levels\\L2Data\\Bonecha2.DUN", 69, 39);
        LoadL2Dungeon("Levels\\L2Data\\Bonecha1.DUN", 69, 39);
        LoadPalette("Levels\\L2Data\\L2_2.pal", palette_buffer);
        CopyPalette(menu_palette, palette_buffer);
        DRLG_ListTrans(sizeof(SkelChamTrans1) / 4, &SkelChamTrans1[0]);
        DRLG_AreaTrans(sizeof(SkelChamTrans2) / 4, &SkelChamTrans2[0]);
        DRLG_ListTrans(sizeof(SkelChamTrans3) / 4, &SkelChamTrans3[0]);
        AddL2Objs(0, 0, MAXDUNX, MAXDUNY);
        AddSChamObjs();
        InitSChambTriggers();
        break;
    case 3:
        LoadPreL1Dungeon("Levels\\L1Data\\Lv1MazeA.DUN", 20, 50);
        LoadL1Dungeon("Levels\\L1Data\\Lv1MazeB.DUN", 20, 50);
        LoadPalette("Levels\\L1Data\\L1_5.pal", palette_buffer);
        CopyPalette(menu_palette, palette_buffer);
        AddL1Objs(0, 0, MAXDUNX, MAXDUNY);
        DRLG_SetMapTrans("Levels\\L1Data\\Lv1MazeA.DUN");
        break;
    }
}
