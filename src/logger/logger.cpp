#include "logger/logger.h"

namespace logger {
LoggerInitializer::LoggerInitializer(std::string logFile)
    : logFile_(std::move(logFile)) {}

LoggerInitializer::~LoggerInitializer() {
    spdlog::shutdown();
}

void LoggerInitializer::init_logger() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

    auto file_sink =
        std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFile_, true);
    file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");

    auto logger = std::make_shared<spdlog::logger>(
        "multi_logger", spdlog::sinks_init_list{console_sink, file_sink});
    logger->flush_on(spdlog::level::info);
    spdlog::set_default_logger(logger);

    spdlog::set_level(spdlog::level::debug);  // or info, warn, etc.
}
}  // namespace logger
