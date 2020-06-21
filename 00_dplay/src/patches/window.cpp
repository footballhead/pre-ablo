#include "patches.hpp"

#include "util.hpp"

void window_main()
{
    bool ok = true;

    *(uint32_t*)0x0061BF48 = FALSE;
    // nop the instruction in WinMain that initializes this variable
    ok &= nop(0x00484A33, 0x00484A3D);

    printf("%s %s\n", __func__, ok ? "success" : "fail");
}
