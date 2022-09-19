#ifndef __DEFINES_H__
#define __DEFINES_H__

#include <windows.h>

// Array extents
#define NUM_INV_GRID_ELEM 40

// Basic window properties
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 8

// Amount of padding added to final image. For some reason, the image buffer
// is not just the resolution but also has some additional space...
//
// My current theory is that this is to avoid performing clipping checking
// in the drawing routines. For example, the tallest sprite is 160px tall. If
// there's blank space up to 160px off the top of the screen then the drawing
// algorithms don't have to waste cycles determining whether or not something
// needs to not be drawn to prevent buffer overrun/underrun.
#define BORDER_LEFT 64
#define BORDER_TOP 160
#define BORDER_RIGHT 64
#define BORDER_BOTTOM 16

// Where the top-left of the screen is inside the actual image buffer
#define SCREEN_X BORDER_LEFT
#define SCREEN_Y BORDER_TOP

// The total size of the image accounts for both the visible and invisible
// portions. The visible portion is sandwiched on all sides by unused space
#define BUFFER_WIDTH (BORDER_LEFT + SCREEN_WIDTH + BORDER_RIGHT)
#define BUFFER_HEIGHT (BORDER_TOP + SCREEN_HEIGHT + BORDER_BOTTOM)

// Find the buffer position of a given mouse coordinate inside the window
#define SCREENXY(x, y) ((x) + SCREEN_X + ((y) + SCREEN_Y) * BUFFER_WIDTH)

// The game treats the top (viewport) and bottom (panel) sections independently.
// The panel size is driven by the dimensions of panel7.cel. The viewport is
// everything that's not the panel.
#define PANEL_WIDTH 640
#define PANEL_HEIGHT 128
//  #define VIEWPORT_WIDTH SCREEN_WIDTH
#define VIEWPORT_HEIGHT (SCREEN_HEIGHT - PANEL_HEIGHT)

#define COL_RED 2

#define NUMLEVELS 17

#endif