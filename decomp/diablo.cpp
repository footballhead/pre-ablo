#include "diablo.h"

#include "directx/DDRAW.H"
#include "storm/storm.h"
#include <direct.h>
#include <stdio.h>
#include <windows.h>

#include "control.h"
#include "cursor.h"
#include "effects.h"
#include "engine.h"
#include "enums.h"
#include "error.h"
#include "gendung.h"
#include "gmenu.h"
#include "interfac.h"
#include "missiles.h"
#include "monster.h"
#include "multi.h"
#include "objects.h"
#include "palette.h"
#include "player.h"
#include "scrollrt.h"
#include "spells.h"
#include "town.h"

//
// forward decl
//

BOOL init_create_window(HINSTANCE hInstance, int nShowCmd);
void game_logic();
void dx_cleanup();

//
// initialized vars (.data:004BC0A8)
//

// wtf is this? nobody references it
char registration_table[128] = "REGISTRATION_TABLE";
// "Mutex" "held" duing PaintEventTimer (set to TRUE on enter, FALSE on return).
// Seems to serve double duty:
//   1. indicate that PaintEventTimer is working
//   2. used to stop PaintEventTimer from painting in that tick interval
BOOL paint_callback_mutex = FALSE;
int force_redraw = 0;
// Used by main loop to determine when to post a WM_DIABPAINT message
BOOL need_to_repaint = FALSE;
// Controlled by ShowProgress. Will be set to TRUE during certain ShowProgress
// blocks, then set to FALSE when done. This will be used as the final check in
// PaintEventTimer to actually post WM_DIABPAINT
BOOL paint_mutex = FALSE;
BOOL svgamode = TRUE;
int unused000 = 0;
int unused001 = 0;
int unused002 = 1;
int unused003 = 1;
int unused004 = -1;
const char *version_string = "V1.0";
BOOL debug_mode = FALSE;
// If TRUE then restrict to dlvl 1 and warrior
BOOL demo_mode = TRUE;
BOOL show_intros = TRUE;
BOOL debugMusicOn = TRUE;
BOOL cheat_mode = FALSE;
BOOL frameflag = FALSE;
// If TRUE then moving the mouse to the edge of the screen scrolls the view
// (think real-time strategy controls a la Age of Empires)
BOOL scrollflag = FALSE;
BOOL light4flag = FALSE;
BOOL musicFromDisk = FALSE;
HANDLE sghMusic = 0;
DWORD PauseMode = 0;
BOOL gbMusicOn = TRUE;
const char *sgszMusicTracks[] = {
    // indexed by leveltype
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
DWORD frames;         // updated regularly
DWORD frames_drawn;   // updated regulary
DWORD draw_framerate; // 1sec snapshot
int framerate;        // 1sec snapshot
// ...
LPDIRECTDRAWSURFACE lpDDSBackBuf;
HWND ghMainWnd;
int MouseX;
int MouseY;
DWORD dword_61B738; // UNUSED
int screenshot_idx;
DWORD dword_61B740; // TODO TODO TODO
DWORD dword_61B744; // UNUSED
BOOL can_fade;
LPDIRECTDRAWPALETTE lpDDPalette;
int main_menu_screen;
BOOL shouldStopPaintTimer;
DWORD dword_61B758; // UNUSED; set to 0, never read
// ...
char savedir_abspath[64];
// ...
BOOL fullscreen;        // only value is TRUE
BOOL dd_use_backbuffer; // only value is FALSE
DWORD dword_61BF50;     // UNUSED
DWORD dword_61BF54;     // UNUSED
DWORD dword_61BF58;     // UNUSED; set to 0, never read
DWORD dword_61BF5C;     // UNUSED; set to 0, never read
DWORD dword_61BF60;     // UNUSED; set to 640, never read
DWORD dword_61BF64;     // UNUSED; set to 480, never read
BOOL gbActive;
LPDIRECTDRAWSURFACE lpDDSPrimary;
DWORD prevTime;
// Compiler-generated alignment to put array on 8-byte boundary
char fileLoadPrefix[64];
DWORD dword_61BFB8;
HANDLE mpq_handle;
DWORD dword_61BFC0; // UNUSED
LPDIRECTDRAW lpDDInterface;
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
    UINT driveType;
    MSG message;
    BOOL error;
    int i;
    int mkdir_error;
    SYSTEM_INFO system_info;

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
    title_allow_loadgame = FALSE;
    fileLoadPrefix[0] = 0;
    savedir_abspath[0] = 0;

    // Try to find a suitable savedir_abspath
    driveType = GetDriveTypeA(NULL);
    if (driveType == DRIVE_CDROM)
    {
        error = FALSE;
        GetCurrentDirectoryA(sizeof(tempstr), tempstr);
        if (GetTempPathA(sizeof(savedir_abspath), savedir_abspath) == 0)
        {
            error = TRUE;
        }
        else
        {
            if (_chdir(savedir_abspath) == -1)
            {
                error = TRUE;
            }
            if (*tempstr == *savedir_abspath)
            {
                error = TRUE;
            }
            _chdir(tempstr);
            sprintf(savedir_abspath, "%sSave", savedir_abspath);
        }

        if (error)
        {
            MessageBoxA(NULL, "No Windows Temp directory found. Cannot continue", "Diablo Demo", MB_ICONEXCLAMATION);
        }
    }
    else
    {
        GetCurrentDirectoryA(sizeof(savedir_abspath), savedir_abspath);
        sprintf(savedir_abspath, "%s\\Save", savedir_abspath);
    }

    mkdir_error = _mkdir(savedir_abspath);
    if (mkdir_error && *_errno() == EEXIST)
    {
        mkdir_error = 0;
    }

    if (mkdir_error != 0 || driveType == DRIVE_UNKNOWN)
    {
        InitDiabloMsg(EMSG_NO_TMP_DIR);
    }

    GetSystemInfo(&system_info);
    if (system_info.wProcessorArchitecture & ~PROCESSOR_ARCHITECTURE_INTEL || system_info.dwProcessorType < PROCESSOR_INTEL_PENTIUM)
    {
        MessageBoxA(NULL, "The Diablo demo requires at least a Pentium.", "Diablo Demo", MB_DEFBUTTON2 | MB_ICONEXCLAMATION | MB_OKCANCEL);
        return 0;
    }

    if (init_create_window(hInstance, nShowCmd) == FALSE)
    {
        return 0;
    }

    //
    // MAIN LOOP
    //

    while (true)
    {
        // Process messages
        if (PeekMessageA(&message, NULL, 0, 0, 0))
        {
            if (!GetMessageA(&message, NULL, 0, 0))
            {
                return message.wParam;
            }
            TranslateMessage(&message);
            DispatchMessageA(&message);
            continue;
        }

        if (gbActivePlayers > 1)
        {
            DPlayHandleMessage();
        }

        // Menus, etc. are handled by their own WndProc. This loop only does
        // core gameplay
        if (main_menu_screen != MODE_GAME)
        {
            continue;
        }

        if (unknown_dplay_get_plractive() == FALSE)
        {
            continue;
        }

        if (PauseMode != 2)
        {
            if (PauseMode == 1)
            {
                PauseMode = 2;
            }

            for (i = 0; gbActivePlayers > i; i++)
            {
                if (plr[i].plractive)
                {
                    plr[i].to_send_message_id += 1;
                }
            }

            allow_mp_action_send = TRUE;

            // ~~~ Where all the magic happens ~~~
            game_logic();

            if (need_to_repaint)
            {
                PostMessageA(ghMainWnd, WM_DIABPAINT, 0, 0);
                need_to_repaint = FALSE;
            }
        }

        CheckCursMove();
        if (scrollflag)
        {
            ScrollView();
        }

        force_redraw = 4;
        if (need_to_repaint)
        {
            PostMessageA(ghMainWnd, WM_DIABPAINT, 0, 0);
            need_to_repaint = FALSE;
        }

        // This is a tight loop without any yields... This probably pegs the
        // CPU as a result!
    }

    return message.wParam; // no breaks; we should never get here!
}

// .text:00484E67
BOOL dx_init(HWND hWnd)
{
    HRESULT hDDVal;
    DDSURFACEDESC ddsd;
    DWORD dwStyle;
    RECT window_rect;
    RECT work_area_rect;

    if (fullscreen == FALSE)
    {
        SetFocus(hWnd);

        hDDVal = DirectDrawCreate(NULL, &lpDDInterface, NULL);
        if (hDDVal != DD_OK)
        {
            goto error0;
        }

        hDDVal = lpDDInterface->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
        if (hDDVal != DD_OK)
        {
            goto error1;
        }

        dwStyle = GetWindowLongA(hWnd, GWL_STYLE);
        dwStyle &= ~WS_POPUP;
        dwStyle |= WS_MINIMIZEBOX | WS_SIZEBOX | WS_CAPTION;
        SetWindowLongA(hWnd, GWL_STYLE, dwStyle);

        // This looks like a bunch of malarkey to make sure the window is
        // 640x480 and visible to the player (actually on a desktop)
        SetRect(&window_rect, 0, 0, 640, 480);
        AdjustWindowRectEx(
            &window_rect,
            GetWindowLongA(hWnd, GWL_STYLE),
            !!GetMenu(hWnd), // TODO disassembly is
                             // call ds:GetMenu
                             // cmp eax, 1
                             // sbb eax, eax
                             // inc eax
                             // push eax
            GetWindowLongA(hWnd, GWL_EXSTYLE));
        SetWindowPos(
            hWnd,
            HWND_TOP,
            0,
            0,
            window_rect.right - window_rect.left,
            window_rect.bottom - window_rect.top,
            SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
        SetWindowPos(
            hWnd,
            HWND_NOTOPMOST,
            0,
            0,
            0,
            0,
            SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
        SystemParametersInfoA(SPI_GETWORKAREA, 0, &work_area_rect, 0);
        GetWindowRect(hWnd, &window_rect);
        if (window_rect.left < work_area_rect.left)
        {
            window_rect.left = work_area_rect.left;
        }
        if (window_rect.top < work_area_rect.top)
        {
            window_rect.top = work_area_rect.top;
        }
        SetWindowPos(
            hWnd,
            HWND_TOP,
            window_rect.left,
            window_rect.top,
            0,
            0,
            SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

        memset(&ddsd, 0, sizeof(ddsd));
        ddsd.dwSize = sizeof(ddsd);

        // start setting parameters...
        ddsd.dwFlags = DDSD_BACKBUFFERCOUNT;
        ddsd.ddsCaps.dwCaps = DDSCAPS_COMPLEX | DDSCAPS_FLIP | DDSCAPS_PRIMARYSURFACE | DDSCAPS_VIDEOMEMORY;

        // psych! override those with what we actually want >:)
        // (methinks a developer just copy-pasted this here for testing...)
        dd_use_backbuffer = FALSE;
        ddsd.dwFlags = DDSD_CAPS;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

        hDDVal = lpDDInterface->CreateSurface(&ddsd, &lpDDSBackBuf, NULL);
        lpDDSPrimary = lpDDSBackBuf;
    }
    else
    { // fullscreen == TRUE
        hDDVal = DirectDrawCreate(NULL, &lpDDInterface, NULL);
        if (hDDVal != DD_OK)
        {
            goto error2;
        }

        hDDVal = lpDDInterface->SetCooperativeLevel(hWnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);
        if (hDDVal != DD_OK)
        {
            goto error3;
        }

        hDDVal = lpDDInterface->SetDisplayMode(640, 480, 8);
        if (hDDVal != DD_OK)
        {
            goto error4;
        }

        memset(&ddsd, 0, sizeof(ddsd));
        ddsd.dwSize = sizeof(ddsd);

        dd_use_backbuffer = FALSE;
        ddsd.dwFlags = DDSD_CAPS;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

        hDDVal = lpDDInterface->CreateSurface(&ddsd, &lpDDSBackBuf, NULL);
        lpDDSPrimary = lpDDSBackBuf;
        if (hDDVal != DD_OK)
        {
            goto error5;
        }
    }

    lpDDInterface->CreatePalette(DDPCAPS_8BIT, system_palette, &lpDDPalette, NULL);
    if (lpDDPalette == NULL)
    {
        goto error6;
    }

    hDDVal = lpDDSBackBuf->SetPalette(lpDDPalette);
    if (hDDVal != DD_OK)
    {
        goto error7;
    }

    SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);

    snd_init(hWnd);
    // SDrawManualInitialize(hWnd, lpDDInterface, lpDDSBackBuf, NULL, NULL, lpDDPalette, NULL);
    // if (debugMusicOn)
    // {
    //     SFileDdaInitialize(sglpDS);
    // }
    // if (musicFromDisk)
    // {
    //     Mopaq_479075(20, 10, 0x8000);
    //     if (SNDCPP_InitThread() == 0)
    //     {
    //         return FALSE;
    //     }
    //     music_start("Music\\Dintro.wav");
    // }

    GdiSetBatchLimit(1);
    ShowCursor(FALSE);
    // init_multi_with_time_srand();

    // TODO

error:
    dx_cleanup();
    MessageBoxA(hWnd, "DirectDraw Init FAILED", "Diablo Demo", MB_OK);
    DestroyWindow(hWnd);
    return FALSE;
error7:
    goto error;
error6:
    goto error;
error5:
    goto error;
error4:
    goto error;
error3:
    goto error;
error2:
    goto error;
error1:
    goto error;
error0:
    goto error;
}

// WNDPROC_mode0_blizzard_intro	00000000004853DA
// diablo_48565F	000000000048565F
// GM_Game	00000000004865C4
// MainWndProc	00000000004881C9

// .text:004882F9
void wait_for_paint()
{
    // TODO
}

// .text:0048833D
// The sole purpose of this timer callback seems to be to send WM_DIABPAINT
// messages every 50ms.
void CALLBACK PaintEventTimer(UINT uTimerID,
                              UINT uMsg,
                              DWORD_PTR dwUser,
                              DWORD_PTR dw1,
                              DWORD_PTR dw2)
{
    if (paint_callback_mutex)
    {
        return;
    }

    paint_callback_mutex = TRUE;
    DWORD now = timeGetTime();
    frames++;
    if (now - prevTime >= 1000)
    {
        framerate = frames;
        frames = 0;

        prevTime = now;

        draw_framerate = frames_drawn;
        frames_drawn = 0;
    }

    if (now - prev_timer_PostMessage_time >= 50)
    {
        if (can_fade == FALSE)
        {
            can_fade = TRUE;
            if (paint_mutex == FALSE)
            {
                PostMessage((HWND)dwUser, WM_DIABPAINT, 0, 0);
            }

            prev_timer_PostMessage_time = now;
        }
    }

    if (shouldStopPaintTimer)
    {
        timeKillEvent(uTimerID);
        ExitThread(0);
    }

    paint_callback_mutex = FALSE;
}

// InitLevelType	0000000000488442
// init_multi_with_time_srand	00000000004884C8

// .text:00488769
BOOL init_create_window(HINSTANCE hInstance, int nShowCmd)
{
    HWND hWnd;
    WNDCLASSA wc;

    wc.style = CS_HREDRAW | CS_VREDRAW;
    // wc.lpfnWndProc = MainWndProc; // TODO!
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wc.hCursor = LoadCursor(hInstance, IDI_APPLICATION);
    wc.hbrBackground = 0;
    wc.lpszMenuName = "Diablo";
    wc.lpszClassName = "Diablo";
    RegisterClassA(&wc);

    hWnd = CreateWindowExA(WS_EX_TOPMOST,
                           "Diablo",
                           "Diablo Game",
                           WS_POPUP,
                           0,
                           0,
                           GetSystemMetrics(SM_CXSCREEN),
                           GetSystemMetrics(SM_CYSCREEN),
                           NULL,
                           NULL,
                           hInstance,
                           NULL);
    if (hWnd == NULL)
    {
        return FALSE;
    }

    ghMainWnd = hWnd;
    ShowWindow(hWnd, nShowCmd);
    UpdateWindow(hWnd);
    dx_init(hWnd);

    return TRUE;
}

// .text:0048885D
void dx_cleanup()
{
    if (lpDDInterface)
    {
        if (lpDDSBackBuf)
        {
            lpDDSBackBuf->Release();
            lpDDSBackBuf = NULL;
        }
        if (lpDDPalette)
        {
            lpDDPalette->Release();
            lpDDPalette = NULL;
        }
        lpDDInterface->Release();
        lpDDInterface = NULL;
    }
}

// .text:004888E2
void FreeGameMem()
{
    MemFreeDbg(pDungeonCels);
    MemFreeDbg(pMegaTiles);
    MemFreeDbg(pLevelPieces);
    MemFreeDbg(pSpecialCels);

    if (leveltype != DTYPE_TOWN)
    {
        MemFreeDbg(pSpeedCels);

        FreeMonsters();
        FreeObjectGFX();
        FreeMissileGFX();
        FreeSpells();
        FreeMonsterSnd();
    }
    else
    {
        FreeTownerGFX();
        FreeMissileGFX();
        FreeTownersEffects();
    }
}

// .text:004889EA
static void LoadLvlGFX()
{
    switch (leveltype)
    {
    case DTYPE_TOWN:
        pDungeonCels = LoadFileInMem("Levels\\TownData\\Town.CEL");
        pMegaTiles = LoadFileInMem("Levels\\TownData\\Town.TIL");
        pLevelPieces = LoadFileInMem("Levels\\TownData\\Town.MIN");
        pSpecialCels = LoadFileInMem("Levels\\TownData\\TownS.CEL");
        break;
    case DTYPE_OLD_CATHEDRAL:
        pDungeonCels = LoadFileInMem("Levels\\L1Data\\L1.CEL");
        pMegaTiles = LoadFileInMem("Levels\\L1Data\\L1.TIL");
        pLevelPieces = LoadFileInMem("Levels\\L1Data\\L1.MIN");
        pSpecialCels = LoadFileInMem("Levels\\L1Data\\L1S.CEL");
        break;
    case DTYPE_CATACOMBS:
        pDungeonCels = LoadFileInMem("Levels\\L2Data\\L2.CEL");
        pMegaTiles = LoadFileInMem("Levels\\L2Data\\L2.TIL");
        pLevelPieces = LoadFileInMem("Levels\\L2Data\\L2.MIN");
        pSpecialCels = LoadFileInMem("Levels\\L2Data\\L2S.CEL");
        break;
    case DTYPE_CAVES:
        pDungeonCels = LoadFileInMem("Levels\\L3Data\\L3.CEL");
        pMegaTiles = LoadFileInMem("Levels\\L3Data\\L3.TIL");
        pLevelPieces = LoadFileInMem("Levels\\L3Data\\L3.MIN");
        pSpecialCels = LoadFileInMem("Levels\\L1Data\\L1S.CEL"); // lol
        break;
    case DTYPE_HELL:
        pDungeonCels = LoadFileInMem("Levels\\L2Data\\L2.CEL");
        pMegaTiles = LoadFileInMem("Levels\\L2Data\\L2.TIL");
        pLevelPieces = LoadFileInMem("Levels\\L2Data\\L2.MIN");
        pSpecialCels = LoadFileInMem("Levels\\L2Data\\L2S.CEL");
        break;
    case DTYPE_CATHEDRAL:
        pDungeonCels = LoadFileInMem("Levels\\L1Data\\L1.CEL");
        pMegaTiles = LoadFileInMem("Levels\\L1Data\\L1.TIL");
        pLevelPieces = LoadFileInMem("Levels\\L1Data\\L1.MIN");
        pSpecialCels = LoadFileInMem("Levels\\L1Data\\L1S.CEL");
        break;
    }
}

// .text:00488BC1
static void LoadAllGFX(BOOL firstflag)
{
    pSpeedCels = (BYTE *)DiabloAllocPtr(0x100000);
    InitMonsterGFX();
    if (firstflag)
    {
        IncProgress();
    }
    InitMonsterSND();
    if (firstflag)
    {
        IncProgress();
    }
    InitObjectGFX();
    if (firstflag)
    {
        IncProgress();
    }
    InitMissileGFX();
    if (firstflag)
    {
        IncProgress();
    }
    InitSpellGFX();
    if (firstflag)
    {
        IncProgress();
    }
}

// CreateLevel	0000000000488C54

void LoadGameLevel(BOOL firstflag, int lvldir, BOOL first_flag)
{
    // TODO
    // calls LoadLevel
}

// InitLevels	000000000048947E
// diablo_489510_plr_rel	0000000000489510

// .text:00489715
void game_logic()
{
    // TODO
}
