#pragma once

#include <string>
#include <string_view>
#include <cstdint>
#include "kalara/Log.hpp"

namespace kalara::core {

struct EngineConfig {
    std::string app_name{"KaLara Engine"};
    uint32_t window_width{1280};
    uint32_t window_height{720};
    bool vsync{true};
    bool fullscreen{false};
    LogLevel log_level{LogLevel::Info};
};

class ConfigManager {
public:
    static ConfigManager& instance() noexcept;

    [[nodiscard]] const EngineConfig& config() const noexcept { return m_config; }
    [[nodiscard]] EngineConfig& config() noexcept { return m_config; }

    void set_config(const EngineConfig& config) noexcept { m_config = config; }

private:
    ConfigManager() = default;
    EngineConfig m_config{};
};

} // namespace kalara::core
