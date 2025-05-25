#pragma once

#include "network/response.h"
#include "storage/storage.h"

namespace storage {
class StorageRouterAdapter {
   public:
    explicit StorageRouterAdapter(const std::string& logFile);

    ~StorageRouterAdapter() = default;

    network::Response get(const json& request);

    network::Response put(const json& request);

    network::Response remove(const json& request);

    network::Response listAllData(const json& request);

   private:
    std::unique_ptr<Storage> storage_;
};
}  // namespace storage
