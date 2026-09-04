#pragma once

#include <iostream>
#include <string_view>
#include <format>
#include <mutex>

namespace kalara::core {

enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Critical
};

class Logger {
public:
    static Logger& instance() noexcept;

    void set_level(LogLevel level) noexcept;
    [[nodiscard]] LogLevel level() const noexcept;

    void log(LogLevel level, std::string_view message);

    template<typename... Args>
    void log_fmt(LogLevel level, std::string_view fmt, Args&&... args) {
        if (level < m_level) return;
        try {
            std::string formatted = std::vformat(fmt, std::make_format_args(args...));
            log(level, formatted);
        } catch (const std::exception& e) {
            log(LogLevel::Error, std::string("Log format error: ") + e.what());
        }
    }

private:
    Logger() = default;

    LogLevel m_level{LogLevel::Info};
    std::mutex m_mutex;
};

constexpr std::string_view log_level_string(LogLevel level) noexcept {
    switch (level) {
        case LogLevel::Trace:    return "TRACE";
        case LogLevel::Debug:    return "DEBUG";
        case LogLevel::Info:     return "INFO";
        case LogLevel::Warn:     return "WARN";
        case LogLevel::Error:    return "ERROR";
        case LogLevel::Critical: return "CRITICAL";
    }
    return "UNKNOWN";
}

} // namespace kalara::core

#define KALARA_LOG_TRACE(...) ::kalara::core::Logger::instance().log_fmt(::kalara::core::LogLevel::Trace, __VA_ARGS__)
#define KALARA_LOG_DEBUG(...) ::kalara::core::Logger::instance().log_fmt(::kalara::core::LogLevel::Debug, __VA_ARGS__)
#define KALARA_LOG_INFO(...)  ::kalara::core::Logger::instance().log_fmt(::kalara::core::LogLevel::Info,  __VA_ARGS__)
#define KALARA_LOG_WARN(...)  ::kalara::core::Logger::instance().log_fmt(::kalara::core::LogLevel::Warn,  __VA_ARGS__)
#define KALARA_LOG_ERROR(...) ::kalara::core::Logger::instance().log_fmt(::kalara::core::LogLevel::Error, __VA_ARGS__)
#define KALARA_LOG_CRITICAL(...) ::kalara::core::Logger::instance().log_fmt(::kalara::core::LogLevel::Critical, __VA_ARGS__)
