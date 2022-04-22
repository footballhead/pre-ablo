#include "diablo.h"

#include <windows.h>

#include "ddraw_stub.h"
#include "enums.h"
#include "error.h"
#include "multi.h"
#include "player.h"

//
// extern
//

extern int leveltype;
extern LPVOID pDungeonCels;
extern LPVOID pMegaTiles;
extern LPVOID pMiniTiles;
extern LPVOID pSpecialCels;

void FreeMonsters();
void FreeObjectGFX();
void FreeMissileGFX();
void FreeSpells();
void FreeEffects();
void FreeTownerGFX();
void FreeTownerEffects();

//
// initialized vars (.data:004BC0A8)
//

char registration_table[128] = "REGISTRATION_TABLE"; // wtf is this?
// Mutex "held" duing PaintCallback (set to TRUE on enter, FALSE on return)
BOOL paint_callback_mutex = FALSE;
int force_redraw = 0;
BOOL need_to_repaint = FALSE;
// Controlled by ShowProgress. Will be set to TRUE during certain ShowProgress
// blocks, then set to FALSE when done. This will be used as the final check in
// PaintTimer to actually post WM_DIABPAINT
BOOL paint_mutex = FALSE;
BOOL svgamode = TRUE;
int unused000 = 0;
int unused001 = 0;
int unused002 = 1;
int unused003 = 1;
int unused004 = -1;
char version_string[8] = "V1.0";
BOOL debug_mode = FALSE;
BOOL demo_mode = TRUE;
BOOL show_intros = TRUE;
BOOL debugMusicOn = TRUE;
BOOL cheat_mode = FALSE;
BOOL frameflag = FALSE;
BOOL scrollflag = FALSE;
BOOL light4flag = FALSE;
BOOL musicFromDisk = FALSE;
HANDLE sghMusic = 0;
DWORD PauseMode = 0;
BOOL gbMusicOn = TRUE;
DWORD dword_61B758 = 0; // UNUSED
const char* sgszMusicTracks[] = { // indexed by leveltype
    "Music\\DTowne.wav",
    "Music\\DLvlA.wav",
    "Music\\DLvlA.wav",
    "Music\\DLvlA.wav",
    "Music\\DLvlA.wav",
    "Music\\DLvlA.wav",
};

//
// uninitialized vars (.data:0061B700)
//

BYTE *gpBuffer;
DWORD frames; // updated regularly
DWORD frames_drawn; // updated regulary
DWORD draw_framerate; // 1sec snapshot
int framerate; // 1sec snapshot
// ...
LPDIRECTDRAWSURFACE lpDDSBackBuf;
HWND ghMainWnd;
int MouseX;
int MouseY;
// ...
BOOL can_fade;
// ...
int main_menu_screen;
BOOL shouldStopPaintTimer;
DWORD dword_61B758; // UNUSED
// ...
char savedir_abspath[64];
// ...
BOOL fullscreen;
// ...
DWORD prevTime;
// ...
DWORD dword_61BFC0; // UNUSED
// ...
BOOL some_DirectDraw_option;
BOOL allow_mp_action_send;
DWORD dword_61BFD0;
DWORD prev_timer_PostMessage_time;

//
// code (.text:00484A20)
//

// .text:00484A20
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                      LPSTR lpCmdLine, int nShowCmd)
{
    gbActivePlayers = 1;
    fullscreen = TRUE;
    if (gbActivePlayers == 1)
    {
        isSinglePlayer = TRUE;
        myplr = 0;
    }

    for (int i = 0; i < MAX_PLRS; i++)
    {
        plr[i].to_send_message_id = 42; // TODO what is 42? (is it just a meme?)
    }

    allow_mp_action_send = TRUE;
    can_fade = TRUE;
    shouldStopPaintTimer = FALSE;
    main_menu_screen = 0;
    some_DirectDraw_option = TRUE;
    frames = 0;
    frames_drawn = 0;
    draw_framerate = 0;
    dword_61B758 = 0;
    dword_61BFC0 = 0;
    msgflag = 0;

    // TODO
}

// dx_init	0000000000484E67
// WNDPROC_mode0_blizzard_intro	00000000004853DA
// diablo_48565F	000000000048565F
// GM_Game	00000000004865C4
// MainWndProc	00000000004881C9
// diablo_store_with_mutex_spinlock	00000000004882F9

// .text:0048833D
// The sole purpose of this timer callback seems to be to send WM_DIABPAINT
// messages every 50ms.
void CALLBACK PaintTimer(UINT uTimerID,
                            UINT uMsg,
                            DWORD_PTR dwUser,
                            DWORD_PTR dw1,
                            DWORD_PTR dw2)
{
    if (paint_callback_mutex) {
        return;
    }

    paint_callback_mutex = TRUE;
    DWORD now = timeGetTime();
    frames++;
    if (now - prevTime >= 1000) {
        framerate = frames;
        frames = 0;

        prevTime = now;

        draw_framerate = frames_drawn;
        frames_drawn = 0;
    }

    if (now - prev_timer_PostMessage_time >= 50) {
        if (can_fade == FALSE) {
            can_fade = TRUE;
            if (paint_mutex == FALSE) {
                PostMessage((HWND)dwUser, 0x400, 0, 0); // 0x400 = WM_DIABPAINT
            }

            prev_timer_PostMessage_time = now;
        }
    }

    if (shouldStopPaintTimer) {
        timeKillEvent(uTimerID);
        ExitThread(0);
    }

    paint_callback_mutex = FALSE;
}

// InitLevelType	0000000000488442
// init_multi_with_time_srand	00000000004884C8
// init_create_window	0000000000488769
// dx_cleanup	000000000048885D

// .text:004888E2
void FreeGameMem()
{
    GlobalUnlock(GlobalHandle(pDungeonCels));
    GlobalFree(GlobalHandle(pDungeonCels));

    GlobalUnlock(GlobalHandle(pMegaTiles));
    GlobalFree(GlobalHandle(pMegaTiles));

    GlobalUnlock(GlobalHandle(pMiniTiles));
    GlobalFree(GlobalHandle(pMiniTiles));

    GlobalUnlock(GlobalHandle(pSpecialCels));
    GlobalFree(GlobalHandle(pSpecialCels));

    if (leveltype != DTYPE_TOWN)
    {
        GlobalUnlock(GlobalHandle(pSpecialCels));
        GlobalFree(GlobalHandle(pSpecialCels));

        FreeMonsters();
        FreeObjectGFX();
        FreeMissileGFX();
        FreeSpells();
        FreeEffects();
    }
    else
    {
        FreeTownerGFX();
        FreeMissileGFX();
        FreeTownerEffects();
    }
}
// LoadLvlGFX	00000000004889EA
// LoadAllGFX	0000000000488BC1
// CreateLevel	0000000000488C54

void LoadGameLevel(BOOL firstflag, int lvldir)
{
    // TODO
    // calls LoadLevel
}

// InitLevels	000000000048947E
// diablo_489510_plr_rel	0000000000489510
// game_logic	0000000000489715
