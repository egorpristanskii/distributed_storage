#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace app {
struct Config {
    Config(std::string logFile, int numThreads, int port);

    const std::string logFile;
    const int numThreads;
    const int port;
};

Config buildConfig(const std::string& configFile);
}  // namespace app
