#include "interfac.h"

#include <stdio.h>
#include <windows.h>
#include "storm/storm.h"

#include "cursor.h"
#include "diablo.h"
#include "effects.h"
#include "engine.h"
#include "enums.h"
#include "gmenu.h"
#include "lighting.h"
#include "palette.h"
#include "player.h"
#include "scrollrt.h"

static void interfac_InitNewGame();

//
// initialized variables (.data:004AA608)
//

char interfac_mfontframe[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 37, 49,
    38, 0, 39, 40, 47, 42, 43, 41, 45, 52, 44, 53, 55, 36,
    27, 28, 29, 30, 31, 32, 33, 34, 35, 51, 50, 0, 46, 0,
    54, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 42,
    0, 43, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
    26, 20, 0, 21, 0, 0};

char interfac_mfontkern[] = {
    18, 33, 21, 26, 28, 19, 19, 26, 25, 11, 12, 25, 19,
    34, 28, 32, 20, 32, 28, 20, 28, 36, 35, 46, 33, 33,
    24, 11, 23, 22, 22, 21, 22, 21, 21, 21, 32, 10, 20,
    36, 31, 17, 13, 12, 13, 18, 16, 11, 20, 21, 11, 10,
    12, 11, 21, 23};

//
// uninitiailzed variables (.data:005DE8D0)
//

int quote_timer;
HANDLE hCurrentVideo;
// ...
char menu_to_draw;         // TODO is this an int?
char menu_allow_key_input; // TODO is this an int?
char fade_param;           // TODO is this an int?
int menu_selection_index;
int pentSpinFrame;
int titleLogoFrame;
// These are reused for quotes screens
BYTE *pMenuBackgroundCel;
BYTE *pTitlTextCel;
BYTE *pPenTitleCel;
BYTE *pTitleQTxtCel;
BYTE *pDiabFrCel;
BYTE *pCelThatIsNeverLoadedSoIdk;
// ...
char byte_5DE90C; // UNUSED; set to 0, never read
// ...
char game00_abspath[256];
char did_find_game00;

//
// code (.text:00418F10)
//

// TODO These functions really need to be renamed

// print_title_str_large	0000000000418F10
// print_title_str_small	0000000000418FB2
// DrawProgress	0000000000419044

// .text:004190AE
void Title_FindSaveGame()
{
    char buffer[52];

    did_find_game00 = 0;
    sprintf(game00_abspath, "%s%s", savedir_abspath, "\\Game00.sav");
    _searchenv(game00_abspath, NULL, buffer);
    if (*buffer)
    {
        did_find_game00 |= 0x80; // wtf
    }
}

// .text:00419111
void mainmenu_draw()
{
    // szOptionTexts
    // TODO
    // int i;

    // for (i = 0; i < 3; i++) {
    //     drawx = 640 - iOptionDrawX
    //     drawy = i * 45 + 450;
    //     CelDraw(563, drawy + 1, pPenTitleCel, pentSpinFrame, 48);
    // }
}

// paint_select_class	00000000004192DE
void paint_select_class()
{
    // TODO
}

// .text:0041962C
// This will crash if menu_selection_index is 6 (that cel is not loaded)
void paint_title_cel()
{
    switch (menu_selection_index)
    {
    case 1:
        CelDraw(64, 639, pMenuBackgroundCel, 1, 640);
        break;
    case 2:
        CelDraw(64, 639, pTitlTextCel, 1, 640);
        break;
    case 3:
        CelDraw(64, 639, pPenTitleCel, 1, 640);
        break;
    case 4:
        CelDraw(64, 639, pTitleQTxtCel, 1, 640);
        break;
    case 5:
        CelDraw(64, 639, pDiabFrCel, 1, 640);
        break;
    case 6:
        // mystery background cel :eyes:
        CelDraw(64, 639, pCelThatIsNeverLoadedSoIdk, 1, 640);
        break;
    }
}

// DrawCutscene	0000000000419746
void DrawCutscene()
{
    // TODO
}

// .text:004197F7
void draw_switch_title_option(int menu_to_draw_index)
{
    byte_5DE90C = 0;
    switch (menu_to_draw_index)
    {
    case 1:
        mainmenu_draw();
        break;
    case 2:
        paint_select_class();
        break;
    case 3:
        ClearScreenBuffer();
        paint_title_cel();
        break;
    case 4:
        DrawCutscene();
        break;
        break;
    }
    force_redraw = 4;
}

// .text:00419882
static void interfac_PaintQuotes(HWND hWnd)
{
    while (paint_callback_mutex)
    {
        // do nothing, wait for mutex
    }

    paint_callback_mutex = TRUE;
    if (fade_state)
    {
        if (fade_state == PALETTE_FADE_IN)
        {
            DoFadeIn();
            if (fade_state == PALETTE_NO_FADE)
            {
                PostMessage(hWnd, WM_DIABMODEINIT, 0, 0);
            }
        }
        if (fade_state == PALETTE_FADE_OUT)
        {
            DoFadeOut();
            if (fade_state == PALETTE_NO_FADE)
            {
                PostMessage(hWnd, delayed_Msg, 0, 0);
            }
        }

        lpDDPalette->SetEntries(0, 0, 256, system_palette);
    }

    if (quote_timer > 0)
    {
        // Post a message to progress the quote_timer. Kind of annoyed that they
        // didn't just inline it here.
        PostMessageA(hWnd, WM_40C, 0, 0);
    }

    scrollrt_draw_game_screen(TRUE);
    did_paint_PostMessage = FALSE;
    paint_callback_mutex = FALSE;
}

// .text:00419982
static void interfac_PaintMainMenu(HWND hWnd)
{
    while (paint_callback_mutex)
    {
        // do nothing, wait for mutex
    }

    paint_callback_mutex = TRUE;
    if (fade_state)
    {
        if (fade_state == PALETTE_FADE_IN)
        {
            DoFadeIn();
            if (fade_state == PALETTE_NO_FADE)
            {
                PostMessage(hWnd, WM_DIABMODEINIT, 0, 0);
            }
        }
        if (fade_state == PALETTE_FADE_OUT)
        {
            if (fade_param & 0x40)
            {
                DoFadeOut();
            }
            else
            {
                fade_state = PALETTE_NO_FADE;
            }

            if (fade_state == PALETTE_NO_FADE)
            {
                PostMessage(hWnd, delayed_Msg, 0, 0);
            }
        }

        lpDDPalette->SetEntries(0, 0, 256, system_palette);
    }

    savecrsr_hide();
    draw_switch_title_option(menu_to_draw);
    if (menu_allow_key_input & 0x80)
    {
        savecrsr_show();
    }
    scrollrt_draw_game_screen(1);
    pentSpinFrame++;
    if (pentSpinFrame == 9)
    {
        pentSpinFrame = 1;
    }
    titleLogoFrame++;
    if (titleLogoFrame == 29)
    {
        titleLogoFrame = 1;
    }
    did_paint_PostMessage = FALSE;
    paint_callback_mutex = FALSE;
}

// .text:00419ADD
static void interfac_PaintProgress(HWND hWnd)
{
    while (paint_callback_mutex)
    {
        // do nothing, wait for mutex
    }

    paint_callback_mutex = TRUE;
    if (fade_state)
    {
        if (fade_state == PALETTE_FADE_IN)
        {
            DoFadeIn();
        }
        if (fade_state == PALETTE_FADE_OUT)
        {
            DoFadeOut();

            if (fade_state == PALETTE_NO_FADE)
            {
                PostMessage(hWnd, delayed_Msg, 0, 0);
            }
        }

        lpDDPalette->SetEntries(0, 0, 256, system_palette);
    }

    did_paint_PostMessage = FALSE;
    paint_callback_mutex = FALSE;
}

// .text:00419B93
// Progress the fade until done
void IncProgress()
{
    while (true)
    {
        if (fade_state)
        {
            if (did_paint_PostMessage != TRUE)
            {
                if (fade_state == PALETTE_FADE_IN)
                {
                    DoFadeIn();
                }
                if (fade_state == PALETTE_FADE_OUT)
                {
                    DoFadeOut();
                }

                lpDDPalette->SetEntries(0, 0, 256, system_palette);
            }

            draw_switch_title_option(menu_to_draw);
            scrollrt_draw_game_screen(TRUE);
            set_did_paint_PostMessage(FALSE);

            continue;
        }
        else
        {
            if (did_paint_PostMessage != TRUE)
            {
                menu_selection_index = 15;
                draw_switch_title_option(menu_to_draw);
                scrollrt_draw_game_screen(TRUE);
                set_did_paint_PostMessage(FALSE);
                return;
            }
        }
    }
}

// .text:00419C60
LRESULT WndProc_IntroVid(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    BYTE *pCelBuff;
    MSG Msg;

    switch (uMsg)
    {
    case WM_ACTIVATEAPP:
        if (gbActive)
        {
            ResetPal();
        }
        return 0;
    case WM_DIABMODEINIT:
        force_redraw = 4;
        set_did_paint_PostMessage(FALSE);
        return 0;
    case WM_40C:
        quote_timer--;
        if (quote_timer <= 0)
        {
            PaletteFadeOut(32);
            delayed_Msg = WM_DIABMODEEXIT;
        }
        return 0;
    case WM_LBUTTONDOWN:
    case WM_CHAR:
    case WM_RBUTTONDOWN:
        if (hCurrentVideo)
        {
            SVidPlayEnd(hCurrentVideo);
            set_did_paint_PostMessage(FALSE);
        }
        break;
    case WM_KEYDOWN:
        if (hCurrentVideo)
        {
            SVidPlayEnd(hCurrentVideo);
            set_did_paint_PostMessage(FALSE);
        }

        if (fade_state)
        {
            break;
        }

        if (wParam == VK_ESCAPE)
        {
            quote_timer = 1;
        }
        return 0;
    case WM_DIABMODEEXIT:
        if (menu_selection_index < 2)
        {
            menu_selection_index++;
            switch (menu_selection_index)
            {
            case 1:
                // Theoretically this branch is necessary but in practice it's
                // not used. menu_selection_index is set to 1 by
                // interfac_PlayLogo_DrawQuotes, then this function increments
                // it to 2 and the `case 2` is used instead.
                LoadPalette("Gendata\\Quotes.PAL", orig_palette);
                pCelBuff = pMenuBackgroundCel;
                quote_timer = 100;
                break;
            case 2:
                SVidPlayBegin("gendata\\diablo.smk", 0, 0, 0, 0x10080200, &hCurrentVideo);
                // This is a while in a switch in a switch ... ugh...
                while (SVidPlayContinue())
                {
                    while (PeekMessageA(&Msg, NULL, 0, 0, PM_REMOVE))
                    {
                        if (Msg.message == WM_QUIT)
                        {
                            ExitProcess(0);
                        }
                        else
                        {
                            TranslateMessage(&Msg);
                            DispatchMessageA(&Msg);
                        }
                    }
                }
                SVidPlayEnd(hCurrentVideo);

                // Normally 0x409 is posted after fade out. Here it's posted
                // right after the video is done. There's also no fade in
                // BlizLogo which makes me think that palette swapping is
                // something unsupported by smacker
                PostMessageA(hWnd, WM_DIABMODEEXIT, 0, 0);
                set_did_paint_PostMessage(FALSE);
                return 0;
            }

            // Again theoretically this is necessary but in practice it's not.
            // If BlizLogo was ignored then case 1 would drop down here and
            // draw the quotes. Makes me think that the bliz logo was kinda
            // a last second addition or even optional.
            CopyPalette(logical_palette, orig_palette);
            PaletteFadeIn(32);
            force_redraw = 4;
            ClearScreenBuffer();
            CelDraw(64, 639, pCelBuff, 1, 640);
        }
        else
        {
            MemFreeDbg(pMenuBackgroundCel);
            interfac_InitMainMenu();
            LoadPalette("Gendata\\Title.pal", orig_palette);
            gMode = MODE_MAINMENU;
            PaletteFadeOut(32);
            CopyPalette(logical_palette, orig_palette);
            PaletteFadeIn(32);
            force_redraw = 4;
        }

        return 0;
    case WM_DESTROY:
        shouldStopPaintTimer = TRUE;
        timeEndPeriod(paint_event_timer_resolution);
        MemFreeDbg(gpBuffer);
        // This seems like a weird thing to free here of all places
        MemFreeDbg(pLightTbl);
        ShowCursor(TRUE);
        PostQuitMessage(0);
        return 0;
    case WM_DIABPAINT:
        interfac_PaintQuotes(hWnd);
        return 0;
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

// .text:0041A06E
LRESULT WndProc_MainMenu(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    int iOldMenuSel;

    switch (Msg)
    {
    case WM_ACTIVATEAPP:
        if (gbActive)
        {
            ResetPal();
        }
        return 0;
    case WM_DIABMODEINIT:
        force_redraw = 4;
        set_did_paint_PostMessage(FALSE);
        return 0;
    case WM_MOUSEMOVE:
        MouseX = lParam & 0xFFFF;
        MouseY = (lParam >> 16) & 0xFFFF;
        return 0;
        break;
    case WM_LBUTTONDOWN:
        if (fade_state)
        {
            break;
        }

        iOldMenuSel = menu_selection_index;
        menu_selection_index = 0; // new game
        if (MouseY >= 290)
        {
            if (title_allow_loadgame == TRUE)
            {
                menu_selection_index = 1; // load game
            }
            else
            {
                menu_selection_index = iOldMenuSel;
            }
        }
        if (MouseY >= 335)
        {
            menu_selection_index = 2; // quit
        }

        PlayRndSFX(IS_TITLSLCT);
        if (MouseY >= 290 && menu_selection_index == 0)
        {
            return 0;
        }

        if (menu_selection_index == 0 || menu_selection_index == 1)
        {
            fade_param |= 64; // sentinel used by WM_DIABMODEEXIT
        }
        else
        {
            PaletteFadeOut(32);
        }
        fade_state = PALETTE_FADE_OUT;
        delayed_Msg = WM_DIABMODEEXIT;
        return 0;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_UP:
            PlayRndSFX(IS_TITLEMOV);
            if (menu_selection_index == 0)
            {
                menu_selection_index = 2;
            }
            else
            {
                menu_selection_index--;
            }
            if (menu_selection_index == 1 && !title_allow_loadgame)
            {
                menu_selection_index = 0;
            }
            return 0;
        case VK_DOWN:
            PlayRndSFX(IS_TITLEMOV);
            menu_selection_index++;
            if (menu_selection_index == 1 && !title_allow_loadgame)
            {
                menu_selection_index++;
            }
            if (menu_selection_index > 2)
            {
                menu_selection_index = 0;
            }
            return 0;
        case VK_RETURN:
            PlayRndSFX(IS_TITLSLCT);
            if (menu_selection_index == 0 || menu_selection_index == 1)
            {
                fade_param |= 64; // sentinel used by WM_DIABMODEEXIT
            }
            else
            {
                PaletteFadeOut(32);
            }
            fade_state = PALETTE_FADE_OUT;
            delayed_Msg = WM_DIABMODEEXIT;
            return 0;
            break;
        }

        break;
    case WM_DIABMODEEXIT:
        if (fade_param & 0x40)
        {
            MemFreeDbg(pMenuBackgroundCel);
            MemFreeDbg(pTitlTextCel);
            MemFreeDbg(pPenTitleCel);
            MemFreeDbg(pTitleQTxtCel);
            MemFreeDbg(pDiabFrCel);
        }

        switch (menu_selection_index)
        {
        case 0: // new game
            interfac_InitNewGame();
            gMode = MODE_NEWGAME;
            force_redraw = 4;
            set_did_paint_PostMessage(FALSE);
            break;
        case 1: // load game
            delayed_Msg = WM_DIABLOADGAME;
            InitCutscene();
            fade_state = PALETTE_FADE_OUT;
            gMode = MODE_PROGRESS;
            break;
        case 2: // quit
            interfac_InitDemoEnd();
            LoadPalette("Gendata\\Screen01.pal", orig_palette);
            fade_state = PALETTE_FADE_OUT;
            delayed_Msg = WM_DIABMODEEXIT;
            gMode = MODE_DEMO_END;
            break;
            break;
        }

        force_redraw = 4;
        return 0;
    case WM_DESTROY:
        shouldStopPaintTimer = TRUE;
        timeEndPeriod(paint_event_timer_resolution);
        MemFreeDbg(gpBuffer);
        // This seems like a weird thing to free here of all places
        MemFreeDbg(pLightTbl);
        ShowCursor(TRUE);
        PostQuitMessage(0);
        return 0;
    case WM_DIABPAINT:
        interfac_PaintMainMenu(hWnd);
        return 0;
    }

    return DefWindowProcA(hWnd, Msg, wParam, lParam);
}

// WNDPROC_mode2_selclass_entname	000000000041A5DD
// WNDPROC_mode13_exit_5screenshots	000000000041AD3E

// ShowProgress	000000000041B180
LRESULT ShowProgress(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg)
    {
    case WM_ACTIVATEAPP:
        gbActive = wParam;
        if (gbActive)
        {
            ResetPal();
        }
        return 0;
    case WM_DESTROY:
        shouldStopPaintTimer = TRUE;
        // TODO
        break;
        // TODO other cases
    }

    return DefWindowProc(hWnd, Msg, wParam, lParam);
}

// .text:0041BC8B
// Play the Blizzard Logo. This technically blocks until the end of the video,
// but not completely since it runs the event loop. At the end of the video,
// the quotes are drawn and a timer is started.
//
// Only called if show_intros == TRUE
void interfac_PlayLogo_DrawQuotes()
{
    MSG Msg;

    SVidPlayBegin("gendata\\logo.smk", 0, 0, 0, 0x10080200, &hCurrentVideo); // TODO what is 0x10080200
    while (SVidPlayContinue())
    {
        while (PeekMessageA(&Msg, NULL, 0, 0, PM_REMOVE))
        {
            if (Msg.message == WM_QUIT)
            {
                ExitProcess(0);
            }
            else
            {
                TranslateMessage(&Msg);
                DispatchMessageA(&Msg);
            }
        }
    }
    SVidPlayEnd(hCurrentVideo);

    pMenuBackgroundCel = LoadFileInMem("Gendata\\Quotes.CEL");
    // This skips parts of WndProc_IntroVid (since this function takes care of
    // that stuff anyway)
    menu_selection_index = 1;
    // The results of this one draw persists for 5s!
    CelDraw(64, 639, pMenuBackgroundCel, 1, 640);
    quote_timer = 100; // 5s at 20FPS
}

// .text:0041BD5E
void interfac_InitMainMenu()
{
    fade_param = 0;
    menu_allow_key_input = 0;

    if (!sghMusic && debugMusicOn)
    {
        SFileOpenFile("Music\\Dintro.WAV", &sghMusic);
        SFileDdaBegin(sghMusic, 0x40000, 0x40000);
    }

    pMenuBackgroundCel = LoadFileInMem("Gendata\\Titlgray.CEL");
    pTitlTextCel = LoadFileInMem("Gendata\\Titltext.CEL");
    pPenTitleCel = LoadFileInMem("Gendata\\Pentitle.CEL");
    pTitleQTxtCel = LoadFileInMem("Gendata\\Titlqtxt.CEL");
    pDiabFrCel = LoadFileInMem("Gendata\\Diabfr.CEL");

    menu_selection_index = 0;
    pentSpinFrame = 1;
    titleLogoFrame = 1;

    Title_FindSaveGame();
    if (did_find_game00)
    {
        title_allow_loadgame = TRUE;
    }

    menu_to_draw = 1;
    draw_switch_title_option(menu_to_draw);
    SetCursor_(CURSOR_HAND);
}

// .text:0041BE60
static void interfac_InitNewGame()
{
    plr[myplr]._pName[0] = '-';
    plr[myplr]._pName[1] = '\0';
    menu_to_draw = 2;
    menu_allow_key_input = 0;
    menu_selection_index = 2;
}

// .text:0041BEC4
void interfac_InitDemoEnd()
{
    if (!sghMusic && debugMusicOn)
    {
        SFileOpenFile("Music\\Dintro.WAV", &sghMusic);
        SFileDdaBegin(sghMusic, 0x40000, 0x40000);
    }
    pMenuBackgroundCel = LoadFileInMem("Gendata\\Screen01.CEL");
    pTitlTextCel = LoadFileInMem("Gendata\\Screen02.CEL");
    pPenTitleCel = LoadFileInMem("Gendata\\Screen03.CEL");
    pTitleQTxtCel = LoadFileInMem("Gendata\\Screen04.CEL");
    pDiabFrCel = LoadFileInMem("Gendata\\Screen05.CEL");
    menu_selection_index = 0;
    quote_timer = 0;
    menu_to_draw = 3;
}

// .text:0041BF78
void InitCutscene()
{
    // TODO
}