#pragma once

#include "network/types.h"

#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

namespace network {
struct Request {
    explicit Request(const std::string& raw_request);

    Request(const HTTPMethod& http_method, std::string path, std::string body);

    HTTPMethod method;
    std::string path;
    std::string body;
    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> params;

    std::string_view methodToString() const {
        return kHTTPMethodStringMap.at(method);
    }

    nlohmann::json parseBody() const {
        nlohmann::json json_body;
        if (!body.empty()) {
            json_body = nlohmann::json::parse(body);
        }
        return json_body;
    }
};
}  // namespace network
