#pragma once

#include <cstdint>
#include <cstdio>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

constexpr uint8_t nop_opcode = 0x90;
constexpr auto kCallInstructionSize = 5;

template <typename T>
bool patch(void* addr_to_patch, T new_val) {
  // If we don't turn the .text address to be PAGE_EXECUTE_READWRITE then the
  // game crashes
  DWORD oldProtect = 0;
  if (!VirtualProtect(addr_to_patch, sizeof(T), PAGE_EXECUTE_READWRITE,
                      &oldProtect)) {
    printf("%s: failed to mark address +xrw: 0x%p\n", __func__, addr_to_patch);
    return false;
  }
  // Do the actual patch
  *(T*)(addr_to_patch) = new_val;
  return true;
}

template <typename T>
bool patch(uint32_t address, T new_val) {
  return patch((void*)address, new_val);
}

// Replaces bytes between [start, end) with x86 NOP instructions.
bool nop(uint32_t address_start, uint32_t address_end);

// Overwrites the contents starting at the given address with `call fn`
bool patch_call(uint32_t address, void* fn);

// Overwrites the contents starting at the given address with `call fn`
template <typename T>
bool patch_call(uint32_t address, T fn) {
  return patch_call(address, reinterpret_cast<void*>(fn));
}

// Overwrites the contents starting at the given address with `jmp to`
bool patch_jmp(uint32_t address, void* to);

template <typename T>
bool patch_jmp(uint32_t address, T to)
{
  return patch_jmp(address, reinterpret_cast<void*>(to));
}

// Overwrites the contents starting at the given address with `push global_var`
bool patch_push(uint32_t address, void* global_var);

bool patch_bytes(uint32_t address, uint8_t const* patch, size_t size);

inline bool patch_dword(uint32_t address, uint32_t dword) {
  return patch_bytes(address, reinterpret_cast<uint8_t const*>(&dword),
                     sizeof(dword));
}

inline bool patch_byte(uint32_t address, uint8_t byte) {
  return patch_bytes(address, reinterpret_cast<uint8_t const*>(&byte),
                     sizeof(byte));
}