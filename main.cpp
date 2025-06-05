#include "cli/launcher.h"
#include "launcher.h"
#include "logger/logger.h"

int main(int argc, char* argv[]) {
    try {
        cli::CLILauncher cli_launcher;
        int error_code = cli_launcher.parse(argc, argv);
        if (error_code != 0) {
            return error_code;
        }

        app::Config config = app::buildConfig(cli_launcher.getConfigFile());
        auto logger_initializer = logger::LoggerInitializer(config.logFile);
        logger_initializer.init_logger();
        app::Launcher launcher(config);
        launcher.run(config);

    } catch (const std::exception& e) {
        LOG_ERROR("Fatal error: {}", e.what());
    }
}
