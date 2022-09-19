#ifndef __INTERFAC_H__
#define __INTERFAC_H__

#include <windows.h>

//
// variables
//

HANDLE hCurrentVideo;

//
// functions
//

void IncProgress();
LRESULT ShowProgress(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
void interfac_play_vid_draw_quotes();
void interfac_init_title_play_music();

#endif