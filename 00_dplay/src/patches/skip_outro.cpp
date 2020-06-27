#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME skip_outro
DESCRIBE_PATCH("BROKEN! DO NOT USE!\n\nSkip the outro slideshow on the main menu. Makes quitting faster.")

// unfortunately I think this causes a rare hang on shutdown trying to free gfx...
PATCH_MAIN
{
    bool ok = true;
    // Remove all logic related to cycling through screenshots, skip straight to free & quit
    ok &= nop(0x0041AD4D, 0x0041AF40);
    // Don't early return from quit logic, instead fall through to WM_DESTROY logic
    ok &= nop(0x0041B042, 0x0041B049);
    return ok;
}
