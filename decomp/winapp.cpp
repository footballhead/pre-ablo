#include "winapp.h"

// .text:004776F0
// No side effects? does nothing?
void winapp_print_fatal(LPCTSTR msg, int lineno)
{
    TermMsg(TEXT("FATAL %s:%d"), msg, lineno);
}

// text:0047771E
// THIS FUNCTION IS NEVER CALLED. This seems like something that could have
// been swapped for TermMsg at compile-time based on a macro...
static void __dc_diab_msg_box(LPCTSTR lpFormat, ...)
{
    TCHAR buffer[512];
    va_list args;
    va_start(args, lpFormat);
    wvsprintf(buffer, lpFormat, args);
    va_end(args);
    MessageBox(NULL, buffer, TEXT("Diablo"), MB_TASKMODAL);
}

// .text:00477768
// No side effects? does nothing?
void TermMsg(LPCTSTR lpFormat, ...)
{
    TCHAR buffer[512];
    va_list args;
    va_start(args, lpFormat);
    wvsprintf(buffer, lpFormat, args);
    va_end(args);
}

// .text:00477799
// Wrapper around malloc that (tries to) print errors
void *winapp_malloc(size_t size)
{
    void *rv = malloc(size);
    if (rv == NULL)
    {
        TermMsg(TEXT("Out of memory"));
    }
    return rv;
}

// .text:004777D8
// Wrapper around free that (tries to) print errors
void winapp_free(void *memblock)
{
    if (memblock == NULL)
    {
        winapp_print_fatal(TEXT("C:\\Diablo\\Direct\\Winapp.cpp"), 57);
    }
    else
    {
        free(memblock);
    }
}
