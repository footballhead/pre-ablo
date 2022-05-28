@echo off

REM Run the compiler on files to catch stilly mistakes.
REM This is not guaranteed to link (I can guarantee it won't!) so we don't bother trying
REM Run vcvars first! E.g. use 

if %Platform% neq x86 (
    echo Need a 32-bit compiler! Run vcvars32.bat!
    exit 1
)

cl /c typechek.cpp

cl /c automap.cpp
cl /c control.cpp
cl /c cursor.cpp
cl /c dead.cpp
cl /c debug.cpp
cl /c diablo.cpp
cl /c draw_asm.cpp
cl /c drlg_l1.cpp
cl /c drlg_l2.cpp
cl /c drlg_l3.cpp
cl /c drlg_l4.cpp
cl /c effects.cpp
cl /c engine.cpp
cl /c error.cpp
cl /c gendung.cpp
cl /c gmenu.cpp
cl /c help.cpp
cl /c interfac.cpp
cl /c inv.cpp
cl /c itemdat.cpp
cl /c items.cpp
cl /c lighting.cpp
cl /c minitext.cpp
cl /c missiles.cpp
cl /c monst2.cpp
cl /c monstdat.cpp
cl /c monster.cpp
cl /c mopaq.cpp
cl /c multi.cpp
cl /c objdat.cpp
cl /c objects.cpp
cl /c palette.cpp
cl /c player.cpp
cl /c quests.cpp
cl /c scrollrt.cpp
cl /c setmaps.cpp
cl /c snd.cpp
cl /c sound.cpp
cl /c spells.cpp
cl /c stores.cpp
cl /c themes.cpp
cl /c town.cpp
cl /c trigs.cpp
cl /c unused.cpp
cl /c wave2.cpp
cl /c winapp.cpp