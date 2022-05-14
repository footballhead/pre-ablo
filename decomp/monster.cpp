#include "monster.h"

//
// initialized variables (.data:004A2040)
//

int MWVel[24][3] = {
    {256, 512, 1024},
    {128, 256, 512},
    {85, 170, 341},
    {64, 128, 256},
    {51, 102, 204},
    {42, 85, 170},
    {36, 73, 146},
    {32, 64, 128},
    {28, 56, 113},
    {26, 51, 102},
    {23, 46, 93},
    {21, 42, 85},
    {19, 39, 78},
    {18, 36, 73},
    {17, 34, 68},
    {16, 32, 64},
    {15, 30, 60},
    {14, 28, 57},
    {13, 26, 54},
    {12, 25, 51},
    {12, 24, 48},
    {11, 23, 46},
    {11, 22, 44},
    {10, 21, 42}};
char animletter[7] = "nwahds";
int left[8] = {7, 0, 1, 2, 3, 4, 5, 6};
int right[8] = {1, 2, 3, 4, 5, 6, 7, 0};
int opposite[8] = {4, 5, 6, 7, 0, 1, 2, 3};
int offset_x[8] = {1, 0, -1, -1, -1, 0, 1, 1};
int offset_y[8] = {1, 1, 1, 0, -1, -1, -1, 0};
int rnd5[4] = {5, 10, 15, 20};   // unused
int rnd10[4] = {10, 15, 20, 30}; // unused
int rnd20[4] = {20, 30, 40, 50}; // unused
int rnd60[4] = {60, 70, 80, 90}; // unused
void (*AiProc[])(int i) = {
    &MAI_Zombie,
    &MAI_Fat,
    &MAI_SkelSd,
    &MAI_SkelBow,
    &MAI_Scav,
    &MAI_Rhino,
    &MAI_GoatMc,
    &MAI_GoatBow,
    &MAI_Fallen,
    &MAI_Magma,
    &MAI_SkelKing,
    &MAI_Bat,
    &MAI_Garg,
    &MAI_Cleaver,
    &MAI_Succ,
    &MAI_Sneak,
    &MAI_Storm};

//
// code (.text:00401000)
//

// TranslateMonsterGFX	0000000000401000
// __dc_MonstTaken	0000000000401117
// InitLevelMonsters_and_GetLevelMTypes	0000000000401217
// InitMonsterGFX	0000000000401414
// ClearMVars	00000000004019B0
// InitMonster	0000000000401A8E
// ClrAllMonsters	0000000000401F77
// MonstPlace	0000000000402198
// monster_some_crypt	000000000040226F
// PlaceMonster	00000000004023BC
// PlaceMonstersOldDrlg	0000000000402486
// __dc_LoadUniMonstTrans	0000000000402748
// PlaceUniqueMonst	000000000040275E
// PlaceGroup	0000000000402D34
// StartPlacingMonsters	0000000000403095
// InitMonsters	0000000000403177
// SetMapMonsters	000000000040329B
// DeleteMonster	00000000004035FB
// AddMonster	000000000040364A
// NewMonsterAnim	000000000040373E
// M_Enemy	000000000040380E
// M_CheckEFlag	0000000000403973
// M_StartStand	0000000000403A64
// M_StartDelay	0000000000403BAB
// M_StartSpStand	0000000000403BFF
// M_StartWalk	0000000000403CFE
// M_StartWalk2	0000000000403ED9
// M_StartWalk3	00000000004041E3
// M_StartAttack	0000000000404529
// M_StartRAttack	000000000040465F
// M_StartRSpAttack	00000000004047B0
// M_StartSpAttack	0000000000404904
// M_StartEat	0000000000404A3D
// M_ClearSquares	0000000000404B33
// M_StartHit	0000000000404DCC
// M_StartKill	0000000000404F8D
// M_StartFadein	0000000000405179
// M_StartFadeout	000000000040528E
// M_StartHeal	00000000004053D3
// M_ChangeLightOffset	0000000000405468
// M_DoWalk	0000000000405565
// M_DoWalk2	0000000000405821
// M_DoWalk3	0000000000405A4A
// M_TryH2HHit	0000000000405D7F
// M_DoAttack	000000000040641F
// M_DoRAttack	00000000004066ED
// M_DoRSpAttack	0000000000406859
// M_DoSAttack	00000000004069CB
// M_DoFadein	0000000000406ADF
// M_DoFadeout	0000000000406B53
// M_DoHeal	0000000000406BC9
// M_Teleport	0000000000406C72
// M_DoGotHit	0000000000406E63
// M_DoDeath	0000000000406EDD
// M_DoSpStand	0000000000407181
// M_DoDelay	0000000000407237
// M_DoStone	000000000040736F
// M_WalkDir	00000000004073FB
// GroupUnity	00000000004075E4
// M_CallWalk	0000000000407A10
// M_PathWalk	0000000000407C1A
// M_RoundWalk	0000000000407C59
// __dc_M_Face	0000000000407D84

// MAI_Zombie	0000000000407E52
void MAI_Zombie(int i)
{
    // TODO
}

// MAI_SkelSd	000000000040805B
void MAI_SkelSd(int i)
{
    // TODO
}
// MAI_Bat	0000000000408257
void MAI_Bat(int i)
{
    // TODO
}
// MAI_SkelBow	000000000040869A
void MAI_SkelBow(int i)
{
    // TODO
}
// MAI_Fat	00000000004088CA
void MAI_Fat(int i)
{
    // TODO
}
// MAI_Sneak	0000000000408B9C
void MAI_Sneak(int i)
{
    // TODO
}
// MAI_Fallen	0000000000408F04
void MAI_Fallen(int i)
{
    // TODO
}
// MAI_Cleaver	000000000040928B
void MAI_Cleaver(int i)
{
    // TODO
}
// MAI_Round	00000000004093C0
// MAI_GoatMc	0000000000409809
void MAI_GoatMc(int i)
{
    // TODO
}
// MAI_Ranged	000000000040982C
// MAI_GoatBow	0000000000409A55
void MAI_GoatBow(int i)
{
    // TODO
}
// MAI_Succ	0000000000409A75
void MAI_Succ(int i)
{
    // TODO
}
// MAI_Scav	0000000000409A98
void MAI_Scav(int i)
{
    // TODO
}
// MAI_Garg	0000000000409EA9
void MAI_Garg(int i)
{
    // TODO
}
// MAI_RoundRanged	000000000040A04A
// MAI_Magma	000000000040A5B6
void MAI_Magma(int i)
{
    // TODO
}
// MAI_Storm	000000000040A5D9
void MAI_Storm(int i)
{
    // TODO
}
// MAI_SkelKing	000000000040A5FC
void MAI_SkelKing(int i)
{
    // TODO
}
// MAI_Rhino	000000000040AB24
void MAI_Rhino(int i)
{
    // TODO
}

// ProcessMonsters	000000000040B08A
// FreeMonsters	000000000040B67A
// DirOK	000000000040B876
// LineClear	000000000040BDB5
// PosOkMissile	000000000040BDE9
// CheckNoSolid	000000000040BE53
// CheckNoSolid_2	000000000040BE9D
// LineClearF	000000000040BEE7
// SyncMonsterAnim	000000000040BFA3
// M_FallenFear	000000000040C495
// PrintMonstHistory	000000000040C67F
// MissToMonst	000000000040C7B0
// PosOkMonst	000000000040CA2D
// IsSkel	000000000040CB5F
// M_SpawnSkel	000000000040CBBD
// SpawnSkeleton	000000000040CCDD
//