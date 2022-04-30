#include "cursor.h"

#include "defines.h"
#include "diablo.h"
#include "engine.h"
#include "missiles.h"

#include <windows.h>

//
// imports
//

extern int ViewX;
extern int ViewY;
extern int MouseOffX; // This value is used but never initalized :X
extern int MouseOffY; // This value is used but never initalized :X
                      // bottom-right of cursor sprite in gpBuffer
extern BYTE *gpBufCursPos;

//
// initialized vars (.data:004BBB08)
//

int InvItemWidth[] = {
    0,
    33,
    33,
    32,
    32,
    32,
    32,
    32,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56};
int InvItemHeight[] = {
    0,
    29,
    29,
    32,
    32,
    32,
    32,
    32,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    28,
    56,
    56,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    56,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
    84,
};

//
// uninitialized vars (.data:00617530)
//

// DWORD
// DWORD
// DWORD
BYTE *pCursCels; // Loaded but never freed :(
char pcursinvitem;
char pcursobj;
int pcurstemp;
int cursH;
int cursW;
int pcurs;
BYTE sgSaveBack[8192]; // Only the first 256 bytes are zeroed
// DWORD
// DWORD
int cursmx;
int cursmy;
int pcursmonst;
char pcursitem;

//
// code (.text:00479C90)
//

// .text:00479C90
void InitCursor()
{
    pCursCels = LoadFileInMem("Data\\Inv\\Objcurs.CEL");
}

// .text:00479CAF
void SetCursor_(int i)
{
    pcurs = i;
    cursW = InvItemWidth[i];
    cursH = InvItemHeight[i];
}

// .text:00479CEF
void InitLevelCursor(HWND hWnd)
{
    RECT client_rect;
    int view_center_x; // Written, never read
    int view_center_y; // Written, never read
    int i;

    SetCursor_(CURSOR_HAND);
    cursmx = ViewX;
    cursmy = ViewY;
    pcurstemp = -1;
    pcursmonst = -1;
    pcursobj = -1;
    pcursitem = -1;

    GetClientRect(hWnd, &client_rect);
    view_center_x = client_rect.left + MouseOffX + SCREEN_WIDTH / 2;
    view_center_y = client_rect.top + MouseOffY + VIEWPORT_HEIGHT / 2;

    // TODO: Set a bunch of globals to 0

    gpBufCursPos = gpBuffer;
    for (i = 0; i < 256; i++)
    {
        sgSaveBack[i] = 0;
    }
}

// .text:00479DFA
// Dead code. This code is the same as parts of InitLevelCursor. There is no
// return and no noticable side effects.
void sub_479DFA(HWND hWnd)
{
    RECT client_rect;
    int view_center_x; // Written, never read
    int view_center_y; // Written, never read

    GetClientRect(hWnd, &client_rect);
    view_center_x = client_rect.left + MouseOffX + SCREEN_WIDTH / 2;
    view_center_y = client_rect.top + MouseOffY + VIEWPORT_HEIGHT / 2;
}

// .text:00479E40
static void CheckTown()
{
    int i, mx;

    // TODO

    // for (i = 0; i < nummissiles; i++) {
    // 	mx = missileactive[i];
    // 	if (missile[mx]._mitype == MIS_TOWN) {
    // 		if (cursmx == missile[mx]._mix - 1 && cursmy == missile[mx]._miy
    // 		    || cursmx == missile[mx]._mix && cursmy == missile[mx]._miy - 1
    // 		    || cursmx == missile[mx]._mix - 1 && cursmy == missile[mx]._miy - 1
    // 		    || cursmx == missile[mx]._mix - 2 && cursmy == missile[mx]._miy - 1
    // 		    || cursmx == missile[mx]._mix - 2 && cursmy == missile[mx]._miy - 2
    // 		    || cursmx == missile[mx]._mix - 1 && cursmy == missile[mx]._miy - 2
    // 		    || cursmx == missile[mx]._mix && cursmy == missile[mx]._miy) {
    // 			trigflag = TRUE;
    // 			ClearPanel();
    // 			strcpy(infostr, "Town Portal");
    // 			sprintf(tempstr, "from %s", plr[missile[mx]._misource]._pName);
    // 			AddPanelString(tempstr, TRUE);
    // 			cursmx = missile[mx]._mix;
    // 			cursmy = missile[mx]._miy;
    // 		}
    // 	}
    // }
}

// CheckCursMove	000000000047A069
// savecrsr_hide	000000000047BAA1
// savecrsr_show	000000000047BB2F
