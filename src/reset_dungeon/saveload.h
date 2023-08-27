#ifndef __SAVELOAD_H__
#define __SAVELOAD_H__

#include "structs.h"
#include <windows.h>

// This must be set in order to use LoadGame/SaveGame
extern BYTE *tbuff;

// These variables will be set by LoadGame and used by SaveGame
extern PlayerStruct plr[MAX_PLRS];
extern QuestStruct quests[MAXQUESTS];
extern int numquests;
extern int leveltype;
extern int gbActivePlayers;

// Pre-condition: tbuff is set to a valid pointer
void LoadGame();
// Pre-condition: tbuff is set to a valid pointer
void SaveGame();

#endif
