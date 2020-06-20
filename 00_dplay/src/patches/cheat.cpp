#include "patches.hpp"

#include "util.hpp"

void cheat_main()
{
    patch<uint32_t>(0x004BC168, TRUE);
}
