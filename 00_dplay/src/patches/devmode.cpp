#include "patches.hpp"

#include "util.hpp"

void devmode_main()
{
    patch<uint32_t>(0x004BC158, TRUE);
}
