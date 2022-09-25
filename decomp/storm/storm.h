#ifndef __STORM_H__
#define __STORM_H__

#include <windows.h>

#include "../directx/DDRAW.H"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef STORMAPI
#define STORMAPI __stdcall
#endif

BOOL STORMAPI SFileOpenArchive(const char *szMpqName, DWORD dwPriority, DWORD dwFlags, HANDLE *phMpq);
BOOL STORMAPI SFileCloseArchive(HANDLE hArchive);

BOOL STORMAPI SFileOpenFile(const char *filename, HANDLE *phFile);
BOOL STORMAPI SFileReadFile(HANDLE hFile, void *buffer, DWORD nNumberOfBytesToRead, DWORD *read, LONG *lpDistanceToMoveHigh);
BOOL STORMAPI SFileCloseFile(HANDLE hFile);

LONG STORMAPI SFileGetFileSize(HANDLE hFile, LPDWORD lpFileSizeHigh);

int __stdcall SFileSetFilePointer(HANDLE,int,HANDLE,int);

/*  SDrawManualInitialize
 *
 *  Sets the DirectDraw variables to be referenced in Storm.
 *
 *  hWnd:           The handle of the DirectDraw window.
 *  ddInterface:    The DirectDraw interface.
 *  primarySurface: The first and primary surface.
 *  surface2:       A second surface. Behaviour not completely known.
 *  backSurface:    The fourth and final surface. The back surface.
 *  ddPalette:      The DirectDraw palette if the application requires it.
 *  hPalette:       The palette handle that belongs to the window.
 *                  If this is NULL and ddPalette is specified, then it
 *                  will be created automatically. A palette can be created
 *                  using the CreatePalette WinAPI function.
 *
 *  Returns FALSE if no variables were specified.
 */
BOOL
STORMAPI
SDrawManualInitialize(
     HWND hWnd,
     LPDIRECTDRAW ddInterface,
     LPDIRECTDRAWSURFACE primarySurface,
     LPDIRECTDRAWSURFACE surface2,
     LPDIRECTDRAWSURFACE backSurface,
     LPDIRECTDRAWPALETTE ddPalette,
     HPALETTE hPalette);

BOOL STORMAPI SFileDdaInitialize(HANDLE directsound);
BOOL STORMAPI SVidPlayEnd(HANDLE video);
BOOL STORMAPI SVidPlayBegin(const char *filename, int a2, int a3, int a4, int flags, HANDLE *video);
BOOL __cdecl SVidPlayContinue(void);
BOOL STORMAPI SFileDdaBegin(HANDLE hFile, DWORD flags, DWORD mask);

#ifdef __cplusplus
}
#endif

#endif