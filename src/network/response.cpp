#include "network/response.h"

#include "network/http_codes.h"

#include <format>

namespace network {
std::string Response::toString() const {
    return std::format(
        "HTTP/1.1 {}\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: {}\r\n\r\n"
        "{}",
        kHTTPCodeToString.at(status_code), response_data.size(), response_data);
}

std::string_view Response::HTTPCodeToString() const {
    return kHTTPCodeToString.at(status_code);
}
}  // namespace network
