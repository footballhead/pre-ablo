#include "engine.h"

#include "diablo.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

//
// uninitialized vars (.data )
//

char file_to_load_with_base[MAX_PATH];
DWORD dword_61B6F4;
DWORD dword_61B6F8;
char byte_61B6FC;

//
// code (.text:00482790)
//

// LoadFileInMem	0000000000482790
// LoadFileWithMem	000000000048283B
// get_alloc_file_4828D6	00000000004828D6
// get_seek_file_48297B	000000000048297B

// FileGetSize	0000000000482A2A
// Demo specific!
DWORD FileGetSize(const char *filename)
{
    char buffer[256];
    sprintf(buffer, "%s%s", fileLoadPrefix, filename);
    // SFileOpenFile
    // SFileGetFileSize
    // SFileCloseFile
    //  TODO
    return 0;
}

// FileAddLoadPrefix	0000000000482A9D
// DecodeFullCel	0000000000482AD3
// CelDraw	0000000000482B53
// engine_cel_482BB2	0000000000482BB2
// DrawSlabCel	0000000000482BFB
// CelDecodeHdrOnly	0000000000482CBC
// DecodeFullCelL	0000000000482D5F
// DrawFullCelL_xbytes	0000000000482DDC
// CelDecDatLightTrans	0000000000482E56
// engine_cel_482F54	0000000000482F54
// CelDrawLight	0000000000483046
// engine_cel_48312C	000000000048312C
// CelDecodeHdrLightTrans	00000000004831F4
// CelDrawLightTbl	00000000004832E1
// CDecodeFullCel	0000000000483477
// CDrawSlabCel	000000000048350C
// CDrawSlabCelP	00000000004835CD
// Cel2DecDatLightOnly	0000000000483670
// Cel2DecDatLightEntry	0000000000483702
// Cel2DecDatLightTrans	000000000048377C
// CDrawSlabCelL	000000000048388F
// engine_cel_483975	0000000000483975
// Cel2DecodeLightTrans	0000000000483A3D
// CDrawSlabCelI	0000000000483B2A
// CelDecodeRect	0000000000483CD5
// OutlineSlabCel	0000000000483D7E
// CelDrawHdrClrHL	0000000000483E99

// .text:00483FFB
void CelApplyTrans(BYTE *p, BYTE *ttbl, int nCel)
{
    int i;

    for (i = 1; i <= nCel; i++)
    {
        // The star of the show is the XLAT instruction
        // the rest just sets up pointers and loop sizes
        __asm {
            mov     ebx, p
            mov     eax, i
            shl     eax, 2
            add     ebx, eax // ebx is the offset within the group for the frame data

            mov     esi, p // Load group
            add     esi, [ebx] ; // Find frame i data
            add     esi, 10 // Ignore skip header

            mov     edx, [ebx+4]
            sub     edx, [ebx]
            sub     edx, 10 ; // Size is (next offset) - (this offset) - (skip header size)

            mov     edi, esi
            mov     ebx, ttbl // Used in XLAT instruction below

        top:
            xor     eax, eax
            lodsb // Load+store CEL "instruction"
            stosb
            dec     edx
            jz      done

            or      al, al
            js      top // "Transparency" instruction (eax < 0) has no data following

                // "Copy" instruction (eax > 0) is followed by data
                // the byte of instruction is the size of that data
            sub     edx, eax
            mov     ecx, eax

        apply_trans:
            lodsb
                    // Here's a weird instruction: https://www.felixcloutier.com/x86/xlat:xlatb
            xlat // AL = ttbl[AL] in our case, where AL is the pixel 256-color
            stosb
            loop apply_trans

            or      edx, edx
            jnz     top

        done:
            nop // Go to next for-loop iteration
        }

        // I bet the only reason they didn't write this in C++ is that they
        // didn't like the number of instructions that the non-optimized debug
        // compiler generated...
    }
}

// ENG_set_pixel	000000000048407B
// engine_draw_pixel	00000000004840F9
// DrawLine	00000000004841F1
// GetDirection	00000000004848A1

// .text:004849E2
int random_(int v)
{
    if (v <= 0)
    {
        return 0;
    }
    return rand() % v;
}
