#ifndef __EFFECTS_H__
#define __EFFECTS_H__

//
// enums
//

enum _sfx_id
{
    IS_DEAD = 0,
    IS_FMAG,
    IS_LMAG,
    IS_TMAG,
    IS_LGHIT,
    IS_LGHIT1,
    IS_SWING,
    IS_BFIRE,
    IS_WALK1,
    IS_WALK2,
    IS_WALK3,
    IS_WALK4,
    IS_BHIT,
    IS_BHIT1,
    IS_CHEST,
    IS_DOORCLOS,
    IS_DOOROPEN,
    IS_FAXE,
    IS_FBOW,
    IS_FCAP,
    IS_FHARM,
    IS_FLARM,
    IS_FLIPMAG,
    IS_FLIPMAG1,
    IS_FPOT,
    IS_FRING,
    IS_FSCRL,
    IS_FSHLD,
    IS_FSTAF,
    IS_FSWOR,
    IS_GOLD,
    IS_IAXE,
    IS_IBOW,
    IS_ICAP,
    IS_IGRAB,
    IS_IHARM,
    IS_ILARM,
    IS_IPOT,
    IS_IRING,
    IS_ISCROL,
    IS_ISHIEL,
    IS_ISTAF,
    IS_ISWORD,
    IS_MAGIC,
    IS_MAGIC1,
    IS_SARC,
    IS_TITLSLCT,
    IS_TITLEMOV,
    IS_TRAP,
    IS_LEVER,
    IS_BARREL,
    IS_BARFIRE,
    IS_WARRIOR1,
    IS_WARRIOR2,
    IS_WARRIOR6,
    IS_WARRIOR7,
    IS_WARRIOR3,
    IS_WARRIOR4,
    IS_WARRIOR5,
    IS_WARRIOR8,
    IS_CAST2,
    IS_CAST4,
    IS_CAST6,
    IS_CAST8,
    IS_REPAIR,
    IS_ELECIMP1,
    IS_ELEMENTL,
    IS_FBOLT1,
    IS_FIRIMP2,
    IS_INFRAVIS,
    IS_LNING1,
    IS_NOVA,
    IS_SENTINEL,
    IS_WALLLOOP,
    IS_WALLSTRT,
};

//
// functions
//

void PlayRndSFX(int psfx);

#endif