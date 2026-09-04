#pragma once

#include <string_view>
#include <cstdint>

struct SDL_Window;
typedef struct SDL_GLContextState* SDL_GLContext;

namespace kalara::runtime {

struct WindowProps {
    std::string_view title{"KaLara Engine"};
    uint32_t width{1280};
    uint32_t height{720};
    bool vsync{true};
    bool fullscreen{false};
};

class Window {
public:
    Window() = default;
    ~Window();

    bool initialize(const WindowProps& props) noexcept;
    void poll_events(bool& running_flag) noexcept;
    void swap_buffers() noexcept;
    void shutdown() noexcept;

    [[nodiscard]] SDL_Window* native_handle() const noexcept { return m_window; }
    [[nodiscard]] SDL_GLContext gl_context() const noexcept { return m_gl_context; }
    [[nodiscard]] uint32_t width() const noexcept { return m_width; }
    [[nodiscard]] uint32_t height() const noexcept { return m_height; }

private:
    SDL_Window* m_window{nullptr};
    SDL_GLContext m_gl_context{nullptr};
    uint32_t m_width{0};
    uint32_t m_height{0};
};

} // namespace kalara::runtime
