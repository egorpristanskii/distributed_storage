#pragma once

#include <algorithm>
#include <cstddef>

namespace storage {

template <size_t N>
struct StringLiteral {
    constexpr explicit StringLiteral(const char (&str)[N]) {
        std::copy_n(str, N, value);
    }

    char value[N];
};

}  // namespace storage
