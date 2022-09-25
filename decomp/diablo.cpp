#include "diablo.h"

#include "directx/DDRAW.H"
#include "storm/storm.h"
#include <direct.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

#include "control.h"
#include "cursor.h"
#include "defines.h"
#include "effects.h"
#include "engine.h"
#include "enums.h"
#include "error.h"
#include "gendung.h"
#include "gmenu.h"
#include "interfac.h"
#include "lighting.h"
#include "missiles.h"
#include "monster.h"
#include "multi.h"
#include "objects.h"
#include "palette.h"
#include "player.h"
#include "scrollrt.h"
#include "sound.h"
#include "spells.h"
#include "town.h"

//
// forward decl (TODO declare in header?)
//

void diablo_init_screen();
BOOL init_create_window(HINSTANCE hInstance, int nShowCmd);
void game_logic();
void dx_cleanup();
void set_did_paint_PostMessage(BOOL b);
void CALLBACK PaintEventTimer(UINT uTimerID,
                              UINT uMsg,
                              DWORD_PTR dwUser,
                              DWORD_PTR dw1,
                              DWORD_PTR dw2);

//
// initialized vars (.data:004BC0A8)
//

// wtf is this? nobody references it
char registration_table[128] = "REGISTRATION_TABLE";
// "Mutex" "held" duing PaintEventTimer (set to TRUE on enter, FALSE on return).
// Kind of a crude signal between main thread and PaintEventThread to coorindate
// when painting is occurring.
//
// Seems to serve double duty:
//   1. indicate that PaintEventTimer is working
//   2. used to stop PaintEventTimer from painting in that tick interval (e.g.
//      I've just issued a draw call)
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
// Enable/disable all music. Kinda intended as a compile-time toggle
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
// Menu option set by player. Compared to debugMusicOn, this is run-time
// settable by the player in the menu.
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
DWORD dword_61B714;   // UNUSED
DWORD dword_61B718;   // UNUSED; set to 0, never read
DWORD dword_61B71C;   // UNUSED; set to 0, never read
DWORD dword_61B720;   // UNUSED; set to 768, never read
DWORD dword_61B724;   // UNUSED; set to 656, never used
LPDIRECTDRAWSURFACE lpDDSBackBuf;
HWND ghMainWnd;
int MouseX;
int MouseY;
DWORD dword_61B738; // UNUSED
int screenshot_idx;
DWORD dword_61B740; // TODO TODO TODO
DWORD dword_61B744; // UNUSED
BOOL did_paint_PostMessage;
LPDIRECTDRAWPALETTE lpDDPalette;
int gMode; // enum game_mode
BOOL shouldStopPaintTimer;
DWORD dword_61B758; // UNUSED; set to 0, never read
UINT paint_event_timer_id;
DWORD dword_61B760; // UNUSED
DWORD dword_61B764; // UNUSED
UINT paint_event_timer_delay;
DWORD dword_61B76C; // UNUSED
char savedir_abspath[64];
TIMECAPS timecaps;
// ...
int timer_expiry_per_second; // UNUSED; set, never read
UINT paint_event_timer_resolution;
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
HANDLE diabdat_mpq;
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
    did_paint_PostMessage = TRUE;
    shouldStopPaintTimer = FALSE;
    gMode = MODE_BLIZ_LOGO;
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

        // The rest of this loop handles updating game state. Input processing
        // and painting for menus is handled by their respective WndProc
        if (gMode != MODE_GAME)
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
            goto dd_create_err;
            goto cleanup;
        }

        hDDVal = lpDDInterface->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
        if (hDDVal != DD_OK)
        {
            goto set_coop_err;
            goto cleanup;
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
            goto dd_create_fs_err;
            goto cleanup;
        }

        hDDVal = lpDDInterface->SetCooperativeLevel(hWnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);
        if (hDDVal != DD_OK)
        {
            goto set_coop_fs_err;
            goto cleanup;
        }

        hDDVal = lpDDInterface->SetDisplayMode(640, 480, 8);
        if (hDDVal != DD_OK)
        {
            goto set_mode_err;
            goto cleanup;
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
            goto surface_err;
            goto cleanup;
        }
    }

    lpDDInterface->CreatePalette(DDPCAPS_8BIT, system_palette, &lpDDPalette, NULL);
    if (lpDDPalette == NULL)
    {
        goto palette_err;
        goto cleanup;
    }

    hDDVal = lpDDSBackBuf->SetPalette(lpDDPalette);
    if (hDDVal != DD_OK)
    {
        goto set_pal_err;
        goto cleanup;
    }

    SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);

    snd_init(hWnd);
    SDrawManualInitialize(hWnd, lpDDInterface, lpDDSBackBuf, NULL, NULL, lpDDPalette, NULL);
    if (debugMusicOn)
    {
        SFileDdaInitialize(sglpDS);
    }

    if (musicFromDisk)
    {
        // TODO
        //     Mopaq_479075(20, 10, 0x8000);
        //     if (SNDCPP_InitThread() == 0)
        //     {
        //         return FALSE;
        //     }
        //     music_start_NoStorm("Music\\Dintro.wav");
    }

    GdiSetBatchLimit(1);
    ShowCursor(FALSE);
    diablo_init_screen();
    gpBuffer = (BYTE *)DiabloAllocPtr(BUFFER_WIDTH * BUFFER_HEIGHT);
    set_did_paint_PostMessage(TRUE);

    timeGetDevCaps(&timecaps, sizeof(TIMECAPS));
    paint_event_timer_resolution = timecaps.wPeriodMin;
    paint_event_timer_delay = timecaps.wPeriodMin <= 5 ? 5 : timecaps.wPeriodMin;
    timer_expiry_per_second = 1000 / paint_event_timer_delay;
    timeBeginPeriod(paint_event_timer_resolution);
    prev_timer_PostMessage_time = timeGetTime();
    prevTime = prev_timer_PostMessage_time;
    paint_event_timer_id = timeSetEvent(paint_event_timer_delay,
                                        paint_event_timer_resolution,
                                        PaintEventTimer,
                                        (DWORD_PTR)hWnd,
                                        TIME_PERIODIC);
    PostMessageA(hWnd, WM_DIABMODEINIT, 0, 0);
    return TRUE;

cleanup:
    dx_cleanup();
    MessageBoxA(hWnd, "DirectDraw Init FAILED", "Diablo Demo", MB_OK);
    DestroyWindow(hWnd);
    return FALSE;
set_pal_err:
    goto cleanup;
palette_err:
    goto cleanup;
surface_err:
    goto cleanup;
set_mode_err:
    goto cleanup;
set_coop_fs_err:
    goto cleanup;
dd_create_fs_err:
    goto cleanup;
set_coop_err:
    goto cleanup;
dd_create_err:
    goto cleanup;
}

// .text:004853DA
// The message processor for the blizzard logo/quotes.cel screen. Skip if
// show_intros == FALSE. Otherwise, next screen is the intro video.
static LRESULT WndProc_BlizLogo(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    BOOL bSuccess; // unused

    switch (Msg)
    {
    case WM_ACTIVATEAPP:
        if (gbActive)
        {
            ResetPal();
        }
        break;
        break; // there are 3 jumps total...
        break;
    case WM_LBUTTONDOWN:
    case WM_KEYDOWN:
    case WM_CHAR:
    case WM_RBUTTONDOWN:
        if (hCurrentVideo)
        {
            SVidPlayEnd(hCurrentVideo);
            // Ending the video allows interfac_PlayLogo_DrawQuotes to finish
        }
        break;
    case WM_DIABMODEINIT:
        bSuccess = SFileOpenArchive("diabdat.mpq", 0, 0, &diabdat_mpq);
        priv_sound_init();
        MakeLightTable();
        InitCursor();
        InitLevelCursor(hWnd);
        if (!show_intros)
        {
            interfac_InitMainMenu();
            // Bug? WndProc_IntroVid loads Title.pal before transitioning to
            // main menu
            LoadPalette("Gendata\\Quotes.pal", orig_palette);
        }
        else
        {
            // This will block until the video is done playing
            interfac_PlayLogo_DrawQuotes();
            LoadPalette("Gendata\\Quotes.pal", orig_palette);
        }

        set_did_paint_PostMessage(FALSE);
        if (!show_intros)
        {
            gMode = MODE_MAINMENU;
        }
        else
        {
            gMode = MODE_INTRO_VID;
        }

        CopyPalette(logical_palette, orig_palette);
        PaletteFadeIn(32);
        force_redraw = 4;

        break;
    case WM_DIABPAINT:
        while (paint_callback_mutex)
        {
            // do nothing, wait for mutex
        }

        // This doesn't call DoFadeIn/DoFadeOut so fade will never progress.
        // Probably fine since only a video is being played and when its over
        // we transition to a new screen
        paint_callback_mutex = TRUE;
        if (fade_state == PALETTE_FADE_IN) {
            PostMessageA(hWnd, WM_DIABMODEINIT, 0, 0);
            fade_state = PALETTE_NO_FADE;
        }
        if (fade_state == PALETTE_FADE_OUT) {
            PostMessageA(hWnd, delayed_Msg, 0, 0);
            fade_state = PALETTE_NO_FADE;
        }
        lpDDPalette->SetEntries(0, 0, 256, system_palette);
        frames_drawn++;
        did_paint_PostMessage = FALSE;
        paint_callback_mutex = FALSE;

        return 0;
    case 0x3B9: // ???
        return 0;
    }

    return DefWindowProcA(hWnd, Msg, wParam, lParam);
}

// diablo_48565F	000000000048565F
// GM_Game	00000000004865C4

// .text:004881C9
// This is THE wndproc used by the game window. It delegates to subroutines
// depending on game mode (gMode) but the wndproc itself is never changes.
//
// This largely just handles event processing and other non-game state.
LRESULT __stdcall MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (gMode)
    {
    case MODE_BLIZ_LOGO:
        return WndProc_BlizLogo(hWnd, Msg, wParam, lParam);
    case MODE_MAINMENU:
        return WndProc_MainMenu(hWnd, Msg, wParam, lParam);
    case MODE_NEWGAME:
        // TODO
        break;
    case MODE_GAME:
        // TODO
        // GM_Game does input processing. game_logic uses that to progress game state.
        break;
    case MODE_INTRO_VID:
        return WndProc_IntroVid(hWnd, Msg, wParam, lParam);
    case MODE_DEMO_END:
        // TODO
        break;
    case MODE_PROGRESS:
        // TODO
        break;
    }

    return DefWindowProcA(hWnd, Msg, wParam, lParam);
}

// .text:004882F9
void set_did_paint_PostMessage(BOOL b)
{
    while (paint_callback_mutex)
    {
        // loop until set to 0
    }

    paint_callback_mutex = TRUE;
    did_paint_PostMessage = b;
    paint_callback_mutex = FALSE;
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
        if (did_paint_PostMessage == FALSE)
        {
            did_paint_PostMessage = TRUE;
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

// .text:00488442
int InitLevelType(int l)
{
    if (l == 0)
        return DTYPE_TOWN;
    if (l >= 1 && l <= 4)
        return DTYPE_CATHEDRAL;
    if (l >= 5 && l <= 8)
        return DTYPE_CATACOMBS;
    if (l >= 9 && l <= 12)
        return DTYPE_CAVES;

    return DTYPE_HELL;
}

// .text:004884C8
void diablo_init_screen()
{
    int i;
    time_t seed_time;
    int j;

    // Set, but never read again...
    dword_61B718 = 0;
    dword_61B71C = 0;
    dword_61B720 = BUFFER_WIDTH;
    dword_61B724 = BUFFER_HEIGHT;
    dword_61BF58 = 0;
    dword_61BF5C = 0;
    dword_61BF60 = SCREEN_WIDTH;
    dword_61BF64 = SCREEN_HEIGHT;

    if (debug_mode)
    {
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < NUMLEVELS; j++)
            {
                plr[i]._pSeedTbl[j] = j;
                plr[i]._pLevelTypeTbl[j] = InitLevelType(j);
            }
        }
    }
    else
    {
        srand(time(&seed_time));
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < NUMLEVELS; j++)
            {
                plr[i]._pSeedTbl[j] = random_(0x8000);
                plr[i]._pLevelTypeTbl[j] = InitLevelType(j);
            }
        }
    }

    MouseX = SCREEN_WIDTH / 2;
    MouseY = SCREEN_HEIGHT / 2;
    fade_state = 0;
    ScrollInfo._sdx = 0;
    ScrollInfo._sdy = 0;
    ScrollInfo._sxoff = 0;
    ScrollInfo._syoff = 0;
    ScrollInfo._sdir = 0;
    screenshot_idx = 0;

    for (j = 0; j < 1024; j++)
    {
        PitchTbl[i] = i * BUFFER_WIDTH;
    }

    // All of these are set but never used.
    dword_615F18 = 0;
    dword_605474 = 0;
    dword_615F14 = 0;
    dword_605390 = 0; // This might actually be used
    dword_615F1C = 0;
    dword_615F20 = 0;
    dword_615F24 = 0;
    dword_615F28 = 0;
    dword_615F2C = 0;
    dword_615F30 = 0;
    dword_615F34 = 0;
    dword_615F38 = 0;
}

// .text:00488769
BOOL init_create_window(HINSTANCE hInstance, int nShowCmd)
{
    HWND hWnd;
    WNDCLASSA wc;

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = &MainWndProc;
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
