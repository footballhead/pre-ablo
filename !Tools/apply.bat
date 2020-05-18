del DIABLO.EXE
copy _ORIGINAL\DIABLO.EXE DIABLO.EXE
REM call :apply_patch devmode\diablo.exe.vcdiff
call :apply_patch fullgame\diablo.exe.vcdiff
call :apply_patch skip_intros\diablo.exe.vcdiff
REM call :apply_patch cheat\diablo.exe.vcdiff
call :apply_patch music-nompq-fix\diablo.exe.vcdiff
call :apply_patch infraring_fix\diablo.exe.vcdiff
REM call :apply_patch lmh_nocrash\diablo.exe.vcdiff
call :apply_patch always_load_flare\diablo.exe.vcdiff
call :apply_patch snake_frame_fix\diablo.exe.vcdiff
REM call :apply_patch old_drlg\diablo.exe.vcdiff
call :apply_patch no_tp_light\diablo.exe.vcdiff
call :apply_patch hell_automap\diablo.exe.vcdiff
call :apply_patch undead_crown\diablo.exe.vcdiff
call :apply_patch tp_setlvl_fix\diablo.exe.vcdiff
call :apply_patch too_much_hp_crash\diablo.exe.vcdiff
call :apply_patch thunder-demon-missile-fix\diablo.exe.vcdiff
pause
exit /b

REM Apply a vcdiff patch.
REM Expects DIABLO.EXE to exist. 
REM Creates tmp.exe as a side effect
REM %1 is the vcdiff patch filename
:apply_patch
vcdiff.exe decode --dictionary DIABLO.EXE --delta %1 --target tmp.exe
del DIABLO.EXE
rename tmp.exe DIABLO.EXE
exit /b
