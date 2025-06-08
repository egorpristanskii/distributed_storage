#include "storage/storage_router_adapter.h"

#include "network/http_codes.h"
#include "network/response.h"
#include "storage/value.h"

#include <logger/logger.h>
#include <nlohmann/json_fwd.hpp>
#include <string>

namespace storage {
StorageRouterAdapter::StorageRouterAdapter(const std::string& logFile)
    : storage_(std::make_unique<Storage>(logFile)) {}

network::Response StorageRouterAdapter::get(const json& request) {
    LOG_DEBUG("Call get with request {}", request.dump());
    std::string key = request["id"];

    ValuePtr value = storage_->get(key);
    nlohmann::json response_json;

    if (value == nullptr) {
        response_json["error"] = "Key not found";
        return network::Response(network::HTTPCode::NotFound,
                                 response_json.dump());
    }

    response_json["key"] = key;
    response_json["value"] = value->toString();
    response_json["type"] = value->typeName();
    return network::Response(network::HTTPCode::OK, response_json.dump());
}

network::Response StorageRouterAdapter::put(const json& request) {
    LOG_DEBUG("Call put with request {}", request.dump());
    std::string key = request["id"];
    std::string value = request["value"];
    std::string type = request["type"];

    std::unique_ptr<Value> result;
    nlohmann::json response_json;

    if (type == "int") {
        result = storage_->put(key, std::make_unique<IntData>(value));
    } else if (type == "string") {
        result = storage_->put(key, std::make_unique<StringData>(value));
    } else {
        response_json["error"] = "Unsupported type";
        return network::Response(network::HTTPCode::BadRequest,
                                 response_json.dump());
    }

    response_json["key"] = key;
    response_json["value"] = result->toString();
    response_json["type"] = result->typeName();
    return network::Response(network::HTTPCode::OK, response_json.dump());
}

network::Response StorageRouterAdapter::remove(const json& request) {
    std::string key = request["id"];
    bool result = storage_->remove(key);
    nlohmann::json response_json;

    if (result) {
        return network::Response(network::HTTPCode::NoContent, "");
    }

    response_json["error"] = "Key not found";
    return network::Response(network::HTTPCode::NotFound, response_json.dump());
}

network::Response StorageRouterAdapter::listAllData(const json& request) {
    LOG_DEBUG("Call list all data with request {}", request.dump());
    auto response_data = storage_->listAllData();
    if (response_data.empty()) {
        return network::Response(network::HTTPCode::NoContent, "");
    }
    return network::Response(network::HTTPCode::OK, response_data.dump());
}
}  // namespace storage
