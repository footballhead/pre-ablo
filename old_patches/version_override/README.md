# Version Override

Replaces the version string.

Needs to be updated each release.

## On the Main Menu

The game version shows up in the main menu in the bottom-left corner.

This is the code that runs on the main menu:

```
.text:004191EB 83 3D 5C C1 4B 00 00                          cmp     demo_mode, 0
.text:004191F2 0F 84 17 00 00 00                             jz      version_no_demo
.text:004191F8 68 F4 A6 4A 00                                push    offset aPreReleaseDemo ; "Pre-release Demo"
.text:004191FD 68 28 DE 5D 00                                push    offset tempstr
.text:00419202 E8 B9 E9 07 00                                call    strcpy
.text:00419207 83 C4 08                                      add     esp, 8
.text:0041920A E9 12 00 00 00                                jmp     draw_version
.text:0041920F                               ; ---------------------------------------------------------------------------
.text:0041920F
.text:0041920F                               version_no_demo:                        ; CODE XREF: mainmenu_draw+E1↑j
.text:0041920F 68 50 C1 4B 00                                push    offset aV10     ; "V1.0"
.text:00419214 68 28 DE 5D 00                                push    offset tempstr
.text:00419219 E8 A2 E9 07 00                                call    strcpy
.text:0041921E 83 C4 08                                      add     esp, 8
.text:00419221
.text:00419221                               draw_version:                           ; CODE XREF: mainmenu_draw+F9↑j
.text:00419221 68 28 DE 5D 00                                push    offset tempstr
.text:00419226 BA 70 02 00 00                                mov     edx, 624
.text:0041922B B9 50 00 00 00                                mov     ecx, 80
.text:00419230 E8 7D FD FF FF                                call    print_title_str_small
.text:00419235 C7 45 C8 00 00 00 00                          mov     [ebp+i], 0
.text:0041923C E9 03 00 00 00                                jmp     draw_menu_options


.data:004AA6F4 50 72 65 2D 72 65 6C 65 61 73+aPreReleaseDemo db 'Pre-release Demo',0
.data:004BC150 56 31 2E 30 00                aV10            db 'V1.0',0
```

We need to change two things:

 * Can remove the `demo_mode` check, we don't care about the distinction
 * Change a string to have our version. Would prefer `aPreReleaseDemo` instead because it has a lot of space

```
.text:004191EB 90 90 90 90 90 90 90                          nop (x7)   ; BINOFF 0x185EB
.text:004191F2 90 90 90 90 90 90                             nop (x6)
.text:004191F8 68 F4 A6 4A 00                                push    offset aPreReleaseDemo
.text:004191FD 68 28 DE 5D 00                                push    offset tempstr
.text:00419202 E8 B9 E9 07 00                                call    strcpy
.text:00419207 83 C4 08                                      add     esp, 8
.text:0041920A 90 90 90 90 90                                nop (x5)
.text:0041920F 90 90 90 90 90                                nop (x5)
.text:00419214 90 90 90 90 90                                nop (x5)
.text:00419219 90 90 90 90 90                                nop (x5)
.text:0041921E 90 90 90                                      nop (x3)
.text:00419221 68 28 DE 5D 00                                push    offset tempstr        ; The rest of the code from here is the same
.text:00419226 BA 70 02 00 00                                mov     edx, 624
.text:0041922B B9 50 00 00 00                                mov     ecx, 80
.text:00419230 E8 7D FD FF FF                                call    print_title_str_small ; .text:00418FB2
.text:00419235 C7 45 C8 00 00 00 00                          mov     [ebp+i], 0
.text:0041923C E9 03 00 00 00                                jmp     draw_menu_options


.data:004AA6F4                               aPreReleaseDemo db 'v0.1.0',0              ; BINOFF 0xA8EF4
                                                                                        ; Replace contents with null terminated string of choice
.data:004BC150 56 31 2E 30 00                aV10            db 'V1.0',0                ; This is now only used by the game version (see next section)
```

## In game

Pressing V in game prints the version to the control panel.

This is the machine code in GM_Game that runs when `V`/`v` is pressed:

```
.text:004873A7                               V_KEY:                                  ; CODE XREF: GM_Game+ECE↓j
.text:004873A7                                                                       ; DATA XREF: GM_Game:off_487499↓o
.text:004873A7 83 3D 5C C1 4B 00 00                          cmp     demo_mode, 0    ; case 'V':
.text:004873A7                                                                       ; case 'v':
.text:004873AE 0F 84 17 00 00 00                             jz      version_no_demo
.text:004873B4 68 70 C3 4B 00                                push    offset aPreReleaseDemo_0 ; "Pre-release Demo"
.text:004873B9 68 28 DE 5D 00                                push    offset tempstr
.text:004873BE E8 FD 07 01 00                                call    strcpy
.text:004873C3 83 C4 08                                      add     esp, 8
.text:004873C6 E9 12 00 00 00                                jmp     copy_version_to_panel
.text:004873CB                               ; ---------------------------------------------------------------------------
.text:004873CB
.text:004873CB                               version_no_demo:                        ; CODE XREF: GM_Game+DEA↑j
.text:004873CB 68 50 C1 4B 00                                push    offset aV10     ; "V1.0"
.text:004873D0 68 28 DE 5D 00                                push    offset tempstr
.text:004873D5 E8 E6 07 01 00                                call    strcpy
.text:004873DA 83 C4 08                                      add     esp, 8
.text:004873DD
.text:004873DD                               copy_version_to_panel:                  ; CODE XREF: GM_Game+E02↑j
.text:004873DD B9 28 DE 5D 00                                mov     ecx, offset tempstr ; str
.text:004873E2 BA 01 00 00 00                                mov     edx, 1          ; just
.text:004873E7 E8 24 8B F8 FF                                call    AddPanelString
.text:004873EC 33 C0                                         xor     eax, eax
.text:004873EE E9 CF 0D 00 00                                jmp     end_of_function
```

We can do the same as before:

* Remove the `demo_mode` check
* Prefer our version (can reuse the same variable)

```
.text:004873A7 90 90 90 90 90 90 90                          nop (x7)   ; BINOFF 0x867A7
.text:004873AE 90 90 90 90 90 90                             nop (x6)
.text:004873B4 68 F4 A6 4A 00                                push    offset aPreReleaseDemo ; NOTE: This address has changed!
                                                                                            ; For some reason, v1.0 is shared but Pre-release Demo is not
                                                                                            ; This is the string we've manipulated to be our version
.text:004873B9 68 28 DE 5D 00                                push    offset tempstr
.text:004873BE E8 FD 07 01 00                                call    strcpy
.text:004873C3 83 C4 08                                      add     esp, 8
.text:004873C6 90 90 90 90 90                                nop (x5)
.text:004873CB 90 90 90 90 90                                nop (x5)
.text:004873D0 90 90 90 90 90                                nop (x5)
.text:004873D5 90 90 90 90 90                                nop (x5)
.text:004873DA 90 90 90                                      nop (x3)
.text:004873DD B9 28 DE 5D 00                                mov     ecx, offset tempstr     ; The rest from here is the same
.text:004873E2 BA 01 00 00 00                                mov     edx, 1
.text:004873E7 E8 24 8B F8 FF                                call    AddPanelString
.text:004873EC 33 C0                                         xor     eax, eax
.text:004873EE E9 CF 0D 00 00                                jmp     end_of_function
```