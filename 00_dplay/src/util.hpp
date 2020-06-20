#pragma once

#include <cstdint>
#include <cstdio>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

constexpr uint8_t nop_opcode = 0x90;
constexpr uint8_t call_opcode = 0xE8;
constexpr uint8_t jmp_opcode = 0xE9;
constexpr uint8_t push_opcode = 0x68;

template <typename T>
bool patch(void* addr_to_patch, T new_val)
{
    // If we don't turn the .text address to be PAGE_EXECUTE_READWRITE then the game crashes
    DWORD oldProtect = 0;
    if (!VirtualProtect(addr_to_patch, sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtect)) {
        printf("%s: failed to mark address +xrw: 0x%X\n", __func__, addr_to_patch);
        return false;
    }
    // Do the actual patch
    *(T*)(addr_to_patch) = new_val;
    return true;
}

template <typename T>
bool patch(uint32_t address, T new_val)
{
    return patch((void*)address, new_val);
}
