#include "launcher.h"

#include "listener.h"
#include "storage/wal_logger.h"

#include <asio/awaitable.hpp>
#include <asio/detached.hpp>
#include <asio/signal_set.hpp>
#include <chrono>
#include <memory>

namespace {
constexpr int kUpdateRate = 10;
}

namespace app {

Launcher::Launcher(const Config& config)
    : operation_logger_(
          std::make_shared<storage::WALLogger>(config.storageFile)) {
    app = std::make_shared<Application>(operation_logger_);
}

void Launcher::run(const Config& config) {
    auto work_guard = asio::make_work_guard(ctx_);
    int port = config.port;
    std::chrono::milliseconds update_rate{1000};
    asio::co_spawn(
        ctx_,
        [this, port]() -> asio::awaitable<void> {
            co_await app::Listener(port, ctx_, app)();
        },
        asio::detached);

    // Run task for flushing wall log data
    asio::co_spawn(
        ctx_,
        [this, update_rate]() -> asio::awaitable<void> {
            asio::steady_timer timer(co_await asio::this_coro::executor);
            while (true) {
                timer.expires_after(update_rate);
                co_await timer.async_wait(asio::use_awaitable);
                this->operation_logger_->flushLog();
            }
        },
        asio::detached);
    // For gracefully asio terminate
    asio::signal_set signals(ctx_, SIGINT, SIGTERM);
    signals.async_wait([&](auto, auto) { ctx_.stop(); });
    ctx_.run();
}
}  // namespace app
