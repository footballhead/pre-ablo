#include "patches.hpp"

#include "util.hpp"

bool devmode_main()
{
    // This is the data segment, no need to adjust permissions
    *(uint32_t*)0x004BC158 = TRUE;
    return true;
}
