#ifndef __IO_H__
#define __IO_H__

#include <windows.h>

// Read contents of SAVE\Game00.sav
// Caller must GlobalFree the returned pointer
LPVOID ReadSaveFile(DWORD* dwBytes);

// Write buffer to SAVE\Game00.sav
BOOL WriteSaveFile(LPVOID lpBuffer, DWORD dwBytes);

#endif