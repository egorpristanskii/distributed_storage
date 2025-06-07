#pragma once
#include "network/response.h"
#include "network/types.h"

#include <functional>
#include <nlohmann/json.hpp>
#include <regex>
#include <string>
#include <type_traits>

namespace router {
using json = nlohmann::json;
using Handler = std::function<network::Response(const json&)>;

template <typename TDelegator, typename THandler>
concept ValidHandler = std::is_member_function_pointer_v<THandler>;

struct Route {
    network::HTTPMethod method;
    std::regex path_template;
    std::vector<std::string> path_params;
    Handler handler;
};

class Router {
   public:
    template <typename TDelegator, typename THandler>
        requires ValidHandler<TDelegator, THandler>
    void addRoute(const network::HTTPMethod& method, const std::string& path,
                  std::shared_ptr<TDelegator> delegator, THandler handler) {
        auto [pattern, param_names] = parsePath(path);
        Route route{
            method, std::regex(pattern), param_names,
            [delegator = std::move(delegator), handler = std::move(handler)](
                const json& request) -> network::Response {
                return std::mem_fn(handler)(delegator, request);
            }};
        route_list_.emplace_back(std::move(route));
    }

    network::Response handleRoute(const network::HTTPMethod& method,
                                  const std::string& path,
                                  const json& requestBody) {
        for (const auto& route : route_list_) {
            std::smatch match;
            if (route.method != method) {
                continue;
            }
            if (std::regex_match(path, match, route.path_template)) {
                json request = requestBody;
                for (size_t i = 0; i < route.path_params.size(); ++i) {
                    request[route.path_params[i]] = match[i + 1].str();
                }
                return route.handler(request);
            }
        }
        return network::Response(404, "Not Found");
    }

   private:
    static std::pair<std::string, std::vector<std::string>> parsePath(
        const std::string& path) {

        std::vector<std::string> param_names;
        std::string pattern;
        std::regex param_regex(R"(\{(\w+)\})");

        size_t last_pos = 0;
        auto begin =
            std::sregex_iterator(path.begin(), path.end(), param_regex);
        auto end = std::sregex_iterator();

        for (auto it = begin; it != end; ++it) {
            // Append everything before the match
            pattern += std::regex_replace(
                path.substr(last_pos, it->position() - last_pos),
                std::regex(R"([.^$|()\\+*\[\]])"), R"(\$&)");  // escape regex

            pattern += "([^/]+)";
            param_names.push_back(it->str(1));
            last_pos = it->position() + it->length();
        }

        pattern += std::regex_replace(path.substr(last_pos),
                                      std::regex(R"([.^$|()\\+*\[\]])"),
                                      R"(\$&)");  // escape regex

        return {pattern, param_names};
    }

    std::vector<Route> route_list_;
};
}  // namespace router
