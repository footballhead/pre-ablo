#include "items.h"

#include "control.h"
#include "effects.h"
#include "engine.h"
#include "player.h"

#include <stdio.h>

extern BOOL pinfoflag;
void AddPanelString(const char *str, BOOL just);
extern BOOL drawhpflag;
extern BOOL drawmanaflag;

//
// initialized variables (.data:004AAB20)
//

// (objcurs.cel frame - 8) => ItemDropNames index
BYTE ItemCAnimTbl[] = {
    20, 16, 0, 12, 12, 4, 4, 4, 2, 21, 12, 12, 12, 12, 12,
    12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 19, 22, 23,
    24, 25, 26, 27, 8, 8, 8, 8, 8, 6, 8, 8, 8, 6, 8, 8,
    6, 8, 8, 5, 9, 13, 13, 13, 0, 0, 5, 15, 5, 5, 18, 18,
    18, 0, 5, 5, 7, 1, 3, 17, 1, 15, 10, 14, 3, 11, 8, 0,
    1, 7, 0, 7, 6, 7, 3, 3, 3, 6, 6, 11, 11, 11, 0, 14,
    14, 14, 6, 6, 7, 3, 8, 14, 14, 14, 14, 0, 0, 1, 1, 1,
    1, 1, 7, 7, 7, 14, 14,
    // These two are past the end of objcurs.cel...
    4, 0};
const char *ItemDropNames[] = {
    "Armor2",
    "Axe",
    "FBttle",
    "Bow",
    "GoldFlip",
    "Helmut",
    "Mace",
    "Shield",
    "SwrdFlip",
    "Rock",
    "Cleaver",
    "Staff",
    "Ring",
    "CrownF",
    "LArmor",
    "WShield",
    "Scroll",
    "FPlateAr",
    "FBook",
    "Food",
    "FBttleBB",
    "FBttleDY",
    "FBttleOR",
    "FBttleBR",
    "FBttleBL",
    "FBttleBY",
    "FBttleWH",
    "FBttleDB"};
BYTE ItemAnimLs[] = {
    15,
    13,
    16,
    13,
    10,
    13,
    13,
    13,
    13,
    10,
    13,
    13,
    13,
    13,
    13,
    13,
    13,
    13,
    13,
    1,
    16,
    16,
    16,
    16,
    16,
    16,
    16,
    16};
// TODO: Sound enums
int ItemDropSnds[] = {
    IS_FHARM,
    IS_FAXE,
    IS_FPOT,
    IS_FBOW,
    IS_GOLD,
    IS_FCAP,
    IS_FSWOR,
    IS_FSHLD,
    IS_FSWOR,
    IS_MAGIC,
    IS_FAXE,
    IS_FSTAF,
    IS_FRING,
    IS_FCAP,
    IS_FLARM,
    IS_FSHLD,
    IS_FSCRL,
    IS_FHARM,
    IS_FLARM,
    IS_FLARM,
    IS_FPOT,
    IS_FPOT,
    IS_FPOT,
    IS_FPOT,
    IS_FPOT,
    IS_FPOT,
    IS_FPOT,
    IS_FPOT};
int ItemInvSnds[] = {
    IS_IHARM,
    IS_IAXE,
    IS_IPOT,
    IS_IBOW,
    IS_GOLD,
    IS_ICAP,
    IS_ISWORD,
    IS_ISHIEL,
    IS_ISWORD,
    IS_IGRAB,
    IS_IAXE,
    IS_ISTAF,
    IS_IRING,
    IS_ICAP,
    IS_ILARM,
    IS_ISHIEL,
    IS_ISCROL,
    IS_IHARM,
    IS_IHARM,
    IS_IHARM,
    IS_IPOT,
    IS_IPOT,
    IS_IPOT,
    IS_IPOT,
    IS_IPOT,
    IS_IPOT,
    IS_IPOT,
    IS_IPOT};

//
// uninitialized variables
//

// ???
ItemStruct curruitem;
BOOL uitemflag;
// ???
BYTE *itemanims[ITEMTYPES];
ItemStruct item[MAXITEMS];
// ???

//
// code (.data:0041C230)
//

// InitItemGFX	000000000041C230
// ItemPlace	000000000041C2BE
// AddInitItems	000000000041C3CE
// InitItems	000000000041C581
// CalcPlrItemVals	000000000041C736
// CalcPlrScrolls	000000000041D71F
// CalcPlrStaff	000000000041D868
// ItemMinStats	000000000041D8FD
// CalcPlrItemReqs	000000000041D9AC
// CalcPlrInv	000000000041DEBD
// CalcPlrItemDurs	000000000041DF08
// SetPlrHandItem	000000000041E060
// CreatePlrItems	000000000041E35D
// GetItemSpace	000000000041E58F
// GetBookSpell	000000000041EAAC
// GetStaffPower	000000000041EC27
// GetStaffSpell	000000000041F1E2
// GetItemAttrs	000000000041F43C
// RndPL	000000000041FB27
// PLVal	000000000041FB51
// SaveItemPower	000000000041FBC0
// GetItemPower	00000000004205CD
// GetItemBonus	0000000000420F52

// SetupItem	0000000000421087
void SetupItem(int i)
{

    int it;

    it = ItemCAnimTbl[item[i]._iCurs];
    item[i]._iAnimData = itemanims[it];
    item[i]._iAnimLen = ItemAnimLs[it];
    item[i]._iAnimFrame = 1;
    item[i]._iAnimFlag = TRUE;
    item[i]._iDelFlag = FALSE;
    item[i]._iAnimWidth = 96;
    item[i]._iAnimWidth2 = 16;
    item[i]._iSelFlag = 0;
    item[i]._iIdentified = FALSE;
    item[i]._iPostDraw = FALSE;
}

// RndItem	00000000004211B7
// RndUItem	0000000000421348
// RndAllItems	00000000004214FE
// RndTypeItems	00000000004215AF
// CheckUnique	00000000004216EB
// GetUniqueItem	00000000004218ED
// SpawnUnique	0000000000421CDC
// SpawnItem	0000000000421DAC
// CreateItem	0000000000422056
// CreateRndItem	0000000000422142
// CreateRndUseful	000000000042232B
// CreateTypeItem	000000000042249F
// SpawnRock	0000000000422688
// RespawnItem	0000000000422841
// DeleteItem	000000000042298C
// ProcessItems	00000000004229F1
// FreeItemGFX	0000000000422C65
// GetItemFrm	0000000000422CC6
// GetItemStr	0000000000422D1A
// DoIdentify	0000000000422E5C
// RepairItem	0000000000422FF4
// DoRepair	00000000004230B3
// RechargeItem	00000000004232A3
// DoRecharge	0000000000423336

// .text:0042352F
void PrintItemPower(char plidx, ItemStruct x)
{
    int light;

    switch (plidx)
    {
    case IPL_TOHIT:
    case IPL_TOHIT_CURSE:
        sprintf(tempstr, "To Hit : %+i%%", x._iPLToHit);
        break;
    case IPL_DAMP:
    case IPL_DAMP_CURSE:
        sprintf(tempstr, "Damage : %+i%%", x._iPLDam);
        break;
    case IPL_TOHIT_DAMP:
    case IPL_TOHIT_DAMP_CURSE:
        sprintf(tempstr, "Damage : %+i%%  To Hit : %+i%%", x._iPLDam, x._iPLToHit);
        break;
    case IPL_ACP:
    case IPL_ACP_CURSE:
        sprintf(tempstr, "Armor Class : %+i", x._iPLAC);
        break;
    case IPL_FIRERES:
        sprintf(tempstr, "Resist Fire : %+i%%", x._iPLFR);
        break;
    case IPL_LIGHTRES:
        sprintf(tempstr, "Resist Lightning : %+i%%", x._iPLLR);
        break;
    case IPL_MAGICRES:
        sprintf(tempstr, "Resist General Magic : %+i%%", x._iPLMR);
        break;
    case IPL_ALLRES:
        sprintf(tempstr, "Resist All : %+i%%", x._iPLFR);
        break;
    case IPL_SPLCOST:
        sprintf(tempstr, "Spell cost -%i%%", x._iPLSplCost);
        break;
    case IPL_SPLDUR:
        sprintf(tempstr, "Spell duration +%i%%", x._iPLSplDur);
        break;
    case IPL_SPLLVLADD:
        sprintf(tempstr, "All Spell levels +%i", x._iSplLvlAdd);
        break;
    case IPL_CHARGES:
        strcpy(tempstr, "Extra charges");
        break;
    case IPL_FIREDAM:
        strcpy(tempstr, "Fire hit (NW)");
        break;
    case IPL_LIGHTDAM:
        strcpy(tempstr, "Lightning hit (NW)");
        break;
    case IPL_RANDSPL:
        strcpy(tempstr, "Random spell every 5 min (NW)");
        break;
    case IPL_STR:
    case IPL_STR_CURSE:
        sprintf(tempstr, "Strength : %+i", x._iPLStr);
        break;
    case IPL_MAG:
    case IPL_MAG_CURSE:
        sprintf(tempstr, "Magic : %+i", x._iPLMag);
        break;
    case IPL_DEX:
    case IPL_DEX_CURSE:
        sprintf(tempstr, "Dexterity : %+i", x._iPLDex);
        break;
    case IPL_VIT:
    case IPL_VIT_CURSE:
        sprintf(tempstr, "Vitality : %+i", x._iPLVit);
        break;
    case IPL_ATTRIBS:
    case IPL_ATTRIBS_CURSE:
        sprintf(tempstr, "All attributes : %+i", x._iPLStr);
        break;
    case IPL_GETHIT_CURSE:
    case IPL_GETHIT:
        sprintf(tempstr, "Every hit you recieve : %+i", x._iPLGetHit);
        break;
    case IPL_LIFE:
    case IPL_LIFE_CURSE:
        sprintf(tempstr, "Hit Points : %+i", x._iPLHP >> 6);
        break;
    case IPL_MANA:
    case IPL_MANA_CURSE:
        sprintf(tempstr, "Mana : %+i", x._iPLMana >> 6);
        break;
    case IPL_DUR:
        strcpy(tempstr, "Increase in durability");
        break;
    case IPL_DUR_CURSE:
        strcpy(tempstr, "Decrease in durability");
        break;
    case IPL_INDESTRUCTIBLE:
        strcpy(tempstr, "Infinite durability");
        break;
    case IPL_LIGHT:
        light = 10 * x._iPLLight;
        sprintf(tempstr, "Increases light radius %i%%", light);
        break;
    case IPL_LIGHT_CURSE:
        light = -(10 * x._iPLLight);
        sprintf(tempstr, "Decreases light radius %i%%", light);
        break;
    case IPL_INVIS:
        sprintf(tempstr, "Invisibility (NW)", light);
        break;
    case IPL_MULT_ARROWS:
        sprintf(tempstr, "Increase arrows per shot (NW)");
        break;
    case IPL_FIRE_ARROWS:
        sprintf(tempstr, "Fire arrows");
        break;
    case IPL_LIGHT_ARROWS:
        sprintf(tempstr, "Lightning arrows (NW)");
        break;
    case IPL_HOMING_ARROWS:
        sprintf(tempstr, "Homing arrows (NW)");
        break;
    case IPL_THORNS:
        strcpy(tempstr, "You get hurt each use (NW)");
        break;
    case IPL_NOMANA:
        strcpy(tempstr, "Lose all mana, no regen (NW)");
        break;
    case IPL_NOHEALPLR:
        strcpy(tempstr, "You can't heal (NW)");
        break;
    case IPL_SCARE:
        strcpy(tempstr, "Scare monster away (NW)");
        break;
    case IPL_DOUBLE_STRIKE:
        strcpy(tempstr, "Attacks square twice (NW)");
        break;
    case IPL_EXP_DAM:
        strcpy(tempstr, "Exponential damage (NW)");
        break;
    case IPL_SEE_INVIS:
        strcpy(tempstr, "See invisible (NW)");
        break;
    case IPL_ABSHALFTRAP:
        strcpy(tempstr, "Half trap damage (NW)");
        break;
    case IPL_KNOCKBACK:
        strcpy(tempstr, "Knock enemies back (NW)");
        break;
    case IPL_NOHEALMON:
        strcpy(tempstr, "Enemy stops healing (NW)");
        break;
    case IPL_DAMMOD:
        sprintf(tempstr, "+%i damage to each hit", x._iPLDamMod);
        break;
    case IPL_SETDAM:
        sprintf(tempstr, "Unusual item damage");
        break;
    case IPL_SETDUR:
        strcpy(tempstr, "Enhanced durability");
        break;
    case IPL_RNDARROWVEL:
        strcpy(tempstr, "Fires random speed arrows");
        break;
    case IPL_DRAINLIFE:
        strcpy(tempstr, "You constantly lose hit points");
        break;
    default:
        strcpy(tempstr, "Another ability (NW)");
        break;
    }
}

// DrawUniqueInfo	0000000000423BA4

// .text:00423E53
void PrintItemDetails(ItemStruct x)
{
    if (x._iMagical != ITEM_QUALITY_UNIQUE && x._iPrePower != -1)
    {
        PrintItemPower(x._iPrePower, x);
        AddPanelString(tempstr, TRUE);
    }

    if (x._iSufPower != -1)
    {
        PrintItemPower(x._iSufPower, x);
        AddPanelString(tempstr, TRUE);
    }

    if (x._iMagical == ITEM_QUALITY_UNIQUE)
    {
        AddPanelString("unique item", TRUE);
        uitemflag = TRUE;
        curruitem = x;
    }

    if (x._iMiscID == IMISC_STAFF && x._iMaxCharges != 0)
    {
        sprintf(tempstr, "Charges : %i/%i", x._iCharges, x._iMaxCharges);
        AddPanelString(tempstr, TRUE);
    }

    if (x._iClass == ICLASS_WEAPON || x._iClass == ICLASS_ARMOR)
    {
        if (x._iMaxDur == 1000)
        {
            strcpy(tempstr, "Indestructible");
        }
        else
        {
            sprintf(tempstr, "Durability : %i/%i", x._iDurability, x._iMaxDur);
        }
        AddPanelString(tempstr, TRUE);
    }

    if (x._iMinStr + x._iMinMag + x._iMinDex)
    {
        strcpy(tempstr, "Required :");
        if (x._iMinStr)
        {
            sprintf(tempstr, "%s %i Str", tempstr, x._iMinStr);
        }
        if (x._iMinMag)
        {
            sprintf(tempstr, "%s %i Mag", tempstr, x._iMinMag);
        }
        if (x._iMinDex)
        {
            sprintf(tempstr, "%s %i Dex", tempstr, x._iMinDex);
        }
        AddPanelString(tempstr, TRUE);
    }

    pinfoflag = TRUE;
}

// PrintItemDur	00000000004240B2

// .text:00424284
void UseItem(int p, int Mid, int spl)
{
    int rnd, j;

    switch (Mid)
    {
    case IMISC_HEAL:
    case IMISC_FOOD:
        j = plr[p]._pMaxHP >> 8;
        j = ((j >> 1) + random_(j)) << 6;
        if (plr[p]._pClass == PC_WARRIOR)
        {
            j <<= 1;
        }
        if (plr[p]._pClass == PC_ROGUE)
        {
            j += j >> 1;
        }
        plr[p]._pHitPoints += j;
        if (plr[p]._pMaxHP < plr[p]._pHitPoints)
        {
            plr[p]._pHitPoints = plr[p]._pMaxHP;
        }
        plr[p]._pHPBase += j;
        if (plr[p]._pHPBase > plr[p]._pMaxHPBase)
        {
            plr[p]._pHPBase = plr[p]._pMaxHPBase;
        }
        drawhpflag = TRUE;
        break;
    case IMISC_FULLHEAL:
        plr[p]._pHitPoints = plr[p]._pMaxHP;
        plr[p]._pHPBase = plr[p]._pMaxHPBase;
        drawhpflag = TRUE;
        break;
    case IMISC_MANA:
        j = plr[p]._pMaxMana >> 8;
        j = ((j >> 1) + random_(j)) << 6;
        if (plr[p]._pClass == PC_SORCERER)
        {
            j <<= 1;
        }
        if (plr[p]._pClass == PC_ROGUE)
        {
            j += j >> 1;
        }
        plr[p]._pMana += j;
        if (plr[p]._pMana > plr[p]._pMaxMana)
        {
            plr[p]._pMana = plr[p]._pMaxMana;
        }
        plr[p]._pManaBase += j;
        if (plr[p]._pManaBase > plr[p]._pMaxManaBase)
        {
            plr[p]._pManaBase = plr[p]._pMaxManaBase;
        }
        drawmanaflag = TRUE;
        break;
    case IMISC_FULLMANA:
        plr[p]._pMana = plr[p]._pMaxMana;
        plr[p]._pManaBase = plr[p]._pMaxManaBase;
        drawmanaflag = TRUE;
        break;
    case IMISC_POTEXP:
        AddPlrExperience(p, plr[p]._pLevel, plr[p]._pExperience);
        break;
    case IMISC_ELIXSTR:
        rnd = random_(3) + 1;
        plr[p]._pStrength += rnd;
        plr[p]._pBaseStr += rnd;
        break;
    case IMISC_ELIXMAG:
        rnd = random_(3) + 1;
        plr[p]._pMagic += rnd;
        plr[p]._pBaseMag += rnd;
        break;
    case IMISC_ELIXDEX:
        rnd = random_(3) + 1;
        plr[p]._pDexterity += rnd;
        plr[p]._pBaseDex += rnd;
        break;
    case IMISC_ELIXVIT:
        rnd = random_(3) + 1;
        plr[p]._pVitality += rnd;
        plr[p]._pBaseVit += rnd;
        break;
    case IMISC_ELIXWEAK:
        rnd = random_(3) + 1;
        plr[p]._pStrength -= rnd;
        plr[p]._pBaseStr -= rnd;
        break;
    case IMISC_ELIXDIS:
        rnd = random_(3) + 1;
        plr[p]._pMagic -= rnd;
        plr[p]._pBaseMag -= rnd;
        break;
    case IMISC_ELIXCLUM:
        rnd = random_(3) + 1;
        plr[p]._pDexterity -= rnd;
        plr[p]._pBaseDex -= rnd;
        break;
    case IMISC_ELIXSICK:
        rnd = random_(3) + 1;
        plr[p]._pVitality -= rnd;
        plr[p]._pBaseVit -= rnd;
        break;
    case IMISC_BOOK:
        plr[p]._pMemSpells |= SPELLBIT(spl);
        plr[p]._pSplLvl[spl]++;
        plr[p]._pMana += spelldata_sManaCost[spl] << 6;
        if (plr[p]._pMana > plr[p]._pMaxMana)
        {
            plr[p]._pMana = plr[p]._pMaxMana;
        }
        plr[p]._pManaBase += spelldata_sManaCost[spl] << 6;
        if (plr[p]._pManaBase > plr[p]._pMaxManaBase)
        {
            plr[p]._pManaBase = plr[p]._pMaxManaBase;
        }
        drawmanaflag = TRUE;
        break;
    }
}

// SmithItemOk	0000000000424C0F
// RndSmithItem	0000000000424CA6
// SpawnSmith	0000000000424D67
// WitchItemOk	0000000000424FC7
// RndWitchItem	0000000000425058
// SpawnWitch	0000000000425119
// RndBoyItem	00000000004253AD
// SpawnBoy	000000000042545B
