#include "player.h"

//
// uninitialized vars (.data:)
//

// ...
int gbActivePlayers;
// ...
PlayerStruct plr[MAX_PLRS];
int myplr;
BOOL deathflag;
// ...

// SetAnimPtrs	0000000000464BB0	
// SetAllAnimPtrs	0000000000464C0D	
// LoadPlrGFX	0000000000464EE0	
// InitPlayerGFX	0000000000465887	
// InitPlrGFXMem	0000000000465DC2	
// FreePlayerGFX	0000000000465F7F	
// NewPlrAnim	000000000046632D	
// ClearPlrPVars	0000000000466421	
// SetPlrAnims	000000000046652F	
// CreatePlayer	0000000000466C74	
// __dc_FillHealthAndMana	0000000000467601	
// NextPlrLevel	0000000000467683	

// AddPlrExperience	0000000000467988	
void AddPlrExperience(int pnum, int lvl, int exp)
{
    // TODO
}

// ClearPlrRVars	0000000000467B02	
// InitPlayer	0000000000467C48	
// InitMultiView	000000000046841D	
// InitPlayerLoc	000000000046845E	
// PlayerGetDirection	0000000000468739	
// PlrDirOK	00000000004688DC	
// PlrClrTrans	0000000000468A82	
// PlrDoTrans	0000000000468B06	
// StartStand	0000000000468BF1	
// PM_ChangeLightOff	0000000000468E08	
// PM_ChangeOffset	0000000000468F49	
// StartWalk	000000000046911D	
// StartWalk2	0000000000469548	
// StartWalk3	0000000000469AD2	
// StartAttack	000000000046A03C	
// StartRangeAttack	000000000046A253	
// StartPlrBlock	000000000046A4A9	
// StartSpell	000000000046A6CA	
// RemovePlrFromMap	000000000046AAD5	

// StartPlrHit	000000000046AC51	
void StartPlrHit(int pnum)
{
    // TODO
}

// SyncPlrKill	000000000046AF2C	
void SyncPlrKill(int pnum)
{
    // TODO
}

// StartNewLvl	000000000046B1F5	
// StartWarpLvl	000000000046B451	
// StartWarpLvl_2	000000000046B91D	
// StartNewGame	000000000046BBC6	
// PM_DoStand	000000000046BDE9	
// PM_DoWalk	000000000046BE01	
// PM_DoWalk2	000000000046C1C6	
// PM_DoWalk3	000000000046C4D3	
// WeaponDur	000000000046C929	
// PlrHitMonst	000000000046CCD4	
// PlrHitObj	000000000046D48D	
// PM_DoAttack	000000000046D541	
// PM_DoRangeAttack	000000000046D858	
// ShieldDur	000000000046DA04	
// PM_DoBlock	000000000046DB94	
// PM_DoSpell	000000000046DC2A	
// ArmorDur	000000000046DEE5	
// PM_DoGotHit	000000000046E12C	
// PM_DoDeath	000000000046E1E2	
// PM_DoNewLvl	000000000046E319	
// ParseCmd	000000000046E331	
// NetSendCmd	000000000046ED1E	
// Player_SerializeNextAction	000000000046EDD0	
// Player_DeserializeNextAction	000000000046F14A	
// player_action_dequeue	000000000046F425	
// PlrDeathModeOK	000000000046FF2B	
// ProcessPlayers	000000000046FFC7	
// PosOkPlayer	0000000000470372	
// MakePlrPath	0000000000470514	
// player_4708D4	00000000004708D4	
// PlayerAppendPathCommand	0000000000470C20	
// CheckPlrSpell	0000000000470CC8	
// SyncPlrAnim	0000000000470F44	
// SyncInitPlrPos	000000000047130D	

// ModifyPlrStr	0000000000471414	
void ModifyPlrStr(int p, int l)
{
    // TODO
}

// ModifyPlrMag	00000000004714CC	
void ModifyPlrMag(int p, int l)
{
    // TODO
}

// ModifyPlrDex	00000000004715CD	
void ModifyPlrDex(int p, int l)
{
    // TODO
}

// ModifyPlrVit	000000000047162A	
void ModifyPlrVit(int p, int l)
{
    // TODO
}
