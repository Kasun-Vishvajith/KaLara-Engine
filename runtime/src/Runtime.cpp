#include "kalara/Runtime.hpp"
#include "kalara/Renderer.hpp"
#include "kalara/AssetManager.hpp"
#include "kalara/Input.hpp"
#include "kalara/Camera2D.hpp"
#include "kalara/Log.hpp"

namespace kalara::runtime {

static std::shared_ptr<Texture2D> s_checkerboard_texture = nullptr;
static Camera2D s_camera{1280.0f, 720.0f};
static core::Vector2 s_player_pos{540.0f, 285.0f};

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

    s_camera.set_projection(static_cast<float>(m_window->width()), static_cast<float>(m_window->height()));

    // Create checkerboard texture asset
    constexpr uint32_t tex_dim = 64;
    std::vector<uint8_t> pixels(tex_dim * tex_dim * 4);
    for (uint32_t y = 0; y < tex_dim; ++y) {
        for (uint32_t x = 0; x < tex_dim; ++x) {
            bool is_white = ((x / 8) + (y / 8)) % 2 == 0;
            size_t idx = (y * tex_dim + x) * 4;
            pixels[idx + 0] = is_white ? 240 : 60;
            pixels[idx + 1] = is_white ? 240 : 120;
            pixels[idx + 2] = is_white ? 240 : 200;
            pixels[idx + 3] = 255;
        }
    }

    auto img = Image::create_from_memory(tex_dim, tex_dim, 4, pixels.data());
    s_checkerboard_texture = Texture2D::create(*img);
    AssetManager::instance().register_asset(s_checkerboard_texture);

    m_running = true;
    m_frame_count = 0;
    return true;
}

void ApplicationRuntime::run() noexcept {
    KALARA_LOG_INFO("Entering main application loop.");
    
    double delta_time = 1.0 / 60.0;
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

    // Player movement controls via Input abstraction
    float speed = 300.0f * static_cast<float>(delta_time);
    if (Input::is_key_pressed(KeyCode::W) || Input::is_key_pressed(KeyCode::Up)) s_player_pos.y -= speed;
    if (Input::is_key_pressed(KeyCode::S) || Input::is_key_pressed(KeyCode::Down)) s_player_pos.y += speed;
    if (Input::is_key_pressed(KeyCode::A) || Input::is_key_pressed(KeyCode::Left)) s_player_pos.x -= speed;
    if (Input::is_key_pressed(KeyCode::D) || Input::is_key_pressed(KeyCode::Right)) s_player_pos.x += speed;

    // Smooth camera tracking
    s_camera.set_position(s_player_pos.x - (m_window->width() * 0.5f) + 100.0f,
                          s_player_pos.y - (m_window->height() * 0.5f) + 75.0f);
}

void ApplicationRuntime::render() noexcept {
    if (!m_renderer || !m_window) return;

    m_renderer->begin_frame();

    float win_w = static_cast<float>(m_window->width());
    float win_h = static_cast<float>(m_window->height());

    // Render player textured quad
    if (s_checkerboard_texture) {
        m_renderer->draw_textured_quad(s_player_pos.x, s_player_pos.y,
                                       200.0f, 150.0f, s_checkerboard_texture->renderer_id());
    }

    // Top Triangle
    m_renderer->draw_triangle({win_w * 0.5f, 80.0f}, {win_w * 0.5f - 80.0f, 180.0f}, {win_w * 0.5f + 80.0f, 180.0f}, {0.9f, 0.35f, 0.35f, 1.0f});

    // Border Line
    m_renderer->draw_line({50.0f, win_h - 50.0f}, {win_w - 50.0f, win_h - 50.0f}, {0.95f, 0.85f, 0.3f, 1.0f}, 3.0f);

    m_renderer->end_frame();
}

void ApplicationRuntime::shutdown() noexcept {
    if (!m_running && !m_window) return;

    KALARA_LOG_INFO("Shutting down ApplicationRuntime (Processed {} frames)", m_frame_count);

    s_checkerboard_texture.reset();
    AssetManager::instance().clear_cache();
    Input::instance().clear();

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
