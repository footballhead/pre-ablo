#include "patches.hpp"

#include "util.hpp"

void music_nompq_fix_main()
{
    patch<uint32_t>(0x004BC17C, TRUE);
}
