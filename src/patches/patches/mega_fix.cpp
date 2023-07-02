#include "patches.hpp"

#include "util.hpp"
#include "variables.hpp"

#define PATCH_NAME mega_fix
DESCRIBE_PATCH(R"txt(Modify Mega Demon monster data; decrease mImgSize so Mega Demons appear in the caves and beyond

Toggling will ruin old saves, so it's recommended to start a new character.)txt")

namespace {

constexpr int new_mImgSize = 2220;

} // namespace

PATCH_MAIN
{
    // This is the data segment, no need to adjust permissions
    for (auto i = 83; i <= 86; ++i) {
        monsterdata[i].mImgSize = new_mImgSize;
    }
    return true;
}
