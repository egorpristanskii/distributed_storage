#pragma once

#include "network/response.h"
#include "storage/storage.h"
#include "storage/wal_logger.h"

#include <memory>

namespace storage {
class StorageRouterAdapter {
   public:
    explicit StorageRouterAdapter(std::shared_ptr<WALLogger> operation_logger);

    ~StorageRouterAdapter() = default;

    network::Response get(const json& request);

    network::Response put(const json& request);

    network::Response remove(const json& request);

    network::Response listAllData(const json& request);

   private:
    std::unique_ptr<Storage> storage_;
};
}  // namespace storage
