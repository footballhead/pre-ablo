#ifndef __STORM_H__
#define __STORM_H__

#include <windows.h>

#ifndef STORMAPI
#define STORMAPI __stdcall
#endif

BOOL STORMAPI SFileOpenFile(const char *filename, HANDLE *phFile);
BOOL STORMAPI SFileReadFile(HANDLE hFile, void *buffer, DWORD nNumberOfBytesToRead, DWORD *read, LONG *lpDistanceToMoveHigh);
BOOL STORMAPI SFileCloseFile(HANDLE hFile);

#endif