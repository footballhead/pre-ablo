// Theme rooms are rooms of interest placed in the dungeon in addition to the
// regular dynamic random level generator. E.g. A library is a theme room, as
// is a shrine room, etc.

#ifndef __THEMES_H__
#define __THEMES_H__

//
// defines
//

#define MAXTHEMES 50

//
// enums
//

enum theme_id
{
    THEME_BARREL = 0x0,
    THEME_SHRINE = 0x1,
    THEME_MONSTPIT = 0x2,
    THEME_SKELROOM = 0x3,
    THEME_TREASURE = 0x4,
    THEME_LIBRARY = 0x5,
    THEME_TORTURE = 0x6,
    THEME_BLOODFOUNTAIN = 0x7,
    THEME_DECAPITATED = 0x8,
    THEME_NONE = -1,
};

//
// structs
//

struct ThemeStruct
{
    // one of theme_id
    char ttype; /* aligned 4 */
    // padding (3 bytes)
    int ttval;
};

//
// variables
//

extern int numthemes;
extern ThemeStruct themes[MAXTHEMES];

//
// functions
//

// Figure out which theme rooms fit in the generated level. Can access `themes`
// after calling up to `numthemes`.
void InitThemes();

#endif