#include "network/session.h"

#include "app.h"
#include "logger/logger.h"
#include "network/http_codes.h"
#include "network/request.h"
#include "network/response.h"
#include "network/types.h"

#include <asio/awaitable.hpp>

namespace {
constexpr int kBufferSize = 2048;
}

namespace network {
Session::Session(asio::ip::tcp::socket socket,
                 std::shared_ptr<app::Application> app)
    : socket_(std::move(socket)), app_(std::move(app)) {}

asio::awaitable<void> Session::operator()() {
    asio::streambuf buffer;

    std::size_t n = co_await asio::async_read_until(socket_, buffer, "\r\n\r\n",
                                                    asio::use_awaitable);

    std::istream stream(&buffer);
    std::string raw_request_headers;
    raw_request_headers.resize(n);
    stream.read(raw_request_headers.data(), n);

    network::Request parsed_request(raw_request_headers);

    size_t content_length = 0;
    if (parsed_request.headers.contains("Content-Length")) {
        content_length = std::stoul(parsed_request.headers["Content-Length"]);
    }

    if (content_length > 0) {
        std::vector<char> body_buf(content_length);

        std::size_t bytes_already = buffer.size();

        if (bytes_already >= content_length) {
            stream.read(body_buf.data(), content_length);
        } else {
            stream.read(body_buf.data(), bytes_already);

            std::size_t bytes_to_read = content_length - bytes_already;
            co_await asio::async_read(
                socket_,
                asio::buffer(body_buf.data() + bytes_already, bytes_to_read),
                asio::use_awaitable);
        }

        parsed_request.body.assign(body_buf.begin(), body_buf.end());
    }

    nlohmann::json json_body;
    if (!parsed_request.body.empty()) {
        json_body = nlohmann::json::parse(parsed_request.body);
    }

    LOG_INFO("Received {} request endpoint {} with body: {}",
             kHTTPMethodStringMap.at(parsed_request.method),
             parsed_request.path, parsed_request.body);

    network::Response response = app_->handleRequest(
        parsed_request.method, parsed_request.path, json_body);

    co_await asio::async_write(socket_, asio::buffer(response.toString()),
                               asio::use_awaitable);

    LOG_INFO("Sent response {} with status code: {}", response.response_data,
             kHTTPCodeToString.at(response.status_code));

    socket_.shutdown(asio::ip::tcp::socket::shutdown_both);
    socket_.close();
}

}  // namespace network
