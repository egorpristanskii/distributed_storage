#include <asio.hpp>
#include <asio/awaitable.hpp>
#include <asio/detached.hpp>
#include <asio/use_awaitable.hpp>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

using asio::awaitable;
using asio::use_awaitable;
using asio::ip::tcp;
using namespace std::literals;

class Router {
   public:
    using Handler = std::function<std::string()>;

    // Register handler with a path
    template <typename T>
    void add_route(std::string path, T& handler,
                   std::string (T::*method)() const) {
        routes_[std::move(path)] = [method, &handler]() -> std::string {
            return (handler.*method)();  // Call the handler method
        };
    }

    std::string handle_request(const std::string& path) const {
        auto it = routes_.find(path);
        if (it != routes_.end()) {
            return it->second();
        }
        return "HTTP/1.1 404 Not Found\r\n\r\nNot Found";
    }

   private:
    std::unordered_map<std::string, Handler> routes_;
};

class MyHandler {
   public:
    std::string ping() const { return "HTTP/1.1 200 OK\r\n\r\npong"; }

    std::string hello() const { return "HTTP/1.1 200 OK\r\n\r\nHello, world!"; }
};

awaitable<void> session(tcp::socket socket, const Router& router) {
    try {
        char data[1024];
        std::size_t n =
            co_await socket.async_read_some(asio::buffer(data), use_awaitable);

        std::string request(data, n);
        auto path_start = request.find(' ');
        auto path_end = request.find(' ', path_start + 1);
        std::string path =
            request.substr(path_start + 1, path_end - path_start - 1);

        std::string response = router.handle_request(path);

        co_await asio::async_write(socket, asio::buffer(response),
                                   use_awaitable);
    } catch (...) {
        // Ignore connection errors
    }
}

awaitable<void> listener(asio::io_context& ctx, unsigned short port,
                         const Router& router) {
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
                Router router;
                MyHandler handler;

                // Register routes using template
                router.add_route("/ping", handler, &MyHandler::ping);
                router.add_route("/hello", handler, &MyHandler::hello);

                co_await listener(ctx, 8080, router);
            }(),
            asio::detached);

        ctx.run();
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}
