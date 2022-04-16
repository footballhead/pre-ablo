#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <windows.h>
#include <strsafe.h>

#include "structs.h"

#define LOCAL_SAVE_GAME00 "SAVE\\Game00.sav"
#define PLR_START_OFFSET 0x20
#define WLOAD_SIZE 2
#define MAGIC_SIZE 3

BYTE *tbuff;
PlayerStruct plr[MAX_PLRS];
QuestStruct quests[MAXQUESTS];
int numquests;
int leveltype;
int gbActivePlayers;

// https://docs.microsoft.com/en-us/windows/win32/debug/retrieving-the-last-error-code
static void ShowError(LPTSTR lpszFunction)
{
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
                                      (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
                    LocalSize(lpDisplayBuf) / sizeof(TCHAR),
                    TEXT("%s failed with error %d: %s"),
                    lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK | MB_ICONERROR);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}

// Client must GlobalFree the returned pointer
// Inspired by GM_LoadGame(.text:00460D3F)
static LPVOID ReadSaveFile(DWORD* dwBytes) {
    HFILE hFile = _lopen(LOCAL_SAVE_GAME00, OF_SHARE_DENY_WRITE);
    if (hFile == HFILE_ERROR) {
        if (GetLastError() == ERROR_PATH_NOT_FOUND) {
            MessageBox(NULL,
                       TEXT("Did not find save file at " LOCAL_SAVE_GAME00 ". "
                            "Are you running from the same directory as "
                            "DIABLO.EXE?"),
                       TEXT("Error"),
                       MB_OK | MB_ICONERROR);
            return NULL;
        }

        ShowError(TEXT("_lopen"));
        return NULL;
    }

    *dwBytes = _llseek(hFile, 0, 2);
    LPVOID lpBuffer = GlobalLock(GlobalAlloc(GMEM_FIXED, *dwBytes));
    _llseek(hFile, 0, 0);

    UINT read_bytes = _lread(hFile, lpBuffer, *dwBytes);
    _lclose(hFile);

    if (read_bytes < *dwBytes) {
        ShowError(TEXT("_lread"));
        return NULL;
    }
    
    return lpBuffer;
}

// .text:00460977
static int WLoad()
{
    int rv;
    if (*tbuff & 0x80) {
        rv = 0xFFFF0000;
    } else {
        rv = 0;
    }
    rv |= *tbuff++ << 8;
    rv |= *tbuff++;

    return rv;
}

static BOOL OLoad()
{
    if (*tbuff++ == TRUE)
        return TRUE;
    else
        return FALSE;
}

static void LoadPlayer(int i)
{
    // Demo difference! Omit the final 9 (not 10) pointers
    memcpy(&plr[i], tbuff, sizeof(*plr) - (9 * sizeof(void *)));
    tbuff += sizeof(*plr) - (9 * sizeof(void *));
}

static void LoadQuest(int i) {
    memcpy(&quests[i], tbuff, sizeof(*quests));
    tbuff += sizeof(*quests);
}

static void LoadGame() {
    tbuff += MAGIC_SIZE; // skip magic number
    BOOL setlevel = OLoad();
    int setlvlnum = WLoad();
    int currlevel = WLoad();
    leveltype = WLoad();
    int _ViewX = WLoad();
    int _ViewY = WLoad();
    BOOL invflag = OLoad();
    BOOL chrflag = OLoad();
    gbActivePlayers = WLoad();
    int _nummonsters = WLoad();
    int _numitems = WLoad();
    int _nummissiles = WLoad();
    int _numspells = WLoad();
    int _numobjects = WLoad();
    plr[0].plractive = OLoad();
    plr[1].plractive = OLoad();
    plr[2].plractive = OLoad();
    plr[3].plractive = OLoad();
    for (int i = 0; i < gbActivePlayers; ++i) {
        if (plr[i].plractive) {
            LoadPlayer(i);
        }
    }
    numquests = WLoad();
    for (int i = 0; i < MAXQUESTS; ++i) {
        LoadQuest(i);
    }

    // Don't need to load the rest
}

static void SavePlayer(int i)
{
    // Demo difference! Omit the final 9 (not 10) pointers
    memcpy(tbuff, &plr[i], sizeof(*plr) - (9 * sizeof(void *)));
    tbuff += sizeof(*plr) - (9 * sizeof(void *));
}

static void SaveQuest(int i)
{
    memcpy(tbuff, &quests[i], sizeof(*quests));
    tbuff += sizeof(*quests);
}

static void SaveGame() {
    tbuff += PLR_START_OFFSET; // plr[0] starts at 0x20
    SavePlayer(0);

    tbuff += WLOAD_SIZE; // skip numquests (2 bytes)
    for (int i = 0; i < numquests; ++i) {
        SaveQuest(i);
    }
}

static BOOL WriteSaveFile(LPVOID lpBuffer, DWORD dwBytes) {
    HANDLE hFile = CreateFile(TEXT(LOCAL_SAVE_GAME00), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
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

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   PSTR lpCmdLine, INT nCmdShow)
{
    int choice = MessageBox(NULL,
        TEXT("This tool will reset the dungeon and quests so you can "
             "experience them gain. You will lose items in the dungeon that "
             "you have not picked up. Anything in town will be kept. Your "
             "character will be left alone. Reset your progress?"),
        TEXT("Reset your progress?"),
        MB_YESNO | MB_ICONWARNING);
    if (choice != IDYES) {
        return 1;
    }

    DWORD dwBytes;
    LPVOID lpBuffer = ReadSaveFile(&dwBytes);
    if (!lpBuffer) {
        return 1;
    }

    tbuff = lpBuffer;
    LoadGame();

    if (leveltype != DTYPE_TOWN) {
        // Would like to place player back in town but that's complicated so...
        MessageBox(NULL, TEXT("Return to town first!"), TEXT("Error"), MB_OK | MB_ICONERROR);
        GlobalUnlock(GlobalHandle(lpBuffer));
        GlobalFree(GlobalHandle(lpBuffer));
        return 1;
    }

    if (gbActivePlayers != 1) {
        MessageBox(NULL, TEXT("Expected gbActivePlayers == 1!"), TEXT("Error"), MB_OK | MB_ICONERROR);
        GlobalUnlock(GlobalHandle(lpBuffer));
        GlobalFree(GlobalHandle(lpBuffer));
        return 1;
    }

    // Reset player
    srand(time(NULL));
    for (int i = 0; i < 17; ++i)
    {
        plr[0]._pSeedTbl[i] = rand();
        plr[0]._pLvlVisited[i] = FALSE;
        plr[0]._pSLvlVisited[i] = FALSE;
    }

    // Reset quests
    for (int i = 0; i < numquests; ++i) {
        quests[i]._qactive = QUEST_INIT;
    }

    tbuff = lpBuffer;
    SaveGame();

    if (!WriteSaveFile(lpBuffer, dwBytes)) {
        GlobalUnlock(GlobalHandle(lpBuffer));
        GlobalFree(GlobalHandle(lpBuffer));
        return 1;
    }

    // Cleanup
    GlobalUnlock(GlobalHandle(lpBuffer));
    GlobalFree(GlobalHandle(lpBuffer));

    MessageBox(NULL, TEXT("Success! Reload your game and walk to the cathedral."), TEXT("Success!"), MB_OK | MB_ICONINFORMATION);

    return 0;
}
