#include "patches.hpp"

#include "util.hpp"
#include "variables.hpp"

namespace {

constexpr int new_mImgSize = 2220;
constexpr int new_Frames[] = {12, 11, 13, 5, 18, 16}; // only [1] and [2] differ

} // namespace

bool snake_frame_fix_main()
{
    // This is the data segment, no need to adjust permissions
    for (auto i = 70; i <= 73; ++i) {
        for (auto j = 0; j < 6; ++j) {
            monsterdata[i].Frames[j] = new_Frames[j];
        }
    }
    return true;
}
