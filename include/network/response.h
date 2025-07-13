#pragma once
#include "network/http_codes.h"

#include <string>

namespace network {
struct Response {
    HTTPCode status_code;
    std::string response_data;

    std::string toString() const;

    std::string_view HTTPCodeToString() const;
};
}  // namespace network
