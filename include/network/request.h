#pragma once

#include "network/types.h"

#include <string>
#include <unordered_map>

namespace network {
struct Request {
    explicit Request(const std::string& raw_request);

    std::string path;
    std::string body;
    HTTPMethod method;
    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> params;
};
}  // namespace network
