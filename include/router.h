#pragma once
#include <functional>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <type_traits>

namespace router {
using json = nlohmann::json;
using Handler = std::function<std::string(const json&)>;

template <typename TDelegator, typename THandler>
concept ValidHandler = std::is_member_function_pointer_v<THandler>;

class Router {
   public:
    template <typename TDelegator, typename THandler>
        requires ValidHandler<TDelegator, THandler>
    void addRoute(const std::string& path, TDelegator& delegator,
                  THandler handler) {
        routes_[path] = [&delegator,
                         handler](const json& request) -> std::string {
            return std::invoke(handler, delegator, request);
        };
    }

    std::string handleRoute(const std::string& path, const json& request) {
        return routes_[path](request);
    }

   private:
    std::map<std::string, Handler> routes_;
};
}  // namespace router
