#include "themes.h"

#include <windows.h>

#include "engine.h"
#include "enums.h"
#include "objects.h"

extern int leveltype;

//
// defines
//

#define NUM_GOOD_THEMES 3
#define NUM_THEME_IDS 9

//
// initialized variables (.data:004BB760)
//

int ThemeGood[NUM_GOOD_THEMES] = {1, 3, 5}; // TODO magic numbers
int trm5x[] = {
    -2, -1, 0, 1, 2,
    -2, -1, 0, 1, 2,
    -2, -1, 0, 1, 2,
    -2, -1, 0, 1, 2,
    -2, -1, 0, 1, 2};
int trm5y[] = {
    -2, -2, -2, -2, -2,
    -1, -1, -1, -1, -1,
    0, 0, 0, 0, 0,
    1, 1, 1, 1, 1,
    2, 2, 2, 2, 2};
// trm3x is unused
int trm3x[] = {
    -1, 0, 1,
    -1, 0, 1,
    -1, 0, 1};
int trm3y[] = {
    -1, -1, -1,
    0, 0, 0,
    1, 1, 1};

//
// uninitialized variables (.data:00617320)
//

DWORD dword_617318;
DWORD dword_61731C;
DWORD dword_617320;
DWORD dword_617324;
DWORD dword_617328;
int numthemes;
ThemeStruct themes[MAXTHEMES];
BOOL ThemeGoodIn[NUM_GOOD_THEMES];

//
// code (.text:00477820)
//

// TFit_themes_482730	0000000000477820
// TFit_Shrine	00000000004778F4
// TFit_Obj5	0000000000477BC4
// TFit_SkelRoom	0000000000477D78

// .text:00477E0B
BOOL SpecialThemeFit(int i, int t)
{
    // TODO
}

// .text:00477EB2
BOOL CheckThemeRoom(int i)
{
    // TODO
}

// .text:0047815D
void InitThemes()
{
    int j;
    int i;

    numthemes = 0;
    // Only try to place theme rooms in the cathedral
    if (leveltype == DTYPE_CATHEDRAL)
    {
        for (i = 0; i < NUM_GOOD_THEMES; i++)
        {
            ThemeGoodIn[i] = FALSE;
        }

        for (i = 0; i < 256 && numthemes < MAXTHEMES; i++)
        {
            if (CheckThemeRoom(i))
            {
                themes[numthemes].ttval = i;
            }

            j = ThemeGood[random_(NUM_GOOD_THEMES)];
            while (TRUE)
            {
                if (!SpecialThemeFit(numthemes, j))
                {
                    j = random_(NUM_THEME_IDS);
                }
            }

            themes[numthemes].ttype = j;
            numthemes++;
        }
    }
}

// HoldThemeRooms	0000000000478258
// CheckThemeObj3	000000000047832B
// PlaceThemeMonsts	0000000000478457

// Theme_Barrel	0000000000478599
static void Theme_Barrel(int i);

// Theme_Shrine	000000000047867E
static void Theme_Shrine(int i);

// Theme_MonstPit	0000000000478736
static void Theme_MonstPit(int i);

// Theme_SkelRoom	0000000000478876
static void Theme_SkelRoom(int i);

// Theme_Treasure	0000000000478A83
static void Theme_Treasure(int i);

// Theme_Library	0000000000478BC5
static void Theme_Library(int i);

// Theme_Torture	0000000000478CF0
static void Theme_Torture(int i);

// Theme_BloodFountain	0000000000478D9A
static void Theme_BloodFountain(int i);

// Theme_Decap	0000000000478DDB
static void Theme_Decap(int i);

// .text:0000000000478E7D
void CreateThemeRooms()
{
    int i;

    InitObjFlag = TRUE;
    for (i = 0; i < numthemes; i++)
    {
        switch (themes[i].ttype)
        {
        case THEME_BARREL:
            Theme_Barrel(i);
            break;
        case THEME_SHRINE:
            Theme_Shrine(i);
            break;
        case THEME_MONSTPIT:
            Theme_MonstPit(i);
            break;
        case THEME_SKELROOM:
            Theme_SkelRoom(i);
            break;
        case THEME_TREASURE:
            Theme_Treasure(i);
            break;
        case THEME_LIBRARY:
            Theme_Library(i);
            break;
        case THEME_TORTURE:
            Theme_Torture(i);
            break;
        case THEME_BLOODFOUNTAIN:
            Theme_BloodFountain(i);
            break;
        case THEME_DECAPITATED:
            Theme_Decap(i);
            break;
        }
    }

    InitObjFlag = FALSE;
}