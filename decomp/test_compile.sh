#!/bin/bash

# Assumes you have Visual C++ 4.0 (and wine) installed
# https://winworldpc.com/product/visual-c/4x
#
# In GNOME 42.4 (Fedora 36) I just had to double-click the ISO then double-click setup.exe

# _STATIC_ASSERT is from after Visual C++ 4.0
# wine cl /c /nologo /MT typechek.cpp

wine cl /c /nologo /MT automap.cpp
wine cl /c /nologo /MT control.cpp
wine cl /c /nologo /MT cursor.cpp
wine cl /c /nologo /MT dead.cpp
wine cl /c /nologo /MT debug.cpp
wine cl /c /nologo /MT diablo.cpp
wine cl /c /nologo /MT draw_asm.cpp
wine cl /c /nologo /MT drlg_l1.cpp
wine cl /c /nologo /MT drlg_l2.cpp
wine cl /c /nologo /MT drlg_l3.cpp
wine cl /c /nologo /MT drlg_l4.cpp
wine cl /c /nologo /MT effects.cpp
wine cl /c /nologo /MT engine.cpp
wine cl /c /nologo /MT error.cpp
wine cl /c /nologo /MT gendung.cpp
wine cl /c /nologo /MT gmenu.cpp
wine cl /c /nologo /MT help.cpp
wine cl /c /nologo /MT interfac.cpp
wine cl /c /nologo /MT inv.cpp
wine cl /c /nologo /MT itemdat.cpp
wine cl /c /nologo /MT items.cpp
wine cl /c /nologo /MT lighting.cpp
wine cl /c /nologo /MT minitext.cpp
wine cl /c /nologo /MT missiles.cpp
wine cl /c /nologo /MT monst2.cpp
wine cl /c /nologo /MT monstdat.cpp
wine cl /c /nologo /MT monster.cpp
wine cl /c /nologo /MT mopaq.cpp
wine cl /c /nologo /MT multi.cpp
wine cl /c /nologo /MT objdat.cpp
wine cl /c /nologo /MT objects.cpp
wine cl /c /nologo /MT palette.cpp
wine cl /c /nologo /MT player.cpp
wine cl /c /nologo /MT quests.cpp
wine cl /c /nologo /MT scrollrt.cpp
wine cl /c /nologo /MT setmaps.cpp
wine cl /c /nologo /MT snd.cpp
wine cl /c /nologo /MT sound.cpp
wine cl /c /nologo /MT spells.cpp
wine cl /c /nologo /MT stores.cpp
wine cl /c /nologo /MT themes.cpp
wine cl /c /nologo /MT town.cpp
wine cl /c /nologo /MT trigs.cpp
wine cl /c /nologo /MT unused.cpp
wine cl /c /nologo /MT wave2.cpp
wine cl /c /nologo /MT winapp.cpp
