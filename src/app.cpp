#include "app.h"

namespace app {
Application::Application(const std::string& logFile)
    : router_(std::make_shared<router::Router>()),
      handler_(std::make_shared<storage::StorageRouterAdapter>(logFile)) {
    router_->addRoute("get", handler_, &storage::StorageRouterAdapter::get);
    router_->addRoute("put", handler_, &storage::StorageRouterAdapter::put);
    router_->addRoute("remove", handler_,
                      &storage::StorageRouterAdapter::remove);
    router_->addRoute("allkeys", handler_,
                      &storage::StorageRouterAdapter::listAllData);
}

network::Response Application::handleRequest(const std::string& path,
                                             const nlohmann::json& request) {
    return router_->handleRoute(path, request);
}
}  // namespace app
