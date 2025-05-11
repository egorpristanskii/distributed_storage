#include "network/response.h"

#include <format>

namespace network {
std::string Response::toString() const {
    return std::format(
        "HTTP/1.1 {} OK\r\n"  // TODO (e_pristanskiy) update status codes creation
        "Content-Type: application/json\r\n"
        "Content-Length: {}\r\n\r\n"
        "{}",
        status_code, response_data.size(), response_data);
}
}  // namespace network
