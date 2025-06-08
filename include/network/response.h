#pragma once
#include "network/http_codes.h"

#include <string>

namespace network {
struct Response {
    HTTPCode status_code;
    const std::string response_data;

    std::string toString() const;
};
}  // namespace network
