#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME music_nompq_fix
DESCRIBE_PATCH(R"txt(NOT RECOMMENDED!

This is an old fix to allow playing music from the filesystem instead of the MPQ. Now that the files are packaged in the MPQ, this is not recommended.)txt");

PATCH_MAIN
{
    // This is the data segment, no need to adjust permissions
    *(uint32_t*)0x004BC17C = TRUE;
    return true;
}
