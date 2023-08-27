#include "io.h"

#include "error.h"

#define LOCAL_SAVE_GAME00 "SAVE\\Game00.sav"

// Inspired by GM_LoadGame(.text:00460D3F)
LPVOID ReadSaveFile(DWORD* dwBytes) {
  HFILE hFile = _lopen(LOCAL_SAVE_GAME00, OF_SHARE_DENY_WRITE);
  if (hFile == HFILE_ERROR) {
    if (GetLastError() == ERROR_PATH_NOT_FOUND) {
      MessageBox(NULL,
                 TEXT("Did not find save file at " LOCAL_SAVE_GAME00 ". "
                      "Are you running from the same directory as "
                      "DIABLO.EXE?"),
                 TEXT("Error"), MB_OK | MB_ICONERROR);
      return NULL;
    }

    ShowError(TEXT("_lopen"));
    return NULL;
  }

  *dwBytes = _llseek(hFile, /*lOffset=*/0, /*iOrigin=*/2);
  LPVOID lpBuffer = GlobalLock(GlobalAlloc(GMEM_FIXED, *dwBytes));
  _llseek(hFile, /*lOffset=*/0, /*iOrigin=*/0);

  UINT read_bytes = _lread(hFile, lpBuffer, *dwBytes);
  _lclose(hFile);

  if (read_bytes < *dwBytes) {
    ShowError(TEXT("_lread"));
    return NULL;
  }

  return lpBuffer;
}

BOOL WriteSaveFile(LPVOID lpBuffer, DWORD dwBytes) {
  HANDLE hFile = CreateFile(TEXT(LOCAL_SAVE_GAME00), GENERIC_WRITE, 0, NULL,
                            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE) {
    ShowError(TEXT("CreateFile"));
    return FALSE;
  }

  DWORD unused;
  BOOL did_write = WriteFile(hFile, lpBuffer, dwBytes, &unused, 0);
  CloseHandle(hFile);

  if (!did_write) {
    ShowError(TEXT("WriteFile"));
    return FALSE;
  }

  return TRUE;
}
