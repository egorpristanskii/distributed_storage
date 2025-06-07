#pragma once

#include <CLI/CLI.hpp>
#include <string>

namespace cli {
class CLILauncher {
   public:
    CLILauncher();

    int parse(int argc, char* argv[]);

    [[nodiscard]] std::string getConfigFile() const;

   private:
    CLI::App cli_app_;
    std::string config_file_;
};
}  // namespace cli
