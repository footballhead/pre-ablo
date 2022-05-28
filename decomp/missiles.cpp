#include "missiles.h"

#include "dead.h"
#include "effects.h"
#include "engine.h"
#include "enums.h"
#include "gendung.h"
#include "monster.h"
#include "player.h"

#include <stdio.h>

int AddLight(int x, int y, int r);
// CrawlTable is weird because it is structured data being represented in a flat array.
// See Devilution for one explanation.
// I like to think of them as tree rings or a bullseye: concentric rings starting from inside and going outward.
// Each ring is defined by a number of (x,y) offsets, followed by a list of (x,y) offsets
// These offsets are relative to a location and together will form a ring around that location.
// Since larger rings have more offsets, each ring is a different size in the array.
// Typically this will be indexed by something like CrawlNum, which will specify the offset for each ring.
// I'm guessing that this is a pre-computed alternative to euclidean distance up to a certain integer distance
extern char CrawlTable[2749];

//
// Initialized variables (.text:004AE070)
//

// TODO: What does mType represent? Looks like an enum
char missiledata_mType[] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 1, 1, 1, 1, 2, 1, 0, 1, 2, 1, 1, 1, 1, 1,
    2, 2};
char missiledata_mResist[] = {
    MISR_NONE,
    MISR_FIRE,
    MISR_NONE,
    MISR_NONE,
    MISR_LIGHTNING,
    MISR_FIRE,
    MISR_FIRE,
    MISR_LIGHTNING,
    MISR_LIGHTNING,
    MISR_NONE,
    MISR_MAGIC, // MIS_TOWN
    MISR_MAGIC, // MIS_FLASH
    MISR_MAGIC, // MIS_FLASH2
    MISR_MAGIC, // MIS_MANASHIELD
    MISR_FIRE,
    MISR_MAGIC, // MIS_SENTINAL
    MISR_LIGHTNING,
    MISR_LIGHTNING,
    MISR_NONE,
    MISR_NONE,
    MISR_NONE,
    MISR_NONE,
    MISR_FIRE,
    MISR_LIGHTNING,
    MISR_LIGHTNING,
    MISR_MAGIC, // MIS_FLARE
    MISR_MAGIC, // MIS_MISEXP2
    MISR_NONE,
    MISR_FIRE,
    MISR_MAGIC, // MIS_DOOMSERP
    MISR_FIRE,
    MISR_MAGIC, // MIS_STONE
    MISR_NONE,
    MISR_NONE,
    MISR_NONE,
    MISR_NONE,
    MISR_NONE,
    MISR_NONE,
};
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
BYTE *pBlodBursCel; // In demo MPQ, bloodburs is the same as blood2
BYTE *pMindMaceCel; // Loaded but never unused :( Also never freed?
BYTE *pBone3Cel;
BYTE *pManashieldCel;
BYTE *pBlood4Cel; // Loaded but never unused... in demo MPQ, blood4 is the same as blood1
BYTE *pBloodCelBkup;
BYTE *pMtlHitCelBkup;
int missileavail[MAXMISSILES];
BYTE *pLightningCel_0; // Loaded twice :/
BYTE *pMetalHit1Cel;
BYTE *pSentoutCel;
BYTE *pShatter1Cel;
BYTE *pArrowCel;
BYTE *pFlareCel; // Never loaded?
BYTE *pFirewal1Cel;
BYTE *pBlodbur0Cel; // in demo MPQ, bloodburs0 is the same as blood3
BYTE *pDoomCels[8];
BYTE *pBlood1Cel;
BYTE *pMagBlosCel; // Loaded but never freed
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
DWORD dword_5FC09C;    // TODO something chain lightning related
BYTE *pThinLghningCel; // Loaded but never freed
BYTE *pMetalHit3Cel;
// TODO: a bunch of chain lightning vars should go here
BYTE *pBlodBursCel_0;
BYTE *pPortalCel;
BYTE *pBluexbk;
BYTE *pBoneCelBkup;
BYTE *pBone2Cel;
BYTE *pNewexpCel; // Apocalypse
BYTE *pBigexpCel; // Fireball
BYTE *pBlood3Cel;
int missileactive[MAXMISSILES];
BYTE *pFlarexpCel; // Loaded, never freed

//
// Code (.text:004357A0)
//

// GetDamageAmt	00000000004357A0
// CheckBlock	000000000043580F

// .text:004358D7
int GetDirection16(int x1, int y1, int x2, int y2)
{
    BYTE lrtoul[5] = {14, 15, 0, 1, 2};
    BYTE ultolr[5] = {6, 5, 4, 3, 2};
    int md;
    BYTE lltour[5] = {14, 13, 12, 11, 10};
    BYTE urtoll[5] = {6, 7, 8, 9, 10};
    int mx;
    int my;
    BYTE Dirs[16][16] = {
        {99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 3, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {4, 3, 3, 2, 2, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {4, 4, 3, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {4, 4, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {4, 4, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1},
        {4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1},
        {4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1},
        {4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1},
        {4, 4, 4, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 1, 1},
        {4, 4, 4, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 1},
        {4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2},
        {4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2},
        {4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2},
        {4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2}};

    mx = abs(x2 - x1);
    if (mx > 15)
    {
        mx = 15;
    }

    my = abs(y2 - y1);
    if (my > 15)
    {
        my = 15;
    }

    md = Dirs[my][mx];
    if (x2 < x1)
    {
        if (y2 < y1)
        {
            md = urtoll[md];
        }
        else
        {
            md = ultolr[md];
        }
    }
    else if (y1 > y2)
    {
        md = lltour[md];
    }
    else
    {
        md = lrtoul[md];
    }

    return md;
}

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

// .text:004365CB
void DeleteMissile(int mi, int i)
{
    missileavail[MAXMISSILES - nummissiles] = mi;
    nummissiles--;
    if (nummissiles > 0 && i != nummissiles)
    {
        missileactive[i] = missileactive[nummissiles];
    }
}

// GetMissileVel	0000000000436630
void GetMissileVel(int i, int sx, int sy, int dx, int dy, int v)
{
    // TODO
}

// .text:0043671F
void PutMissile(int i)
{
    int x, y;

    if (!missile[i]._miDelFlag)
    {
        x = missile[i]._mix;
        y = missile[i]._miy;

        dFlags[x][y] |= BFLAG_MISSILE;
        if (dMissile[x][y] == 0)
        {
            dMissile[x][y] = i + 1;
        }
        else
        {
            dMissile[x][y] = -1;
        }
        if (missile[i]._miPreFlag)
        {
            MissilePreFlag = TRUE;
        }
    }
}

// .text:00436819
void AddBoom(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int dam)
{
    missile[mi]._mix = dx;
    missile[mi]._miy = dy;
    missile[mi]._misx = dx;
    missile[mi]._misy = dy;
    missile[mi]._mixvel = 0;
    missile[mi]._miyvel = 0;
    missile[mi]._midam = dam;
    missile[mi]._miAnimData = pNewexpCel;
    missile[mi]._miAnimDelay = 1;
    missile[mi]._miAnimLen = 15;
    missile[mi]._miAnimFrame = 1;
    missile[mi]._miAnimFlags = TRUE;
    missile[mi]._miAnimWidth = 96;
    missile[mi]._miAnimWidth2 = 16;
    missile[mi]._miAnimAdd = 1;
    missile[mi]._mirange = missile[mi]._miAnimLen;
    missile[mi]._mirnd = 0;
    missile[mi]._miVar1 = 0;
}

// .text:00436999
void AddStone(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    int CrawlNum[6] = {0, 3, 12, 45, 94, 159}; // offsets into crawl table for rings 0 through 5 (up to 5 tiles away)
    int j, l, i, mid, k;

    missile[mi]._misource = id;
    missile[mi]._miAnimFlags = 0;
    missile[mi]._miAnimData = NULL;

    // Start seraching from a monster up to 5 rings away
    for (i = 0; i < 6; i++)
    {
        k = CrawlNum[i]; // k is offset to beginning of ring in CrawlTable
        l = k + 1;       // l (or k+1) is the offset to the first (x,y) pair
                         // l will be the offset for X
                         // l+1 is the offset for Y
        for (j = (BYTE)CrawlTable[k]; j > 0; j--)
        {
            mid = dMonster[dx + CrawlTable[l]][dy + CrawlTable[l + 1]];
            if (mid > 0)
            {
                mid--;
            }
            else
            {
                mid = -(mid + 1);
            }

            if (mid > 0)
            {
                // Stop looking, we found one!
                // Combined with the break, this will escape both for-loops
                j = -99;
                i = 6;

                // missile var1 is backup monster mmode
                missile[mi]._miVar1 = monster[mid]._mmode;
                // missile var2 is the monster ID we've turned to stone
                missile[mi]._miVar1 = monster[mid]._mmode;

                monster[mid]._mmode = MM_STONE;
                break;
            }

            // Keep looking, advance l to next (x,y) pair
            l += 2;
        }
    }

    if (j != -99)
    {
        // No monster in 5-ring radius, delete the missile
        missile[mi]._miDelFlag = TRUE;
        DeleteMissile(mi, mi);
    }
    else
    {
        // Found a monster, keep the missile to track the effect
        missile[mi]._mix = CrawlTable[l];
        missile[mi]._miy = CrawlTable[l + 1];
        missile[mi]._misx = missile[mi]._mix;
        missile[mi]._misy = missile[mi]._miy;

        missile[mi]._mirange = plr[id]._pLevel;
        for (j = GetSpellLevel(id, SPL_STONE); j > 0; j--)
        {
            missile[mi]._mirange += ((missile[mi]._mirange << 4) >> 3);
        }
    }
}

// .text:00436C3D
// Dead code.
// This is guessed to be Add*() for MIS_32 (based on function order relative to
// AddStone). It doesn't acutally do anything though so...
void sub_436C3D(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    missile[mi]._mirange = plr[id]._pLevel << 5;
    missile[mi]._miDelFlag = TRUE;
    DeleteMissile(mi, mi);
}

// .text:00436CA4
// Dead code. Name is guessed since it uses SPL_INVISIBIL
void AddInvisible(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    int slvl;

    missile[mi]._mirange = plr[id]._pLevel << 4;
    for (slvl = GetSpellLevel(id, SPL_INVISIBIL); slvl > 0; slvl--)
    {
        missile[mi]._mirange += (missile[mi]._mirange << 4) >> 3;
    }

    missile[mi]._miDelFlag = TRUE;
    DeleteMissile(mi, mi);
}

// .text:00436D56
// Dead code. Name is guessed since it uses SPL_GOLEM
void AddGolem(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    int slvl;

    missile[mi]._mirange = plr[id]._pLevel << 5;
    for (slvl = GetSpellLevel(id, SPL_GOLEM); slvl > 0; slvl--)
    {
        missile[mi]._mirange += (missile[mi]._mirange << 4) >> 3;
    }

    missile[mi]._miDelFlag = TRUE;
    DeleteMissile(mi, mi);
}

// .text:00436E08
// Dead code. Name is guessed since it uses SPL_ETHEREALIZE
void AddEther(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    int slvl;

    missile[mi]._mirange = plr[id]._pLevel << 4;
    for (slvl = GetSpellLevel(id, SPL_ETHEREALIZE); slvl > 0; slvl--)
    {
        missile[mi]._mirange += (missile[mi]._mirange << 4) >> 3;
    }

    missile[mi]._miDelFlag = TRUE;
    DeleteMissile(mi, mi);
}

// .text:00436EBA
void AddFirebolt(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    int sp; // missile speed

    // Is the intent to do do different things for player-cast VS trap?
    if (id != -1)
    {
        sp = GetSpellLevel(id, SPL_FIREBOLT) * 2 + 16;
        if (sp >= 63)
        {
            sp = 63;
        }
    }
    else
    {
        sp = 16;
    }

    GetMissileVel(mi, sx, sy, dx, dy, sp);
    missile[mi]._mimfnum = GetDirection16(sx, sy, dx, dy);
    missile[mi]._miAnimData = pFirebaCels[missile[mi]._mimfnum];
    missile[mi]._miAnimLen = 14;
    missile[mi]._miAnimFrame = 1;
    missile[mi]._miAnimFlags = TRUE;
    missile[mi]._miAnimWidth = 96;
    missile[mi]._miAnimWidth2 = 16;
    missile[mi]._miAnimAdd = 1;
    missile[mi]._miPreFlag = FALSE;
    missile[mi]._mirange = 256;
    missile[mi]._mirnd = 0;
    missile[mi]._miVar1 = sx;
    missile[mi]._miVar2 = sy;
    missile[mi]._mlid = AddLight(sx, sy, 8);
    PutMissile(mi);
}

// .text:00437079
void AddLightctrl(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    missile[mi]._miVar1 = sx;
    missile[mi]._miVar2 = sy;
    GetMissileVel(mi, sx, sy, dx, dy, 32);
    missile[mi]._miAnimData = pLightningCel;
    missile[mi]._miAnimDelay = 0;
    missile[mi]._miAnimLen = 8;
    missile[mi]._miAnimDelay = 0;
    missile[mi]._miAnimFrame = random_(8) + 1;
    missile[mi]._miAnimFlags = TRUE;
    missile[mi]._miAnimWidth = 96;
    missile[mi]._miAnimWidth2 = 16;
    missile[mi]._miAnimAdd = 1;
    missile[mi]._miLightFlag = FALSE;
    missile[mi]._miPreFlag = FALSE;
    missile[mi]._mirange = 256;
    missile[mi]._mirnd = 0;
}

// .text:004371BD
void AddLightning(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    missile[mi]._misx = dx;
    missile[mi]._misy = dy;
    if (midir >= 0)
    {
        missile[mi]._mixoff = missile[midir]._mixoff;
        missile[mi]._miyoff = missile[midir]._miyoff;
        missile[mi]._mitxoff = missile[midir]._mitxoff;
        missile[mi]._mityoff = missile[midir]._mityoff;
    }
    missile[mi]._miAnimData = pLightningCel;
    missile[mi]._miAnimDelay = 0;
    missile[mi]._miAnimLen = 8;
    missile[mi]._miAnimDelay = 0;
    missile[mi]._miAnimFrame = random_(8) + 1;
    missile[mi]._miAnimFlags = TRUE;
    missile[mi]._miAnimWidth = 96;
    missile[mi]._miAnimWidth2 = 16;
    missile[mi]._miAnimAdd = 1;
    // TODO: what is micaster? what is 24?
    if (midir >= 0 && micaster != 24 && id != -1)
    {
        missile[mi]._mirange = GetSpellLevel(id, SPL_LIGHTNING);
    }
    else if (midir >= 0 && id != -1)
    {
        missile[mi]._mirange = 10;
    }
    else
    {
        missile[mi]._mirange = 8;
    }

    missile[mi]._mirnd = 0;
    missile[mi]._mlid = AddLight(missile[mi]._mix, missile[mi]._miy, 4);
    PutMissile(mi);
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
    int i, j, k, l, mid, dir;                  // TODO var order
    int CrawlNum[6] = {0, 3, 12, 45, 94, 159}; // offsets into crawl table for rings 0 through 5

    // Start seraching from a monster up to 5 rings away from player click location
    for (i = 0; i < 6; i++)
    {
        k = CrawlNum[i]; // k is offset to beginning of ring in CrawlTable
        l = k + 1;       // l (or k+1) is the offset to the first (x,y) pair
                         // l will be the offset for X
                         // l+1 is the offset for Y

        for (j = (BYTE)CrawlTable[k]; j > 0; j--)
        {
            mid = dMonster[dx + CrawlTable[l]][dy + CrawlTable[l + 1]];

            if (mid > 0)
            {
                mid--;
            }
            else
            {
                mid = -(mid + 1);
            }

            if ((monster[mid]._mhitpoints & 0xFFFFFFC0) > (0 & 0xFFFFFFC0))
            {
                // Found a live monster, record the location
                missile[mi]._miVar1 = dx + CrawlTable[l];
                missile[mi]._miVar2 = dy + CrawlTable[l + 1];
                missile[mi]._miVar3 = mid;

                j = -99;
                i = 6;
                break;
            }

            l += 2;
        }
    }

    if (j != -99)
    {
        // No monster in 5-ring radius, delete the missile
        missile[mi]._miDelFlag = TRUE;
        DeleteMissile(mi, mi);
    }
    else
    {
        GetMissileVel(mi, sx, sy, missile[mi]._miVar1, missile[mi]._miVar2, 16);
        dir = GetDirection(sx, sy, missile[mi]._miVar1, missile[mi]._miVar2);

        missile[mi]._mix = sx;
        missile[mi]._miy = sy;
        missile[mi]._misx = sx;
        missile[mi]._misy = sy;
        missile[mi]._misource = id;
        missile[mi]._midam = 26; // huh... is this... good?
        missile[mi]._mimfnum = dir;
        missile[mi]._miAnimData = pDoomCels[dir];
        missile[mi]._miAnimDelay = 1;
        missile[mi]._miAnimLen = 15;
        missile[mi]._miAnimFrame = 1;
        missile[mi]._miAnimFlags = 1;
        missile[mi]._miAnimWidth = 96;
        missile[mi]._miAnimWidth2 = 16;
        missile[mi]._miAnimAdd = 1;
        missile[mi]._miLightFlag = FALSE;
        missile[mi]._miPreFlag = FALSE;
        missile[mi]._midist = 0;

        missile[mi]._mirange = ((plr[id]._pLevel << 4) >> 2) + (plr[id]._pLevel << 4);
        for (j = GetSpellLevel(id, SPL_DOOMSERP); j > 0; j--)
        {
            missile[mi]._mirange += ((missile[mi]._mirange << 4) >> 3);
        }

        missile[mi]._mirnd = 0;
        PutMissile(mi);
    }
}

// AddNova	00000000004390C4
void AddNova(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    // TODO
}

// .text:004391D8
// dx is [0..3] for blood1/blood2/blood3/blood3
void AddBlood(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    missile[mi]._midam = 0;
    missile[mi]._miAnimWidth = 96;

    // weird switch param but OK
    switch (dx)
    {
    case 0:
        missile[mi]._miAnimLen = 15;
        missile[mi]._miAnimData = pBlood1Cel;
        break;
    case 1:
        missile[mi]._miAnimLen = 8;
        missile[mi]._miAnimData = pBlood2Cel;
        missile[mi]._miAnimWidth = 128;
        break;
    case 2:
        missile[mi]._miAnimLen = 8;
        missile[mi]._miAnimData = pBlood3Cel;
        missile[mi]._miAnimWidth = 128;
        break;
    case 3:
        missile[mi]._miAnimLen = 8;
        missile[mi]._miAnimData = pBlood4Cel;
        missile[mi]._miAnimWidth = 128;
        break;
    }

    pBloodCelBkup = missile[mi]._miAnimData;
    missile[mi]._miAnimFrame = 1;
    missile[mi]._miAnimFlags = TRUE;
    missile[mi]._miAnimWidth2 = (missile[mi]._miAnimWidth - 64) >> 1;
    missile[mi]._miAnimAdd = 1;
    missile[mi]._miLightFlag = TRUE;
    missile[mi]._miPreFlag = FALSE;
    missile[mi]._mirange = 250;
    missile[mi]._mirnd = 0;
    PutMissile(mi);
}

// .text:004393FA
// dx is [0..3] for bone1/bone2/bone3/bone3
void AddBone(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    missile[mi]._midam = 0;
    missile[mi]._miAnimWidth = 96;

    // weird switch param but OK
    switch (dx)
    {
    case 0:
        missile[mi]._miAnimLen = 8;
        missile[mi]._miAnimData = pBone1Cel;
        break;
    case 1:
        missile[mi]._miAnimLen = 8;
        missile[mi]._miAnimData = pBone2Cel;
        break;
    case 2:
    case 3:
        missile[mi]._miAnimLen = 8;
        missile[mi]._miAnimData = pBone3Cel;
        break;
    }

    pBoneCelBkup = missile[mi]._miAnimData;
    missile[mi]._miAnimDelay = 2;
    missile[mi]._miAnimFrame = 1;
    missile[mi]._miAnimFlags = TRUE;
    missile[mi]._miAnimWidth = 128;
    missile[mi]._miAnimWidth2 = 32;
    missile[mi]._miAnimAdd = 1;
    missile[mi]._miLightFlag = TRUE;
    missile[mi]._miPreFlag = FALSE;
    missile[mi]._mirange = 250;
    missile[mi]._mirnd = 0;
    PutMissile(mi);
}

// .text:004395BA
// dx is [0..3] for Metlhit1/Metlhit2/Metlhit3/Metlhit3
void AddMetal(int mi, int sx, int sy, int dx, int dy, int midir, int micaster, int id, int midam)
{
    missile[mi]._midam = 0;
    missile[mi]._miAnimWidth = 96;

    // weird switch param but OK
    switch (dx)
    {
    case 0:
        missile[mi]._miAnimLen = 10;
        missile[mi]._miAnimData = pMetalHit1Cel;
        break;
    case 1:
        missile[mi]._miAnimLen = 10;
        missile[mi]._miAnimData = pMetalHit2Cel;
        break;
    case 2:
    case 3:
        missile[mi]._miAnimLen = 10;
        missile[mi]._miAnimData = pMetalHit3Cel;
        break;
    }

    pMtlHitCelBkup = missile[mi]._miAnimData;
    missile[mi]._miAnimDelay = 2;
    missile[mi]._miAnimFrame = 1;
    missile[mi]._miAnimFlags = TRUE;
    missile[mi]._miAnimWidth = 96;
    missile[mi]._miAnimWidth2 = 16;
    missile[mi]._miAnimAdd = 1;
    missile[mi]._miLightFlag = TRUE;
    missile[mi]._miPreFlag = FALSE;
    missile[mi]._mirange = missile[mi]._miAnimLen;
    missile[mi]._mirnd = 0;
    PutMissile(mi);
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
void GetMissilePos(int i)
{
    // TODO
}

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
void MI_Stone(int i)
{
    int m;

    missile[i]._mirange--;
    m = missile[i]._miVar2; // index into `monster` of frozen monster
    if (monster[m]._mhitpoints == 0 && missile[m]._miAnimData != pShatter1Cel)
    {
        missile[m]._miAnimData = pShatter1Cel;
        missile[m]._miAnimDelay = 1;
        missile[m]._miAnimCnt = 0;
        missile[m]._miAnimLen = 12;
        missile[m]._miAnimFrame = 1;
        missile[m]._miAnimFlags = 1;
        missile[m]._miAnimWidth = 128;
        missile[m]._miAnimWidth2 = 32;
        missile[m]._mirange = 11;
    }

    // When the spell wears off...
    if (missile[i]._mirange == 0)
    {
        missile[i]._miDelFlag = TRUE;

        if (monster[m]._mhitpoints > 0)
        {
            // ... unfreeze the monster if they're still alive
            monster[m]._mmode = missile[i]._miVar1;
        }
        else
        {
            // ... create a shatter effect if they're dead
            AddDead(monster[m]._mx, monster[m]._my, stonendx, monster[m]._mdir);
        }
    }
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
    int mid, i, j, k, l;                       // TODO var order
    int CrawlNum[6] = {0, 3, 12, 45, 94, 159}; // offsets into crawl table for rings 0 through 5

    missile[mi]._mirange--;
    mid = dMonster[missile[mi]._miVar1][missile[mi]._miVar2];

    if (mid > 0)
    {
        mid--;
    }
    else
    {
        mid = -(mid + 1);
    }

    // Enemy moved, refresh target location
    if (missile[mi]._miVar3 != mid)
    {
        for (i = 0; i < 6; i++)
        {
            k = CrawlNum[i];
            l = k + 1;

            for (j = (BYTE)CrawlTable[k]; j > 0; j--)
            {
                mid = dMonster[missile[mi]._miVar1 + CrawlTable[l]][missile[mi]._miVar2 + CrawlTable[l + 1]];
                if (mid > 0)
                {
                    mid--;
                }
                else
                {
                    mid = -(mid + 1);
                }

                if (missile[mi]._miVar3 == mid)
                {
                    // Found the monster we were originally targetting, refresh location
                    missile[mi]._miVar1 += CrawlTable[l];
                    missile[mi]._miVar2 += CrawlTable[l + 1];

                    j = -99;
                    i = 6;
                    break;
                }

                l += 2;
            }
        }

        if (j != -99)
        {
            // Can't find the monster we were tracking! Panic
            missile[mi]._miDelFlag = TRUE;
            PutMissile(mi); // BUG???? Shouldn't this be DeleteMissile???
            // realistically, when can this happen? we course correct every game tick
            // unless the monster dies before the missile gets to them?
            // or the monster teleports
        }
        else
        {
            GetMissileVel(mi, missile[mi]._mix, missile[mi]._miy, missile[mi]._miVar1, missile[mi]._miVar2, 16);
            missile[mi]._mimfnum = GetDirection(missile[mi]._mix, missile[mi]._miy, missile[mi]._miVar1, missile[mi]._miVar2);
            missile[mi]._miAnimData = pDoomCels[missile[mi]._mimfnum];
            if (missile[mi]._miAnimData != pDoomexpCel)
            {
                missile[mi]._mitxoff += missile[mi]._mixvel;
                missile[mi]._mityoff += missile[mi]._miyvel;
                GetMissilePos(mi);

                mid = dMonster[missile[mi]._mix][missile[mi]._miy];
                if (mid > 0)
                {
                    mid--;
                }
                else
                {
                    mid = -(mid + 1);
                }

                // TODO
            }

            // TODO
        }

        // TODO
    }

    // TODO
}

// MI_Lightball	00000000004407E7
void MI_Lightball(int mi)
{
    // TODO
}

// .text:00440906
// Used by MIS_BLOOD, MIS_BONE, MIS_METLHIT, and MIS_BLODBUR
void MI_Blood(int mi)
{
    missile[mi]._mirange--;
    if (missile[mi]._mirange == 0)
    {
        missile[mi]._miDelFlag = TRUE;
    }
    if (missile[mi]._miAnimFrame == missile[mi]._miAnimLen)
    {
        missile[mi]._miAnimFlags = FALSE;
        missile[mi]._miPreFlag = TRUE;
    }
    PutMissile(mi);
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
    switch (missile[mi]._mitype)
    {
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