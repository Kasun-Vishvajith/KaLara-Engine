#include "kalara/Runtime.hpp"
#include "kalara/Log.hpp"

namespace kalara::runtime {

bool ApplicationRuntime::initialize(const core::EngineConfig& config) noexcept {
    m_config = config;
    core::Logger::instance().set_level(m_config.log_level);
    core::initialize();
    
    KALARA_LOG_INFO("Initializing KaLara Runtime [App: {}, Resolution: {}x{}]",
                    m_config.app_name, m_config.window_width, m_config.window_height);
    
    m_running = true;
    m_frame_count = 0;
    return true;
}

void ApplicationRuntime::run_frame() noexcept {
    if (!m_running) return;
    m_frame_count++;
    KALARA_LOG_TRACE("Runtime frame update: {}", m_frame_count);
}

void ApplicationRuntime::shutdown() noexcept {
    if (!m_running) return;
    KALARA_LOG_INFO("Shutting down KaLara Runtime (Total frames processed: {})", m_frame_count);
    core::shutdown();
    m_running = false;
}

} // namespace kalara::runtime
