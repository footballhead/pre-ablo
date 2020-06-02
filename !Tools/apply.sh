#!/bin/bash

set -eux

rm -f DIABLO.EXE
cp _ORIGINAL/DIABLO.EXE DIABLO.EXE
# vcdiff decode --dictionary DIABLO.EXE --delta devmode/diablo.exe.vcdiff --target DIABLO.EXE
vcdiff decode --dictionary DIABLO.EXE --delta fullgame/diablo.exe.vcdiff --target DIABLO.EXE
# vcdiff decode --dictionary DIABLO.EXE --delta skip_intros/diablo.exe.vcdiff --target DIABLO.EXE
# vcdiff decode --dictionary DIABLO.EXE --delta cheat/diablo.exe.vcdiff --target DIABLO.EXE
# vcdiff decode --dictionary DIABLO.EXE --delta music-nompq-fix/diablo.exe.vcdiff --target DIABLO.EXE
vcdiff decode --dictionary DIABLO.EXE --delta infraring_fix/diablo.exe.vcdiff --target DIABLO.EXE
# vcdiff decode --dictionary DIABLO.EXE --delta lmh_nocrash/diablo.exe.vcdiff --target DIABLO.EXE
vcdiff decode --dictionary DIABLO.EXE --delta always_load_flare/diablo.exe.vcdiff --target DIABLO.EXE
vcdiff decode --dictionary DIABLO.EXE --delta snake_frame_fix/diablo.exe.vcdiff --target DIABLO.EXE
# vcdiff decode --dictionary DIABLO.EXE --delta old_drlg/diablo.exe.vcdiff --target DIABLO.EXE
vcdiff decode --dictionary DIABLO.EXE --delta no_tp_light/diablo.exe.vcdiff --target DIABLO.EXE
vcdiff decode --dictionary DIABLO.EXE --delta hell_automap/diablo.exe.vcdiff --target DIABLO.EXE
vcdiff decode --dictionary DIABLO.EXE --delta undead_crown/diablo.exe.vcdiff --target DIABLO.EXE
vcdiff decode --dictionary DIABLO.EXE --delta tp_setlvl_fix/diablo.exe.vcdiff --target DIABLO.EXE
vcdiff decode --dictionary DIABLO.EXE --delta too_much_hp_crash/diablo.exe.vcdiff --target DIABLO.EXE
vcdiff decode --dictionary DIABLO.EXE --delta thunder-demon-missile-fix/diablo.exe.vcdiff --target DIABLO.EXE
vcdiff decode --dictionary DIABLO.EXE --delta mega_fix/diablo.exe.vcdiff --target DIABLO.EXE
vcdiff decode --dictionary DIABLO.EXE --delta stone-curse-missile-fix/diablo.exe.vcdiff --target DIABLO.EXE
