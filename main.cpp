#include "launcher.h"

#include <iostream>

int main() {
    try {
        app::Config config = app::buildConfig("/tmp/config.json");
        app::Launcher launcher(config);
        launcher.run(config);

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
    }
}
