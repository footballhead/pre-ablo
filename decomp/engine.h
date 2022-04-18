#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <windows.h>

void CelDraw(int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth);

BYTE* LoadFileInMem(const char *pszName);

#endif