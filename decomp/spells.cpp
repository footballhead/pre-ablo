#include "spells.h"

#include "cursor.h"
#include "defines.h"
#include "dead.h"
#include "diablo.h"
#include "engine.h"
#include "enums.h"
#include "inv.h"
#include "missiles.h"
#include "monster.h"
#include "player.h"
#include "structs.h"

//
// imports
//

void CalcPlrItemVals(int pnum);

extern int dPiece[MAXDUNX][MAXDUNY];
extern BOOLEAN nSolidTable[MAXTILES + 1];
extern BOOL drawmanaflag;
extern BOOL drawhpflag;
extern char dFlags[MAXDUNX][MAXDUNY];
extern char dObject[MAXDUNX][MAXDUNY];
extern char dMonster[MAXDUNX][MAXDUNY];
extern BYTE vCrawlTable[23][30];
extern MonsterStruct monster[MAXMONSTERS];

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
        spells[i].animFlag = FALSE;
        spells[i].field_30 = 0;
        spells[i].var1 = 0;
        spells[i].var2 = 0;
        spells[i].var3 = 0;
        spells[i].var4 = 0;
        spells[i].delFlag = FALSE;
        availspells[i] = i;
        spellsactive[i] = 0;
    }
}

// .text:00453C0D
static void SetSpellAnims(int i, BYTE *animdata, int len, int delay, int width, int arg_c)
{
    spells[i].animdata = animdata;
    spells[i].animLen = len;
    spells[i].animFrame = 1;
    spells[i].animCnt = 0;
    spells[i].animDelay = delay;
    spells[i].animWidth = width;
    spells[i].animWidth2 = (width - 64) >> 1;
    spells[i].animFlag = TRUE;
    spells[i].field_10 = arg_c;
}

// .text:00453CDB
int GetSpellLevel(int id, int sn)
{
    return plr[id]._pSplLvl[sn] + plr[id]._pISplLvlAdd;
}

// .text:00453D32
static int GetManaAdjust(int pnum, int spell)
{
    int sl;
    int ma;

    sl = GetSpellLevel(pnum, spell);
    switch (spell)
    {
    case SPL_FIREBOLT:
    case SPL_LIGHTNING:
        ma = -sl; // 1
        break;
    case SPL_HEAL:
        if (sl > 10)
        {
            sl = 10;
        }
        ma = -(sl * 2); // 2
        break;
    case SPL_TOWN:
    case SPL_WAVE:
    case SPL_TELEPORT:
        ma = -(sl * 2 + sl); // 3
        break;
    case SPL_FLASH:
    case SPL_IDENTIFY:
    case SPL_FIREWALL:
    case SPL_STONE:
    case SPL_RNDTELEPORT:
    case SPL_MANASHIELD:
    case SPL_BLODBOIL:
        ma = -(sl * 2); // 2
        break;
    case SPL_INFRA:
    case SPL_DOOMSERP:
    case SPL_INVISIBIL:
        ma = -(sl * 4 + sl); // 5
        break;
    case SPL_FIREBALL:
    case SPL_CHAIN:
    case SPL_NOVA:
    case SPL_SENTINEL:
        ma = -(sl * 4); // 4
        break;
    case SPL_GOLEM:
    case SPL_APOCA:
    case SPL_ETHEREALIZE:
        ma = -(sl * 4 + sl * 2); // 6
        break;
    case SPL_GUARDIAN:
        ma = 0; // 0
        break;
    default:
        ma = 0; // 0
        break;
    }

    return ma;
}

// .text:00453E86
int GetManaAmount(int id, int sn)
{
    int adj;
    int ma;

    adj = GetManaAdjust(id, sn);

    if (sn == SPL_HEAL)
    {
        ma = (2 * plr[id]._pLevel + spelldata_sManaCost[sn] + adj) << 6;
    }
    else
    {
        ma = (spelldata_sManaCost[sn] + adj) << 6;
    }

    if (plr[id]._pClass == PC_SORCERER)
    {
        // Do nothing? This is literally the code:
        //    cmp eax, PC_SORCERER
        //    jnz $+6
        // But jnz is 6 bytes long ._.
        // So PC_SORCERER or not, you're executing the same code
    }

    if (plr[id]._pClass == PC_ROGUE)
    {
        ma -= ma >> 2;
    }

    if (ma < 0)
    {
        ma = 0;
    }

    ma = ((100 - plr[id]._pISplCost) * ma) / 100;
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
static void DeleteSpell(int si, int i)
{
    availspells[MAX_SPELLS - numspells] = si;
    numspells--;
    if (numspells > 0 && i != numspells)
    {
        spellsactive[i] = spellsactive[numspells];
    }
}

// .text:004542F1
static void cast_firebolt(int id, int si, int sx, int sy, int dx, int dy)
{
    if (sx == dx && sy == dy)
    {
        dx += XDirAdd[plr[id]._pdir];
        dy += XDirAdd[plr[id]._pdir];
    }

    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_FIREBOLT, 0, id, 0);
    UseMana(id, SPL_FIREBOLT);
    drawmanaflag = TRUE;
    spells[si].delFlag = TRUE;
}

// .text:004543D2
static void cast_healing(int id, int si, int sx, int sy, int dx, int dy)
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
    spells[si].delFlag = TRUE;
}

// .text:004545F8
static void cast_lightning(int id, int si, int sx, int sy, int dx, int dy)
{
    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_LIGHTCTRL, 0, id, 0);
    UseMana(id, SPL_LIGHTNING);
    drawmanaflag = TRUE;
    spells[si].delFlag = TRUE;
}

// .text:00454677
static void cast_identify(int id, int si, int sx, int sy, int dx, int dy)
{
    spells[si].delFlag = TRUE;
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
static void cast_firewall(int id, int si, int sx, int sy, int dx, int dy)
{
    int dir;

    if (dFlags[dx][dy] & BFLAG_LIT)
    {
        spells[si].type = SPL_FIREWALL;
        spells[si].x = sx;
        spells[si].y = sy;
        spells[si].source = id;

        spells[si].var1 = dx;
        spells[si].var2 = dy;
        spells[si].var5 = dx;
        spells[si].var6 = dy;
        spells[si].var7 = 0;
        spells[si].var8 = 0;
        dir = GetDirection(sx, sy, spells[si].var1, spells[si].var2);
        spells[si].var3 = (dir - 2) & 7;
        spells[si].var4 = (dir + 2) & 7;
        spells[si].range = 20;

        SetSpellAnims(si, pSpellLghningCel, 8, 0, 96, 0);
        spells[si].field_30 = 0;
        spells[si].delFlag = FALSE;

        UseMana(id, SPL_FIREWALL);
        drawmanaflag = TRUE;
    }
    else
    {
        spells[si].delFlag = TRUE;
    }
}

// .text:004548DA
static void cast_town(int id, int si, int sx, int sy, int dx, int dy)
{
    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_TOWN, 0, id, 0);
    UseMana(id, SPL_TOWN);
    drawmanaflag = TRUE;
    spells[si].delFlag = TRUE;
}

// .text:00454959
static void cast_flash(int id, int si, int sx, int sy, int dx, int dy)
{
    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_FLASH, 0, id, 0);
    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_FLASH2, 0, id, 0);
    UseMana(id, SPL_FLASH);
    drawmanaflag = TRUE;
    spells[si].delFlag = TRUE;
}

// .text:00454A11
static void cast_infravision(int id, int si, int sx, int sy, int dx, int dy)
{
    spells[si].type = SPL_INFRA;
    spells[si].x = sx;
    spells[si].y = sy;
    spells[si].source = id;

    spells[si].range = 255;

    SetSpellAnims(si, pSpellLghningCel, 8, 0, 96, 0);
    spells[si].field_30 = 0;
    spells[si].delFlag = FALSE;

    UseMana(id, SPL_INFRA);
    drawmanaflag = TRUE;
}

// .text:00454AE2
static void cast_phasing(int id, int si, int sx, int sy, int dx, int dy)
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
    spells[si].delFlag = TRUE;
}

// .text:00454C38
static void cast_manashield(int id, int si, int sx, int sy, int dx, int dy)
{
    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_MANASHIELD, 0, id, 0);
    UseMana(id, SPL_MANASHIELD);
    drawmanaflag = TRUE;
    spells[si].delFlag = TRUE;
}

// .text:00454CB7
// Construct a moving firewall, starting from the center and moving to the outside.
// The two sides of the center are distinguished by `a`/`1` and `b`/`2`
// (avoids using relative terms like left/right or clockwise/counter-clockwise)
static void cast_flamewave(int id, int si, int sx, int sy, int v1, int v2)
{
    int sd, nxa, dira, nxb, dirb;
    BOOL f1;
    int j, nya;
    BOOL f2;
    int nyb, pn;

    spells[si].type = SPL_WAVE;

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

    spells[si].delFlag = TRUE;
    UseMana(id, SPL_WAVE);
    drawmanaflag = TRUE;
}

// .text:00454FB0
static void cast_fireball(int id, int si, int sx, int sy, int dx, int dy)
{
    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_FIREBALL, 0, id, 0);
    UseMana(id, SPL_FIREBALL);
    drawmanaflag = TRUE;
    spells[si].delFlag = TRUE;
}

// .text:0045502F
static void cast_chainlightning(int id, int si, int sx, int sy, int dx, int dy)
{
    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_CHAIN, 0, id, 0);
    UseMana(id, SPL_CHAIN);
    drawmanaflag = TRUE;
    spells[si].delFlag = TRUE;
}

// .text:004550AE
static void cast_sentinal(int id, int si, int sx, int sy, int dx, int dy)
{
    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_SENTINAL, 0, id, 0);
    UseMana(id, SPL_SENTINEL);
    drawmanaflag = TRUE;
    spells[si].delFlag = TRUE;
}

// .text:0045512D
static void cast_doomserpents(int id, int si, int sx, int sy, int dx, int dy)
{
    int slvl; // never read

    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_DOOMSERP, 0, id, 0);
    slvl = GetSpellLevel(id, SPL_DOOMSERP); // slvl is unused
    UseMana(id, SPL_DOOMSERP);
    drawmanaflag = TRUE;
    spells[si].delFlag = TRUE;
}

// .text:004551BC
static void cast_nova(int id, int si, int sx, int sy, int dx, int dy)
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
    spells[si].delFlag = TRUE;
}

// .text:004554C3
static void cast_bloodboil(int id, int si, int sx, int sy, int dx, int dy)
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

        UseMana(id, SPL_BLODBOIL);
        drawmanaflag = TRUE;
    }

    spells[si].delFlag = TRUE;
}

// .text:00455829
static void cast_teleport(int id, int si, int sx, int sy, int dx, int dy)
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

    spells[si].delFlag = TRUE;
}

// .text:004559B4
static void cast_itemrepair(int id, int si, int sx, int sy, int dx, int dy)
{
    spells[si].delFlag = TRUE;

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
static void cast_staffrecharge(int id, int si, int sx, int sy, int dx, int dy)
{
    spells[si].delFlag = TRUE;

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
static void cast_trapdisarm(int id, int si, int sx, int sy, int dx, int dy)
{
    spells[si].delFlag = TRUE;

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
static void cast_apoca(int id, int si, int sx, int sy, int dx, int dy)
{
    int i;

    spells[si].type = SPL_APOCA;
    spells[si].x = sx;
    spells[si].y = sy;
    spells[si].source = id;
    spells[si].var1 = 8;
    spells[si].var2 = sy - spells[si].var1;
    spells[si].var3 = spells[si].var1 + sy;
    spells[si].var4 = sx - spells[si].var1;
    spells[si].var5 = spells[si].var1 + sx;
    spells[si].var6 = spells[si].var4;

    for (i = 0; plr[id]._pLevel > i; i++)
    {
        spells[si].dam += random_(6) + 1;
    }

    spells[si].range = 255;
    SetSpellAnims(si, pSpellLghningCel, 8, 0, 96, 0);
    spells[si].field_30 = 0;
    spells[si].delFlag = FALSE;

    UseMana(id, SPL_APOCA);
    drawmanaflag = TRUE;
}

// .text:00455D0B
static void cast_guardian(int id, int si, int sx, int sy, int dx, int dy)
{
    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_GUARDIAN, 0, id, 0);
    UseMana(id, SPL_GUARDIAN);
    drawmanaflag = TRUE;
    spells[si].delFlag = TRUE;
}

// .text:00455D8A
static void cast_stonecurse(int id, int si, int sx, int sy, int dx, int dy)
{
    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_STONE, 0, id, 0);
    UseMana(id, SPL_STONE);
    drawmanaflag = TRUE;
    spells[si].delFlag = TRUE;
}

// .text:00455E09
static void cast_invis(int id, int si, int sx, int sy, int dx, int dy)
{
    int slvl; // never read

    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_INVISIBL, 0, id, 0);
    slvl = GetSpellLevel(id, SPL_INVISIBIL); // slvl is unused
    UseMana(id, SPL_INVISIBIL);
    drawmanaflag = TRUE;
    spells[si].delFlag = TRUE;
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
static void cast_golem(int id, int si, int sx, int sy, int dx, int dy)
{
    int slvl; // never read

    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_GOLEM, 0, id, 0);
    slvl = GetSpellLevel(id, SPL_GOLEM); // slvl is unused
    UseMana(id, SPL_GOLEM);
    drawmanaflag = TRUE;
    spells[si].delFlag = TRUE;
}

// .text:00456141
static void cast_ethereal(int id, int si, int sx, int sy, int dx, int dy)
{
    int slvl; // never read

    AddMissile(sx, sy, dx, dy, plr[id]._pdir, MIS_ETHEREALIZE, 0, id, 0);
    slvl = GetSpellLevel(id, SPL_ETHEREALIZE); // slvl is unused
    UseMana(id, SPL_ETHEREALIZE);
    drawmanaflag = TRUE;
    spells[si].delFlag = TRUE;
}

// .text:004561D0
void CastSpell(int id, int sn, int sx, int sy, int dx, int dy)
{
    int si;

    if (numspells >= MAX_SPELLS)
    {
        return;
    }

    si = availspells[0];
    availspells[0] = availspells[MAX_SPELLS - numspells - 1];
    spellsactive[numspells] = si;

    switch (sn)
    {
    case SPL_STONE:
        cast_stonecurse(id, si, sx, sy, dx, dy);
        break;
    case SPL_WAVE:
        cast_flamewave(id, si, sx, sy, dx, dy);
        break;
    case SPL_DOOMSERP:
        cast_doomserpents(id, si, sx, sy, dx, dy);
        break;
    case SPL_BLODRIT:
        cast_blood_ritual(id, si, sx, sy, dx, dy);
        break;
    case SPL_INVISIBIL:
        cast_invis(id, si, sx, sy, dx, dy);
        break;
    case SPL_GOLEM:
        cast_golem(id, si, sx, sy, dx, dy);
        break;
    case SPL_SENTINEL:
        cast_sentinal(id, si, sx, sy, dx, dy);
        break;
    case SPL_BLODBOIL:
        cast_bloodboil(id, si, sx, sy, dx, dy);
        break;
    case SPL_TELEPORT:
        cast_teleport(id, si, sx, sy, dx, dy);
        break;
    case SPL_FIREBOLT:
        cast_firebolt(id, si, sx, sy, dx, dy);
        break;
    case SPL_ETHEREALIZE:
        cast_ethereal(id, si, sx, sy, dx, dy);
        break;
    case SPL_HEAL:
        cast_healing(id, si, sx, sy, dx, dy);
        break;
    case SPL_LIGHTNING:
        cast_lightning(id, si, sx, sy, dx, dy);
        break;
    case SPL_IDENTIFY:
        cast_identify(id, si, sx, sy, dx, dy);
        break;
    case SPL_FLASH:
        cast_flash(id, si, sx, sy, dx, dy);
        break;
    case SPL_TOWN:
        cast_town(id, si, sx, sy, dx, dy);
        break;
    case SPL_FIREWALL:
        cast_firewall(id, si, sx, sy, dx, dy);
        break;
    case SPL_RNDTELEPORT:
        cast_phasing(id, si, sx, sy, dx, dy);
        break;
    case SPL_FIREBALL:
        cast_fireball(id, si, sx, sy, dx, dy);
        break;
    case SPL_GUARDIAN:
        cast_guardian(id, si, sx, sy, dx, dy);
        break;
    case SPL_CHAIN:
        cast_chainlightning(id, si, sx, sy, dx, dy);
        break;
    case SPL_NOVA:
        cast_nova(id, si, sx, sy, dx, dy);
        break;
    case SPL_APOCA:
        cast_apoca(id, si, sx, sy, dx, dy);
        break;
    case SPL_INFRA:
        cast_infravision(id, si, sx, sy, dx, dy);
        break;
    case SPL_MANASHIELD:
        cast_manashield(id, si, sx, sy, dx, dy);
        break;
    case SPL_REPAIR:
        cast_itemrepair(id, si, sx, sy, dx, dy);
        break;
    case SPL_RECHARGE:
        cast_staffrecharge(id, si, sx, sy, dx, dy);
        break;
    case SPL_DISARM:
        cast_trapdisarm(id, si, sx, sy, dx, dy);
        break;
    }

    numspells++;
}

// TODO: __dc_spells_hit_monst	0000000000456644

// .text:004567CC
// Replaced by MI_FirewallC
// The firewall is incrementally grown from the middle toward each end
// each tick until it reaches its full extent (or hits a wall).
// Use of myplr makes this multiplayer unfriendly :(
static void process_firewall_spell(int i)
{
    int dp;

    spells[i].range--;
    if (spells[i].range == 0)
    {
        spells[i].delFlag = TRUE;
        return;
    }

    // var1 == X location for next missile to grow side A
    // var2 == Y location for next missile to grow side A
    // var3 == direction to grow side A
    // var4 == direction to grow side B
    // var5 == X location for next missile to grow side B
    // var6 == Y location for next missile to grow side B
    // var7 == bool; stop growing side B
    // var8 == bool; stop growing side A

    // On initial cast, var1 == var2 == var5 == var6, which means this will
    // create overlapping missiles in the middle of the firewall (where the
    // player clicked). Does this mean the middle of the firewall does more
    // damage???

    // Process Side A
    dp = dPiece[spells[i].var1][spells[i].var2];
    if (nSolidTable[dp] == 0 && spells[i].var8 == 0)
    {
        AddMissile(spells[i].var1, spells[i].var2,
                   spells[i].var1, spells[i].var2,
                   plr[myplr]._pdir, MIS_FIREWALL, 0, myplr, 0);

        spells[i].var1 += XDirAdd[spells[i].var3];
        spells[i].var2 += YDirAdd[spells[i].var3];
    }
    else
    {
        spells[i].var8 = 1;
    }

    // Process Side B
    dp = dPiece[spells[i].var5][spells[i].var6];
    if (nSolidTable[dp] == 0 && spells[i].var7 == 0)
    {
        AddMissile(spells[i].var5, spells[i].var6,
                   spells[i].var5, spells[i].var6,
                   plr[myplr]._pdir, MIS_FIREWALL, 0, myplr, 0);

        spells[i].var5 += XDirAdd[spells[i].var4];
        spells[i].var6 += YDirAdd[spells[i].var4];
    }
    else
    {
        spells[i].var7 = 1;
    }
}

// .text:00456A8A
static void process_infravision_spell(int i)
{
    spells[i].range--;
    plr[spells[i].source]._pInfraFlag = TRUE;
    if (spells[i].range == 0)
    {
        spells[i].delFlag = TRUE;
        CalcPlrItemVals(spells[i].source);
    }
}

// .text:00456B1C
// Scan one cell per tick inside a rectangle. Blow up any monsters found.
static void process_apoc_spell(int i)
{
    BOOL exit;
    int j, k;

    // var2: Y location of cell to check
    // var3: extent of Y to check
    // var4: X location of cell to check
    // var5: extent of X to check
    // var6: X at beginning of row

    exit = FALSE;
    for (j = spells[i].var2; spells[i].var3 > j && exit == FALSE; j++)
    {
        for (k = spells[i].var4; spells[i].var5 > k && exit == FALSE; k++)
        {
            if (dMonster[k][j] > 0)
            {
                AddMissile(k, j, k, j, plr[myplr]._pdir, MIS_BOOM, 0, myplr, spells[i].dam);
                exit = TRUE;
            }
        }

        if (exit == FALSE)
        {
            spells[i].var4 = spells[i].var6;
        }
    }

    if (exit == TRUE)
    {
        spells[i].var2 = j - 1;
        spells[i].var4 = k;
    }
    else
    {
        spells[i].delFlag = TRUE;
    }
}

// .text:00456CA4
// This function is not called by anyone
static void __dc_process_unk_spell_456CA4(int i)
{
    int dir;
    int x;
    int y;
    int pn;

    spells[i].range--;
    if (spells[i].range == 0)
    {
        spells[i].delFlag = TRUE;
        return;
    }

    if (spells[i].var4 != spells[i].var1 || spells[i].var5 != spells[i].var2)
    {
        spells[i].var3 = GetDirection(spells[i].var4, spells[i].var5, spells[i].var1, spells[i].var2);
        spells[i].var4 += XDirAdd[spells[i].var3];
        spells[i].var5 += YDirAdd[spells[i].var3];
        pn = dPiece[spells[i].var4][spells[i].var5];
        if (nSolidTable[pn] != 0)
        {
            spells[i].range++;
            spells[i].var4 -= 0 - XDirAdd[spells[i].var3];
            spells[i].var5 -= 0 - YDirAdd[spells[i].var3];
            spells[i].var1 = spells[i].var4;
            spells[i].var2 = spells[i].var5;
        }
        else
        {
            spells[i].range++;
            dir = spells[i].var8 >> 28;
            if (dir != 15)
            {
                // TODO
            }
            else
            {
                // TODO
            }

            // TODO
        }
    }
    else
    {
        x = spells[i].var4;
        y = spells[i].var5;
        if (spells[i].var8 & 0xF != 0xF)
        {
            dir = (spells[i].var8 + 4) & 0x7;
            spells[i].var4 = XDirAdd[dir] + x;
            spells[i].var5 = YDirAdd[dir] + y;
            spells[i].var1 = spells[i].var4;
            spells[i].var2 = spells[i].var5;
            spells[i].var8 >>= 4;
            spells[i].var8 = (spells[i].var8 & 0xF0000000);
        }
    }
}

// .text:00457686
void ProcessSpells()
{
    int si;
    int i;

    for (i = 0; i < numspells; i++)
    {
        si = spellsactive[i];
        switch (spells[si].type)
        {
        case SPL_FIREBOLT:
        case SPL_HEAL:
        case SPL_LIGHTNING:
        case SPL_FLASH:
        case SPL_IDENTIFY:
        case SPL_TOWN:
        case SPL_STONE:
        case SPL_RNDTELEPORT:
        case SPL_MANASHIELD:
        case SPL_FIREBALL:
        case SPL_GUARDIAN:
        case SPL_WAVE:
        case SPL_DOOMSERP:
        case SPL_BLODRIT:
        case SPL_NOVA:
        case SPL_INVISIBIL:
        case SPL_SENTINEL:
        case SPL_GOLEM:
        case SPL_BLODBOIL:
        case SPL_TELEPORT:
        case SPL_ETHEREALIZE:
        case SPL_REPAIR:
        case SPL_RECHARGE:
        case SPL_DISARM:
            // Don't process spells that either don't have logic after cast, or
            // will be handled instead by ProcessMissiles
            break;
        case SPL_INFRA:
            process_infravision_spell(si);
            break;
        case SPL_FIREWALL:
            process_firewall_spell(si);
            break;
        case SPL_CHAIN:
            // Yes this is empty.
            break;
        case SPL_APOCA:
            process_apoc_spell(si);
            break;
        }

        if (spells[si].animFlag)
        {
            if (spells[si].animDelay <= ++spells[si].animCnt)
            {
                spells[si].animCnt = 0;
                if (++spells[si].animFrame > spells[si].animLen)
                {
                    spells[si].animFrame = 1;
                }
            }
        }
    }

    i = 0;
    while (i < numspells)
    {
        si = spellsactive[i];
        if (spells[si].delFlag)
        {
            DeleteSpell(si, i);
            i = 0;
            continue;
        }
        else
        {
            i++;
        }
    }
}

// .text:0045787C
void FreeSpells()
{
    GlobalUnlock(GlobalHandle(pSpellLghningCel));
    GlobalFree(GlobalHandle(pSpellLghningCel));
}

// .text:004578B2
// Called by LoadSpell and some multiplayer code. Theorized to be actual
// graphics loading code that has since been removed (due to the migration from
// spells to missiles)
void sub_4578B2(int i)
{
    // This space intentionally left blank.
}
