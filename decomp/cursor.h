#ifndef __CURSOR_H__
#define __CURSOR_H__

#include <windows.h>

//
// defines
//

//
// enums
//

enum cursor_id
{
    CURSOR_NONE = 0x1,
    CURSOR_HAND = 0x2,
    CURSOR_IDENTIFY = 0x3,
    CURSOR_REPAIR = 0x4,
    CURSOR_RECHARGE = 0x5,
    CURSOR_DISARM = 0x6,
    CURSOR_OIL = 0x7,
    CURSOR_FIRSTITEM = 0x8,
    CURSOR_LASTITEM = 0x7D,
};

//
// structs
//

//
// variables
//

extern BYTE *pCursCels;
extern char pcursinvitem;
extern char pcursobj;
extern int pcurstemp;
extern int cursH;
extern int cursW;
extern int pcurs;
extern int cursmx;
extern int cursmy;
extern int pcursmonst;
extern char pcursitem;

//
// functions
//

void InitCursor();
void SetCursor_(int i);
void InitLevelCursor(HWND hWnd);
void CheckCursMove();
void savecrsr_hide();
void savecrsr_show();

#endif