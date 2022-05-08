#include "diablo.h"

#include <windows.h>

void palette_update();

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
// IntCheck	0000000000419B93	
// WNDPROC_mode12_quotes_crow_video	0000000000419C60	
// WNDPROC_mode1_title_newloadquit	000000000041A06E	
// WNDPROC_mode2_selclass_entname	000000000041A5DD	
// WNDPROC_mode13_exit_5screenshots	000000000041AD3E	

// ShowProgress	000000000041B180	
LRESULT ShowProgress(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg) {
    case WM_ACTIVATEAPP:
        gbActive = wParam;
        if (gbActive) {
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
