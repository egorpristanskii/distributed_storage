#pragma once
#include <string>
#include <unordered_map>

namespace network {
struct Request {
    explicit Request(const std::string& raw_request);

    std::string method;
    std::string path;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};
}  // namespace network
