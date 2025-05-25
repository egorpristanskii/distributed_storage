#pragma once

#include "network/router.h"
#include "storage/storage_router_adapter.h"

namespace app {
class Application {
   public:
    explicit Application(const std::string& logFile);

    [[nodiscard]] network::Response handleRequest(
        const std::string& path, const nlohmann::json& request);

   private:
    std::shared_ptr<router::Router> router_;
    std::shared_ptr<storage::StorageRouterAdapter> handler_;
};
}  // namespace app
