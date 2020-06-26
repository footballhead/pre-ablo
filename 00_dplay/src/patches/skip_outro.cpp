#include "patches.hpp"

#include "util.hpp"

// unfortunately I think this causes a rare hang on shutdown trying to free gfx...
bool skip_outro_main()
{
    bool ok = true;
    // Remove all logic related to cycling through screenshots, skip straight to free & quit
    ok &= nop(0x0041AD4D, 0x0041AF40);
    // Don't early return from quit logic, instead fall through to WM_DESTROY logic
    ok &= nop(0x0041B042, 0x0041B049);
    return ok;
}
