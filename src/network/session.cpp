#include "network/session.h"

#include "app.h"
#include "logger/logger.h"
#include "network/request.h"
#include "network/response.h"

#include <asio/awaitable.hpp>

namespace {
constexpr int kBufferSize = 1024;
}

namespace network {
Session::Session(asio::ip::tcp::socket socket,
                 std::shared_ptr<app::Application> app)
    : socket_(std::move(socket)), app_(std::move(app)) {}

asio::awaitable<void> Session::operator()() {
    std::string raw_request;
    char buf[kBufferSize];
    std::size_t n = co_await socket_.async_read_some(asio::buffer(buf),
                                                     asio::use_awaitable);
    raw_request.append(buf, n);

    network::Request parsed_request(raw_request);

    nlohmann::json json_body;
    if (!parsed_request.body.empty()) {
        json_body = nlohmann::json::parse(parsed_request.body);
    }
    LOG_INFO("Received {} request endpoint {} with body: {}",
             parsed_request.method, parsed_request.path, parsed_request.body);
    network::Response response =
        app_->handleRequest(parsed_request.path, json_body);
    co_await asio::async_write(socket_, asio::buffer(response.toString()),
                               asio::use_awaitable);
    LOG_INFO("Sent response {} with status code: {}", response.response_data,
             response.status_code);
}
}  // namespace network
