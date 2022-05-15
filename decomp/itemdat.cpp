#include "itemdat.h"

PLStruct PL_Prefix[] = {
{TRUE,  "Tin",                  IPL_TOHIT_CURSE,        6,      10,     3,      PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   FALSE,  0,      0,      -3,     },
{TRUE,  "Brass",                IPL_TOHIT_CURSE,        1,      5,      1,      PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   FALSE,  0,      0,      -2,     },
{TRUE,  "Bronze",               IPL_TOHIT,              1,      5,      1,      PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   TRUE,   0,      0,      2,      },
{TRUE,  "Iron",                 IPL_TOHIT,              6,      10,     3,      PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   TRUE,   0,      0,      3,      },
{TRUE,  "Steel",                IPL_TOHIT,              11,     15,     5,      PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   TRUE,   0,      0,      5,      },
{TRUE,  "Silver",               IPL_TOHIT,              16,     20,     8,      PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_GOOD,       TRUE,   TRUE,   0,      0,      7,      },
{TRUE,  "Gold",                 IPL_TOHIT,              21,     30,     11,     PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_GOOD,       TRUE,   TRUE,   0,      0,      9,      },
{TRUE,  "Platinum",             IPL_TOHIT,              31,     40,     15,     PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_GOOD,       TRUE,   TRUE,   0,      0,      11,     },
{TRUE,  "Mithril",              IPL_TOHIT,              41,     60,     19,     PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_GOOD,       TRUE,   TRUE,   0,      0,      13,     },
{TRUE,  "Metioric",             IPL_TOHIT,              61,     100,    23,     PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   TRUE,   0,      0,      15,     },
{TRUE,  "Useless",              IPL_DAMP_CURSE,         100,    100,    5,                 PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   FALSE,  0,      0,      -8,     },
{TRUE,  "Bent",                 IPL_DAMP_CURSE,         50,     75,     3,                 PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   FALSE,  0,      0,      -4,     },
{TRUE,  "Weak",                 IPL_DAMP_CURSE,         25,     45,     1,                 PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   FALSE,  0,      0,      -3,     },
{TRUE,  "Jagged",               IPL_DAMP,               25,     45,     1,                 PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   TRUE,   0,      0,      3,      },
{TRUE,  "Deadly",               IPL_DAMP,               50,     70,     3,                 PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   TRUE,   0,      0,      4,      },
{TRUE,  "Heavy",                IPL_DAMP,               75,     95,     5,                 PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   TRUE,   0,      0,      5,      },
{TRUE,  "Vicious",              IPL_DAMP,               100,    145,    9,                 PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_EVIL,       TRUE,   TRUE,   0,      0,      8,      },
{TRUE,  "Brutal",               IPL_DAMP,               150,    195,    14,                PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   TRUE,   0,      0,      10,     },
{TRUE,  "Massive",              IPL_DAMP,               200,    245,    19,                PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   TRUE,   0,      0,      13,     },
{TRUE,  "Savage",               IPL_DAMP,               250,    300,    25,                PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   TRUE,   0,      0,      15,     },
{TRUE,  "Clumsy",               IPL_TOHIT_DAMP_CURSE,   50,     75,     5,                 PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   FALSE,  0,      0,      -7,     },
{TRUE,  "Dull",                 IPL_TOHIT_DAMP_CURSE,   25,     45,     1,                 PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   FALSE,  0,      0,      -5,     },
{TRUE,  "Sharp",                IPL_TOHIT_DAMP,         25,     45,     1,                 PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   FALSE,  0,      0,      5,      },
{TRUE,  "Fine",                 IPL_TOHIT_DAMP,         50,     70,     5,                 PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   TRUE,   0,      0,      7,      },
{TRUE,  "Warrior's",            IPL_TOHIT_DAMP,         75,     95,     10,                PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   TRUE,   0,      0,      13,     },
{TRUE,  "Soldier's",            IPL_TOHIT_DAMP,         100,    145,    15,                PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   TRUE,   0,      0,      17,     },
{TRUE,  "Lord's",               IPL_TOHIT_DAMP,         150,    195,    20,                PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   TRUE,   0,      0,      21,     },
{TRUE,  "Knight's",             IPL_TOHIT_DAMP,         200,    245,    25,                PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   TRUE,   0,      0,      26,     },
{TRUE,  "Master's",             IPL_TOHIT_DAMP,         250,    300,    30,                PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        TRUE,   TRUE,   0,      0,      30,     },
{TRUE,  "Vulnerable",           IPL_ACP_CURSE,          6,      10,     3,      PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_ANY,        TRUE,   FALSE,  0,      0,      -3,     },
{TRUE,  "Dilapidated",          IPL_ACP_CURSE,          1,      5,      1,      PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_ANY,        TRUE,   FALSE,  0,      0,      -2,     },
{TRUE,  "Sturdy",               IPL_ACP,                1,      5,      1,      PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_ANY,        TRUE,   TRUE,   0,      0,      2,      },
{TRUE,  "Strong",               IPL_ACP,                6,      10,     3,      PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_ANY,        TRUE,   TRUE,   0,      0,      3,      },
{TRUE,  "Mighty",               IPL_ACP,                11,     15,     5,      PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_ANY,        TRUE,   TRUE,   0,      0,      5,      },
{TRUE,  "Valient",              IPL_ACP,                16,     20,     9,      PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_ANY,        TRUE,   TRUE,   0,      0,      7,      },
{TRUE,  "Glorious",             IPL_ACP,                21,     30,     13,     PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_GOOD,       TRUE,   TRUE,   0,      0,      9,      },
{TRUE,  "Blessed",              IPL_ACP,                31,     40,     18,     PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_GOOD,       TRUE,   TRUE,   0,      0,      11,     },
{TRUE,  "Saintly",              IPL_ACP,                41,     60,     23,     PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_GOOD,       TRUE,   TRUE,   0,      0,      13,     },
{TRUE,  "Miraculous",           IPL_ACP,                61,     100,    28,     PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_GOOD,       TRUE,   TRUE,   0,      0,      15,     },
{TRUE,  "Red",                  IPL_FIRERES,            10,     25,     2,      PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   1000,   2500,   1,      },
{TRUE,  "Crimson",              IPL_FIRERES,            26,     40,     8,      PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   2600,   4000,   1,      },
{TRUE,  "Burgandy",             IPL_FIRERES,            41,     60,     14,     PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   4100,   6000,   1,      },
{TRUE,  "Garnet",               IPL_FIRERES,            61,     95,     20,     PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   6100,   9500,   1,      },
{TRUE,  "Ruby",                 IPL_FIRERES,            100,    100,    26,     PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   20000,  20000,  1,      },
{TRUE,  "Blue",                 IPL_LIGHTRES,           10,     25,     2,      PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   1000,   2500,   1,      },
{TRUE,  "Azure",                IPL_LIGHTRES,           26,     40,     8,      PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   2600,   4000,   1,      },
{TRUE,  "Lapis",                IPL_LIGHTRES,           41,     60,     14,     PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   4100,   6000,   1,      },
{TRUE,  "Cobalt",               IPL_LIGHTRES,           61,     95,     20,     PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   6100,   9500,   1,      },
{TRUE,  "Sapphire",             IPL_LIGHTRES,           100,    100,    26,     PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   20000,  20000,  1,      },
{TRUE,  "White",                IPL_MAGICRES,           10,     25,     2,      PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   1000,   2500,   1,      },
{TRUE,  "Pearl",                IPL_MAGICRES,           26,     40,     8,      PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   2600,   4000,   1,      },
{TRUE,  "Ivory",                IPL_MAGICRES,           41,     60,     14,     PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   4100,   6000,   1,      },
{TRUE,  "Crystal",              IPL_MAGICRES,           61,     95,     20,     PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   6100,   9500,   1,      },
{TRUE,  "Diamond",              IPL_MAGICRES,           100,    100,    26,     PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   20000,  20000,  1,      },
{TRUE,  "Topaz",                IPL_ALLRES,             10,     25,     5,      PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   3000,   7500,   1,      },
{TRUE,  "Amber",                IPL_ALLRES,             26,     40,     10,     PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   7800,   12000,  1,      },
{TRUE,  "Jade",                 IPL_ALLRES,             41,     60,     17,     PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   12300,  18000,  1,      },
{TRUE,  "Obsidian",             IPL_ALLRES,             61,     95,     24,     PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   18300,  28500,  1,      },
{TRUE,  "Emerald",              IPL_ALLRES,             100,    100,    31,     PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   60000,  60000,  1,      },
{TRUE,  "Monk's",               IPL_SPLCOST,            10,     10,     3,                           PLT_STAFF,                                     GOE_ANY,        FALSE,  TRUE,   600,    600,    1,      },
{TRUE,  "Priest's",             IPL_SPLCOST,            25,     25,     8,                           PLT_STAFF,                                     GOE_ANY,        FALSE,  TRUE,   1500,   1500,   1,      },
{TRUE,  "Bishop's",             IPL_SPLCOST,            33,     33,     15,                          PLT_STAFF,                                     GOE_GOOD,       FALSE,  TRUE,   2000,   2000,   1,      },
{TRUE,  "Cardinal's",           IPL_SPLCOST,            50,     50,     22,                          PLT_STAFF,                                     GOE_GOOD,       FALSE,  TRUE,   3000,   3000,   1,      },
{TRUE,  "Glowing",              IPL_SPLDUR,             25,     25,     2,                           PLT_STAFF,                                     GOE_ANY,        FALSE,  TRUE,   1000,   1000,   1,      },
{TRUE,  "Cryptic",              IPL_SPLDUR,             50,     50,     7,                           PLT_STAFF,                                     GOE_ANY,        FALSE,  TRUE,   1500,   1500,   1,      },
{TRUE,  "Arcane",               IPL_SPLDUR,             100,    100,    12,                          PLT_STAFF,                                     GOE_ANY,        FALSE,  TRUE,   3000,   3000,   1,      },
{TRUE,  "Magestic",             IPL_SPLDUR,             200,    200,    20,                          PLT_STAFF,                                     GOE_ANY,        FALSE,  TRUE,   5000,   5000,   1,      },
{TRUE,  "Angel's",              IPL_SPLLVLADD,          1,      1,      15,     PLT_MISC |           PLT_STAFF,                                     GOE_GOOD,       FALSE,  TRUE,   2500,   2500,   1,      },
{TRUE,  "Arch-Angel's",         IPL_SPLLVLADD,          2,      2,      25,     PLT_MISC |           PLT_STAFF,                                     GOE_GOOD,       FALSE,  TRUE,   5000,   5000,   1,      },
{TRUE,  "Plentiful",            IPL_CHARGES,            2,      2,      4,                           PLT_STAFF,                                     GOE_ANY,        FALSE,  TRUE,   2000,   2000,   1,      },
{TRUE,  "Bountiful",            IPL_CHARGES,            3,      3,      9,                           PLT_STAFF,                                     GOE_ANY,        FALSE,  TRUE,   3000,   3000,   1,      },
{FALSE, "Flaming",              IPL_FIREDAM,            1,      10,     7,                                       PLT_WEAP,                          GOE_ANY,        FALSE,  TRUE,   1500,   1500,   1,      },
{FALSE, "Lightning",            IPL_LIGHTDAM,           2,      20,     18,                          PLT_STAFF,                                     GOE_ANY,        FALSE,  TRUE,   2500,   2500,   1,      },
{FALSE, "Chaos",                IPL_RANDSPL,            0,      0,      13,                          PLT_STAFF,                                     GOE_ANY,        FALSE,  TRUE,   1000,   1000,   1,      },
{FALSE, "",                     IPL_INVALID,            0,      0,      0,      0,                                                                  GOE_ANY,        FALSE,  FALSE,  0,      0,      0,      },
};

PLStruct PL_Suffix[] = {
{TRUE,  "quality",              IPL_DAMMOD,             1,      2,      2,      PLT_MISC | PLT_BOW |             PLT_WEAP,                          GOE_ANY,        FALSE,  TRUE,   250,    500,    1,      },
{TRUE,  "maiming",              IPL_DAMMOD,             3,      5,      6,      PLT_MISC | PLT_BOW |             PLT_WEAP,                          GOE_ANY,        FALSE,  TRUE,   750,    1250,   1,      },
{TRUE,  "slaying",              IPL_DAMMOD,             6,      10,     12,     PLT_MISC | PLT_BOW |             PLT_WEAP,                          GOE_ANY,        FALSE,  TRUE,   1500,   2500,   1,      },
{TRUE,  "gore",                 IPL_DAMMOD,             11,     20,     24,     PLT_MISC | PLT_BOW |             PLT_WEAP,                          GOE_ANY,        FALSE,  TRUE,   2750,    5000,  1,      },
{TRUE,  "pain",                 IPL_GETHIT_CURSE,       2,      4,      4,      PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_EVIL,       FALSE,  FALSE,  0,      0,      -4,     },
{TRUE,  "tears",                IPL_GETHIT_CURSE,       1,      1,      2,      PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_EVIL,       FALSE,  FALSE,  0,      0,      -2,     },
{TRUE,  "health",               IPL_GETHIT,             1,      1,      2,      PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_GOOD,       FALSE,  TRUE,   0,      0,      2,      },
{TRUE,  "protection",           IPL_GETHIT,             2,      4,      6,      PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_GOOD,       FALSE,  TRUE,   0,      0,      4,      },
{TRUE,  "absorbtion",           IPL_GETHIT,             5,      10,     12,     PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_GOOD,       FALSE,  TRUE,   0,      0,      10,     },
{TRUE,  "deflection",           IPL_GETHIT,             10,     20,     20,     PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_GOOD,       FALSE,  TRUE,   0,      0,      15,     },
{TRUE,  "frailty",              IPL_STR_CURSE,          6,      10,     3,      PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_EVIL,       FALSE,  FALSE,  -1800,  -3000,  1,      },
{TRUE,  "weakness",             IPL_STR_CURSE,          1,      5,      1,      PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_EVIL,       FALSE,  FALSE,  -300,   -1500,  1,      },
{TRUE,  "strength",             IPL_STR,                1,      5,      1,      PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   300,    1500,   1,      },
{TRUE,  "might",                IPL_STR,                6,      10,     5,      PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   1800,   3000,   1,      },
{TRUE,  "power",                IPL_STR,                11,     15,     11,     PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   3300,   4500,   1,      },
{TRUE,  "the giant",            IPL_STR,                16,     25,     17,     PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   4800,   7500,   1,      },
{TRUE,  "the titan",            IPL_STR,                26,     50,     23,     PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   7800,   15000,  1,      },
{TRUE,  "paralysis",            IPL_DEX_CURSE,          6,      10,     3,      PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_EVIL,       FALSE,  FALSE,  -1800,  -3000,  1,      },
{TRUE,  "atrophy",              IPL_DEX_CURSE,          1,      5,      1,      PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_EVIL,       FALSE,  FALSE,  -300,   -1500,  1,      },
{TRUE,  "dexterity",            IPL_DEX,                1,      5,      1,      PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   300,    1500,   1,      },
{TRUE,  "proficiency",          IPL_DEX,                6,      10,     5,      PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   1800,   3000,   1,      },
{TRUE,  "accuracy",             IPL_DEX,                11,     15,     11,     PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   3300,   4500,   1,      },
{TRUE,  "precision",            IPL_DEX,                16,     25,     17,     PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   4800,   7500,   1,      },
{TRUE,  "perfection",           IPL_DEX,                26,     50,     23,     PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   7800,   15000,  1,      },
{TRUE,  "draining",             IPL_MAG_CURSE,          6,      10,     3,      PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_EVIL,       FALSE,  FALSE,  -1800,  -3000,  1,      },
{TRUE,  "dyslexia",             IPL_MAG_CURSE,          1,      5,      1,      PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_EVIL,       FALSE,  FALSE,  -300,   -1500,  1,      },
{TRUE,  "magic",                IPL_MAG,                1,      5,      1,      PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   300,    1500,   1,      },
{TRUE,  "the mind",             IPL_MAG,                6,      10,     5,      PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   1800,   3000,   1,      },
{TRUE,  "brilliance",           IPL_MAG,                11,     15,     11,     PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   3300,   4500,   1,      },
{TRUE,  "sorcery",              IPL_MAG,                16,     25,     17,     PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   4800,   7500,   1,      },
{TRUE,  "wizardry",             IPL_MAG,                26,     50,     23,     PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   7800,   15000,  1,      },
{TRUE,  "decrepitude",          IPL_VIT_CURSE,          6,      10,     3,      PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_EVIL,       FALSE,  FALSE,  -1800,  -3000,  1,      },
{TRUE,  "disease",              IPL_VIT_CURSE,          1,      5,      1,      PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_EVIL,       FALSE,  FALSE,  -300,   -1500,  1,      },
{TRUE,  "vitality",             IPL_VIT,                1,      5,      1,      PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_GOOD,       FALSE,  TRUE,   300,    1500,   1,      },
{TRUE,  "zest",                 IPL_VIT,                6,      10,     5,      PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_GOOD,       FALSE,  TRUE,   1800,   3000,   1,      },
{TRUE,  "vim",                  IPL_VIT,                11,     15,     11,     PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_GOOD,       FALSE,  TRUE,   3300,   4500,   1,      },
{TRUE,  "vigor",                IPL_VIT,                16,     25,     17,     PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_GOOD,       FALSE,  TRUE,   4800,   7500,   1,      },
{TRUE,  "life",                 IPL_VIT,                26,     50,     23,     PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_GOOD,       FALSE,  TRUE,   7800,   15000,  1,      },
{TRUE,  "trouble",              IPL_ATTRIBS_CURSE,      6,      10,     12,     PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_EVIL,       FALSE,  FALSE,  -3100,  -3500,  1,      },
{TRUE,  "the pit",              IPL_ATTRIBS_CURSE,      1,      5,      5,      PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_EVIL,       FALSE,  FALSE,  -2600,  -3000,  1,      },
{TRUE,  "the sky",              IPL_ATTRIBS,            1,      5,      5,      PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   1200,   6000,   1,      },
{TRUE,  "the moon",             IPL_ATTRIBS,            6,      10,     11,     PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   7200,   12000,  1,      },
{TRUE,  "the stars",            IPL_ATTRIBS,            11,     15,     17,     PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   13200,  18000,  1,      },
{TRUE,  "the heavans",          IPL_ATTRIBS,            16,     25,     25,     PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   19200,  30000,  1,      },
{TRUE,  "the zodiac",           IPL_ATTRIBS,            26,     50,     30,     PLT_MISC | PLT_BOW |             PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   31200,  60000,  1,      },
{TRUE,  "the vulture",          IPL_LIFE_CURSE,         6,      10,     4,      PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_EVIL,       FALSE,  FALSE,  0,      0,      -4,     },
{TRUE,  "the jackal",           IPL_LIFE_CURSE,         1,      5,      1,      PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_EVIL,       FALSE,  FALSE,  0,      0,      -2,     },
{TRUE,  "the fox",              IPL_LIFE,               1,      5,      1,      PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   0,      0,      2,      },
{TRUE,  "the jaguar",           IPL_LIFE,               6,      10,     3,      PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   0,      0,      4,      },
{TRUE,  "the eagle",            IPL_LIFE,               11,     15,     7,      PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   0,      0,      6,      },
{TRUE,  "the wolf",             IPL_LIFE,               16,     25,     13,     PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   0,      0,      8,      },
{TRUE,  "the bear",             IPL_LIFE,               26,     40,     19,     PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   0,      0,      10,     },
{TRUE,  "the lion",             IPL_LIFE,               41,     100,    27,     PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   0,      0,      12,     },
{TRUE,  "the spider",           IPL_MANA_CURSE,         6,      10,     4,      PLT_MISC,                                                           GOE_EVIL,       FALSE,  FALSE,  0,      0,      -4,     },
{TRUE,  "the frog",             IPL_MANA_CURSE,         1,      5,      1,      PLT_MISC,                                                           GOE_EVIL,       FALSE,  FALSE,  0,      0,      -2,     },
{TRUE,  "the hyena",            IPL_MANA,               1,      5,      1,      PLT_MISC |           PLT_STAFF,                                     GOE_ANY,        FALSE,  TRUE,   0,      0,      2,      },
{TRUE,  "the raven",            IPL_MANA,               6,      10,     3,      PLT_MISC |           PLT_STAFF,                                     GOE_ANY,        FALSE,  TRUE,   0,      0,      4,      },
{TRUE,  "the snake",            IPL_MANA,               11,     15,     7,      PLT_MISC |           PLT_STAFF,                                     GOE_ANY,        FALSE,  TRUE,   0,      0,      6,      },
{TRUE,  "the serpant",          IPL_MANA,               16,     25,     13,     PLT_MISC |           PLT_STAFF,                                     GOE_ANY,        FALSE,  TRUE,   0,      0,      8,      },
{TRUE,  "the drake",            IPL_MANA,               26,     40,     19,     PLT_MISC |           PLT_STAFF,                                     GOE_ANY,        FALSE,  TRUE,   0,      0,      10,     },
{TRUE,  "the dragon",           IPL_MANA,               41,     100,    27,     PLT_MISC |           PLT_STAFF,                                     GOE_ANY,        FALSE,  TRUE,   0,      0,      12,     },
{TRUE,  "fragility",            IPL_DUR_CURSE,          100,    100,    3,      PLT_WEAP |                                  PLT_SHLD | PLT_ARMO,    GOE_EVIL,       FALSE,  FALSE,  0,      0,      -15,    },
{TRUE,  "brittleness",          IPL_DUR_CURSE,          26,     75,     1,      PLT_WEAP |                                  PLT_SHLD | PLT_ARMO,    GOE_EVIL,       FALSE,  FALSE,  0,      0,      -5,     },
{TRUE,  "sturdiness",           IPL_DUR,                26,     75,     1,      PLT_WEAP |                                  PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   0,      0,      5,      },
{TRUE,  "craftsmanship",        IPL_DUR,                51,     100,    6,      PLT_WEAP |                                  PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   0,      0,      10,     },
{TRUE,  "structure",            IPL_DUR,                101,    200,    12,     PLT_WEAP |                                  PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   0,      0,      15,     },
{TRUE,  "indestructible",       IPL_INDESTRUCTIBLE,     0,      0,      25,     PLT_WEAP |                                  PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   0,      0,      30,     },
{TRUE,  "the dark",             IPL_LIGHT_CURSE,        4,      4,      6,      PLT_MISC |                       PLT_WEAP |            PLT_ARMO,    GOE_EVIL,       FALSE,  FALSE,  -1500,  -1500,  1,      },
{TRUE,  "the night",            IPL_LIGHT_CURSE,        2,      2,      3,      PLT_MISC |                       PLT_WEAP |            PLT_ARMO,    GOE_EVIL,       FALSE,  FALSE,  -750,   -750,   1,      },
{TRUE,  "light",                IPL_LIGHT,              2,      2,      4,      PLT_MISC |                       PLT_WEAP |            PLT_ARMO,    GOE_GOOD,       FALSE,  TRUE,   750,    750,    1,      },
{TRUE,  "radiance",             IPL_LIGHT,              4,      4,      8,      PLT_MISC |                       PLT_WEAP |            PLT_ARMO,    GOE_GOOD,       FALSE,  TRUE,   1500,   1500,   1,      },
{FALSE, "stealth",              IPL_INVIS,              7,      7,      7,      PLT_MISC |                                             PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   1000,   1000,   1,      },
{FALSE, "hiding",               IPL_INVIS,              5,      5,      15,     PLT_MISC |                                             PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   3000,   3000,   1,      },
{FALSE, "obfuscation",          IPL_INVIS,              3,      3,      22,     PLT_MISC |                                             PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   5000,   5000,   1,      },
{FALSE, "the thief",            IPL_INVIS,              1,      1,      30,     PLT_MISC |                                             PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   7000,   7000,   1,      },
{FALSE, "doubling",             IPL_MULT_ARROWS,        2,      2,      8,                 PLT_BOW,                                                 GOE_ANY,        FALSE,  TRUE,   1000,   1000,   1,      },
{FALSE, "tripling",             IPL_MULT_ARROWS,        3,      3,      16,                PLT_BOW,                                                 GOE_ANY,        FALSE,  TRUE,   2000,   2000,   1,      },
{TRUE,  "fire",                 IPL_FIRE_ARROWS,        0,      0,      5,                 PLT_BOW,                                                 GOE_ANY,        FALSE,  TRUE,   1500,   1500,   1,      },
{FALSE, "lightning",            IPL_LIGHT_ARROWS,       0,      0,      11,                PLT_BOW,                                                 GOE_ANY,        FALSE,  TRUE,   2500,   2500,   1,      },
{TRUE,  "many",                 IPL_DUR,                100,    100,    3,                 PLT_BOW,                                                 GOE_ANY,        FALSE,  TRUE,   750,    750,    1,      },
{TRUE,  "plenty",               IPL_DUR,                200,    200,    7,                 PLT_BOW,                                                 GOE_ANY,        FALSE,  TRUE,   1500,   1500,   1,      },
{FALSE, "homing",               IPL_HOMING_ARROWS,      0,      0,      13,                PLT_BOW,                                                 GOE_ANY,        FALSE,  TRUE,   3000,   3000,   1,      },
{FALSE, "thorns",               IPL_THORNS,             1,      3,      8,      PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   -100,   -100,   1,      },
{FALSE, "spikes",               IPL_THORNS,             1,      6,      14,     PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   -200,   -200,   1,      },
{FALSE, "corruption",           IPL_NOMANA,             0,      0,      5,                                       PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_EVIL,       FALSE,  FALSE,  -1000,  -1000,  1,      },
{FALSE, "pox",                  IPL_NOHEALPLR,          0,      0,      6,      PLT_MISC |           PLT_STAFF,                                     GOE_EVIL,       FALSE,  FALSE,  -1500,  -1500,  1,      },
{FALSE, "fear",                 IPL_SCARE,              0,      0,      10,                PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        FALSE,  TRUE,   1000,   1000,   1,      },
{FALSE, "rabies",               IPL_DOUBLE_STRIKE,      0,      0,      4,                 PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        FALSE,  FALSE,  1000,   1000,   1,      },
{FALSE, "the crusader",         IPL_EXP_DAM,            0,      0,      13,                                      PLT_WEAP,                          GOE_GOOD,       FALSE,  TRUE,   3000,   3000,   1,      },
{FALSE, "vision",               IPL_SEE_INVIS,          0,      0,      9,      PLT_MISC | PLT_BOW | PLT_STAFF | PLT_WEAP | PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   1500,   1500,   1,      },
{FALSE, "the feather",          IPL_ABSHALFTRAP,        0,      0,      11,     PLT_MISC |                                  PLT_SHLD | PLT_ARMO,    GOE_ANY,        FALSE,  TRUE,   1000,   1000,   1,      },
{FALSE, "the bear",             IPL_KNOCKBACK,          0,      0,      5,                 PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_EVIL,       FALSE,  TRUE,   750,    750,    1,      },
{FALSE, "vileness",             IPL_NOHEALMON,          0,      0,      6,                 PLT_BOW | PLT_STAFF | PLT_WEAP,                          GOE_ANY,        FALSE,  TRUE,   1000,   1000,   1,      },
{FALSE, "",                     IPL_INVALID,            0,      0,      0,      0,                                                                  GOE_ANY,        FALSE,  FALSE,  0,      0,      0,      },
};