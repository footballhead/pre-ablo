#include <windows.h>

#include "diablo.h"
#include "enums.h"

extern int leveltype;
extern LPVOID pDungeonCels;
extern LPVOID pMegaTiles;
extern LPVOID pMiniTiles;
extern LPVOID pSpecialCels;

void FreeMonsters();
void FreeObjectGFX();
void FreeMissileGFX();
void FreeSpells();
void FreeEffects();
void FreeTownerGFX();
void FreeTownerEffects();

void FreeGameMem() {
    GlobalUnlock(GlobalHandle(pDungeonCels));
    GlobalFree(GlobalHandle(pDungeonCels));

    GlobalUnlock(GlobalHandle(pMegaTiles));
    GlobalFree(GlobalHandle(pMegaTiles));

    GlobalUnlock(GlobalHandle(pMiniTiles));
    GlobalFree(GlobalHandle(pMiniTiles));

    GlobalUnlock(GlobalHandle(pSpecialCels));
    GlobalFree(GlobalHandle(pSpecialCels));

    if (leveltype != DTYPE_TOWN) {
        GlobalUnlock(GlobalHandle(pSpecialCels));
        GlobalFree(GlobalHandle(pSpecialCels));

        FreeMonsters();
        FreeObjectGFX();
        FreeMissileGFX();
        FreeSpells();
        FreeEffects();
    } else {
        FreeTownerGFX();
        FreeMissileGFX();
        FreeTownerEffects();
    }
}

// LoadLvlGFX
// LoadAllGFX
// CreateLevel

void LoadGameLevel(BOOL firstflag, int lvldir) {
    // TODO
    // calls LoadLevel
}