#include "app.h"
#include "config.h"

#include <asio.hpp>
#include <asio/awaitable.hpp>
#include <asio/detached.hpp>
#include <asio/executor_work_guard.hpp>
#include <asio/use_awaitable.hpp>

namespace app {
class Launcher {
   public:
    explicit Launcher(const Config& config);

    void run(const Config& config);

    std::shared_ptr<Application> app;

   private:
    asio::io_context ctx_;
};
}  // namespace app
