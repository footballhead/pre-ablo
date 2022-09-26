#!/bin/bash

# see test_compile.sh for more info

# TODO link order
wine link /PDB:diablo.pdb \
    /out:diablo.exe \
    /DEBUG:FULL \
    automap.obj \
    control.obj \
    cursor.obj \
    dead.obj \
    debug.obj \
    diablo.obj \
    draw_asm.obj \
    drlg_l1.obj \
    drlg_l2.obj \
    drlg_l3.obj \
    drlg_l4.obj \
    effects.obj \
    engine.obj \
    error.obj \
    gendung.obj \
    gmenu.obj \
    help.obj \
    interfac.obj \
    inv.obj \
    itemdat.obj \
    items.obj \
    lighting.obj \
    minitext.obj \
    missiles.obj \
    monst2.obj \
    monstdat.obj \
    monster.obj \
    mopaq.obj \
    multi.obj \
    objdat.obj \
    objects.obj \
    palette.obj \
    player.obj \
    quests.obj \
    scrollrt.obj \
    setmaps.obj \
    snd.obj \
    sound.obj \
    spells.obj \
    stores.obj \
    themes.obj \
    town.obj \
    trigs.obj \
    unused.obj \
    wave2.obj \
    winapp.obj \
    /subsystem:windows \
    user32.lib \
    gdi32.lib \
    winmm.lib \
    Msacm32.lib \
    directx/ddraw.lib \
    directx/dplay.lib \
    directx/dsound.lib \
    storm/storm.lib