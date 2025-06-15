#pragma once

#include "promise.h"

#include <coroutine>

namespace async {
template <typename T>
struct Task {
    struct promise_type : Promise<T> {  // NOLINT

        Task get_return_object() {
            return Task<T>{
                std::coroutine_handle<promise_type>::from_promise(*this)};
        }
    };

    bool await_ready() const noexcept { return !handler || handler.done(); }

    T await_resume() noexcept { return handler.promise().response; }

    void await_suspend(std::coroutine_handle<> awaitable) noexcept {
        handler.promise().awaitable = awaitable;
        handler.resume();
    }

    T getValue() const { return handler.promise().response; }

    std::coroutine_handle<promise_type> handler;
};
}  // namespace async
