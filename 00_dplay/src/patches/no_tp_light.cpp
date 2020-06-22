#include "patches.hpp"

#include "util.hpp"

void no_tp_light_main()
{
    bool ok = true;
    ok &= nop(0x0043DE87, 0x0043DF14);
    printf("%s %s\n", __func__, ok ? "success" : "fail");
}
