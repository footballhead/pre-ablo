#include "patches.hpp"

#include "util.hpp"

void fullgame_main()
{
    patch<uint32_t>(0x004BC15C, FALSE);
}
