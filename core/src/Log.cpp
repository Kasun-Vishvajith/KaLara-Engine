#include "kalara/Log.hpp"

#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace kalara::core {

Logger& Logger::instance() noexcept {
    static Logger logger_instance;
    return logger_instance;
}

void Logger::set_level(LogLevel level) noexcept {
    m_level = level;
}

LogLevel Logger::level() const noexcept {
    return m_level;
}

void Logger::log(LogLevel level, std::string_view message) {
    if (level < m_level) return;

    std::lock_guard<std::mutex> lock(m_mutex);

    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::tm tm_buf{};
#if defined(_WIN32)
    localtime_s(&tm_buf, &time_t_now);
#else
    localtime_r(&time_t_now, &tm_buf);
#endif

    std::cout << '['
              << std::put_time(&tm_buf, "%H:%M:%S")
              << '.' << std::setfill('0') << std::setw(3) << ms.count()
              << "] [" << log_level_string(level) << "] "
              << message << '\n';
}

} // namespace kalara::core
