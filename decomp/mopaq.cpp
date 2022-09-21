#include <windows.h>

#include "winapp.h"

// TODO add to header
void sub_478FA5();
void sub_478FCA();
void sub_478FEF();

//
// initialized variables (.data:004BB88C)
//

HANDLE mopaq_obj = INVALID_HANDLE_VALUE;

//
// code (.text:00478F90)
//

// TODO sub_478F90/sub_478FB5/sub_478FDA need to be called from CRT static initialization

// .text:00478F90
// This is actually a static variable constructor (like quest text)
void sub_478F90()
{
    sub_478FA5();
}

// .text:00478FA5
void sub_478FA5()
{
    // do nothing
}

// .text:00478FB5
// This is actually a static variable constructor (like quest text)
void sub_478FB5()
{
    sub_478FCA();
}

// .text:00478FCA
void sub_478FCA()
{
    // do nothing
}

// .text:00478FDA
// This is actually a static variable constructor (like quest text)
void sub_478FDA()
{
    sub_478FEF();
}

// .text:00478FEF
void sub_478FEF()
{
    // do nothing
}

// .text:00478FFF
// I have a feeling this was supposed to quit if something was awry... but
// winapp_print_fatal doesn't do that
// TODO what is type of ptr?
void Mopaq_AssertState(void *ptr)
{
    if (ptr == NULL)
    {
        winapp_print_fatal(TEXT("C:\\Diablo\\Direct\\Mopaq.cpp"), 87);
    }
    // TODO lolwut this is obviously some member reference
    if (*(DWORD *)((BYTE *)ptr + 0x1c) == 0xFFFFFFFF)
    {
        winapp_print_fatal(TEXT("C:\\Diablo\\Direct\\Mopaq.cpp"), 88);
    }
    if (mopaq_obj == INVALID_HANDLE_VALUE)
    {
        winapp_print_fatal(TEXT("C:\\Diablo\\Direct\\Mopaq.cpp"), 89);
    }
}

// .text:0000000000479075
// One caller: dx_init
// a = 20
// b = 10
// c = 0x8000
void Mopaq_Init(DWORD a, DWORD b, DWORD c)
{
    if (a == 0)
    {
        winapp_print_fatal(TEXT("C:\\Diablo\\Direct\\Mopaq.cpp"), 166);
    }
    if (b >= a)
    {
        winapp_print_fatal(TEXT("C:\\Diablo\\Direct\\Mopaq.cpp"), 167);
    }
    if (c < 0x400)
    {
        winapp_print_fatal(TEXT("C:\\Diablo\\Direct\\Mopaq.cpp"), 168);
    }

    // TODO
}
// Mopaq_47925E	000000000047925E
// Mopaq_4792B4_read	00000000004792B4
// Mopaq_free	000000000047947B
// Mopaq_479595_new	0000000000479595
// Mopaq_4796CA	00000000004796CA

// Mopaq_47972C	000000000047972C
// never called

// Mopaq_4797D8	00000000004797D8
// ReadMemFile	0000000000479870
// Mopaq_4799AB	00000000004799AB
// Mopaq_4799F5	00000000004799F5
// Mopaq_479A19	0000000000479A19
// Mopaq_479AB1	0000000000479AB1
