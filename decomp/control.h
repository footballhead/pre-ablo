#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <windows.h>

//
// variables
//

extern char infostr[64];
extern char tempstr[64];
extern char infoclr;
extern BOOL drawhpflag;
extern BOOL drawmanaflag;
extern char infoclr;
extern BOOL chrflag;
extern BOOL drawpotboxflag;
extern BOOL pinfoflag;

//
// functions
//

void AddPanelString(const char *str, BOOL just);

#endif