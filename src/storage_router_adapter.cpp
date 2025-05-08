#include "storage_router_adapter.h"

#include "value.h"

#include <iostream>

namespace storage {
StorageRouterAdapter::StorageRouterAdapter(const std::string& logFile)
    : storage_(std::make_unique<Storage>(logFile)) {}

std::string StorageRouterAdapter::get(const json& request) {
    std::cout << "Call get with request" << request.dump(4) << std::endl;
    std::string key = request["key"];

    ValuePtr value = storage_->get(key);

    if (value == nullptr) {
        return "";
    }

    return value->toString();
}

std::string StorageRouterAdapter::put(const json& request) {
    std::cout << "Call put with request" << request.dump(4) << std::endl;
    std::string key = request["key"];
    std::string value = request["value"];

    bool result = storage_->put(key, std::make_unique<StringData>(value));
    if (result) {
        return "success";
    }
    return "failed";
}

std::string StorageRouterAdapter::remove(const json& request) {
    std::string key = request["key"];
    bool result = storage_->remove(key);

    if (result) {
        return "success";
    }

    return "failed";
}
}  // namespace storage
