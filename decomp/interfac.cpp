#include "diablo.h"

#include "palette.h"

#include <windows.h>

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
// code (.text:00418F10)
//

// print_title_str_large	0000000000418F10
// print_title_str_small	0000000000418FB2
// DrawProgress	0000000000419044
// Title_FindSaveGame	00000000004190AE
// mainmenu_draw	0000000000419111
// paint_select_class	00000000004192DE
// paint_title_cel	000000000041962C
// DrawCutscene	0000000000419746
// switch_title_option	00000000004197F7
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
            palette_update();
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

// interfac_play_vid_draw_quotes	000000000041BC8B
// interfac_init_title_play_music	000000000041BD5E
// interfac_set_player_some	000000000041BE60
// interfac_init_exit_screens	000000000041BEC4
// InitCutscene	000000000041BF78
