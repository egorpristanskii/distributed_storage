#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace app {
struct Config {
    Config(int numThreads, int port, std::string storageFile,
           std::string logFile);

    const int numThreads;
    const int port;
    const std::string storageFile;
    const std::string logFile;
};

Config buildConfig(const std::string& configFile);
}  // namespace app
