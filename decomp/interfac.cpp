#include "interfac.h"

#include <stdio.h>
#include <windows.h>
#include "storm/storm.h"

#include "scrollrt.h"
#include "diablo.h"
#include "engine.h"
#include "gmenu.h"
#include "palette.h"

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
        did_find_game00 = did_find_game00 | 0x80; // wtf
    }
}

// mainmenu_draw	0000000000419111
void mainmenu_draw()
{
    // TODO
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

// interface_pump_quotes_screens	0000000000419882
// interface_pump_title_message	0000000000419982
// interface_pump_progress	0000000000419ADD

// IncProgress	0000000000419B93
void IncProgress()
{
    // TODO
}

// WNDPROC_mode12_quotes_crow_video	0000000000419C60
// WNDPROC_mode1_title_newloadquit	000000000041A06E
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
    menu_selection_index = 1;
    CelDraw(64, 639, pMenuBackgroundCel, 1, 640);
    quote_timer = 100;
}

// .text:0041BD5E
void interfac_init_title_play_music()
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
    // TODO
}
// interfac_set_player_some	000000000041BE60
// interfac_init_exit_screens	000000000041BEC4
// InitCutscene	000000000041BF78
