#pragma once

#include "kalara/Core.hpp"
#include "kalara/Window.hpp"
#include "kalara/IRenderer.hpp"
#include <memory>

namespace kalara::runtime {

class ApplicationRuntime {
public:
    ApplicationRuntime() = default;
    ~ApplicationRuntime();

    bool initialize(const core::EngineConfig& config = {}) noexcept;
    void run() noexcept;
    void update(double delta_time) noexcept;
    void render() noexcept;
    void shutdown() noexcept;

    [[nodiscard]] bool is_running() const noexcept { return m_running; }
    [[nodiscard]] const core::EngineConfig& config() const noexcept { return m_config; }
    [[nodiscard]] Window* window() noexcept { return m_window.get(); }
    [[nodiscard]] IRenderer* renderer() noexcept { return m_renderer.get(); }

private:
    core::EngineConfig m_config{};
    std::unique_ptr<Window> m_window;
    std::unique_ptr<IRenderer> m_renderer;
    bool m_running{false};
    uint64_t m_frame_count{0};
};

[[nodiscard]] constexpr int foundation_status() noexcept {
    return 1;
}

} // namespace kalara::runtime
