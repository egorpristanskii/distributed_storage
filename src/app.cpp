#include "app.h"

#include "network/router.h"

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
}

network::Response Application::handleRequest(const network::HTTPMethod& method,
                                             const std::string& path,
                                             const nlohmann::json& request) {
    return router_->handleRoute(method, path, request);
}
}  // namespace app
