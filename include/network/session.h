#pragma once

#include "app.h"

#include <asio.hpp>
#include <asio/awaitable.hpp>

namespace network {
class Session {
   public:
    Session(asio::ip::tcp::socket, std::shared_ptr<app::Application> app);

    asio::awaitable<void> operator()();

   private:
    asio::ip::tcp::socket socket_;
    std::shared_ptr<app::Application> app_;
};
}  // namespace network
