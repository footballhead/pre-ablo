#include "inv.h"

#include "cursor.h"
#include "effects.h"
#include "itemdat.h"
#include "items.h"
#include "player.h"

extern BOOL drawpotboxflag;
extern char* ItemCAnimTbl;
extern int* ItemInvSnds;
void PlayRndSFX(int psfx);
void RemoveInvItem(int pnum, int iv);

//
// code (.text:0044B810)
//

// InitInv	000000000044B810	
// InvDrawSlotBack	000000000044B839	
// DrawInv	000000000044B8B2	
// FreeInvGFX	000000000044C4D7	
// AutoPlace	000000000044C50D	
// GoldAutoPlace	000000000044C810	
// WeaponAutoPlace	000000000044CFC8	
// SwapItem	000000000044D151	
// CheckInvPaste	000000000044D1B6	
// CheckInvCut	000000000044EE51	
// RemoveInvItem	000000000044F7D0	
// CheckInvItem	000000000044FB2C	
// CheckItemStats	000000000044FB58	
// AutoGetItem	000000000044FC6E	
// CanPut	0000000000450240	
// InvPutItem	00000000004504DE	
// CheckInvHLight	00000000004507E9	
// RemoveScroll	0000000000451620	
// UseScroll	0000000000451741	
// UseStaffCharge	0000000000451864	
// UseStaff	000000000045194E	

// .text:00451A2B
void UseInvItem(BOOL use_potbox)
{
    int c;
    int idata;
    int idx;

    if (drawpotboxflag) {
        return;
    }

    // potion box, bottom left corner
    if (use_potbox) {
        c = plr[myplr].potbox_item;
        idata = ItemCAnimTbl[plr[myplr].InvList[c]._iCurs];
        PlayRndSFX(ItemInvSnds[idata]);
        UseItem(myplr, plr[myplr].InvList[c]._iMiscID, plr[myplr].InvList[c]._iSpell);
        RemoveInvItem(myplr, c);
    } else {
        if (pcursinvitem <= 5) {
            return;
        }
        c = pcursinvitem - 6;
        idx = plr[myplr].InvList[c].IDidx;
        if (plr[myplr].InvList[c]._iStatFlag == FALSE) {
            PlayRndSFX(IS_WARRIOR1); // "I can not use this yet"
        }
        if (AllItemsList[idx].iUsable != FALSE && plr[myplr].InvList[c]._iStatFlag != FALSE) {
            idata = ItemCAnimTbl[plr[myplr].InvList[c]._iCurs];
            PlayRndSFX(ItemInvSnds[idata]);
            UseItem(myplr, plr[myplr].InvList[c]._iMiscID, plr[myplr].InvList[c]._iSpell);
            RemoveInvItem(myplr, c);
        }
    }
}
