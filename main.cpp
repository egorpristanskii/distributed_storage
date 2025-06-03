#include "launcher.h"
#include "logger/logger.h"

#include <iostream>

int main() {
    init_logger();
    try {
        app::Config config = app::buildConfig("/tmp/config.json");
        app::Launcher launcher(config);
        launcher.run(config);

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
    }
}
