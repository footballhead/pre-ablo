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

// WinMain(x,x,x,x)	0000000000484A20	
// dx_init	0000000000484E67	
// WNDPROC_mode0_blizzard_intro	00000000004853DA	
// diablo_48565F	000000000048565F	
// GM_Game	00000000004865C4	
// MainWndProc	00000000004881C9	
// diablo_store_with_mutex_spinlock	00000000004882F9	
// timer_something	000000000048833D	
// InitLevelType	0000000000488442	
// init_multi_with_time_srand	00000000004884C8	
// init_create_window	0000000000488769	
// dx_cleanup	000000000048885D	

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
// LoadLvlGFX	00000000004889EA	
// LoadAllGFX	0000000000488BC1	
// CreateLevel	0000000000488C54	

void LoadGameLevel(BOOL firstflag, int lvldir) {
    // TODO
    // calls LoadLevel
}

// InitLevels	000000000048947E	
// diablo_489510_plr_rel	0000000000489510	
// game_logic	0000000000489715	
