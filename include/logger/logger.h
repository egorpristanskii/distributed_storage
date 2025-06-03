#pragma once

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

inline void init_logger() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

    auto logger = std::make_shared<spdlog::logger>("app", console_sink);
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::debug);  // or info, trace, etc.
}
