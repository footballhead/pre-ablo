#include "patches.hpp"

#include "util.hpp"

namespace {

// This was painstakingly reconstructed with IDA and Devilution
struct MonsterData {
    int mAnimWidth;
    int mImgSize;
    void *GraphicType;
    int has_special;
    void *sndfile;
    int snd_special;
    int transflag;
    void *TransFile;
    int Frames[6];
    int Rate[6];
    void *mName;
    char mMinDLvl;
    char mMaxDLvl;
    char mLevel;
    char mMinHP;
    char mMaxHP;
    char mAi;
    char mInt;
    char mHit;
    char mAFNum;
    char mMinDamage;
    char mMaxDamage;
    char mHit2;
    char mAFNum2;
    char mMinDamage2;
    char mMaxDamage2;
    char mArmorClass;
    char mMonstClass;
    // padding
    __int16 mMagicRes;
    __int16 mTreasure;
    char mSelFlag;
    // padding
    int mExp;
};

// To make offset math work, we need the compiled MonsterData size to match what's already there.
// This will probably only work on 32 bit machines that use 4 byte alignment
static_assert(sizeof(MonsterData) == 0x70, "MonsterData compiled to wrong size");

auto* const monsterdata = reinterpret_cast<MonsterData*>(0x004AE410);

constexpr int new_mImgSize = 2220;
constexpr int new_Frames[] = {6, 7, 14, 1, 24, 5};
constexpr int new_Rate[] = {3, 0, 0, 0, 2, 0};

} // namespace

void mega_fix_main()
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
}

