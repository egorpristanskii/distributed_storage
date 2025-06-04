#pragma once

#include <spdlog/common.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#define LOG_INFO(msg, ...) \
    spdlog::info("[{}] " msg, __PRETTY_FUNCTION__, ##__VA_ARGS__)

#define LOG_DEBUG(msg, ...) \
    spdlog::debug("[{}] " msg, __PRETTY_FUNCTION__, ##__VA_ARGS__)

#define LOG_ERROR(msg, ...) \
    spdlog::error("[{}] " msg, __PRETTY_FUNCTION__, ##__VA_ARGS__)

#define LOG_WARN(msg, ...) \
    spdlog::warn("[{}] " msg, __PRETTY_FUNCTION__, ##__VA_ARGS__)

#define LOG_TRACE(msg, ...) \
    spdlog::trace("[{}] " msg, __PRETTY_FUNCTION__, ##__VA_ARGS__)

#define LOG_CRITICAL(msg, ...) \
    spdlog::critical("[{}] " msg, __PRETTY_FUNCTION__, ##__VA_ARGS__)

namespace logger {
class LoggerInitializer {
   public:
    explicit LoggerInitializer(std::string logFile);

    ~LoggerInitializer();

    void init_logger();

   private:
    const std::string logFile_;
};
}  // namespace logger
