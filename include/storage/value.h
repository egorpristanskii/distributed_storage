#pragma once

#include "types.h"

#include <memory>
#include <string>
#include <type_traits>

namespace storage {
class Value {
   public:
    virtual ~Value() = default;
    virtual std::string toString() const = 0;
    virtual std::unique_ptr<Value> cloneData() const = 0;
    virtual std::string_view typeName() const = 0;
    [[nodiscard]] virtual std::map<std::string, std::string> mapView()
        const = 0;
};

using ValuePtr = std::unique_ptr<Value>;

template <typename TData>
concept NotString = !std::same_as<TData, std::string>;

template <typename TData, NamedType TName = TypeName<TData>::kValue>
class TypedData : public Value {
   public:
    explicit TypedData(TData data) : data_(std::move(data)) {}

    template <NotString Udata = TData>
    explicit TypedData(const std::string& data) : data_(fromString(data)) {}

    inline TData fromString(const std::string& data);

    std::string toString() const override {
        if constexpr (std::is_same_v<TData, std::string>) {
            return data_;
        } else {
            return std::to_string(data_);
        }
    }

    ValuePtr cloneData() const override {
        return std::make_unique<TypedData<TData>>(data_);
    }

    [[nodiscard]] std::map<std::string, std::string> mapView() const override {
        return {{"value", toString()}, {"type", std::string(typeName())}};
    }

    [[nodiscard]] std::string_view typeName() const override {
        return TName.view();
    }

   protected:
    TData data_;
};

template <>
inline int TypedData<int>::fromString(const std::string& data) {
    return std::stoi(data);
}

template <>
inline std::string TypedData<std::string>::fromString(const std::string& data) {
    return data;
}

using StringData = TypedData<std::string>;
using IntData = TypedData<int>;
}  // namespace storage
