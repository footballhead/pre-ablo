#include "control.h"

#include "spells.h"

#include <windows.h>

//
// initialized vars (.data:004A3450)
//

// wtf is this? nobody references it
static char registration_block[128] = "REGISTRATION_BLOCK";
const BYTE fontframe[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 54, 44, 57, 0, 56, 55, 47, 40, 41, 0, 39, 50, 37, 51, 52,
    36, 27, 28, 29, 30, 31, 32, 33, 34, 35, 48, 49, 58, 38, 59, 53,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 42, 0, 43, 0, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 40, 0, 41, 0, 0};
// TODO: Use SCREENXY macros
int lineOffsets[4][4] = {
    {0x70BF1, 0x6000, 0x6000, 0x6000},
    {0x6EDF1, 0x72CF1, 0x6000, 0x6000},
    {0x6E4F1, 0x70BF1, 0x735F1, 0x6000},
    {0x6DBF1, 0x6FCF1, 0x71DF1, 0x73EF1}};
BOOL spselflag = FALSE;
char SpellITbl[MAX_SPELLS] = {
    1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 37, 13, 12, 17, 16, 14,
    18, 19, 11, 20, 15, 21, 23, 24, 25, 22, 34, 38, 46};
// TODO rect struct
int PanBtnPos[6][4] = {
    {15, 367, 52, 24},
    {573, 367, 52, 24},
    {15, 404, 27, 24},
    {15, 441, 27, 24},
    {599, 404, 27, 24},
    {599, 441, 27, 24}};
const char *PanBtnStr[] = {
    "Inventory",
    "Character Information",
    "Automap",
    "Main menu",
    "Send multiplayer message",
    "Message filter"};
// TODO rect struct
int ChrBtnsRect[6][4] = {
    {336, 90, 95, 22},
    {457, 138, 41, 22},
    {457, 166, 41, 22},
    {457, 195, 41, 22},
    {457, 223, 41, 22},
    {0, 0, 0, 0}};

//
// uninitialized vars (.data:)
//

BOOL drawbtnflag;
BOOL drawhpflag;
// Index into plr[myplr].InvList.
// if -1 then there is nothing being hovered
// Set by draw_potbox string. Used to set player potbox selection in control_do_update_potbox.
int potbox_hover;
BOOL durflag;
BOOL drawPanelString;
BYTE *pPanelText;
BYTE *pStatusPanel;
char infostr[64];
// TODO dword_5DDE00
// dword_5DDE04
// dword_5DDE08
// dword_5DDE0C
// dword_5DDE10
BYTE *pBtmBuff;
BYTE *pSpelIcon_cel;
BYTE *pManaBuff;
char tempstr[64];
BOOL left_mb_pressed;
// TODO
BOOL drawmanaflag;
// TODO
BOOL drawpotboxflag;
// TODO
BOOL pinfoflag;
// TODO
char infoclr;
// TODO
BOOL chrflag;

//
// code (.text:000000000040EF80)
//

// DrawSpellCel	000000000040EF80
// SetSpellTrans	000000000040F059
// DrawSpell	000000000040F28D
// DrawSpellList	000000000040F3C8
// SetSpell	000000000040F95C
// SetSpeedSpell	000000000040F9D8
// ToggleSpell	000000000040FA49
// CPrintString	000000000040FC09
// control_draw_some_string	000000000040FDCE

// AddPanelString	000000000040FF10
void AddPanelString(const char *str, BOOL just)
{
    // TODO
}

// ClearPanel	000000000040FF70
// CopyCtrlPan	000000000041001E
// control_4100A7	00000000004100A7
// InitPanelStr	0000000000410125
// BuffCopy	0000000000410162
// TransBuffCopy	00000000004101CC
// DrawHealthTop	000000000041021C
// DrawHealthBar	00000000004102D5
// DrawManaTop	00000000004103E6
// DrawManaBar	000000000041049F
// InitControlPan	00000000004105F9
// control_416813	000000000041090E
// ClearCtrlPan	0000000000410946
// DrawCtrlPan	000000000041097D
// DoSpeedBook	0000000000410A61
// draw_potion_box	0000000000410C67
// control_potbox_checkitem	0000000000410D66
// draw_potbox_string	0000000000410E94
// control_do_update_potbox	00000000004111C5
// DeterminePotionBoxItems	000000000041121E
// control_check_btn_press	000000000041147C
// CheckPanelInfo	00000000004115F1
// CheckBtnUp	0000000000411D75
// FreeControlPan	0000000000411F94
// control_WriteStringToBuffer	00000000004120FA
// control_print_info_str	0000000000412188
// DrawInfoBox	00000000004122A1
// ADD_PlrStringXY	00000000004125E4
// DrawChr	0000000000412719
// CheckLvlBtn	0000000000413704
// ReleaseLvlBtn	0000000000413770
// DrawLevelUpIcon	00000000004137D4
// CheckChrBtns	000000000041383D
// ReleaseChrBtns	0000000000413B08
// DrawDurIcon	0000000000413D0C
// RedBack	000000000041414F
// DrawPause	00000000004141B4
