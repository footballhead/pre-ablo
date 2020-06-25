#include "patches.hpp"

#include "util.hpp"

bool skip_intros_main()
{
    // This is the data segment, no need to adjust permissions
    *(uint32_t*)0x004BC160 = FALSE;
    return true;
}
