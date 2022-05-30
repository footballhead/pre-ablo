#ifndef __DEAD_H__
#define __DEAD_H__

//
// defines
//

#define MAXDEAD 31

//
// enums
//

//
// structs
//

struct DeadStruct
{
    unsigned char *_deadData[8];
    int _deadFrame;
    int _deadWidth;
    int _deadWidth2;
    char _deadtrans;
};

//
// variables
//

extern int stonendx;
extern int spurtndx;

//
// functions
//

void AddDead(int dx, int dy, char dv, int ddir);

#endif