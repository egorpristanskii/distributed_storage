#include "launcher.h"

#include "network/session.h"

namespace app {
asio::awaitable<void> listener(asio::io_context& ctx,
                               unsigned short port,  // NOLINT
                               std::shared_ptr<app::Application> app) {
    asio::ip::tcp::acceptor acceptor(ctx, {asio::ip::tcp::v4(), port});
    for (;;) {
        asio::ip::tcp::socket socket =
            co_await acceptor.async_accept(asio::use_awaitable);
        asio::co_spawn(ctx, network::Session(std::move(socket), app),
                       asio::detached);
    }
}

Launcher::Launcher(const Config& config)
    : app(std::make_shared<Application>(config.storageFile)) {}

void Launcher::run(const Config& config) {
    auto work_guard = asio::make_work_guard(ctx_);
    int port = config.port;
    asio::co_spawn(
        ctx_,
        [this, port]() -> asio::awaitable<void> {
            co_await listener(ctx_, port, app);
        },
        asio::detached);

    ctx_.run();
}
}  // namespace app
