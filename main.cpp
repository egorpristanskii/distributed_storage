#include "network/router.h"
#include "network/types.h"
#include "storage/storage_router_adapter.h"

#include <asio.hpp>
#include <asio/awaitable.hpp>
#include <asio/detached.hpp>
#include <asio/use_awaitable.hpp>
#include <iostream>
#include <string>

using asio::awaitable;
using asio::use_awaitable;
using asio::ip::tcp;
using namespace std::literals;

awaitable<void> session(tcp::socket socket, router::Router& router) {  // NOLINT
    std::string raw_request;
    char buf[1024];
    std::size_t n =
        co_await socket.async_read_some(asio::buffer(buf), use_awaitable);
    raw_request.append(buf, n);

    network::HTTPRequest parsed_request(raw_request);

    // Step 5: Optional: parse JSON
    nlohmann::json json_body;
    if (!parsed_request.body.empty()) {
        json_body = nlohmann::json::parse(parsed_request.body);
    }

    // Step 6: Route
    std::string response_body =
        router.handleRoute(parsed_request.path, json_body);

    nlohmann::json response_json = {{"status", "ok"},
                                    {"message", response_body}};

    std::string body_json = response_json.dump() + '\n';  // compact JSON string
    std::cout << "Body size: " << body_json.size() << std::endl;
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n"
             << "Content-Type: application/json\r\n"
             << "Content-Length: " << body_json.size() << "\r\n"
             << "\r\n"
             << body_json;

    // Step 7: Write response
    std::cout << response.str() << std::endl;
    co_await asio::async_write(socket, asio::buffer(response.str()),
                               use_awaitable);
}

awaitable<void> listener(asio::io_context& ctx, unsigned short port,  // NOLINT
                         router::Router& router) {
    tcp::acceptor acceptor(ctx, {tcp::v4(), port});
    for (;;) {
        tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
        asio::co_spawn(ctx, session(std::move(socket), router), asio::detached);
    }
}

int main() {
    try {
        asio::io_context ctx(1);
        asio::co_spawn(
            ctx,
            [&]() -> awaitable<void> {
                router::Router router;
                storage::StorageRouterAdapter handler("/tmp/test.log");

                // Register routes using template
                router.addRoute("/get", handler,
                                &storage::StorageRouterAdapter::get);
                router.addRoute("/put", handler,
                                &storage::StorageRouterAdapter::put);

                co_await listener(ctx, 8080, router);
            }(),
            asio::detached);

        ctx.run();
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}
