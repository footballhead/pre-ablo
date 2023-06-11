#pragma once

#include "ddraw.h"
#include "log.h"

// I leave a bunch of functions unimplemented, and they all look the same. This
// macro reduces typing and vertical code size

#define STUB(X, ...)                \
  HRESULT X(__VA_ARGS__) noexcept { \
    TRACE(#X "\n");                 \
    return DDERR_UNSUPPORTED;       \
  }