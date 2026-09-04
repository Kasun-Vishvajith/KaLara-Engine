#include "kalara/Window.hpp"
#include "kalara/Log.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

namespace kalara::runtime {

Window::~Window() {
    shutdown();
}

bool Window::initialize(const WindowProps& props) noexcept {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        KALARA_LOG_CRITICAL("SDL_Init failed: {}", SDL_GetError());
        return false;
    }

    // Set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    if (props.fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    m_window = SDL_CreateWindow(props.title.data(), static_cast<int>(props.width), static_cast<int>(props.height), flags);
    if (!m_window) {
        KALARA_LOG_CRITICAL("SDL_CreateWindow failed: {}", SDL_GetError());
        SDL_Quit();
        return false;
    }

    m_gl_context = SDL_GL_CreateContext(m_window);
    if (!m_gl_context) {
        KALARA_LOG_CRITICAL("SDL_GL_CreateContext failed: {}", SDL_GetError());
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    SDL_GL_MakeCurrent(m_window, m_gl_context);
    SDL_GL_SetSwapInterval(props.vsync ? 1 : 0);

    m_width = props.width;
    m_height = props.height;

    KALARA_LOG_INFO("SDL3 Window created successfully ({}x{}) with OpenGL context.", m_width, m_height);
    return true;
}

void Window::poll_events(bool& running_flag) noexcept {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            KALARA_LOG_INFO("Quit event received.");
            running_flag = false;
        } else if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
                KALARA_LOG_INFO("ESC key pressed. Requesting shutdown.");
                running_flag = false;
            }
        } else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
            m_width = static_cast<uint32_t>(event.window.data1);
            m_height = static_cast<uint32_t>(event.window.data2);
            glViewport(0, 0, m_width, m_height);
        }
    }
}

void Window::swap_buffers() noexcept {
    if (m_window) {
        SDL_GL_SwapWindow(m_window);
    }
}

void Window::shutdown() noexcept {
    if (m_gl_context) {
        SDL_GL_DestroyContext(m_gl_context);
        m_gl_context = nullptr;
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    SDL_Quit();
    KALARA_LOG_INFO("Window and SDL3 shut down cleanly.");
}

} // namespace kalara::runtime
