#pragma once

// Debug print macros. Disabled on release builds to improve performance

#ifndef NDEBUG
#include <cstdio>
#define TRACE(msg, ...) printf((msg), ##__VA_ARGS__)
#else
#define TRACE(msg, ...)
#endif