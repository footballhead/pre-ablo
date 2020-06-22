Town Portals crash on save game load
-------------------------------------------------------------------------------

I think it has to do with the lighting that's made by the portal. If we remove the light then (absent of resolving the actual problem) we'll "fix" the crash.

Nop out these lines in MI_Town (starting at binary offest 0x3D287):

```
.text:0043DE87 8B 45 B0                                      mov     eax, [ebp+var_50]
.text:0043DE8A 8D 04 80                                      lea     eax, [eax+eax*4]
.text:0043DE8D C1 E0 05                                      shl     eax, 5
.text:0043DE90 8B 80 EC 72 5F 00                             mov     eax, missile._miVar2[eax]
.text:0043DE96 8B 44 85 B4                                   mov     eax, [ebp+eax*4+r]
.text:0043DE9A 50                                            push    eax             ; r
.text:0043DE9B 8B 45 B0                                      mov     eax, [ebp+var_50]
.text:0043DE9E 8D 04 80                                      lea     eax, [eax+eax*4]
.text:0043DEA1 C1 E0 05                                      shl     eax, 5
.text:0043DEA4 8B 4D B0                                      mov     ecx, [ebp+var_50]
.text:0043DEA7 8D 0C 89                                      lea     ecx, [ecx+ecx*4]
.text:0043DEAA C1 E1 05                                      shl     ecx, 5
.text:0043DEAD 8B 90 70 72 5F 00                             mov     edx, missile.missile_miy[eax] ; y
.text:0043DEB3 8B 89 6C 72 5F 00                             mov     ecx, missile.missile_mix[ecx] ; x
.text:0043DEB9 E8 3C 1D FF FF                                call    AddLight
.text:0043DEBE 8B 4D B0                                      mov     ecx, [ebp+var_50]
.text:0043DEC1 8D 0C 89                                      lea     ecx, [ecx+ecx*4]
.text:0043DEC4 C1 E1 05                                      shl     ecx, 5
.text:0043DEC7 89 81 E4 72 5F 00                             mov     missile._mlid[ecx], eax
.text:0043DECD 8B 45 B0                                      mov     eax, [ebp+var_50]
.text:0043DED0 8D 04 80                                      lea     eax, [eax+eax*4]
.text:0043DED3 C1 E0 05                                      shl     eax, 5
.text:0043DED6 8B 80 EC 72 5F 00                             mov     eax, missile._miVar2[eax]
.text:0043DEDC 8B 44 85 B4                                   mov     eax, [ebp+eax*4+r]
.text:0043DEE0 50                                            push    eax
.text:0043DEE1 8B 45 B0                                      mov     eax, [ebp+var_50]
.text:0043DEE4 8D 04 80                                      lea     eax, [eax+eax*4]
.text:0043DEE7 C1 E0 05                                      shl     eax, 5
.text:0043DEEA 8B 80 70 72 5F 00                             mov     eax, missile.missile_miy[eax]
.text:0043DEF0 50                                            push    eax
.text:0043DEF1 8B 45 B0                                      mov     eax, [ebp+var_50]
.text:0043DEF4 8D 04 80                                      lea     eax, [eax+eax*4]
.text:0043DEF7 C1 E0 05                                      shl     eax, 5
.text:0043DEFA 8B 4D B0                                      mov     ecx, [ebp+var_50]
.text:0043DEFD 8D 0C 89                                      lea     ecx, [ecx+ecx*4]
.text:0043DF00 C1 E1 05                                      shl     ecx, 5
.text:0043DF03 8B 90 6C 72 5F 00                             mov     edx, missile.missile_mix[eax]
.text:0043DF09 8B 89 E4 72 5F 00                             mov     ecx, missile._mlid[ecx]
.text:0043DF0F E8 56 21 FF FF                                call    ChangeLight
```