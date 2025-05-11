#pragma once
#include <string>
#include <unordered_map>

namespace network {
struct HTTPRequest {
    explicit HTTPRequest(const std::string& raw_request);

    std::string method;
    std::string path;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};
}  // namespace network
