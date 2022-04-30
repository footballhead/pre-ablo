#ifndef __ITEMS_H__
#define __ITEMS_H__

#include <windows.h>

//
// structs
//

#pragma pack(push, 8)
struct ItemStruct
{
  int _itype;
  int _ix;
  int _iy;
  int field_C;
  int _iAnimData;
  int field_14;
  BYTE* _iAnimFrame;
  int _iAnimWidth;
  int field_20;
  int field_24;
  char field_28;
  int field_2C;
  int _iIdentified;
  char _iMagical;
  char _iName[64];
  char _iIName[64];
  char _iLoc;
  char _iClass;
  char _iCurs;
  int _ivalue;
  int _iIvalue;
  int _iMinDam;
  int _iMaxDam;
  int _iAC;
  int _iFlags;
  int _iMiscID;
  int _iSpell;
  int _iCharges;
  int _iMaxCharges;
  int _iDurability;
  int _iMaxDur;
  BYTE gapE8[66];
  char _iMinMag;
  int _iStatFlag;
  int IDidx;
  int field_134;
};
#pragma pack(pop)

#endif