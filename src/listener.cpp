#include "listener.h"

#include "network/session.h"

namespace app {
Listener::Listener(uint16_t port, asio::io_context& ctx,
                   std::shared_ptr<Application> app)
    : port_(port), ctx_(ctx), app_(std::move(app)) {}

asio::awaitable<void> Listener::operator()() {
    asio::ip::tcp::acceptor acceptor(ctx_, {asio::ip::tcp::v4(), port_});
    for (;;) {
        asio::ip::tcp::socket socket =
            co_await acceptor.async_accept(asio::use_awaitable);
        asio::co_spawn(ctx_, network::Session(std::move(socket), app_),
                       asio::detached);
    }
}
}  // namespace app
