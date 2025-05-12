#pragma once

#include "network/router.h"

#include <asio.hpp>
#include <asio/awaitable.hpp>

namespace network {
class Session {
   public:
    Session(asio::ip::tcp::socket, std::shared_ptr<router::Router> router);

    asio::awaitable<void> operator()();

   private:
    asio::ip::tcp::socket socket_;
    std::shared_ptr<router::Router> router_;
};
}  // namespace network
