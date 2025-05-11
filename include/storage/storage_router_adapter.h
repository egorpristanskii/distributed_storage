#pragma once

#include "storage/storage.h"

namespace storage {
class StorageRouterAdapter {
   public:
    explicit StorageRouterAdapter(const std::string& logFile);

    ~StorageRouterAdapter() = default;

    std::string get(const json& request);

    std::string put(const json& request);

    std::string remove(const json& request);

    //  std::string listAllData(const json& request);
   private:
    std::unique_ptr<Storage> storage_;
};
}  // namespace storage
