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
    virtual std::string_view typeName() = 0;
};

using ValuePtr = std::unique_ptr<Value>;

template <typename TData, NamedType TName = TypeName<TData>::kValue>
class TypedData : public Value {
   public:
    explicit TypedData(TData data) : data_(std::move(data)) {}

    std::string toString() const override {
        if constexpr (std::is_same_v<TData, std::string>) {
            return data_;
        } else {
            return std::to_string(data_);
        }
    }

    ValuePtr cloneData() const override {
        return std::make_unique<TypedData<TData, TName>>(data_);
    }

    [[nodiscard]] std::string_view typeName() override { return TName.view(); }

   protected:
    TData data_;
};

using StringData = TypedData<std::string>;
using IntData = TypedData<int>;
}  // namespace storage
