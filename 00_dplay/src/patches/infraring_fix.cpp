#include "patches.hpp"

#include "util.hpp"

namespace {

//
// Imports
//

auto const DrawUniqueInfo = reinterpret_cast<void (__fastcall *)()>(0x00423BA4);
// curruitem is an item struct but we only care about this one address
auto* const curruitem_iUid = reinterpret_cast<int8_t* const>(0x005DED6F);

//
// Hooks
//

void __fastcall DrawUniqeInfo_Wrapper()
{
    if (*curruitem_iUid != -1) {
        DrawUniqueInfo();
    }
}

} // namespace

bool infraring_fix_main()
{
    bool ok = true;
    ok &= patch_call(0x00448C2B, (void*)DrawUniqeInfo_Wrapper);
    ok &= patch_call(0x004812F9, (void*)DrawUniqeInfo_Wrapper);
    return ok;
}
