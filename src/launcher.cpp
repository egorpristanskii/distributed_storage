#include "launcher.h"

#include "listener.h"

namespace app {

Launcher::Launcher(const Config& config)
    : app(std::make_shared<Application>(config.storageFile)) {}

void Launcher::run(const Config& config) {
    auto work_guard = asio::make_work_guard(ctx_);
    int port = config.port;
    asio::co_spawn(
        ctx_,
        [this, port]() -> asio::awaitable<void> {
            co_await app::Listener(port, ctx_, app)();
        },
        asio::detached);

    ctx_.run();
}
}  // namespace app
