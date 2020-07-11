#include "util.hpp"

#include <algorithm>

namespace {

constexpr uint8_t call_opcode = 0xE8;
constexpr uint8_t jmp_opcode = 0xE9;
constexpr uint8_t push_opcode = 0x68;

} // namespace

bool nop(uint32_t address_start, uint32_t address_end)
{
    DWORD oldProtect = 0;
    if (!VirtualProtect((void*)address_start, address_end - address_start, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        printf("%s: failed to mark address +xrw: 0x%X\n", __func__, address_start);
        return false;
    }
    // Do the actual patch
    memset((void*)address_start, nop_opcode, address_end - address_start);
    return true;
}

bool patch_call(uint32_t address, void* fn)
{
    DWORD oldProtect = 0;
    if (!VirtualProtect((void*)address, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        printf("%s: failed to mark address +xrw: 0x%X\n", __func__, address);
        return false;
    }
    *(uint8_t*)(address) = call_opcode;
    *(int32_t*)(address + 1) = (int32_t)fn - (address + 5);
    return true;
}

bool patch_jmp(uint32_t address, void* to)
{
    DWORD oldProtect = 0;
    if (!VirtualProtect((void*)address, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        printf("%s: failed to mark address +xrw: 0x%X\n", __func__, address);
        return false;
    }
    *(uint8_t*)(address) = jmp_opcode;
    *(int32_t*)(address + 1) = (int32_t)to - (address + 5);
    return true;
}

bool patch_push(uint32_t address, void* global_var)
{
    DWORD oldProtect = 0;
    if (!VirtualProtect((void*)address, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        printf("%s: failed to mark address +xrw: 0x%X\n", __func__, address);
        return false;
    }
    *(uint8_t*)(address) = push_opcode;
    *(uint32_t*)(address + 1) = (uint32_t)global_var;
    return true;
}

bool patch_bytes(uint32_t address, uint8_t const* patch, size_t size)
{
    DWORD oldProtect = 0;
    if (!VirtualProtect((void*)address, size, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        printf("%s: failed to mark address +xrw: 0x%X\n", __func__, address);
        return false;
    }
    std::copy(patch, patch + size, (uint8_t*)address);
    return true;
}
