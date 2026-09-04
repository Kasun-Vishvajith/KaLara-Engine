#include "kalara/Runtime.hpp"
#include "kalara/Log.hpp"

namespace kalara::runtime {

ApplicationRuntime::~ApplicationRuntime() {
    shutdown();
}

bool ApplicationRuntime::initialize(const core::EngineConfig& config) noexcept {
    m_config = config;
    core::Logger::instance().set_level(m_config.log_level);
    core::initialize();

    KALARA_LOG_INFO("Initializing KaLara Runtime [App: {}, Resolution: {}x{}]",
                    m_config.app_name, m_config.window_width, m_config.window_height);

    m_window = std::make_unique<Window>();
    WindowProps props{
        .title = m_config.app_name,
        .width = m_config.window_width,
        .height = m_config.window_height,
        .vsync = m_config.vsync,
        .fullscreen = m_config.fullscreen
    };

    if (!m_window->initialize(props)) {
        KALARA_LOG_CRITICAL("Failed to initialize Window!");
        return false;
    }

    m_renderer = std::make_unique<Renderer>();
    m_renderer->initialize(m_window->width(), m_window->height());

    m_running = true;
    m_frame_count = 0;
    return true;
}

void ApplicationRuntime::run() noexcept {
    KALARA_LOG_INFO("Entering main application loop.");
    
    double delta_time = 1.0 / 60.0; // Fixed delta time for baseline
    while (m_running) {
        if (m_window) {
            m_window->poll_events(m_running);
        }

        if (!m_running) break;

        update(delta_time);
        render();

        if (m_window) {
            m_window->swap_buffers();
        }
    }

    KALARA_LOG_INFO("Main loop exited cleanly.");
}

void ApplicationRuntime::update([[maybe_unused]] double delta_time) noexcept {
    m_frame_count++;
}

void ApplicationRuntime::render() noexcept {
    if (!m_renderer || !m_window) return;

    // Dark clear background
    m_renderer->begin_frame({0.12f, 0.14f, 0.18f, 1.0f});

    // Visible proof: draw one centered rectangle (teal color)
    float rect_w = 200.0f;
    float rect_h = 150.0f;
    float center_x = (static_cast<float>(m_window->width()) - rect_w) * 0.5f;
    float center_y = (static_cast<float>(m_window->height()) - rect_h) * 0.5f;

    m_renderer->draw_rectangle(center_x, center_y, rect_w, rect_h, {0.2f, 0.75f, 0.65f, 1.0f});

    m_renderer->end_frame();
}

void ApplicationRuntime::shutdown() noexcept {
    if (!m_running && !m_window) return;

    KALARA_LOG_INFO("Shutting down ApplicationRuntime (Processed {} frames)", m_frame_count);

    if (m_renderer) {
        m_renderer.reset();
    }
    if (m_window) {
        m_window->shutdown();
        m_window.reset();
    }

    core::shutdown();
    m_running = false;
}

} // namespace kalara::runtime
