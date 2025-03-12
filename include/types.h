#pragma once

#include <algorithm>
#include <cstddef>
#include <map>
#include <string_view>

namespace storage {
constexpr char kIntTypeName[]{"int"};
constexpr char kStringTypeName[]{"std::string"};

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
    static constexpr NamedType kValue = NamedType{kIntTypeName};
};

template <>
struct TypeName<std::string> {
    static constexpr NamedType kValue = NamedType{kStringTypeName};
};

enum class TypeNameEnum { IntType, StringType };

enum class OperationEnum { Put, Delete, Get };

const std::map<std::string_view, TypeNameEnum> kStringToType = {
    {kIntTypeName, TypeNameEnum::IntType},
    {kStringTypeName, TypeNameEnum::StringType},
};

const std::map<std::string_view, OperationEnum> kStringToOperation = {
    {"PUT", OperationEnum::Put},
    {"DELETE", OperationEnum::Delete},
    {"GET", OperationEnum::Get},
};

// Used for restoring data from log file.
// Recover converts to map with structure
// {"key": {"dataType": "value"}}
using LogOperation = std::map<std::string, std::pair<std::string, std::string>>;

}  // namespace storage
