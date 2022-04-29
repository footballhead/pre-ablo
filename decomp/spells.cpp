#include "defines.h"
#include "diablo.h"
#include "engine.h"
#include "enums.h"
#include "player.h"
#include "structs.h"

//
// imports
//

void RemoveScroll();
void UseStaffCharge();
int GetDirection(int fx, int fy, int tx, int ty);
int AddMissile(int sx, int sy, int dx, int dy, int midir, int mitype, int micaster, int id, int midam);
void savecrsr_hide();
void SetCursor_(int i);
void savecrsr_show();
void M_StartKill(int i, int pnum);
void AddPlrExperience(int pnum, int lvl, int exp);
void AddDead(int dx, int dy, char dv, int ddir);
void SyncPlrKill(int pnum);
void StartPlrHit(int pnum);

extern int dPiece[MAXDUNX][MAXDUNY];
extern BOOLEAN nSolidTable[MAXTILES + 1];
extern int pcurs;
extern BOOL drawmanaflag;
extern BOOL drawhpflag;
extern BOOL invflag;
extern char dFlags[MAXDUNX][MAXDUNY];
extern char dObject[MAXDUNX][MAXDUNY];
extern char dMonster[MAXDUNX][MAXDUNY];
extern BYTE vCrawlTable[23][30];
extern MonsterStruct monster[MAXMONSTERS];
extern int spurtndx;
extern int nummissiles;
extern int missileactive[MAXMISSILES];
extern MissileStruct missile[MAXMISSILES];

//
// initialized vars (.data:004B8218)
//

int XDirAdd[8] = {1, 0, -1, -1, -1, 0, 1, 1};
int YDirAdd[8] = {1, 1, 1, 0, -1, -1, -1, 0};

int spelldata_sManaCost[] = {
    0,
    6,
    8,
    10,
    20,
    13,
    28,
    35,
    18,
    40,
    12,
    15,
    30,
    50,
    25,
    35,
    30,
    0,
    40,
    40,
    50,
    66,
    20,
    35,
    150,
    100,
    0,
    0,
    0,
};
int spelldata_sType[] = {
    STYPE_FIRE,
    STYPE_FIRE,
    STYPE_MAGIC,
    STYPE_LIGHTNING,
    STYPE_LIGHTNING,
    STYPE_MAGIC,
    STYPE_FIRE,
    STYPE_MAGIC,
    STYPE_MAGIC,
    STYPE_MAGIC,
    STYPE_MAGIC,
    STYPE_MAGIC,
    STYPE_FIRE,
    STYPE_FIRE,
    STYPE_LIGHTNING,
    STYPE_FIRE,
    STYPE_LIGHTNING,
    STYPE_MAGIC,
    STYPE_LIGHTNING,
    STYPE_MAGIC,
    STYPE_FIRE,
    STYPE_FIRE,
    STYPE_LIGHTNING,
    STYPE_MAGIC,
    STYPE_FIRE,
    STYPE_MAGIC,
    STYPE_MAGIC,
    STYPE_MAGIC,
    STYPE_MAGIC,
};
const char *spelldata_sNameText[] = {
    " ",
    "Firebolt",
    "Healing",
    "Lightning",
    "Flash",
    "Identify",
    "Fire Wall",
    "Town Portal",
    "Stone Curse",
    "Infravision",
    "Phasing",
    "Mana Shield",
    "Fireball",
    "Guardian",
    "Chain Lightning",
    "Flame Wave",
    "Doom Serpents",
    "Blood Ritual",
    "Nova",
    "Invisibility",
    "Sentinal",
    "Golem",
    "Blood Boil",
    "Teleport",
    "Apocalypse",
    "Etherealize",
    "Item Repair",
    "Staff Recharge",
    "Trap Disarm",
};
const char *spelldata_sSkillText[] = {
    NULL,
    "Firebolt",
    NULL,
    NULL,
    NULL,
    "Identify",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "Item Repair",
    "Staff Recharge",
    "Trap Disarm",
};
int spelldata_sBookLvl[] = {
    -1,
    1,
    1,
    2,
    5,
    1,
    3,
    3,
    6,
    6,
    7,
    6,
    8,
    9,
    8,
    9,
    9,
    2,
    11,
    12,
    9,
    11,
    10,
    14,
    -1,
    -1,
    -1,
    -1,
    -1,
};
int spelldata_sStaffLvl[] = {
    -1,
    1,
    1,
    1,
    4,
    1,
    2,
    3,
    5,
    5,
    6,
    5,
    7,
    8,
    7,
    8,
    8,
    2,
    10,
    10,
    8,
    9,
    9,
    12,
    15,
    12,
    -1,
    -1,
    -1,
};
int spelldata_sMinInt[] = {
    0,
    0,
    17,
    20,
    33,
    23,
    27,
    20,
    51,
    36,
    39,
    25,
    48,
    57,
    54,
    45,
    60,
    21,
    66,
    69,
    57,
    72,
    78,
    81,
    84,
    75,
    -1,
    -1,
    -1,
};
int spelldata_sSFX[] = {
    0,
    60,
    63,
    61,
    61,
    62,
    60,
    62,
    60,
    63,
    60,
    60,
    60,
    60,
    60,
    60,
    60,
    60,
    61,
    60,
    60,
    60,
    63,
    62,
    60,
    60,
    62,
    62,
    62,
};

//
// uninitialized vars (.data:005FF2E8)
//

SpellStruct spells[MAX_SPELLS];
BYTE *pSpellLghningCel;
int spellsactive[MAX_SPELLS];
int numspells;
int availspells[MAX_SPELLS];

//
// code (.text:00453AD0)
//

// .text:00453AD0
void InitSpellGFX()
{
    pSpellLghningCel = LoadFileInMem("Missiles\\Lghning.CEL");
}

// .text:00453AEF
void InitSpells()
{
    int i;

    numspells = 0;
    for (i = 0; i < MAX_SPELLS; i++)
    {
        spells[i].type = SPL_NULL;
        spells[i].x = 0;
        spells[i].y = 0;
        spells[i].anonymous_2 = 0;
        spells[i].anonymous_11 = 0;
        spells[i].var1 = 0;
        spells[i].var2 = 0;
        spells[i].var3 = 0;
        spells[i].var4 = 0;
        spells[i].delFlag = 0;
        availspells[i] = i;
        spellsactive[i] = 0;
    }
}

// .text:00453C0D
void AddSpellStats(int i, BYTE *animdata, int arg_0, int arg_4, int width, int arg_c)
{
    spells[i].animdata = animdata;
    spells[i].anonymous_7 = arg_0;
    spells[i].anonymous_8 = 1;
    spells[i].anonymous_6 = 0;
    spells[i].anonymous_5 = arg_4;
    spells[i].animWidth = width;
    spells[i].animWidth2 = (width - 64) >> 1;
    spells[i].anonymous_2 = 1;
    spells[i].anonymous_3 = arg_c;
}

// .text:00453CDB
int GetSpellLevel(int pnum, int spell)
{
    return plr[pnum]._pSplLvl[spell] + plr[pnum]._pISplLvlAdd;
}

// .text:00453D32
int GetManaAdjust(int pnum, int spell)
{
    int spllvl;
    int cost;

    spllvl = GetSpellLevel(pnum, spell);
    switch (spell)
    {
    case SPL_FIREBOLT:
    case SPL_LIGHTNING:
        cost = -spllvl; // 1
        break;
    case SPL_HEAL:
        if (spllvl > 10)
        {
            spllvl = 10;
        }
        cost = -(spllvl * 2); // 2
        break;
    case SPL_TOWN:
    case SPL_WAVE:
    case SPL_TELEPORT:
        cost = -(spllvl * 2 + spllvl); // 3
        break;
    case SPL_FLASH:
    case SPL_IDENTIFY:
    case SPL_FIREWALL:
    case SPL_STONE:
    case SPL_RNDTELEPORT:
    case SPL_MANASHIELD:
    case SPL_BLODBOIL:
        cost = -(spllvl * 2); // 2
        break;
    case SPL_INFRA:
    case SPL_DOOMSERP:
    case SPL_INVISIBIL:
        cost = -(spllvl * 4 + spllvl); // 5
        break;
    case SPL_FIREBALL:
    case SPL_CHAIN:
    case SPL_NOVA:
    case SPL_SENTINEL:
        cost = -(spllvl * 4); // 4
        break;
    case SPL_GOLEM:
    case SPL_APOCA:
    case SPL_ETHEREALIZE:
        cost = -(spllvl * 4 + spllvl * 2); // 6
        break;
    case SPL_GUARDIAN:
        cost = 0; // 0
        break;
    default:
        cost = 0; // 0
        break;
    }

    return cost;
}

// .text:00453E86
int GetManaAmount(int pnum, int spell)
{
    int adj;
    int ma;

    adj = GetManaAdjust(pnum, spell);

    if (spell == SPL_HEAL)
    {
        ma = (2 * plr[pnum]._pLevel + spelldata_sManaCost[spell] + adj) << 6;
    }
    else
    {
        ma = (spelldata_sManaCost[spell] + adj) << 6;
    }

    if (plr[pnum]._pClass == PC_SORCERER)
    {
        // Do nothing? There's is literally the code:
        //    cmp eax, PC_SORCERER
        //    jnz $+6
        // But jnz is 6 bytes long ._.
        // So PC_SORCERER or not, you're executing the same code
    }

    if (plr[pnum]._pClass == PC_ROGUE)
    {
        ma -= ma >> 2;
    }

    if (ma < 0)
    {
        ma = 0;
    }

    ma = ((100 - plr[pnum]._pISplCost) * ma) / 100;
    return ma;
}

// .text:00453F9F
void UseMana(int id, int sn)
{
    int ma;

    if (plr[id]._pRSplType == RSPLTYPE_SKILL || plr[id]._pRSplType == RSPLTYPE_INVALID)
    {
        return;
    }

    if (plr[id]._pRSplType == RSPLTYPE_SCROLL)
    {
        RemoveScroll();
        return;
    }
    if (plr[id]._pRSplType == RSPLTYPE_CHARGES)
    {
        UseStaffCharge();
        return;
    }
    if (cheat_mode)
    {
        return;
    }

    if (plr[id]._pRSplType == RSPLTYPE_SPELL)
    {
        ma = GetManaAmount(id, sn);
        plr[id]._pMana -= -(0 - ma);
        plr[id]._pManaBase -= -(0 - ma);
    }
}

// .text:004540ED
// This function is only ever called from dead code.
// Check if there is a solid piece between the player coordinates and the given coordinates.
static BOOL __dc_CheckBlockFromPlayer(int tx, int ty)
{
    BOOL coll;
    int fx;
    int pn;
    int fy;
    int piece;

    fx = plr[myplr]._px;
    fy = plr[myplr]._py;

    coll = FALSE;
    while (fx != tx || fy != ty)
    {
        pn = GetDirection(fx, fy, tx, ty);
        fx += XDirAdd[pn];
        fy += YDirAdd[pn];
        piece = dPiece[fx][fy];
        if (nSolidTable[piece])
        {
            coll = TRUE;
        }
    }

    return coll;
}

// .text:004541CF
BOOL CheckSpell(int id, int sn, BOOL manaonly)
{
    int ma;

    if (cheat_mode)
    {
        return TRUE;
    }

    if (!manaonly && pcurs != CURSOR_HAND)
    {
        return FALSE;
    }

    if (plr[id]._pRSplType == RSPLTYPE_SKILL)
    {
        return TRUE;
    }

    ma = GetManaAmount(id, sn);
    if (plr[id]._pMana < ma)
    {
        return FALSE;
    }

    return TRUE;
}

// .text:0045428B
// Very similar to DeleteMissile
void DeleteSpell(int sidx, int i)
{
    availspells[MAX_SPELLS - numspells] = sidx;
    numspells--;
    if (numspells > 0 && i != numspells)
    {
        spellsactive[i] = spellsactive[numspells];
    }
}

// .text:004542F1
static void cast_firebolt(int id, int sidx, int sx, int sy, int dx, int dy)
{
    if (sx == dx && sy == dy)
    {
        dx += XDirAdd[plr[id]._pdir];
        dy += XDirAdd[plr[id]._pdir];
    }

    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_FIREBOLT, 0, id, 0);
    UseMana(id, SPL_FIREBOLT);
    drawmanaflag = TRUE;
    spells[sidx].delFlag = TRUE;
}

// .text:004543D2
static void cast_healing(int id, int sidx, int sx, int sy, int dx, int dy)
{
    int i;
    int HealAmount;

    HealAmount = 0;
    for (i = 0; plr[id]._pLevel + GetSpellLevel(id, SPL_HEAL) > i; i++)
    {
        HealAmount += (random_(4) + 2) << 6;
    }

    if (plr[id]._pClass == PC_WARRIOR)
    {
        HealAmount <<= 1;
    }
    if (plr[id]._pClass == PC_ROGUE)
    {
        HealAmount += HealAmount >> 1;
    }

    plr[id]._pHitPoints += HealAmount;
    if (plr[id]._pMaxHP < plr[id]._pHitPoints)
    {
        plr[id]._pHitPoints = plr[id]._pMaxHP;
    }

    plr[id]._pHPBase += HealAmount;
    if (plr[id]._pHPBase > plr[id]._pMaxHPBase)
    {
        plr[id]._pHPBase = plr[id]._pMaxHPBase;
    }

    UseMana(id, SPL_HEAL);
    drawhpflag = TRUE;
    drawmanaflag = TRUE;
    spells[sidx].delFlag = TRUE;
}

// .text:004545F8
static void cast_lightning(int id, int sidx, int sx, int sy, int dx, int dy)
{
    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_LIGHTCTRL, 0, id, 0);
    UseMana(id, SPL_LIGHTNING);
    drawmanaflag = TRUE;
    spells[sidx].delFlag = TRUE;
}

// .text:00454677
static void cast_identify(int id, int sidx, int sx, int sy, int dx, int dy)
{
    spells[sidx].delFlag = TRUE;
    if (pcurs != CURSOR_NONE)
    {
        return;
    }

    savecrsr_hide();
    SetCursor_(CURSOR_IDENTIFY);
    savecrsr_show();

    if (invflag == FALSE)
    {
        invflag = TRUE;
    }

    UseMana(id, SPL_IDENTIFY);
    drawmanaflag = TRUE;
}

// .text:004546FA
static void cast_firewall(int id, int sidx, int sx, int sy, int dx, int dy)
{
    int dir;

    if (dFlags[dx][dy] & BFLAG_LIT)
    {
        spells[sidx].type = SPL_FIREWALL;
        spells[sidx].x = sx;
        spells[sidx].y = sy;
        spells[sidx].source = id;

        spells[sidx].var1 = dx;
        spells[sidx].var2 = dy;
        spells[sidx].var5 = dx;
        spells[sidx].var6 = dy;
        spells[sidx].var7 = 0;
        spells[sidx].var8 = 0;
        dir = GetDirection(sx, sy, spells[sidx].var1, spells[sidx].var2);
        spells[sidx].var3 = (dir - 2) & 7;
        spells[sidx].var4 = (dir + 2) & 7;
        spells[sidx].range = 20;

        AddSpellStats(sidx, pSpellLghningCel, 8, 0, 96, 0);
        spells[sidx].anonymous_11 = 0;
        spells[sidx].delFlag = FALSE;

        UseMana(id, SPL_FIREWALL);
        drawmanaflag = TRUE;
    }
    else
    {
        spells[sidx].delFlag = TRUE;
    }
}

// .text:004548DA
static void cast_town(int id, int sidx, int sx, int sy, int dx, int dy)
{
    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_TOWN, 0, id, 0);
    UseMana(id, SPL_TOWN);
    drawmanaflag = TRUE;
    spells[sidx].delFlag = TRUE;
}

// .text:00454959
static void cast_flash(int id, int sidx, int sx, int sy, int dx, int dy)
{
    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_FLASH, 0, id, 0);
    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_FLASH2, 0, id, 0);
    UseMana(id, SPL_TOWN);
    drawmanaflag = TRUE;
    spells[sidx].delFlag = TRUE;
}

// .text:00454A11
static void cast_infravision(int id, int sidx, int sx, int sy, int dx, int dy)
{
    spells[sidx].type = SPL_INFRA;
    spells[sidx].x = sx;
    spells[sidx].y = sy;
    spells[sidx].source = id;

    spells[sidx].range = 255;

    AddSpellStats(sidx, pSpellLghningCel, 8, 0, 96, 0);
    spells[sidx].anonymous_11 = 0;
    spells[sidx].delFlag = FALSE;

    UseMana(id, SPL_INFRA);
    drawmanaflag = TRUE;
}

// .text:00454AE2
static void cast_phasing(int id, int sidx, int sx, int sy, int dx, int dy)
{
    int r1, r2, pn;

    do
    {
        r1 = random_(3) + 4;
        r2 = random_(3) + 4;
        if (random_(2) == 1)
        {
            r1 = -r1;
        }
        if (random_(2) == 1)
        {
            r2 = -r2;
        }

        pn = dPiece[sx + r1][sy + r2];
    } while (nSolidTable[pn] || dObject[sx + r1][sy + r2] != 0 || dMonster[sx + r1][sy + r2] != 0);

    AddMissile(sx, sy, sx + r1, sy + r2, plr[id]._pdir, MIS_RNDTELEPORT, 0, id, 0);
    UseMana(id, SPL_RNDTELEPORT);
    drawmanaflag = TRUE;
    spells[sidx].delFlag = TRUE;
}

// .text:00454C38
static void cast_manashield(int id, int sidx, int sx, int sy, int dx, int dy)
{
    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_MANASHIELD, 0, id, 0);
    UseMana(id, SPL_MANASHIELD);
    drawmanaflag = TRUE;
    spells[sidx].delFlag = TRUE;
}

// .text:00454CB7
// Construct a moving firewall, starting from the center and moving to the outside.
// The two sides of the center are distinguished by `a`/`1` and `b`/`2`
// (avoids using relative terms like left/right or clockwise/counter-clockwise)
static void cast_flamewave(int id, int sidx, int sx, int sy, int v1, int v2)
{
    int sd, nxa, dira, nxb, dirb;
    BOOL f1;
    int j, nya;
    BOOL f2;
    int nyb, pn;

    spells[sidx].type = SPL_WAVE;

    f1 = FALSE;
    f2 = FALSE;

    sd = GetDirection(sx, sy, v1, v2);
    dira = (sd - 2) & 7;
    dirb = (sd + 2) & 7;
    nxa = sx + XDirAdd[sd];
    nya = sy + YDirAdd[sd];
    pn = dPiece[nxa][nya];
    if (!nSolidTable[pn])
    {
        AddMissile(nxa, nya, nxa + XDirAdd[sd], nya + YDirAdd[sd], plr[id]._pdir, MIS_FIREMOVE, 0, id, 0);

        nxa += XDirAdd[dira];
        nya += YDirAdd[dira];
        nxb = sx + XDirAdd[sd] + XDirAdd[dirb];
        nyb = sy + YDirAdd[sd] + YDirAdd[dirb];

        for (j = 0; GetSpellLevel(id, SPL_WAVE) + 2 > j; j++)
        {
            pn = dPiece[nxa][nya];
            if (!nSolidTable[pn] && !f1)
            {
                AddMissile(nxa, nya, nxa + XDirAdd[sd], nya + YDirAdd[sd], plr[id]._pdir, MIS_FIREMOVE, 0, id, 0);
                nxa += XDirAdd[dira];
                nya += YDirAdd[dira];
            }
            else
            {
                f1 = TRUE;
            }

            pn = dPiece[nxb][nyb];
            if (!nSolidTable[pn] && !f2)
            {
                AddMissile(nxb, nyb, nxb + XDirAdd[sd], nyb + YDirAdd[sd], plr[id]._pdir, MIS_FIREMOVE, 0, id, 0);
                nxb += XDirAdd[dirb];
                nyb += YDirAdd[dirb];
            }
            else
            {
                f2 = TRUE;
            }
        }
    }

    spells[sidx].delFlag = TRUE;
    UseMana(id, SPL_WAVE);
    drawmanaflag = TRUE;
}

// .text:00454FB0
static void cast_fireball(int id, int sidx, int sx, int sy, int dx, int dy)
{
    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_FIREBALL, 0, id, 0);
    UseMana(id, SPL_FIREBALL);
    drawmanaflag = TRUE;
    spells[sidx].delFlag = TRUE;
}

// .text:0045502F
static void cast_chainlightning(int id, int sidx, int sx, int sy, int dx, int dy)
{
    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_CHAIN, 0, id, 0);
    UseMana(id, SPL_CHAIN);
    drawmanaflag = TRUE;
    spells[sidx].delFlag = TRUE;
}

// .text:004550AE
static void cast_sentinal(int id, int sidx, int sx, int sy, int dx, int dy)
{
    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_SENTINAL, 0, id, 0);
    UseMana(id, SPL_SENTINEL);
    drawmanaflag = TRUE;
    spells[sidx].delFlag = TRUE;
}

// .text:0045512D
static void cast_doomserpents(int id, int sidx, int sx, int sy, int dx, int dy)
{
    int slvl; // never read

    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_DOOMSERP, 0, id, 0);
    slvl = GetSpellLevel(id, SPL_DOOMSERP); // slvl is unused
    UseMana(id, SPL_DOOMSERP);
    drawmanaflag = TRUE;
    spells[sidx].delFlag = TRUE;
}

// .text:004551BC
static void cast_nova(int id, int sidx, int sx, int sy, int dx, int dy)
{
    int v3;
    int sx1;
    int sy1;
    int k;

    sx1 = sy1 = 0;

    v3 = plr[id]._pLevel + random_(6) + random_(6) + random_(6) + random_(6) + random_(6) + 5;
    for (k = GetSpellLevel(id, SPL_NOVA); k > 0; k--)
    {
        v3 += v3 >> 3;
    }

    for (k = 0; k < 23; k++)
    {
        if (sx1 != vCrawlTable[k][6] || sy1 != vCrawlTable[k][7])
        {
            dx = vCrawlTable[k][6] + sx;
            dy = vCrawlTable[k][7] + sy;
            AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_LIGHTBALL, 0, id, v3);
            dx = sx - vCrawlTable[k][6];
            dy = sy - vCrawlTable[k][7];
            AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_LIGHTBALL, 0, id, v3);
            dx = sx - vCrawlTable[k][6];
            dy = vCrawlTable[k][7] + sy;
            AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_LIGHTBALL, 0, id, v3);
            dx = vCrawlTable[k][6] + sx;
            dy = sy - vCrawlTable[k][7];
            AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_LIGHTBALL, 0, id, v3);
            sx1 = vCrawlTable[k][6];
            sy1 = vCrawlTable[k][7];
        }
    }

    UseMana(id, SPL_NOVA);
    drawmanaflag = TRUE;
    spells[sidx].delFlag = TRUE;
}

// .text:004554C3
static void cast_bloodboil(int id, int sidx, int sx, int sy, int dx, int dy)
{
    int mid, splvl;

    mid = dMonster[dx][dy];
    if (mid > 0)
    {
        mid--; // BUG! Cant kill mid == 1
    }
    else
    {
        mid = -(mid + 1);
    }

    if (mid > 0) // BUG! Cant kill mid == 1
    {
        monster[mid]._mhitpoints = 0;
        M_StartKill(mid, id);
        AddPlrExperience(id, monster[mid].mLevel, monster[mid].mExp);
        dMonster[monster[mid]._mx][monster[mid]._my] = 0;
        monster[mid]._mDelFlag = TRUE;

        AddMissile(monster[mid]._mx, monster[mid]._my, monster[mid]._mx, monster[mid]._my, plr[id]._pdir, MIS_BLODBUR, 0, id, random_(2) + 1);
        AddDead(monster[mid]._mx, monster[mid]._my, spurtndx, monster[mid]._mdir);

        plr[id]._pHitPoints -= monster[mid]._mmaxhp;
        plr[id]._pHPBase -= monster[mid]._mmaxhp;

        for (splvl = GetSpellLevel(id, SPL_BLODBOIL); splvl > 0; splvl--)
        {
            plr[id]._pHitPoints += monster[mid]._mmaxhp >> 3;
            plr[id]._pHPBase += monster[mid]._mmaxhp >> 3;
        }

        if (plr[id]._pHitPoints <= 0)
        {
            plr[id]._pHitPoints = 0;
            if (gbActivePlayers == 1)
                SyncPlrKill(id);
        }
        else
        {
            StartPlrHit(id);
        }

        UseMana(id, SPL_BLODBOIL); // SPL_BLODBOIL == 0x16
        drawmanaflag = TRUE;
    }

    spells[sidx].delFlag = TRUE;
}

// .text:00455829
static void cast_teleport(int id, int sidx, int sx, int sy, int dx, int dy)
{
    int i;
    int mi;
    int var; // reused for dPiece and mirange

    var = dPiece[dx][dy];
    if (nSolidTable[var] == 0 && !dObject[dx][dy] && dMonster[dx][dy] == 0)
    {
        // Not sure exactly what this code is doing but it looks like a 100%
        // mana discount when casting teleport back-to-back?
        for (i = 0; i < nummissiles; i++)
        {
            mi = missileactive[i];
            if (missile[mi]._mitype == MIS_TELEPORT && missile[mi]._misource == id)
            {
                break;
            }
        }

        if (i == nummissiles)
        {
            // Didn't find an existing teleport missile, must be new cast
            var = 0;
            UseMana(id, SPL_TELEPORT);
        }
        else
        {
            // Found teleport! Don't use mana, queue other missile for deletion?
            var = missile[mi]._mirange;
            missile[mi]._mirange = 0;
        }

        AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_TELEPORT, 0, id, var);
        drawmanaflag = TRUE;
    }

    spells[sidx].delFlag = TRUE;
}

// .text:004559B4
static void cast_itemrepair(int id, int sidx, int sx, int sy, int dx, int dy)
{
    spells[sidx].delFlag = TRUE;

    if (pcurs != CURSOR_NONE)
    {
        return;
    }

    savecrsr_hide();
    SetCursor_(CURSOR_REPAIR);
    savecrsr_show();

    if (invflag == FALSE)
    {
        invflag = TRUE;
    }

    UseMana(id, SPL_REPAIR);
    drawmanaflag = TRUE;
}

// .text:00455A37
static void cast_staffrecharge(int id, int sidx, int sx, int sy, int dx, int dy)
{
    spells[sidx].delFlag = TRUE;

    if (pcurs != CURSOR_NONE)
    {
        return;
    }

    savecrsr_hide();
    SetCursor_(CURSOR_RECHARGE);
    savecrsr_show();

    if (invflag == FALSE)
    {
        invflag = TRUE;
    }

    UseMana(id, SPL_RECHARGE);
    drawmanaflag = TRUE;
}

// .text:00455ABA
static void cast_trapdisarm(int id, int sidx, int sx, int sy, int dx, int dy)
{
    spells[sidx].delFlag = TRUE;

    if (pcurs != CURSOR_NONE)
    {
        return;
    }

    savecrsr_hide();
    SetCursor_(CURSOR_DISARM);
    savecrsr_show();

    UseMana(id, SPL_DISARM);
    drawmanaflag = TRUE;
}

// .text:00455B26
static void cast_apoca(int id, int sidx, int sx, int sy, int dx, int dy)
{
    int i;

    spells[sidx].type = SPL_APOCA;
    spells[sidx].x = sx;
    spells[sidx].y = sy;
    spells[sidx].source = id;
    spells[sidx].var1 = 8;
    spells[sidx].var2 = sy - spells[sidx].var1;
    spells[sidx].var3 = spells[sidx].var1 + sy;
    spells[sidx].var4 = sx - spells[sidx].var1;
    spells[sidx].var5 = spells[sidx].var1 + sx;
    spells[sidx].var6 = spells[sidx].var4;

    for (i = 0; plr[id]._pLevel > i; i++)
    {
        spells[sidx].dam += random_(6) + 1;
    }

    spells[sidx].range = 255;
    AddSpellStats(sidx, pSpellLghningCel, 8, 0, 96, 0);
    spells[sidx].anonymous_11 = 0;
    spells[sidx].delFlag = FALSE;

    UseMana(id, SPL_APOCA);
    drawmanaflag = TRUE;
}

// .text:00455D0B
static void cast_guardian(int id, int sidx, int sx, int sy, int dx, int dy)
{
    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_GUARDIAN, 0, id, 0);
    UseMana(id, SPL_GUARDIAN);
    drawmanaflag = TRUE;
    spells[sidx].delFlag = TRUE;
}

// .text:00455D8A
static void cast_stonecurse(int id, int sidx, int sx, int sy, int dx, int dy)
{
    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_STONE, 0, id, 0);
    UseMana(id, SPL_STONE);
    drawmanaflag = TRUE;
    spells[sidx].delFlag = TRUE;
}

// .text:00455E09
static void cast_invis(int id, int sidx, int sx, int sy, int dx, int dy)
{
    int slvl; // never read

    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_INVISIBL, 0, id, 0);
    slvl = GetSpellLevel(id, SPL_INVISIBIL); // slvl is unused
    UseMana(id, SPL_INVISIBIL);
    drawmanaflag = TRUE;
    spells[sidx].delFlag = TRUE;
}

// .text:00455E98
static void cast_blood_ritual(int id, int si, int /*sx*/, int /*sy*/, int /*dx*/, int /*dy*/)
{
    int mana;

    plr[id]._pHitPoints -= 10 << 6; // << 6 because HP is fixed point with 6 fractional bits
    plr[id]._pHPBase -= 10 << 6;

    mana = (GetSpellLevel(id, SPL_BLODRIT) + 8) << 6;
    plr[id]._pMana += mana;
    plr[id]._pManaBase += mana;

    if (plr[id]._pMana > plr[id]._pMaxMana)
    {
        plr[id]._pMana = plr[id]._pMaxMana;
    }

    if (plr[id]._pManaBase > plr[id]._pMaxManaBase)
    {
        plr[id]._pManaBase = plr[id]._pMaxManaBase;
    }

    if (plr[id]._pHitPoints <= 0)
    {
        plr[id]._pHitPoints = 0;
        if (gbActivePlayers == 1)
        {
            SyncPlrKill(id);
        }
    }

    UseMana(id, SPL_BLODRIT);
    drawmanaflag = TRUE;
    drawhpflag = TRUE;

    spells[si].delFlag = TRUE;
}

// .text:004560B2
static void cast_golem(int id, int sidx, int sx, int sy, int dx, int dy)
{
    int slvl; // never read

    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_GOLEM, 0, id, 0);
    slvl = GetSpellLevel(id, SPL_GOLEM); // slvl is unused
    UseMana(id, SPL_GOLEM);
    drawmanaflag = TRUE;
    spells[sidx].delFlag = TRUE;
}

// .text:00456141
static void cast_ethereal(int id, int sidx, int sx, int sy, int dx, int dy)
{
    int slvl; // never read

    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_ETHEREALIZE, 0, id, 0);
    slvl = GetSpellLevel(id, SPL_ETHEREALIZE); // slvl is unused
    UseMana(id, SPL_ETHEREALIZE);
    drawmanaflag = TRUE;
    spells[sidx].delFlag = TRUE;
}

// .text:004561D0
void CastSpell(int id, int sn, int sx, int sy, int dx, int dy)
{
    int sidx;

    if (numspells >= MAX_SPELLS)
    {
        return;
    }

    sidx = availspells[0];
    availspells[0] = availspells[MAX_SPELLS - numspells - 1];
    spellsactive[numspells] = sidx;

    switch (sn)
    {
    case SPL_STONE:
        cast_stonecurse(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_WAVE:
        cast_flamewave(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_DOOMSERP:
        cast_doomserpents(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_BLODRIT:
        cast_blood_ritual(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_INVISIBIL:
        cast_invis(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_GOLEM:
        cast_golem(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_SENTINEL:
        cast_sentinal(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_BLODBOIL:
        cast_bloodboil(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_TELEPORT:
        cast_teleport(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_FIREBOLT:
        cast_firebolt(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_ETHEREALIZE:
        cast_ethereal(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_HEAL:
        cast_healing(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_LIGHTNING:
        cast_lightning(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_IDENTIFY:
        cast_identify(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_FLASH:
        cast_flash(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_TOWN:
        cast_town(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_FIREWALL:
        cast_firewall(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_RNDTELEPORT:
        cast_phasing(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_FIREBALL:
        cast_fireball(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_GUARDIAN:
        cast_guardian(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_CHAIN:
        cast_chainlightning(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_NOVA:
        cast_nova(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_APOCA:
        cast_apoca(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_INFRA:
        cast_infravision(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_MANASHIELD:
        cast_manashield(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_REPAIR:
        cast_itemrepair(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_RECHARGE:
        cast_staffrecharge(id, sidx, sx, sy, dx, dy);
        break;
    case SPL_DISARM:
        cast_trapdisarm(id, sidx, sx, sy, dx, dy);
        break;
    }

    numspells++;
}
// __dc_spells_hit_monst	0000000000456644
// process_firewall_spell	00000000004567CC
// process_infravision_spell	0000000000456A8A
// process_apoc_spell	0000000000456B1C
// __dc_process_unk_spell_456CA4	0000000000456CA4
// ProcessSpells	0000000000457686
// FreeSpells	000000000045787C
// __nullsub_SetSpellGFX	00000000004578B2
