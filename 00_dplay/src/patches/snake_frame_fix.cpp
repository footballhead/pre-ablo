#include "patches.hpp"

#include "util.hpp"
#include "variables.hpp"

#define PATCH_NAME snake_frame_fix
DESCRIBE_PATCH("Prevent a crash when drawing the snake enemies by altering the animation frames to match the assets we have.")

namespace {

constexpr int new_mImgSize = 2220;
constexpr int new_Frames[] = {12, 11, 13, 5, 18, 16}; // only [1] and [2] differ

} // namespace

PATCH_MAIN
{
    // This is the data segment, no need to adjust permissions
    for (auto i = 70; i <= 73; ++i) {
        for (auto j = 0; j < 6; ++j) {
            monsterdata[i].Frames[j] = new_Frames[j];
        }
    }
    return true;
}
