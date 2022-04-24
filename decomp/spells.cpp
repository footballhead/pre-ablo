#include "engine.h"
#include "enums.h"
#include "player.h"
#include "structs.h"

//
// initialized vars (.data:004B8218)
//

int XDirAdd[8] = { 1, 0, -1, -1, -1, 0, 1, 1 };
int YDirAdd[8] = { 1, 1, 1, 0, -1, -1, -1, 0 };

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
    0
};
int spelldata_sType[] = 
{
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
const char* spelldata_sNameText[] = {
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
    "Trap Disarm"
};
const char* spelldata_sSkillText[] = {
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
    "Trap Disarm"
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
    -1
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
    -1
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
    -1
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
    62
};

//
// uninitialized vars (.data:005FF2E8)
//

SpellStruct spells[MAX_SPELLS];
BYTE *pSpellLghningCel;
int nSpells[MAX_SPELLS];
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
        spells[i].oldx = 0;
        spells[i].oldy = 0;
        spells[i].dir = 0;
        spells[i].nextx = 0;
        spells[i].delFlag = 0;
        availspells[i] = i;
        nSpells[i] = 0;
    }
}

// .text:00453C0D
void AddSpellStats(int animdata, int i, int arg_0, int arg_4, int width, int arg_c)
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

// GetManaAdjust	0000000000453D32
int GetManaAdjust(int pnum, int spell)
{
    int spllvl;
    int cost;

    spllvl = GetSpellLevel(pnum, spell);
    switch (spell) {
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

// UseMana	0000000000453F9F
// __dc_spells_check_col	00000000004540ED
// CheckSpell	00000000004541CF
// DeleteSpell	000000000045428B
// cast_firebolt	00000000004542F1
// cast_healing	00000000004543D2
// cast_lightning	00000000004545F8
// cast_identify	0000000000454677
// cast_firewall	00000000004546FA
// cast_town	00000000004548DA
// cast_flash	0000000000454959
// cast_infravision	0000000000454A11
// cast_phasing	0000000000454AE2
// cast_manashield	0000000000454C38
// cast_flamewave	0000000000454CB7
// cast_fireball	0000000000454FB0
// cast_chainlightning	000000000045502F
// cast_sentinal	00000000004550AE
// cast_doomserpents	000000000045512D
// cast_nova	00000000004551BC
// cast_bloodboil	00000000004554C3
// cast_teleport	0000000000455829
// cast_itemrepair	00000000004559B4
// cast_staffrecharge	0000000000455A37
// cast_trapdisarm	0000000000455ABA
// cast_apoc	0000000000455B26
// cast_guardian	0000000000455D0B
// cast_stonecurse	0000000000455D8A
// cast_invis	0000000000455E09
// cast_blood_ritual	0000000000455E98
// cast_golem	00000000004560B2
// cast_ethereal	0000000000456141
// CastSpell	00000000004561D0
// __dc_spells_hit_monst	0000000000456644
// process_firewall_spell	00000000004567CC
// process_infravision_spell	0000000000456A8A
// process_apoc_spell	0000000000456B1C
// __dc_process_unk_spell_456CA4	0000000000456CA4
// ProcessSpells	0000000000457686
// FreeSpells	000000000045787C
// __nullsub_SetSpellGFX	00000000004578B2
