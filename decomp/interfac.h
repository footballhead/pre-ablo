#ifndef __INTERFAC_H__
#define __INTERFAC_H__

#include <windows.h>

//
// variables
//

extern HANDLE hCurrentVideo;

//
// functions
//

void IncProgress();
LRESULT ShowProgress(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
void interfac_PlayLogo_DrawQuotes();
void interfac_init_title_play_music();

#endif