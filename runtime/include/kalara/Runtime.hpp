#pragma once

#include "kalara/Core.hpp"

namespace kalara::runtime {

class ApplicationRuntime {
public:
    ApplicationRuntime() = default;
    ~ApplicationRuntime() = default;

    bool initialize(const core::EngineConfig& config = {}) noexcept;
    void run_frame() noexcept;
    void shutdown() noexcept;

    [[nodiscard]] bool is_running() const noexcept { return m_running; }
    [[nodiscard]] const core::EngineConfig& config() const noexcept { return m_config; }

private:
    core::EngineConfig m_config{};
    bool m_running{false};
    uint64_t m_frame_count{0};
};

[[nodiscard]] constexpr int foundation_status() noexcept {
    return 1;
}

} // namespace kalara::runtime
