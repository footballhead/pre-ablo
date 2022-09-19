#ifndef __LIGHTING_H__
#define __LIGHTING_H__

#include <windows.h>

//
// variables
//

extern BOOL lightflag;
// CrawlTable is weird because it is structured data being represented in a flat array.
// See Devilution for one explanation.
// I like to think of them as tree rings or a bullseye: concentric rings starting from inside and going outward.
// Each ring is defined by a number of (x,y) offsets, followed by a list of (x,y) offsets
// These offsets are relative to a location and together will form a ring around that location.
// Since larger rings have more offsets, each ring is a different size in the array.
// Typically this will be indexed by something like CrawlNum, which will specify the offset for each ring.
// I'm guessing that this is a pre-computed alternative to euclidean distance up to a certain integer distance
extern char CrawlTable[];
extern BYTE vCrawlTable[23][30];

//
// functions
//

void MakeLightTable();
int AddLight(int x, int y, int r);
void AddUnLight(int i);
void DoLighting(int nXPos, int nYPos, int nRadius, int Lnum);

#endif