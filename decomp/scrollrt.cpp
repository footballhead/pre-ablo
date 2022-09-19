#include "scrollrt.h"

#include "defines.h"

//
// Uninitialized variables
//

int PitchTbl[1024];

//
// Code
//

// DrawMissile	000000000047BE30
// CDrawMissile	000000000047C1B3
// DrawMonster	000000000047C535
// CDrawMSlabCelL	000000000047C732
// DrawObjCel	000000000047C92F
// CDrawObjCel	000000000047CBCC
// DrawEFlag1	000000000047CE69
// DrawHTLXsub	000000000047D02E
// DrawHTileLineX	000000000047DC7A
// DrawEFlag2	000000000047E2BC
// DrawHTLXsub2	000000000047E547
// DrawHTLX2	000000000047F1E7
// DrawEFlag3	000000000047F6D7
// DrawHTLXsub3	000000000047F8AF
// DrawHTLX3	000000000048054F
// SVGADrawView	0000000000480BD6
// VGADrawView	0000000000480EC1
// DrawView	000000000048120E

// .text:004813C2
// Fill the visible area with 0s
void ClearScreenBuffer()
{
    __asm {
        mov edi, gpBuffer
        add edi, 122944 //; (BUFFER_WIDTH) * BORDER_TOP
        // shouldn't edi have an additional BUFFER_LEFT?
        mov edx, 480 //; SCREEN_HEIGHT
        xor eax, eax

    blank_row:
        mov ecx, 160 //; SCREEN_WIDTH / 4
        rep stosd
        add edi, 128 //; BUFFER_LEFT + BUFFER_RIGHT
        dec edx
        jnz blank_row
    }
}

// ScrollView	00000000004813F9
void ScrollView()
{
    // TODO
}

// DrawMain	0000000000481653
// scrollrt_draw_game_screen	0000000000481AFC
// DrawAndBlit	0000000000481B60
