#include "config.h"

#include "fstream"

namespace app {
Config::Config(int numThreads, int port, std::string storageFile,
               std::string logFile)
    : numThreads(numThreads),
      port(port),
      storageFile(std::move(storageFile)),
      logFile(std::move(logFile)) {}

Config buildConfig(const std::string& configFile) {
    std::ifstream file(configFile);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open config file: " + configFile);
    }
    nlohmann::json json_config;
    file >> json_config;
    return Config(json_config["numThreads"].get<int>(),
                  json_config["port"].get<int>(),
                  json_config["storageFile"].get<std::string>(),
                  json_config["logFile"].get<std::string>());
}
}  // namespace app
