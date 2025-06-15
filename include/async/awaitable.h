#pragma once

#include <coroutine>

namespace async {
struct Awaitable {
    static bool await_ready() noexcept { return false; }

    void await_resume() noexcept {}

    void await_suspend(std::coroutine_handle<>) const noexcept {
        if (handler) {
            handler.resume();
        }
    }

    std::coroutine_handle<> handler = nullptr;
};
}  // namespace async
