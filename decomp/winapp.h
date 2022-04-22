#ifndef __WINAPP_H__
#define __WINAPP_H__

#include <windows.h>

// .text:004776F0
// No side effects? does nothing?
void winapp_print_fatal(LPCTSTR msg, int lineno);

// .text:00477768
// No side effects? does nothing?
void TermMsg(LPCTSTR lpFormat, ...);

// .text:00477799
// Wrapper around malloc that (tries to) print errors
void *winapp_malloc(size_t size);

// .text:004777D8
// Wrapper around free that (tries to) print errors
void winapp_free(void *memblock);

#endif