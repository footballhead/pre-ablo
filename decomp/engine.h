#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <windows.h>

//
// defines
//

#define MemFreeDbg(x) \
    GlobalUnlock(GlobalHandle(x)); \
    GlobalFree(GlobalHandle(x));

#define DiabloAllocPtr(x) \
    GlobalLock(GlobalAlloc(0, x))

//
// functions
//

void CelApplyTrans(BYTE *p, BYTE *ttbl, int nCel);
DWORD FileGetSize(const char* filename);
void CelDraw(int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth);
BYTE* LoadFileInMem(const char *pszName);
int GetDirection(int x1, int y1, int x2, int y2);
int random_(int v);

#endif