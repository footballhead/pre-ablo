#ifndef __INTERFAC_H__
#define __INTERFAC_H__

#include <windows.h>

//
// functions
//

void IncProgress();
LRESULT ShowProgress(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

#endif