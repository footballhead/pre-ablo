#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <windows.h>

#include "sound.h"

//
// defines
//

#define MemFreeDbg(x)              \
    GlobalUnlock(GlobalHandle(x)); \
    GlobalFree(GlobalHandle(x));

#define DiabloAllocPtr(x) \
    GlobalLock(GlobalAlloc(GMEM_FIXED, x))

//
// functions
//

BYTE *LoadFileInMem(const char *pszName);
int LoadWaveFileHeaderInMem(const char *filename, TSnd *pSnd, LPWAVEFORMATEX *pFormat);
int LoadWaveFileWithMem(const char *filename, TSnd *pSnd, LPWAVEFORMATEX *pFormat, void **pcm_data);
DWORD FileGetSize(const char *filename);
char *FileAddLoadPrefix(const char *filename);
void CelDraw(int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth);
void CelApplyTrans(BYTE *p, BYTE *ttbl, int nCel);
int GetDirection(int x1, int y1, int x2, int y2);
int random_(int v);

#endif