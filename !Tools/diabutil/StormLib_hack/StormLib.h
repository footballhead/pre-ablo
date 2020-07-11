#pragma once

// (Windows-only) This bypasses a bunch of #pragma lib malarkey and fixes Windows builds.
#ifdef _MSC_VER
#define __STORMLIB_SELF__
#endif

// Would use include_next but not supported on MSVC
#include <../StormLib/src/StormLib.h>
