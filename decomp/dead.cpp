#include "dead.h"

#include "monster.h"

//
// Uninitialized variables
//

DeadStruct dead[MAXDEAD];
int spurtndx;
int stonendx;

//
// Code
//

// InitDead	0000000000463E20
void InitDead()
{
    int mtypes[MAXMONSTERS];
    int i; // TODO var order
    int nd;
    int d;

    for (i = 0; i < MAXMONSTERS; i++)
    {
        mtypes[i] = 0;
    }

    nd = 0;

    for (i = 0; nummtypes > i; i++)
    {
        if (mtypes[Monsters[i].mtype] == 0)
        {
            for (d = 0; d < 8; d++)
            {
                // TODO
            }
        }
    }

    // TODO
}

// AddDead	00000000004642FD
void AddDead(int dx, int dy, char dv, int ddir)
{
    // TODO
}

// SetDead	0000000000464347
