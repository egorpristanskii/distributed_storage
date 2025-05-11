#include "network/request.h"
#include "network/response.h"
#include "network/router.h"
#include "storage/storage_router_adapter.h"

#include <asio.hpp>
#include <asio/awaitable.hpp>
#include <asio/detached.hpp>
#include <asio/executor_work_guard.hpp>
#include <asio/use_awaitable.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

using asio::awaitable;
using asio::use_awaitable;
using asio::ip::tcp;
using namespace std::literals;

awaitable<void> session(tcp::socket socket,  // NOLINT
                        std::shared_ptr<router::Router> router) {
    std::string raw_request;
    char buf[1024];
    std::size_t n =
        co_await socket.async_read_some(asio::buffer(buf), use_awaitable);
    raw_request.append(buf, n);

    network::Request parsed_request(raw_request);

    nlohmann::json json_body;
    if (!parsed_request.body.empty()) {
        json_body = nlohmann::json::parse(parsed_request.body);
    }

    network::Response response =
        router->handleRoute(parsed_request.path, json_body);
    co_await asio::async_write(socket, asio::buffer(response.toString()),
                               use_awaitable);
}

awaitable<void> listener(asio::io_context& ctx, unsigned short port,  // NOLINT
                         std::shared_ptr<router::Router> router) {
    tcp::acceptor acceptor(ctx, {tcp::v4(), port});
    for (;;) {
        tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
        asio::co_spawn(ctx, session(std::move(socket), router), asio::detached);
    }
}

int main() {
    try {
        int num_threads = 2;

        asio::io_context ctx;
        auto work_guard = asio::make_work_guard(ctx);

        auto router = std::make_shared<router::Router>();
        auto handler =
            std::make_shared<storage::StorageRouterAdapter>("/tmp/test.log");

        router->addRoute("/get", *handler, &storage::StorageRouterAdapter::get);
        router->addRoute("/put", *handler, &storage::StorageRouterAdapter::put);

        asio::co_spawn(
            ctx,
            [router, handler, &ctx]() -> awaitable<void> {
                co_await listener(ctx, 8080, router);
            },
            asio::detached);

        std::vector<std::thread> threads;
        threads.reserve(num_threads);
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back([&ctx]() { ctx.run(); });
        }

        for (auto& t : threads) {
            t.join();
        }

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
    }
}
