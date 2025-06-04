#include "network/request.h"

#include <sstream>
#include <string>

namespace network {
Request::Request(const std::string& raw_request) {
    std::istringstream stream(raw_request);

    std::string line;
    std::string method_str;
    std::getline(stream, line);
    std::istringstream request_line(line);

    request_line >> method_str >> path;
    method = kHTTPMethodMap.at(method_str);

    if (!path.empty() && path.front() == '/') {
        path.erase(0, 1);
    }

    while (std::getline(stream, line) && line != "\r") {
        auto colon = line.find(':');

        if (colon != std::string::npos) {
            std::string key = line.substr(0, colon);
            std::string value = line.substr(colon + 1);
            value.erase(0, value.find_first_not_of(" \t\r\n"));
            value.erase(value.find_last_not_of(" \t\r\n") + 1);
            headers[key] = value;
        }
    }

    if (headers.contains("Content-Length")) {
        size_t content_length = std::stoul(headers["Content-Length"]);

        body.resize(content_length);
        stream.read(body.data(), content_length);
    }
}
}  // namespace network
