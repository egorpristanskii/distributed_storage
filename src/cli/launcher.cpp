#include "cli/launcher.h"

namespace {
constexpr const char* kConfigFileOption = "-c,--config";
constexpr const char* kApplicationDescription = "Application Launcher";
constexpr const char* kConfigFileDescription = "Path to the configuration file";
}  // namespace

namespace cli {
CLILauncher::CLILauncher() : cli_app_(kApplicationDescription) {
    cli_app_
        .add_option(kConfigFileOption, config_file_, kConfigFileDescription)
        ->required();
}

int CLILauncher::parse(int argc, char* argv[]) {
    try {
        cli_app_.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        return cli_app_.exit(e);
    }
    return 0;
}

std::string CLILauncher::getConfigFile() const {
    return config_file_;
}
}  // namespace cli
