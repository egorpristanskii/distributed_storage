#pragma once
#include "network/response.h"

#include <functional>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <type_traits>

namespace router {
using json = nlohmann::json;
using Handler = std::function<network::Response(const json&)>;

template <typename TDelegator, typename THandler>
concept ValidHandler = std::is_member_function_pointer_v<THandler>;

class Router {
   public:
    template <typename TDelegator, typename THandler>
        requires ValidHandler<TDelegator, THandler>
    void addRoute(const std::string& path, TDelegator& delegator,
                  THandler handler) {
        routes_[path] = [&delegator,
                         handler](const json& request) -> network::Response {
            return std::invoke(handler, delegator, request);
        };
    }

    network::Response handleRoute(const std::string& path,
                                  const json& request) {
        return routes_[path](request);
    }

   private:
    std::map<std::string, Handler> routes_;
};
}  // namespace router
