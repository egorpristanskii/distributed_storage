#include "storage/storage_router_adapter.h"

#include "network/response.h"
#include "storage/value.h"

#include <iostream>
#include <string>

namespace storage {
StorageRouterAdapter::StorageRouterAdapter(const std::string& logFile)
    : storage_(std::make_unique<Storage>(logFile)) {
    std::cout << "inited" << std::endl;
}

network::Response StorageRouterAdapter::get(const json& request) {
    std::cout << "Call get with request" << request.dump(4) << std::endl;
    std::string key = request["key"];

    ValuePtr value = storage_->get(key);

    if (value == nullptr) {
        return network::Response(400, "");
    }

    return network::Response(200, value->toString());
}

network::Response StorageRouterAdapter::put(const json& request) {
    std::cout << "Call put with request" << request.dump(4) << std::endl;
    std::string key = request["key"];
    std::string value = request["value"];
    std::string type = request["type"];

    bool result = false;

    if (type == "int") {
        result = storage_->put(key, std::make_unique<IntData>(value));
    } else if (type == "string") {
        result = storage_->put(key, std::make_unique<StringData>(value));
    }
    if (result) {
        return network::Response(200, "success");
    }
    return network::Response(400, "failed");
}

network::Response StorageRouterAdapter::remove(const json& request) {
    std::string key = request["key"];
    bool result = storage_->remove(key);

    if (result) {
        return network::Response(200, "success");
    }
    return network::Response(400, "failed");
}

network::Response StorageRouterAdapter::listAllData(
    const json& request) {  // NOLINT
    std::cout << "Call listAllData with request" << std::endl;
    auto response_data = storage_->listAllData();

    return network::Response(200, response_data.dump());
}
}  // namespace storage
