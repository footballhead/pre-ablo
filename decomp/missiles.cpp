#include "missiles.h"

#include "effects.h"
#include "engine.h"
#include "enums.h"
#include "gendung.h"

#include <stdio.h>

//
// Initialized variables (.text:004AE0C0)
//

char missiledata_mlSFX[] = {
    -1,
    IS_FBOLT1, // MIS_FIREBOLT
    -1,
    -1,
    -1,
    IS_WALLLOOP, // MIS_FIREWALL
    -1,
    -1,
    IS_LNING1, // MIS_LIGHTNING
    -1,
    IS_SENTINEL, // MIS_TOWN
    IS_NOVA,     // MIS_FLASH
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    IS_ELEMENTL, // MIS_TELEPORT
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
};
char missiledata_miSFX[] = {
    -1,
    IS_FIRIMP2, // MIS_FIREBOLT
    -1,
    -1,
    -1,
    IS_FIRIMP2, // MIS_FIREWALL
    -1,
    -1,
    IS_ELECIMP1, // MIS_LIGHTNING
    -1,
    IS_ELEMENTL, // MIS_TOWN (wtf?)
    IS_ELECIMP1, // MIS_FLASH
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
};

//
// Unitialized variables (.text:005F6F48)
//

BYTE *pFirearrowCels[16];
BYTE *pSentupCel;
BYTE *pDoomexpCel;
BYTE *pMagballCels[8];
BYTE *pFirewal2Cel;
BOOL MissilePreFlag;
BYTE *pBlodBursCel;
BYTE *pMindMaceCel; // Loaded but never unused :(
BYTE *pBone3Cel;
BYTE *pManashieldCel;
BYTE *pBlood4Cel;
BYTE *pBloodCelBkup;
BYTE *pMtlHitCelBkup;
int missileavail[MAXMISSILES];
BYTE *pLightningCel_0; // Loaded twice :/
BYTE *pMetalHit1Cel;
BYTE *pSentoutCel;
BYTE *pShatter1Cel;
BYTE *pArrowCel;
BYTE *pFlareCel;
BYTE *pFirewal1Cel;
BYTE *pBlodbur0Cel;
BYTE *pDoomCels[8];
BYTE *pBlood1Cel;
BYTE *pMagBlosCel;
int nummissiles;
BYTE *pMetalHit2Cel;
BYTE *pFirebaCels[16];
BYTE *pPortalUCel;
MissileStruct missile[MAXMISSILES];
BYTE *pSentfrCel;
BYTE *pBluexfrCel;
BYTE *pBone1Cel;
BYTE *pLightningCel; // Loaded twice :/
BYTE *pBlood2Cel;
DWORD dword_5FC09C; // TODO something chain lightning related
BYTE *pThinLghningCel;
BYTE *pMetalHit3Cel;
// TODO: a bunch of chain lightning vars should go here
BYTE *pBlodBursCel_0;
BYTE *pPortalCel;
BYTE *pBluexbk;
BYTE *pBoneCelBkup;
BYTE *pBone2Cel;
BYTE *pNewexpCel;
BYTE *pBigexpCel;
BYTE *pBlood3Cel;
int missileactive[MAXMISSILES];
BYTE *pFlarexpCel;

//
// Code (.text:004357A0)
//

// GetDamageAmt	00000000004357A0
// CheckBlock	000000000043580F
// GetDirection8	00000000004358D7

// .text:0043601F
void InitMissileGFX()
{
    char strbuf[256];
    int i;

    pFirewal1Cel = LoadFileInMem("Missiles\\Firewal1.CEL");
    pFirewal2Cel = LoadFileInMem("Missiles\\Firewal2.CEL");
    pLightningCel = LoadFileInMem("Missiles\\Lghning.CEL");
    pArrowCel = LoadFileInMem("Missiles\\Arrows.CEL");
    pMindMaceCel = LoadFileInMem("Missiles\\Mindmace.CEL");
    pPortalUCel = LoadFileInMem("Missiles\\Portalu.CEL");
    pPortalCel = LoadFileInMem("Missiles\\Portal.CEL");
    pBluexfrCel = LoadFileInMem("Missiles\\Bluexfr.CEL");
    pBluexbk = LoadFileInMem("Missiles\\Bluexbk.CEL");
    pManashieldCel = LoadFileInMem("Missiles\\Manashld.CEL");
    pSentupCel = LoadFileInMem("Missiles\\Sentup.CEL");
    pSentoutCel = LoadFileInMem("Missiles\\Sentout.CEL");
    pSentfrCel = LoadFileInMem("Missiles\\Sentfr.CEL");
    pLightningCel_0 = LoadFileInMem("Missiles\\Lghning.CEL"); // Why load twice?
    pBlood1Cel = LoadFileInMem("Missiles\\Blood1.CEL");
    pBlood2Cel = LoadFileInMem("Missiles\\Blood2.CEL");
    pBlood3Cel = LoadFileInMem("Missiles\\Blood3.CEL");
    pBlood4Cel = LoadFileInMem("Missiles\\Blood4.CEL");
    pBone1Cel = LoadFileInMem("Missiles\\Bone1.CEL");
    pBone2Cel = LoadFileInMem("Missiles\\Bone2.CEL");
    pBone3Cel = LoadFileInMem("Missiles\\Bone3.CEL");
    pMetalHit1Cel = LoadFileInMem("Missiles\\Metlhit1.CEL");
    pMetalHit2Cel = LoadFileInMem("Missiles\\Metlhit2.CEL");
    pMetalHit3Cel = LoadFileInMem("Missiles\\Metlhit3.CEL");
    pShatter1Cel = LoadFileInMem("Missiles\\Shatter1.CEL");
    pBlodBursCel = LoadFileInMem("Missiles\\Blodburs.CEL");
    pBlodBursCel_0 = pBlodBursCel; // ... why?
    pBlodbur0Cel = LoadFileInMem("Missiles\\Blodbur0.CEL");
    pBigexpCel = LoadFileInMem("Missiles\\Bigexp.CEL");
    pNewexpCel = LoadFileInMem("Missiles\\Newexp.CEL");
    for (i = 0; i < 16; i++)
    {
        sprintf(strbuf, "Missiles\\Fireba%i.CEL", i + 1);
        pFirebaCels[i] = LoadFileInMem(strbuf);
    }
    pDoomexpCel = LoadFileInMem("Missiles\\Doomexp.CEL");
    for (i = 0; i < 8; i++)
    {
        sprintf(strbuf, "Missiles\\Doom%i", i + 1);
        pDoomCels[i] = LoadFileInMem(strbuf);
    }
    for (i = 0; i < 16; i++)
    {
        sprintf(strbuf, "Missiles\\Farrow%i.CEL", i + 1);
        pFirearrowCels[i] = LoadFileInMem(strbuf);
    }
    pMagBlosCel = LoadFileInMem("Monsters\\Magma\\MagBlos.CEL");
}

// InitMissiles	0000000000436321

// DeleteMissile	00000000004365CB
void DeleteMissile(int mi, int i)
{
    // TODO
}

// GetMissileVel	0000000000436630
// PutMissile	000000000043671F
// AddBoom	0000000000436819
void AddBoom(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddStone	0000000000436999
void AddStone(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// __dc_AddMissileNo32	0000000000436C3D
// __dc_AddInvisible	0000000000436CA4
// __dc_AddGolem	0000000000436D56
// __dc_AddEther	0000000000436E08
// AddFirebolt	0000000000436EBA
void AddFirebolt(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddLightctrl	0000000000437079
void AddLightctrl(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddLightning	00000000004371BD
void AddLightning(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddFirewall	00000000004373F1
void AddFirewall(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddTown	00000000004375E3
void AddTown(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddFlash	00000000004378DD
void AddFlash(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddFlash2	0000000000437A80
void AddFlash2(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddRndTeleport	0000000000437C23
void AddRndTeleport(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddManashield	0000000000437CD1
void AddManashield(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddFiremove	0000000000437DF4
void AddFiremove(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddFireball	0000000000437F3A
void AddFireball(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddFirewall_0	0000000000438184
void AddFirewall_0(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// add_chain_related	00000000004382CB
void add_chain_related(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddChainLightning	000000000043867E
void AddChainLightning(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddGuardian	00000000004389BB
void AddGuardian(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddSerpent	0000000000438CBC
void AddSerpent(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddNova	00000000004390C4
void AddNova(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddBlood	00000000004391D8
void AddBlood(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddBone	00000000004393FA
void AddBone(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddMetal	00000000004395BA
void AddMetal(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddBlodBoil	0000000000439785
void AddBlodBoil(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddTeleport	000000000043993B
void AddTeleport(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddWarpMissile	0000000000439A25
// AddMisexp	0000000000439A9C
void AddMisexp(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddThinLightctrl	0000000000439C81
void AddThinLightctrl(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddThinLightning	0000000000439CD7
void AddThinLightning(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// __dc_missiles_direction_439D2D	0000000000439D2D

// AddArrow	0000000000439F1E
void AddArrow(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}

// AddLArrow	000000000043A0E6
void AddLArrow(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}

// __dc_missiles_sentout_43A2DF	000000000043A2DF
// AddMagmaball	000000000043A477
void AddMagmaball(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddRhino	000000000043A63A
void AddRhino(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddFlare	000000000043A845
void AddFlare(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}
// AddFlareMisexp	000000000043A993
void AddFlareMisexp(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}

// .text:0043AB65
int AddMissile(int sx, int sy, int dx, int dy, int midir, int mitype, char micaster, int id, int midam)
{
    int mi;
    if (nummissiles < MAXMISSILES)
    {
        mi = missileavail[0];
        missileavail[0] = missileavail[MAXMISSILES - nummissiles - 1];
        missileactive[nummissiles] = mi;
        nummissiles++;

        missile[mi]._mitype = mitype;
        missile[mi]._micaster = micaster;
        missile[mi]._misource = id;
        missile[mi]._miAnimDelay = 1;
        missile[mi]._miAnimCnt = 0;
        missile[mi]._mix = sx;
        missile[mi]._miy = sy;
        missile[mi]._misx = sx;
        missile[mi]._misy = sy;
        missile[mi]._miDelFlag = FALSE;
        missile[mi]._midist = 0;
        missile[mi]._midam = midam;
        missile[mi]._mixoff = 0;
        missile[mi]._miyoff = 0;
        missile[mi]._mitxoff = 0;
        missile[mi]._mityoff = 0;
        missile[mi]._mimfnum = midir;
        missile[mi]._miLightFlag = FALSE;
        missile[mi]._miPreFlag = FALSE;
        missile[mi]._miUniqTrans = 0;
        if (missiledata_mlSFX[mitype] != -1)
        {
            PlayRndSFX(missiledata_mlSFX[mitype]);
        }

        switch (mitype)
        {
        case MIS_FARROW:
            AddLArrow(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_ARROW:
            AddArrow(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_RNDTELEPORT:
            AddRndTeleport(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_FIREBOLT:
            AddFirebolt(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_MAGMABALL:
            AddMagmaball(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_TELEPORT:
            AddTeleport(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_LIGHTBALL:
            AddNova(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_FIREWALL:
            AddFirewall(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_FIREBALL:
            AddFireball(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_LIGHTCTRL:
            AddLightctrl(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_LIGHTCTRL2:
            AddThinLightctrl(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_LIGHTNING:
            AddLightning(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_LIGHTNING2:
            AddThinLightning(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_MISEXP:
            AddMisexp(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_TOWN:
            AddTown(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_FLASH:
            AddFlash(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_FLASH2:
            AddFlash2(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_MANASHIELD:
            AddManashield(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_FIREMOVE:
            AddFiremove(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_GUARDIAN:
        case MIS_SENTINAL:
            AddGuardian(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_CHAIN:
            add_chain_related(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_CHAIN2:
            AddChainLightning(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_BLOOD:
            AddBlood(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_BONE:
            AddBone(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_METLHIT:
            AddMetal(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_RHINO:
            AddRhino(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_FLARE:
            AddFlare(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_MISEXP2:
            AddFlareMisexp(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_DOOMSERP:
            AddSerpent(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_FIREWALLA:
            AddFirewall_0(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_STONE:
        case MIS_32:
        case MIS_INVISIBL:
        case MIS_GOLEM:
        case MIS_ETHEREALIZE:
            AddStone(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_BLODBUR:
            AddBlodBoil(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        case MIS_BOOM:
            AddBoom(mi, sx, sy, dx, dy, midir, micaster, id, midam);
            break;
        }

        nummissiles++;
    }

    return mi; // if nummissiles >= MAXMISSILES then this is uninitialized!
}

// GetMissilePos	000000000043B3C3
// MoveMissilePos	000000000043B52E
// MonsterTrapHit	000000000043B6CA
// MonsterMHit	000000000043BA6D
// PlayerMHit	000000000043C164
// CheckMissileCol	000000000043C958
// mi_chain_related	000000000043CF1E

// MI_Boom	000000000043D133
void MI_Boom(int mi)
{
    // TODO
}

// MI_Stone	000000000043D213
void MI_Stone(int mi)
{
    // TODO
}

// MI_Firebolt	000000000043D409
void MI_Firebolt(int mi)
{
    // TODO
}

// MI_Lightctrl	000000000043D7B0
void MI_Lightctrl(int mi)
{
    // TODO
}

// MI_Lightning	000000000043DB97
void MI_Lightning(int mi)
{
    // TODO
}

// MI_Town	000000000043DCCE
void MI_Town(int mi)
{
    // TODO
}

// MI_Flash	000000000043E0B0
void MI_Flash(int mi)
{
    // TODO
}

// MI_Flash2	000000000043E2CF
void MI_Flash2(int mi)
{
    // TODO
}

// MI_Teleport	000000000043E40B
void MI_Teleport(int mi)
{
    // TODO
}

// MI_Manashield	000000000043E779
void MI_Manashield(int mi)
{
    // TODO
}

// MI_Firemove	000000000043EC06
void MI_Firemove(int mi)
{
    // TODO
}

// MI_Fireball	000000000043EF42
void MI_Fireball(int mi)
{
    // TODO
}

// mis_chain_spoke	000000000043F3A7
void mis_chain_spoke(int mi)
{
    // TODO
}

// MI_Chain	000000000043F4D4
void MI_Chain(int mi)
{
    // TODO
}

// Sentfire	000000000043FA04

// MI_Guardian	000000000043FBE4
void MI_Guardian(int mi)
{
    // TODO
}

// MI_Serpent	0000000000440064
void MI_Serpent(int mi)
{
    // TODO
}

// MI_Lightball	00000000004407E7
void MI_Lightball(int mi)
{
    // TODO
}

// MI_Blood	0000000000440906
void MI_Blood(int mi)
{
    // TODO
}

// MI_Misexp	00000000004409A6
void MI_Misexp(int mi)
{
    // TODO
}

// MI_Firewall	0000000000440B28
void MI_Firewall(int mi)
{
    // TODO
}

// MI_Flare	0000000000440EE3
void MI_Flare(int mi)
{
    // TODO
}

// MI_LArrow	0000000000441261
void MI_LArrow(int mi)
{
    // TODO
}

// MI_Arrow	0000000000441515
void MI_Arrow(int mi)
{
    // TODO
}

// __dc_mis_serpent_firebolt	00000000004416F7
// __dc_sub_441F56	0000000000441F56

// MI_Rhino	0000000000442473
void MI_Rhino(int mi)
{
    // TODO
}

// .text:004426AE
void ProcessMissiles()
{
    int ManashieldFlag;
    int i;
    int mi;

    // Update dFlags and dMissile based on current missile location
    for (i = 0; i < nummissiles; i++)
    {
        mi = missileactive[i];
        dFlags[missile[mi]._mix][missile[mi]._miy] = dFlags[missile[mi]._mix][missile[mi]._miy] & ~BFLAG_MISSILE;
        dMissile[missile[mi]._mix][missile[mi]._miy] = 0;
    }

    MissilePreFlag = FALSE;
    ManashieldFlag = FALSE;

    for (i = 0; i < nummissiles; i++)
    {
        mi = missileactive[i];
        switch (missile[mi]._mitype)
        {
        case MIS_FARROW:
            MI_LArrow(mi);
            break;
        case MIS_ARROW:
            MI_Arrow(mi);
            break;
        case MIS_FIREBOLT:
        case MIS_MAGMABALL:
            MI_Firebolt(mi);
            break;
        case MIS_LIGHTBALL:
            MI_Lightball(mi);
            break;
        case MIS_FIREWALL:
        case MIS_FIREWALLA:
            MI_Firewall(mi);
            break;
        case MIS_FIREBALL:
            MI_Fireball(mi);
            break;
        case MIS_LIGHTCTRL:
        case MIS_LIGHTCTRL2:
            MI_Lightctrl(mi);
            break;
        case MIS_LIGHTNING:
        case MIS_LIGHTNING2:
            MI_Lightning(mi);
            break;
        case MIS_MISEXP:
            MI_Misexp(mi);
            break;
        case MIS_TOWN:
            MI_Town(mi);
            break;
        case MIS_FLASH:
            MI_Flash(mi);
            break;
        case MIS_FLASH2:
            MI_Flash2(mi);
            break;
        case MIS_MANASHIELD:
            ManashieldFlag = TRUE;
            break;
        case MIS_FIREMOVE:
            MI_Firemove(mi);
            break;
        case MIS_GUARDIAN:
        case MIS_SENTINAL:
            MI_Guardian(mi);
            break;
        case MIS_CHAIN:
            mis_chain_spoke(mi);
            break;
        case MIS_CHAIN2:
            MI_Chain(mi);
            break;
        case MIS_BLOOD:
        case MIS_BONE:
        case MIS_METLHIT:
        case MIS_BLODBUR:
            MI_Blood(mi);
            break;
        case MIS_RHINO:
            MI_Rhino(mi);
            break;
        case MIS_FLARE:
            MI_Flare(mi);
            break;
        case MIS_MISEXP2:
            MI_Misexp(mi);
            break;
        case MIS_RNDTELEPORT:
        case MIS_TELEPORT:
            MI_Teleport(mi);
            break;
        case MIS_DOOMSERP:
            MI_Serpent(mi);
            break;
        case MIS_STONE:
            MI_Stone(mi);
            break;
        case MIS_BOOM:
            MI_Boom(mi);
            break;
            // case MIS_32:
            // case MIS_INVISIBL:
            // case MIS_GOLEM:
            // case MIS_ETHEREALIZE:
            //     // these cases are missing which makes them act like a broken stone curse
            //     // possibly related to #110
            //     break;
        }

        if (missile[mi]._miAnimFlags)
        {
            missile[mi]._miAnimCnt++;
            if (missile[mi]._miAnimDelay <= missile[mi]._miAnimCnt)
            {
                missile[mi]._miAnimCnt = 0;
                // Was wondering why miAnimFrame < 1 but miAnimAdd could be negative
                // So this code can play animations forwards and backwards
                missile[mi]._miAnimFrame += missile[mi]._miAnimAdd;
                if (missile[mi]._miAnimFrame > missile[mi]._miAnimLen)
                {
                    missile[mi]._miAnimFrame = 1;
                }
                if (missile[mi]._miAnimFrame < 1)
                {
                    missile[mi]._miAnimFrame = missile[mi]._miAnimLen;
                }
            }
        }
    }

    if (ManashieldFlag > 0)
    {
        for (i = 0; i < nummissiles; i++)
        {
            mi = missileactive[i];
            if (missile[mi]._mitype == MIS_MANASHIELD)
            {
                MI_Manashield(mi);
            }
        }
    }

    i = 0;
    while (i < nummissiles)
    {
        mi = missileactive[i];
        if (missile[mi]._miDelFlag)
        {
            DeleteMissile(mi, i);
        }
        else
        {
            i++;
        }
    }
}

// .text:00442B60
void FreeMissileGFX()
{
    int i;

    MemFreeDbg(pLightningCel);
    MemFreeDbg(pArrowCel);
    MemFreeDbg(pFirewal2Cel);
    MemFreeDbg(pFirewal1Cel);
    MemFreeDbg(pPortalUCel);
    MemFreeDbg(pPortalCel);
    MemFreeDbg(pBluexfrCel);
    MemFreeDbg(pBluexbk);
    MemFreeDbg(pManashieldCel);
    MemFreeDbg(pSentupCel);
    MemFreeDbg(pSentoutCel);
    MemFreeDbg(pSentfrCel);
    MemFreeDbg(pLightningCel_0);
    MemFreeDbg(pBlood1Cel);
    MemFreeDbg(pBlood2Cel);
    MemFreeDbg(pBlood3Cel);
    MemFreeDbg(pBlood4Cel);
    MemFreeDbg(pBone1Cel);
    MemFreeDbg(pBone2Cel);
    MemFreeDbg(pBone3Cel);
    MemFreeDbg(pMetalHit1Cel);
    MemFreeDbg(pMetalHit2Cel);
    MemFreeDbg(pMetalHit3Cel);
    MemFreeDbg(pShatter1Cel);
    MemFreeDbg(pBlodBursCel);
    MemFreeDbg(pBlodbur0Cel);
    MemFreeDbg(pBigexpCel);
    MemFreeDbg(pNewexpCel);

    for (i = 0; i < 16; i++)
    {
        MemFreeDbg(pFirebaCels[i]);
    }
    for (i = 0; i < 16; i++)
    {
        MemFreeDbg(pFirearrowCels[i]);
    }

    MemFreeDbg(pDoomexpCel);
    for (i = 0; i < 8; i++)
    {
        MemFreeDbg(pDoomCels[i]);
    }
}

// .text:004430AB
void SetMissileGFX(int mi)
{
    int mfnum;

    mfnum = missile[mi]._mimfnum;
    switch (missile[mi]._mitype) {
    case MIS_MANASHIELD:
        missile[mi]._miAnimData = pManashieldCel;
        break;
    case MIS_ARROW:
        missile[mi]._miAnimData = pArrowCel;
        break;
    case MIS_FARROW:
        missile[mi]._miAnimData = pFirearrowCels[mfnum];
        break;
    case MIS_FIREBOLT:
        missile[mi]._miAnimData = pFirebaCels[mfnum];
        break;
    case MIS_MAGMABALL:
        missile[mi]._miAnimData = pMagballCels[mfnum];
        break;
    case MIS_LIGHTBALL:
        missile[mi]._miAnimData = pLightningCel;
        break;
    case MIS_FIREWALL:
    case MIS_FIREWALLA:
        missile[mi]._miAnimData = pFirewal1Cel;
        break;
    case MIS_FIREBALL:
        missile[mi]._miAnimData = pFirebaCels[mfnum];
        break;
    case MIS_LIGHTCTRL:
    case MIS_LIGHTNING:
        missile[mi]._miAnimData = pLightningCel;
        break;
    case MIS_LIGHTCTRL2:
    case MIS_LIGHTNING2:
        missile[mi]._miAnimData = pThinLghningCel;
        break;
    case MIS_MISEXP:
        missile[mi]._miAnimData = pLightningCel;
        break;
    case MIS_TOWN:
        missile[mi]._miAnimData = pPortalUCel;
        break;
    case MIS_FLASH:
        missile[mi]._miAnimData = pBluexfrCel;
        break;
    case MIS_FLASH2:
        missile[mi]._miAnimData = pBluexbk;
        break;
    case MIS_FIREMOVE:
        missile[mi]._miAnimData = pFirewal1Cel;
        break;
    case MIS_GUARDIAN:
    case MIS_SENTINAL:
        missile[mi]._miAnimData = pSentoutCel;
        break;
    case MIS_CHAIN:
    case MIS_CHAIN2:
        missile[mi]._miAnimData = pLightningCel_0;
        break;
    case MIS_BLOOD:
        missile[mi]._miAnimData = pBloodCelBkup;
        break;
    case MIS_BONE:
        missile[mi]._miAnimData = pBoneCelBkup;
        break;
    case MIS_METLHIT:
        missile[mi]._miAnimData = pMtlHitCelBkup;
        break;
    case MIS_FLARE:
        missile[mi]._miAnimData = pFlareCel;
        break;
    case MIS_MISEXP2:
        missile[mi]._miAnimData = pFlarexpCel;
        break;
    case MIS_RNDTELEPORT:
    case MIS_TELEPORT:
        // This space intentionally left empty
        break;
    case MIS_DOOMSERP:
        missile[mi]._miAnimData = pDoomCels[mfnum];
        break;
    case MIS_STONE:
    case MIS_32:
    case MIS_INVISIBL:
    case MIS_GOLEM:
    case MIS_ETHEREALIZE:
        missile[mi]._miAnimData = pShatter1Cel;
        break;
    case MIS_BOOM:
        missile[mi]._miAnimData = pNewexpCel;
        break;
    }

    // MIS_BOOM and MIS_RHINO not in above switch/case
}