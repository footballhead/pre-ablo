#include "patches.hpp"

#include "util.hpp"

bool fullgame_main()
{
    // This is the data segment, no need to adjust permissions
    *(uint32_t*)0x004BC15C = FALSE;
    return true;
}
