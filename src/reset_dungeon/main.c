#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "io.h"
#include "saveload.h"

// https://docs.microsoft.com/en-us/windows/win32/debug/retrieving-the-last-error-code

static BOOL IsSaveInValidState() {
  if (leveltype != DTYPE_TOWN) {
    // Would like to place player back in town but that's complicated so...
    MessageBox(NULL, TEXT("Return to town first!"), TEXT("Error"),
               MB_OK | MB_ICONERROR);
    return FALSE;
  }

  if (gbActivePlayers != 1) {
    MessageBox(NULL, TEXT("Expected gbActivePlayers == 1!"), TEXT("Error"),
               MB_OK | MB_ICONERROR);
    return FALSE;
  }

  // TODO: Detect if DIABLO.EXE is running

  return TRUE;
}

static void ResetDungeon() {
  // Reset all special levels
  for (int i = 0; i < NUMLEVELS; ++i) {
    plr[0]._pSLvlVisited[i] = FALSE;
  }

  // Reset other levels (except for town, notice i starts at 1)
  srand(time(NULL));
  for (int i = 1; i < NUMLEVELS; ++i) {
    plr[0]._pSeedTbl[i] = rand();
    plr[0]._pLvlVisited[i] = FALSE;
  }

  // Reset quests
  for (int i = 0; i < numquests; ++i) {
    quests[i]._qactive = QUEST_INIT;
  }
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine,
                   INT nCmdShow) {
  // Do not continue if user says No
  const int choice = MessageBox(
      NULL,
      TEXT("This tool will reset the dungeon and quests so you can "
           "experience them again. You will lose items in the dungeon that "
           "you have not picked up. Anything in town will be kept. Your "
           "character will be left alone. Reset your progress?"),
      TEXT("Reset your progress?"), MB_YESNO | MB_ICONWARNING);
  if (choice != IDYES) {
    return 1;
  }

  // Load Game00.sav into memory
  DWORD dwBytes;
  LPVOID lpBuffer = ReadSaveFile(&dwBytes);
  if (!lpBuffer) {
    return 1;
  }

  // Initialize saveload with Game00.sav contents
  tbuff = lpBuffer;

  // Parse the variables we care about
  LoadGame();

  // Bail if we detect a condition that we cannot correct
  if (!IsSaveInValidState()) {
    return 1;
  }

  // All good, do the reset
  ResetDungeon();

  // Serialize the variables into tbuff (lpBuffer)
  SaveGame();

  // Write modified Game00.sav to disk
  if (!WriteSaveFile(lpBuffer, dwBytes)) {
    GlobalUnlock(GlobalHandle(lpBuffer));
    GlobalFree(GlobalHandle(lpBuffer));
    return 1;
  }

  // Cleanup
  GlobalUnlock(GlobalHandle(lpBuffer));
  GlobalFree(GlobalHandle(lpBuffer));

  MessageBox(NULL, TEXT("Success! Reload your game and walk to the cathedral."),
             TEXT("Success!"), MB_OK | MB_ICONINFORMATION);

  return 0;
}
