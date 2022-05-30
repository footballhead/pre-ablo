#include "monster.h"

#include "debug.h"
#include "diablo.h"
#include "effects.h"
#include "enums.h"
#include "engine.h"
#include "gendung.h"
#include "missiles.h"
#include "player.h"
#include "quests.h"

#include <stdio.h>

//
// Initialized variables (.data:004A2040)
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
// Uninitialized variables (.data:004BE4E0)
//

int nummtypes;
int nummonsters;
MonsterStruct monster[MAXMONSTERS];
int monstactive[MAXMONSTERS];
CMonster Monsters[MAX_LVLMTYPES];
int monstkills[MAXMONSTERS];
char byte_4CAFC8[272]; // Only used by sub_00401117 (dead code)

//
// Code (.text:00401000)
//

// .text:00401000
void InitMonsterTRN(int monst, BOOL special)
{
    BYTE *f;
    int j;
    int i;
    int n;

    f = Monsters[monst].trans_file;
    // The disassembly looked weird enough to be hand-written
    __asm {
        mov     esi, f // array data to manipulate
        mov     ecx, 256 // loop counter
        xor     eax, eax
    top:
        mov     al, [esi]
        cmp     al, 255
        jnz     next
        mov     [esi], ah ; // If (*f == 255) { *f = 0; } (AH is 0 due to xor eax, eax)
    next:
        inc     esi
        dec     ecx
        jnz     top // loop if ecx != 0
    }

    if (special == TRUE)
    {
        n = 6;
    }
    else
    {
        n = 5;
    }

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < 8; j++)
        {
            CelApplyTrans(Monsters[monst].Anims[i].Cels[j], Monsters[monst].trans_file, Monsters[monst].Anims[i].Frames);
        }
    }
}

// .text:00401117
// Dead code
// TODO: Is any of this right?
int sub_00401117(int mtype)
{
    BOOL stop;
    int j;
    int i;

    stop = FALSE;
    for (i = 0; i <= currlevel && stop == FALSE; i++)
    {
        for (j = 0; j < 16 && &byte_4CAFC8[j << 4] != 0 && stop == FALSE; j++)
        {
            if (byte_4CAFC8[j + i << 4] - 1 > mtype &&
                strcmp(monsterdata[mtype].GraphicType, monsterdata[byte_4CAFC8[j + i << 4] - 1].GraphicType) == 0)
            {
                stop = TRUE;
            }
            else
            {
                stop = FALSE;
            }
        }
    }
    return stop;
}

// .text:00401217
// This function is split into 2 in devilution
void InitLevelMonsters_and_GetLevelMTypes()
{
    // TODO proper var order
    int mamask;
    int monstimgtot;
    int minl;
    int mtype;
    int i;
    int randType;
    int typelist[MAXMONSTERS];
    int nt;
    int maxl;

    nummtypes = 0;
    monstimgtot = 0;
    nt = 0;

    if (demo_mode != FALSE)
    {
        mamask = MAT_ALWAYS;
    }
    else
    {
        mamask = MAT_ALWAYS | MAT_RETAIL;
    }

    for (i = 0; i < NUM_MTYPES; i++)
    {
        minl = 15 * monsterdata[i].mMinDLvl / 30 + 1;
        maxl = 15 * monsterdata[i].mMaxDLvl / 30 + 1;

        if (minl <= currlevel && currlevel <= maxl)
        {
            if (MonstAvailTbl[i] & mamask)
            {
                typelist[nt] = i;
                nt++;
            }
        }
    }

    if (quests[Q_BUTCHER]._qlevel == currlevel)
    {
        Monsters[nummtypes].mtype = MT_CLEAVER;
        nummtypes++;
        monstimgtot += monsterdata[MT_CLEAVER].mImgSize;
    }

    // devilution: monstimgtot < 4000!
    // higher monstimgtot means more enemy types in the level
    while (nt > 0 && nummtypes < MAX_LVLMTYPES && monstimgtot < 3000)
    {
        randType = random_(nt);
        mtype = typelist[randType];
        if (monsterdata[mtype].mImgSize + monstimgtot <= 3000)
        {
            Monsters[nummtypes].mtype = mtype;
            nummtypes++;
            monstimgtot += monsterdata[mtype].mImgSize;
        }
        nt--;
        typelist[randType] = typelist[nt];
    }
}

// .text:00401414
void InitMonsterGFX()
{
    BYTE *celBuf;
    char strBuff[256];
    int mtype;
    int i;
    int monst;
    int anim;

    // Debug mode, normal dungeon: only monsters are butcher and axe skeleton
    // Seems like a weird place for this to go (why not in InitLevelMonsters?)
    if (debug_mode != FALSE && setlevel == FALSE)
    {
        nummtypes = 1;
        Monsters[0].mtype = MT_TSKELAX;
        if (currlevel == 1)
        {
            Monsters[nummtypes].mtype = Monsters[0].mtype;
            Monsters[0].mtype = MT_CLEAVER;
            nummtypes++;
        }
    }

    for (monst = 0; nummtypes > monst; monst++)
    {
        mtype = Monsters[monst].mtype;
        for (anim = 0; anim < 6; anim++)
        {
            // Read NWAHD animations and S conditional on has_special
            if (animletter[anim] != 's' || monsterdata[mtype].has_special != FALSE)
            {
                sprintf(strBuff, monsterdata[mtype].GraphicType, animletter[anim]);
                Monsters[monst].Anims[anim].CMem = LoadFileInMem(strBuff);

                // Read group header and store animation for each direction
                celBuf = Monsters[monst].Anims[anim].CMem;
                for (i = 0; i < 8; i++)
                {
                    // ((DWORD*)celBuf)[i] is the group offset
                    Monsters[monst].Anims[anim].Cels[i] = celBuf + ((DWORD *)celBuf)[i];
                }
                Monsters[monst].Anims[anim].Frames = monsterdata[mtype].Frames[anim];
                Monsters[monst].Anims[anim].Rate = monsterdata[mtype].Rate[anim];
            }
        }

        Monsters[monst].width = monsterdata[mtype].width;
        Monsters[monst].width2 = (monsterdata[mtype].width - 64) >> 1;
        Monsters[monst].mMinHP = monsterdata[mtype].mMinHP;
        Monsters[monst].mMaxHP = monsterdata[mtype].mMaxHP;
        Monsters[monst].has_special = monsterdata[mtype].has_special;
        Monsters[monst].mAFNum = monsterdata[mtype].mAFNum;
        Monsters[monst].MData = &monsterdata[mtype];

        if (monsterdata[mtype].has_trans)
        {
            Monsters[monst].trans_file = LoadFileInMem(monsterdata[mtype].TransFile);
            InitMonsterTRN(monst, monsterdata[mtype].has_special);
            MemFreeDbg(Monsters[monst].trans_file);
        }

        if (mtype >= MT_NMAGMA && mtype <= MT_WMAGMA)
        {
            for (i = 0; i < 8; i++)
            {
                sprintf(strBuff, "Monsters\\Magma\\Magball%i.CEL", i + 1);
                pMagballCels[i] = LoadFileInMem(strBuff);
            }
        }

        if (mtype >= MT_STORM && mtype <= MT_MAEL)
        {
            pThinLghningCel = LoadFileInMem("Monsters\\Thin\\Lghning.CEL");
        }

        if (mtype >= MT_SUCCUBUS && mtype <= MT_SOLBRNR)
        {
            pFlareCel = LoadFileInMem("Monsters\\Succ\\Flare.CEL");
            pFlarexpCel = LoadFileInMem("Monsters\\Succ\\Flarexp.CEL");
        }
    }
}

// ClearMVars	00000000004019B0
void ClearMVars(int i)
{
    monster[i]._mVar1 = 0;
    monster[i]._mVar2 = 0;
    monster[i]._mVar3 = 0;
    monster[i]._mVar4 = 0;
    monster[i]._mVar5 = 0;
    monster[i]._mVar6 = 0;
    monster[i]._mVar7 = 0;
    monster[i]._mVar8 = 0;
}

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

// .text:004035FB
void DeleteMonster(int i)
{
    int temp;

    nummonsters--;
    temp = monstactive[nummonsters];
    monstactive[nummonsters] = monstactive[i];
    monstactive[i] = temp;
}

// AddMonster	000000000040364A
// NewMonsterAnim	000000000040373E
// M_Enemy	000000000040380E

// M_CheckEFlag	0000000000403973
void M_CheckEFlag(int i)
{
    int j;
    int f;
    WORD *m;
    int x;
    int y;

    x = monster[i]._mx - 1;
    y = monster[i]._my + 1;
    f = 0;
    m = dpiece_defs_map_2[x][y].mt;
    // j < 10 beacuse there are 10 microtiles per tile in dungeon?
    // j = 2 to ignore the first two floor microtiles?
    for (j = 2; j < 10; j++)
    {
        f |= m[j];
    }

    if (f | dSpecial[x][y])
        monster[i]._meflag = TRUE;
    else
    {
        monster[i]._meflag = FALSE;
    }
}

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
void M_StartKill(int i, int pnum)
{
    // TODO
}

// M_StartFadein	0000000000405179
// M_StartFadeout	000000000040528E
// M_StartHeal	00000000004053D3
// M_ChangeLightOffset	0000000000405468
// M_DoWalk	0000000000405565
BOOL M_DoWalk(int i)
{
    // TODO
    return FALSE;
}
// M_DoWalk2	0000000000405821
BOOL M_DoWalk2(int i)
{
    // TODO
    return FALSE;
}
// M_DoWalk3	0000000000405A4A
BOOL M_DoWalk3(int i)
{
    // TODO
    return FALSE;
}
// M_TryH2HHit	0000000000405D7F
// M_DoAttack	000000000040641F
BOOL M_DoAttack(int i)
{
    // TODO
    return FALSE;
}
// M_DoRAttack	00000000004066ED
BOOL M_DoRAttack(int i)
{
    // TODO
    return FALSE;
}
// M_DoRSpAttack	0000000000406859
BOOL M_DoRSpAttack(int i)
{
    // TODO
    return FALSE;
}
// M_DoSAttack	00000000004069CB
BOOL M_DoSAttack(int i)
{
    // TODO
    return FALSE;
}
// M_DoFadein	0000000000406ADF
BOOL M_DoFadein(int i)
{
    // TODO
    return FALSE;
}
// M_DoFadeout	0000000000406B53
BOOL M_DoFadeout(int i)
{
    // TODO
    return FALSE;
}
// M_DoHeal	0000000000406BC9
BOOL M_DoHeal(int i)
{
    // TODO
    return FALSE;
}
// M_Teleport	0000000000406C72
// M_DoGotHit	0000000000406E63
BOOL M_DoGotHit(int i)
{
    // TODO
    return FALSE;
}
// M_DoDeath	0000000000406EDD
BOOL M_DoDeath(int i)
{
    // TODO
    return FALSE;
}
// M_DoSpStand	0000000000407181
BOOL M_DoSpStand(int i)
{
    // TODO
    return FALSE;
}
// M_DoDelay	0000000000407237
BOOL M_DoDelay(int i)
{
    // TODO
    return FALSE;
}

// .text:0040736F
BOOL M_DoStone(int i)
{
    // Don't update the monster, which makes them act like they're frozen.
    //
    // Remove the enemy if they run out of health. This is in contrast to other
    // ways of death which create a dead body.
    //
    // If this was caused by casting stone curse then MIS_STONE will handle
    // creating the shatter effect.
    if (monster[i]._mhitpoints == 0)
    {
        dMonster[monster[i]._mx][monster[i]._my] = 0;
        monster[i]._mDelFlag = TRUE;
    }
    return FALSE;
}

// M_WalkDir	00000000004073FB
// GroupUnity	00000000004075E4
void GroupUnity(int i)
{
    // TODO
}
// M_CallWalk	0000000000407A10
// M_DumbWalk	0000000000407C1A
// M_RoundWalk	0000000000407C59
// M_Face	0000000000407D84    dead code

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
void MAI_Round(int i, BOOL special)
{
    // TODO
}

// MAI_GoatMc	0000000000409809
void MAI_GoatMc(int i)
{
    MAI_Round(i, TRUE);
}

// MAI_Ranged	000000000040982C
static void MAI_Ranged(int i, int missile_type)
{
    // tODO
}

// .text:00409A55
void MAI_GoatBow(int i)
{
    MAI_Ranged(i, MIS_ARROW);
}

// .text:00409A75
void MAI_Succ(int i)
{
    MAI_Ranged(i, MIS_FLARE);
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
void MAI_RoundRanged(int i, int missile_type)
{
    // TODO
}

// .text:0040A5B6
void MAI_Magma(int i)
{
    MAI_RoundRanged(i, MIS_MAGMABALL);
}

// .text:0040A5D9
void MAI_Storm(int i)
{
    MAI_RoundRanged(i, MIS_LIGHTCTRL2);
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
void ProcessMonsters()
{
    BOOL raflag;
    int i;
    int mx;
    int my;
    int mi;

    for (int i = 0; i < nummonsters; i++)
    {
        mi = monstactive[i];
        raflag = 0;

        // Regenerate monster health based on its level
        // As much as this decomp looks weird I think it's true to the assembly:
        //   mov     eax, monster._mhitpoints[eax*4]
        //   and     eax, 0FFFFFFC0h
        //   xor     ecx, ecx
        //   and     ecx, 0FFFFFFC0h
        //   cmp     eax, ecx
        //   jle     loc_40B144
        if (monster[mi]._mmaxhp > monster[mi]._mhitpoints && (monster[mi]._mhitpoints & 0xFFFFFFC0) > (0 & 0xFFFFFFC0))
        {
            monster[mi]._mhitpoints += monster[mi].mLevel;
        }

        mx = monster[mi]._mx;
        my = monster[mi]._my;

        // Devilution checks BFLAG_VISIBLE instead...
        if (dFlags[mx][my] & BFLAG_LIT && monster[mi]._msquelch == 0 && monster[mi].MType->mtype == MT_CLEAVER)
        {
            PlaySFX();
        }

        if (dFlags[mx][my] & BFLAG_LIT)
        {
            monster[mi]._msquelch = UCHAR_MAX;
            monster[mi]._lastx = plr[monster[mi]._menemy]._pfutx;
            monster[mi]._lasty = plr[monster[mi]._menemy]._pfuty;
        }
        else if (monster[mi]._msquelch != 0)
        {
            monster[mi]._msquelch--;
        }

        do
        {
            AiProc[monster[mi]._mAi](mi);
            switch (monster[mi]._mmode)
            {
            case MM_STAND:
                monster[mi]._mVar2++;
                raflag = FALSE;
                break;
            case MM_WALK:
                raflag = M_DoWalk(mi);
                break;
            case MM_WALK2:
                raflag = M_DoWalk2(mi);
                break;
            case MM_WALK3:
                raflag = M_DoWalk3(mi);
                break;
            case MM_ATTACK:
                raflag = M_DoAttack(mi);
                break;
            case MM_RATTACK:
                raflag = M_DoRAttack(mi);
                break;
            case MM_GOTHIT:
                raflag = M_DoGotHit(mi);
                break;
            case MM_DEATH:
                raflag = M_DoDeath(mi);
                break;
            case MM_SATTACK:
                raflag = M_DoSAttack(mi);
                break;
            case MM_FADEIN:
                raflag = M_DoFadein(mi);
                break;
            case MM_FADEOUT:
                raflag = M_DoFadeout(mi);
                break;
            case MM_SPSTAND:
                raflag = M_DoSpStand(mi);
                break;
            case MM_RSPATTACK:
                raflag = M_DoRSpAttack(mi);
                break;
            case MM_DELAY:
                raflag = M_DoDelay(mi);
                break;
            case MM_CHARGE:
                raflag = FALSE;
                break;
            case MM_STONE:
                raflag = M_DoStone(mi);
                break;
            case MM_HEAL:
                raflag = M_DoHeal(mi);
                break;
            }

            if (raflag)
            {
                GroupUnity(mi);
            }
        } while (raflag);

        if (monster[mi]._mmode != MM_STONE)
        {
            monster[mi]._mAnimCnt++;
            if (monster[mi]._mAnimDelay <= monster[mi]._mAnimCnt)
            {
                monster[mi]._mAnimCnt = 0;
                if (monster[mi]._mFlags & MFLAG_LOCK_ANIMATION)
                {
                    monster[mi]._mAnimFrame--;
                    if (monster[mi]._mAnimFrame == 0)
                    {
                        monster[mi]._mAnimFrame = monster[mi]._mAnimLen;
                    }
                }
                else if (!(monster[mi]._mFlags & MFLAG_ALLOW_SPECIAL))
                {
                    monster[mi]._mAnimFrame++;
                    if (monster[mi]._mAnimLen < monster[mi]._mAnimFrame)
                    {
                        monster[mi]._mAnimFrame = 1;
                    }
                }
            }
        }
    }

    i = 0;
    while (i < nummonsters)
    {
        mi = monstactive[i];
        if (monster->_mDelFlag)
        {
            DeleteMonster(i);
            i = 0;
        }
        else
        {
            i++;
        }
    }
}

// FreeMonsters	000000000040B67A
void FreeMonsters()
{
    // TODO
}

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
void SpawnSkeleton(int x, int y)
{
    // TODO
}
//