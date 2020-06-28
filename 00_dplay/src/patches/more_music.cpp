#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME more_music
DESCRIBE_PATCH("If you get sick of the same short music tracks like I do then this adds retail music.")

PATCH_MAIN
{
    // TODO bundle the music into the MPQ as part of build (if this ships)
    *(char*)0x004BC1D6 = 'B';
    *(char*)0x004BC1E6 = 'C';
    *(char*)0x004BC1F6 = 'D';
    return true;
}
