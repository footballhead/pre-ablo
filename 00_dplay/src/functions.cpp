#include "functions.hpp"

BYTE* (__fastcall * LoadFileInMem)(const char* pszName) = reinterpret_cast<BYTE* (__fastcall *)(const char* pszName)>(0x00482790);
