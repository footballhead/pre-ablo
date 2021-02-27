#pragma once

// These are the coordinates of the top-left corner of the screen.
// (General reminder: sprite origin is bottom-left corner! So X must account for height!)
#define DRAW_ORIGIN_X 64
#define DRAW_ORIGIN_Y 160

#define QTYPE_MAZE 4

#define MAXDUNX 112
#define MAXDUNY 112

#define MAXOBJECTS 127

// TODO: Map out all object types
#define OBJ_BOOKMAZE 0x34

// TODO: Map out all net commands
#define CMD_NEWLVL 0x15