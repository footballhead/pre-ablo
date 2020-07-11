#include "patches.hpp"

#include "util.hpp"
#include "variables.hpp"

#define PATCH_NAME thunder_demon_missile_fix
DESCRIBE_PATCH("Prevent a crash when Thunder Demons attack with Lightning.")

namespace {

//
// Imports
//

constexpr auto GetSpellLevel_addr = 0x00453CDB;
auto const GetSpellLevel = reinterpret_cast<int (__fastcall *)(int id, int sn)>(GetSpellLevel_addr);

//
// Hooks
//

int __fastcall GetSpellLevel_wrapper(int id, int sn)
{
    return id == *myplr ? GetSpellLevel(id, sn) : 1;
}

} // namespace

PATCH_MAIN
{
    bool ok = true;

    // Because I'm lazy, and the only known issue is Lightning related, only patch Lightning
    ok &= patch_call(0x0043733B, (void*)GetSpellLevel_wrapper);

    return ok;
}
