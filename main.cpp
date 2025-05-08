#include "router.h"
#include "storage_router_adapter.h"

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
    asio::streambuf buffer;

    // Step 1: Read headers
    std::size_t n = co_await asio::async_read_until(socket, buffer, "\r\n\r\n",
                                                    use_awaitable);
    std::istream stream(&buffer);
    std::string request_line;
    std::getline(stream, request_line);

    // Step 2: Parse path
    auto path_start = request_line.find(' ');
    auto path_end = request_line.find(' ', path_start + 1);
    std::string path =
        request_line.substr(path_start + 1, path_end - path_start - 1);

    std::cerr << path << std::endl;

    // Step 3: Parse headers
    std::string header_line;
    std::size_t content_length = 0;
    while (std::getline(stream, header_line) && header_line != "\r") {
        if (!header_line.empty() && header_line.back() == '\r') {
            header_line.pop_back();
        }

        if (header_line.starts_with("Content-Length:")) {  // NOLINT
            content_length = std::stoul(header_line.substr(15));
        }
    }

    // Step 4: Read body (if needed)
    std::string body;
    if (content_length > 0) {
        if (buffer.size() < content_length) {
            co_await asio::async_read(
                socket, buffer,
                asio::transfer_exactly(content_length - buffer.size()),
                use_awaitable);
        }
        std::stringbuf body_buf;
        body_buf.sputn(asio::buffer_cast<const char*>(buffer.data()),
                       content_length);
        body = body_buf.str();
    }

    // Step 5: Optional: parse JSON
    nlohmann::json json_body;
    if (!body.empty()) {
        json_body = nlohmann::json::parse(body);
    }

    // Step 6: Route
    std::string response_body = router.handleRoute(path, json_body);

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
