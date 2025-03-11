#pragma once

#include <algorithm>
#include <cstddef>
#include <string_view>

namespace storage {

template <size_t Length>
class NamedType {
   public:
    consteval explicit NamedType(const char (&new_literal)[Length]) {
        std::copy_n(new_literal, kLength, literal);
    }

    [[nodiscard]] constexpr std::string_view view() const {
        // -1 is required for excluding null terminator
        return {literal, literal + kLength - 1};
    }

    char literal[Length]{};
    static constexpr std::size_t kLength = Length;
};

template <typename TData>
struct TypeName;

template <>
struct TypeName<int> {
    static constexpr NamedType kValue = NamedType{"int"};
};

template <>
struct TypeName<std::string> {
    static constexpr NamedType kValue = NamedType{"std::string"};
};

}  // namespace storage
