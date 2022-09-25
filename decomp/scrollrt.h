#ifndef __SCROLLRT_H__
#define __SCROLLRT_H__

#include <windows.h>

//
// variables
//

extern int PitchTbl[1024];

//
// functions
//

void ScrollView();
void ClearScreenBuffer();
void scrollrt_draw_game_screen(BOOL draw_cursor);

#endif