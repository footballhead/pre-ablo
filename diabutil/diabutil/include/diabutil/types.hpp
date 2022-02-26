#pragma once

#include <diabutil/span.hpp>

namespace diabutil {
using byte_vector = std::vector<std::byte>;
using const_byte_span = span<std::byte const>;
}  // namespace diabutil
