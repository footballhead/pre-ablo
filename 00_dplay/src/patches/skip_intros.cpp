#include "patches.hpp"

#include "util.hpp"

void skip_intros_main()
{
    patch<uint32_t>(0x004BC160, FALSE);
}
