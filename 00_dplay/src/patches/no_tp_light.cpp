#include "patches.hpp"

#include "util.hpp"

bool no_tp_light_main()
{
    bool ok = true;
    ok &= nop(0x0043DE87, 0x0043DF14);
    return ok;
}
