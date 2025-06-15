#pragma once

#include "awaitable.h"

#include <exception>

namespace async {
template <typename T>
struct Promise {
    static std::suspend_always initial_suspend() noexcept { return {}; }

    Awaitable final_suspend() const noexcept { return Awaitable{awaitable}; }

    void unhandled_exception() { std::terminate(); }

    void return_value(T value) { response = std::move(value); }

    T response;
    std::coroutine_handle<> awaitable = nullptr;
};
}  // namespace async
