#include "patches.hpp"

#include "util.hpp"
#include "variables.hpp"

namespace {

constexpr int new_mImgSize = 2220;
constexpr int new_Frames[] = {6, 7, 14, 1, 24, 5};
constexpr int new_Rate[] = {3, 0, 0, 0, 2, 0};

} // namespace

bool mega_fix_main()
{
    // This is the data segment, no need to adjust permissions
    for (auto i = 83; i <= 86; ++i) {
        monsterdata[i].mImgSize = new_mImgSize;

        // For whatever reason, std::copy produced bad results...
        for (auto j = 0; j < 6; ++j) {
            monsterdata[i].Frames[j] = new_Frames[j];
            monsterdata[i].Rate[j] = new_Rate[j];
        }
    }
    return true;
}
