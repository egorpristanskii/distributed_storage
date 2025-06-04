#pragma once
#include <string>
#include <unordered_map>

namespace network {
enum class HTTPMethod { GET, POST, PUT, DELETE };

const std::unordered_map<std::string, HTTPMethod> kHTTPMethodMap = {
    {"GET", HTTPMethod::GET},
    {"POST", HTTPMethod::POST},
    {"PUT", HTTPMethod::PUT},
    {"DELETE", HTTPMethod::DELETE}};

const std::unordered_map<HTTPMethod, std::string> kHTTPMethodStringMap = {
    {HTTPMethod::GET, "GET"},
    {HTTPMethod::POST, "POST"},
    {HTTPMethod::PUT, "PUT"},
    {HTTPMethod::DELETE, "DELETE"}};
}  // namespace network
