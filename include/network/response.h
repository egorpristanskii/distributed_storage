#pragma once
#include <string>

namespace network {
struct Response {
    const int status_code;
    const std::string response_data;

    std::string toString() const;
};
}  // namespace network
