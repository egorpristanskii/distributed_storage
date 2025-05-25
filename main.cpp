#include "app.h"
#include "network/session.h"

#include <asio.hpp>
#include <asio/awaitable.hpp>
#include <asio/detached.hpp>
#include <asio/executor_work_guard.hpp>
#include <asio/use_awaitable.hpp>
#include <iostream>
#include <memory>
#include <thread>

using asio::awaitable;
using asio::use_awaitable;
using asio::ip::tcp;
using namespace std::literals;

awaitable<void> listener(asio::io_context& ctx, unsigned short port,  // NOLINT
                         std::shared_ptr<app::Application> app) {
    tcp::acceptor acceptor(ctx, {tcp::v4(), port});
    for (;;) {
        tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
        asio::co_spawn(ctx, network::Session(std::move(socket), app),
                       asio::detached);
    }
}

int main() {
    try {
        int num_threads = 2;

        asio::io_context ctx;
        auto work_guard = asio::make_work_guard(ctx);

        auto app = std::make_shared<app::Application>("/tmp/test.log");

        asio::co_spawn(
            ctx,
            [app, &ctx]() -> awaitable<void> {
                co_await listener(ctx, 8080, app);
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
