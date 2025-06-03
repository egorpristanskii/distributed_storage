#include "config.h"

#include "fstream"

namespace app {
Config::Config(std::string logFile, int numThreads, int port)
    : logFile(std::move(logFile)), numThreads(numThreads), port(port) {}

Config buildConfig(const std::string& configFile) {
    std::ifstream file(configFile);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open config file: " + configFile);
    }
    nlohmann::json json_config;
    file >> json_config;
    return Config(json_config["logFile"].get<std::string>(),
                  json_config["numThreads"].get<int>(),
                  json_config["port"].get<int>());
}
}  // namespace app
