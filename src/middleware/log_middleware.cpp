#include "middleware/log_middleware.h"

#include "logger/logger.h"
#include "network/response.h"

namespace middleware {
void LogMiddleware::preprocess(const network::Request& request) const {
    LOG_INFO("Received {} request path {} with body {}",
             request.methodToString(), request.path, request.body);
}

void LogMiddleware::postprocess(const network::Response& response) const {
    LOG_INFO("Response {} with status code {}", response.response_data,
             response.HTTPCodeToString());
}
}  // namespace middleware
