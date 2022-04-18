// This might be called loadsave? Hard to tell since this has both loadsave and gmenu functions

#include "gmenu.h"

#include "defines.h"
#include "diablo.h"
#include "enums.h"
#include "structs.h"

#include <windows.h>

extern char savedir_abspath[64];

extern char* tbuff;
extern BOOL demo_mode;
extern BOOL setlevel;
extern int setlvlnum;
extern PlayerStruct plr[MAX_PLRS];
extern int currlevel;
extern int leveltype;
extern BOOL chrflag;
extern BOOL invflag;
extern int gbActivePlayers;
extern int numquests;
extern QuestStruct quests[MAXQUESTS];
extern int monstkills[200];

// BEGIN FUNCTIONS (no functions before this point)

// gmenu_init_menu
// DrawBigFontXY
// gmenu_draw
// gmenu_draw_0 (bad name...)
// gmenu_up
// gmenu_down
// BLoad

int WLoad() {
    int rv;
    if (*tbuff & 0x80) {
        rv = 0xFFFF0000;
    } else {
        rv = 0;
    }
    rv |= *tbuff++ << 8;
    rv |= *tbuff++;
    return rv;
}

// ILoad

BOOL OLoad() {
    char* v = tbuff++;
    if (*v == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static void LoadPlayer(int i)
{
    // Demo difference! Omit the final 9 (not 10) pointers
    memcpy(&plr[i], tbuff, sizeof(*plr) - (9 * sizeof(void *)));
    tbuff += sizeof(*plr) - (9 * sizeof(void *));
}

// LoadMonster
// LoadMisile
// LoadSpell
// LoadObject
// LoadItem

static void LoadQuest(int i) {
    memcpy(&quests[i], tbuff, sizeof(*quests));
    tbuff += sizeof(*quests);
}

// LoadLighting
// LoadVision

// .text:00460D3F
void LoadGame(BOOL firstflag) {
    int i;

    if (firstflag == FALSE) {
        FreeGameMem();
    }

    char path_name[128];

    _sprintf(path_name, "%s\\Game00.sav", savedir_abspath);

    HFILE hFile = _lopen(path_name, OF_SHARE_DENY_WRITE | OF_READ);
    LONG size = _llseek(hFile, 0, 2);
    LPVOID lpBuffer = GlobalLock(GlobalAlloc(GMEM_FIXED, size));
    tbuff = lpBuffer;
    _llseek(hFile, 0, 0);
    _lread(hFile, lpBuffer, size);
    _lclose(hFile);

    // Yes this is literally the decomp.
    if (demo_mode != FALSE) {
        tbuff += 3;
    } else {
        tbuff += 3;
    }

    setlevel = OLoad();
    setlvlnum = WLoad();
    currlevel = WLoad();
    leveltype = WLoad();
    int _ViewX = WLoad();
    int _ViewY = WLoad();
    invflag = OLoad();
    chrflag = OLoad();
    gbActivePlayers = WLoad();
    int _nummonsters = WLoad();
    int _numitems = WLoad();
    int _nummissiles = WLoad();
    int _numspells = WLoad();
    int _numobjects = WLoad();
    plr[0].plractive = OLoad();
    plr[1].plractive = OLoad();
    plr[2].plractive = OLoad();
    plr[3].plractive = OLoad();

    for (i = 0; i < gbActivePlayers; ++i) {
        if (plr[i].plractive) {
            LoadPlayer(i);
        }
    }

    numquests = WLoad();
    for (i = 0; i < MAXQUESTS; i++) {
        LoadQuest(i);
    }

    LoadGameLevel(firstflag, ENTRY_LOAD);

    // for (i = 0; i < MAXMONSTERS; i++) {
    //     monstkills[i] = ILoad();
    // }

    // TODO
}

// BSave
// WSave
// ISave
// OSave
// SavePlayer
// SaveMonster
// SaveMissile
// SaveSpell
// SaveObject
// SaveItem
// SaveQuest
// SaveLighting
// SaveVision
// save_game
// gmenu_send_quit_cmd
// gmenu_select_option
// gmenu_left_mouse
// FreeGMenu
// SaveLevel
// LoadLevel

// END FUNCTIONS (no functions after this point)