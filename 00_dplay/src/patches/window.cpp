#include "patches.hpp"

#include "util.hpp"

void window_main()
{
    *(uint32_t*)0x0061BF48 = FALSE;
    // nop the instruction in WinMain that initializes this variable
    nop(0x00484A33, 0x00484A3D);
}
