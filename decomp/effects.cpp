#include "effects.h"

#include "engine.h"

//
// initialized vars
//

const char *sgSFX_pszName[] = {
    "Sfx\\Dead.wav",
    "Sfx\\Fmag.wav",
    "Sfx\\Lmag.wav",
    "Sfx\\Tmag.wav",
    "Sfx\\Lghit.wav",
    "Sfx\\Lghit1.wav",
    "Sfx\\Swing.wav",
    "Sfx\\Bfire.wav",
    "Sfx\\Walk1.wav",
    "Sfx\\Walk2.wav",
    "Sfx\\Walk3.wav",
    "Sfx\\Walk4.wav",
    "Sfxitem\\Bhit.wav", // Unused?
    "Sfxitem\\Bhit1.wav", // Unused?
    "Sfxitem\\Chest.wav",
    "Sfxitem\\Doorclos.wav",
    "Sfxitem\\Dooropen.wav",
    "Sfxitem\\Flipaxe.wav",
    "Sfxitem\\Flipbow.wav",
    "Sfxitem\\Flipcap.wav",
    "Sfxitem\\Flipharm.wav",
    "Sfxitem\\Fliplarm.wav",
    "Sfxitem\\Flipmag.wav",
    "Sfxitem\\Flipmag1.wav",
    "Sfxitem\\Flippot.wav",
    "Sfxitem\\Flipring.wav",
    "Sfxitem\\Flipscrl.wav",
    "Sfxitem\\Flipshld.wav",
    "Sfxitem\\Flipstaf.wav",
    "Sfxitem\\Flipswor.wav",
    "Sfxitem\\Gold.wav",
    "Sfxitem\\Invaxe.wav",
    "Sfxitem\\Invbow.wav",
    "Sfxitem\\Invcap.wav",
    "Sfxitem\\Invgrab.wav",
    "Sfxitem\\Invharm.wav",
    "Sfxitem\\Invlarm.wav",
    "Sfxitem\\Invpot.wav",
    "Sfxitem\\Invring.wav",
    "Sfxitem\\Invscrol.wav",
    "Sfxitem\\Invshiel.wav",
    "Sfxitem\\Invstaf.wav",
    "Sfxitem\\Invsword.wav",
    "Sfxitem\\Magic.wav",
    "Sfxitem\\Magic1.wav",
    "Sfxitem\\Sarc.wav",
    "Sfxitem\\Titlslct.wav",
    "Sfxitem\\Titlemov.wav",
    "Sfxitem\\Trap.wav",
    "Sfxitem\\Lever.wav",
    "Sfxitem\\Barrel.wav",
    "Sfxitem\\Barfire.wav",
    "Sfx\\Warrior1.wav",
    "Sfx\\Warrior2.wav",
    "Sfx\\Warrior6.wav",
    "Sfx\\Warrior7.wav",
    "Sfx\\Warrior3.wav",
    "Sfx\\Warrior4.wav",
    "Sfx\\Warrior5.wav",
    "Sfx\\Warrior8.wav",
    "Sfx\\CAST2.wav",
    "Sfx\\CAST4.wav",
    "Sfx\\CAST6.wav",
    "Sfx\\CAST8.wav",
    "Sfx\\REPAIR.wav",
    "Sfx\\ELECIMP1.wav",
    "Sfx\\ELEMENTL.wav",
    "Sfx\\FBOLT1.wav",
    "Sfx\\FIRIMP2.wav",
    "Sfx\\INFRAVIS.wav",
    "Sfx\\LNING1.wav",
    "Sfx\\NOVA.wav",
    "Sfx\\SENTINEL.wav",
    "Sfx\\WALLLOOP.wav",
    "Sfx\\WALLSTRT.wav",
};
const char *townerSndPaths[] = {
    "Sfx\\BlckSmth.wav",
    "Sfx\\Witch.wav",
    "Sfx\\DeadGuy2.wav",
    "Sfx\\TavLove.wav",
    "Sfx\\Tavern24.wav",
    "Sfx\\Tavern23.wav",
    "Sfx\\PegBoy.wav",
};
char MonstSndChar[] = {'a', 'h', 'd', 's'};

//
// code (.text:00464460)
//

// priv_sound_init	0000000000464460
// sound_stop	00000000004644B3
// InitMonsterSND	00000000004644FC
// FreeEffects	000000000046467F
// InitTownersSND	0000000000464785
// FreeTownersEffects	00000000004647D8
// PlayEffect	0000000000464821
// PlaySFX	00000000004648D5

// .text:0046491F
void PlayRndSFX(int psfx)
{
    if (psfx == IS_LGHIT)
    {
        // warrior hit sounds (IS_LGHIT/IS_LGHIT1)
        psfx += random_(2);
    }
    if (psfx == IS_FLIPMAG)
    {
        // magic rock (IS_FLIPMAG/IS_FLIPMAG1)
        psfx += random_(2);
    }
    if (psfx == IS_MAGIC)
    {
        // magic rock (IS_MAGIC/IS_MAGIC1)
        psfx += random_(2);
    }
    if (psfx == IS_BHIT)
    {
        // unused (IS_BHIT/IS_BHIT1)
        psfx += random_(2);
    }
    if (psfx == IS_WALK1)
    {
        // walk (IS_WALK1/IS_WALK2/IS_WALK3/IS_WALK4)
        psfx += random_(4);
    }
    if (psfx == IS_WARRIOR2)
    {
        // full inventory (IS_WARRIOR2/IS_WARRIOR6/IS_WARRIOR7)
        psfx += random_(3);
    }

    // TODO
}

// PlayTownerSFX	0000000000464A3B
// sfx_stop	0000000000464ACD
