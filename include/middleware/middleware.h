#pragma once
#include "network/request.h"
#include "network/response.h"

#include <asio/awaitable.hpp>
#include <functional>
#include <initializer_list>
#include <memory>
#include <vector>

namespace middleware {
using AsyncTask =
    std::function<asio::awaitable<network::Response>(const network::Request&)>;

class IMiddleware {
   public:
    asio::awaitable<network::Response> execute(const AsyncTask& task,
                                               const network::Request& request);

   private:
    virtual void preprocess(const network::Request& request) const = 0;

    virtual void postprocess(const network::Response& response) const = 0;
};

class MiddleWareChain {
   public:
    MiddleWareChain(
        std::initializer_list<std::shared_ptr<IMiddleware>> middlewares);

    asio::awaitable<network::Response> operator()(
        const network::Request& request) const;

    AsyncTask buildPipeline(AsyncTask handler);

   private:
    std::vector<std::shared_ptr<IMiddleware>> middlewares_;
    AsyncTask pipeline_;
};
}  // namespace middleware
