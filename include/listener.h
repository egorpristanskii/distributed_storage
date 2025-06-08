#pragma once

#include "app.h"

#include <asio.hpp>
#include <asio/awaitable.hpp>
#include <asio/detached.hpp>
#include <asio/executor_work_guard.hpp>
#include <asio/io_context.hpp>
#include <asio/use_awaitable.hpp>

namespace app {
class Listener {
   public:
    Listener(uint16_t port, asio::io_context& ctx,
             std::shared_ptr<Application> app);

    asio::awaitable<void> operator()();

   private:
    uint16_t port_;
    asio::io_context& ctx_;
    std::shared_ptr<Application> app_;
};
}  // namespace app
