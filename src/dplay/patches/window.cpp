#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME window
DESCRIBE_PATCH("BROKEN! DO NOT USE!\n\nTurn on old windowed mode. Doesn't work on Windows 10!")

PATCH_MAIN
{
    bool ok = true;

    *(uint32_t*)0x0061BF48 = FALSE;
    // nop the instruction in WinMain that initializes this variable
    ok &= nop(0x00484A33, 0x00484A3D);

    return ok;
}
