#include "middleware/middleware.h"

#include <ranges>
#include <utility>

namespace middleware {

asio::awaitable<network::Response> IMiddleware::execute(
    const AsyncTask& task, const network::Request& request) {
    preprocess(request);
    network::Response response = co_await task(request);
    postprocess(response);
    co_return response;
}

MiddleWareChain::MiddleWareChain(
    std::initializer_list<std::shared_ptr<IMiddleware>> middlewares)
    : middlewares_(middlewares) {}

AsyncTask MiddleWareChain::buildPipeline(AsyncTask handler) {
    auto next = std::move(handler);
    for (const auto& middleware : std::ranges::reverse_view(middlewares_)) {
        auto prev = next;
        next = [middleware, prev](const network::Request& request)
            -> asio::awaitable<network::Response> {
            return middleware->execute(prev, request);
        };
    }

    return next;
}
}  // namespace middleware
