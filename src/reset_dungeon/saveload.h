// These functions are modified versions from decomp. I chose to reuse them
// because they work.
//
// Expected usage:
//
// 1. Set `tbuff`
// 2. Call `LoadGame()`
// 3. Modify `plr`, `quests`, etc
// 4. Reset `tbuff`
// 5. Call `SaveGame()`

#pragma once

#include <windows.h>

#include "structs.h"

// `plr` offset inside the save file.
constexpr int kPlrOffset = 0x20;
// Size of the word loaded by `WLoad()`
constexpr int kWordSize = 2;
// Smallest save file size we can accept before accessing out of bounds memory.
constexpr int kMinimalSaveSize = kPlrOffset + sizeof(PlayerStruct) * MAX_PLRS +
                                 kWordSize + sizeof(QuestStruct) * MAXQUESTS;

// This must be set in order to use `LoadGame()` or `SaveGame()`.
extern BYTE *tbuff;

// These variables will be set by `LoadGame()` and read by `SaveGame()`
extern PlayerStruct plr[MAX_PLRS];
extern QuestStruct quests[MAXQUESTS];
// These variables will be set by `LoadGame()`. Any changes will be ignored by
// `SaveGame()`.
extern int numquests;
extern int leveltype;
extern int gbActivePlayers;

// Deserealize from `tbuff` into the static variables like `plr`, `quests`, etc.
//
// `tbuff` must be a valid pointer
void LoadGame();

// Serialize `plr` and `quests` to `tbuff`.
//
// `tbuff` must be a valid pointer
void SaveGame();
