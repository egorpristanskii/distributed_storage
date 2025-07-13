#include "app.h"

#include "middleware/middleware.h"
#include "network/request.h"
#include "network/response.h"
#include "network/router.h"

#include <asio/awaitable.hpp>

namespace app {
Application::Application(const std::string& logFile)
    : router_(std::make_shared<router::Router>()),
      handler_(std::make_shared<storage::StorageRouterAdapter>(logFile)) {
    router_->addRoute(network::HTTPMethod::GET, "keys/{id}", handler_,
                      &storage::StorageRouterAdapter::get);
    router_->addRoute(network::HTTPMethod::POST, "keys/{id}", handler_,
                      &storage::StorageRouterAdapter::put);
    router_->addRoute(network::HTTPMethod::DELETE, "keys/{id}", handler_,
                      &storage::StorageRouterAdapter::remove);
    router_->addRoute(network::HTTPMethod::GET, "allkeys", handler_,
                      &storage::StorageRouterAdapter::listAllData);

    auto app_handler = [this](const network::Request& request) {
        return this->asyncHandleRequest(request);
    };
    pipeline_ = middleware::MiddleWareChain(
                    {std::make_shared<middleware::LogMiddleware>()})
                    .buildPipeline(app_handler);
}

asio::awaitable<network::Response> Application::processRequest(
    const network::Request& request) {
    auto response = co_await pipeline_(request);
    co_return response;
}

asio::awaitable<network::Response> Application::asyncHandleRequest(
    const network::Request& request) {
    co_return router_->handleRoute(request.method, request.path,
                                   request.parseBody());
}

network::Response Application::handleRequest(const network::Request& request) {
    return router_->handleRoute(request.method, request.path,
                                request.parseBody());
}
}  // namespace app
