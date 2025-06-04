#include "launcher.h"
#include "logger/logger.h"

int main() {
    try {
        app::Config config = app::buildConfig("/tmp/config.json");
        auto logger_initializer = logger::LoggerInitializer(config.logFile);
        logger_initializer.init_logger();
        app::Launcher launcher(config);
        launcher.run(config);

    } catch (const std::exception& e) {
        LOG_ERROR("Fatal error: {}", e.what());
    }
}
