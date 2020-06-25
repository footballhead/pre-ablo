#include "patches.hpp"

#include "util.hpp"

bool music_nompq_fix_main()
{
    // This is the data segment, no need to adjust permissions
    *(uint32_t*)0x004BC17C = TRUE;
    return true;
}
