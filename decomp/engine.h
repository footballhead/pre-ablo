#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <windows.h>

//
// functions
//

DWORD FileGetSize(const char* filename);
void CelDraw(int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth);
BYTE* LoadFileInMem(const char *pszName);
int GetDirection(int x1, int y1, int x2, int y2);
int random_(int v);

#endif