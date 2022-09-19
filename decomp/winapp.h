#ifndef __WINAPP_H__
#define __WINAPP_H__

#include <windows.h>

// Does nothing.
// Uses TermMsg, which means this will crash if lpFormat + args are > 512 chars
// Despite the name, this does not halt the program
void winapp_print_fatal(LPCTSTR msg, int lineno);

// Calls wvsprintf but does nothing with the result.
// Will crash if lpFormat + args are > 512 chars
void TermMsg(LPCTSTR lpFormat, ...);

// .text:00477799
// Wrapper around malloc that (tries to) print errors
void *winapp_malloc(size_t size);

// .text:004777D8
// Wrapper around free that (tries to) print errors
void winapp_free(void *memblock);

#endif