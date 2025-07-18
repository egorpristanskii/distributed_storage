#pragma once

#include "middleware/log_middleware.h"
#include "network/request.h"
#include "network/response.h"
#include "network/router.h"
#include "storage/storage_router_adapter.h"

#include <asio/awaitable.hpp>

namespace app {
class Application {
   public:
    explicit Application(std::shared_ptr<storage::WALLogger> operation_logger);

    [[nodiscard]] network::Response handleRequest(
        const network::Request& request);

    [[nodiscard]] asio::awaitable<network::Response> asyncProcessRequest(
        network::Request request);

   private:
    std::shared_ptr<router::Router> router_;
    std::shared_ptr<storage::StorageRouterAdapter> handler_;
    middleware::AsyncTask pipeline_;

    [[nodiscard]] asio::awaitable<network::Response> asyncHandleRequest(
        const network::Request& request);
};
}  // namespace app
