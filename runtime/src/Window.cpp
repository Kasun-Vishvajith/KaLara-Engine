#include "kalara/Window.hpp"
#include "kalara/Input.hpp"
#include "kalara/Log.hpp"

#include <SDL3/SDL.h>

namespace kalara::runtime {

static KeyCode translate_sdl_scancode(SDL_Scancode scancode) {
    switch (scancode) {
        case SDL_SCANCODE_W: return KeyCode::W;
        case SDL_SCANCODE_A: return KeyCode::A;
        case SDL_SCANCODE_S: return KeyCode::S;
        case SDL_SCANCODE_D: return KeyCode::D;
        case SDL_SCANCODE_UP: return KeyCode::Up;
        case SDL_SCANCODE_DOWN: return KeyCode::Down;
        case SDL_SCANCODE_LEFT: return KeyCode::Left;
        case SDL_SCANCODE_RIGHT: return KeyCode::Right;
        case SDL_SCANCODE_SPACE: return KeyCode::Space;
        case SDL_SCANCODE_ESCAPE: return KeyCode::Escape;
        case SDL_SCANCODE_RETURN: return KeyCode::Enter;
        default: return KeyCode::Unknown;
    }
}

Window::~Window() {
    shutdown();
}

bool Window::initialize(const WindowProps& props) noexcept {
    m_width = props.width;
    m_height = props.height;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        KALARA_LOG_CRITICAL("Failed to initialize SDL3: {}", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    if (props.fullscreen) flags |= SDL_WINDOW_FULLSCREEN;

    m_window = SDL_CreateWindow(props.title.data(), static_cast<int>(props.width), static_cast<int>(props.height), flags);
    if (!m_window) {
        KALARA_LOG_CRITICAL("Failed to create SDL3 window: {}", SDL_GetError());
        return false;
    }

    m_gl_context = SDL_GL_CreateContext(m_window);
    if (!m_gl_context) {
        KALARA_LOG_CRITICAL("Failed to create OpenGL context: {}", SDL_GetError());
        return false;
    }

    SDL_GL_SetSwapInterval(props.vsync ? 1 : 0);
    KALARA_LOG_INFO("Window initialized [Title: '{}', Resolution: {}x{}]", props.title, m_width, m_height);
    return true;
}

void Window::poll_events(bool& running_flag) noexcept {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            running_flag = false;
        } else if (event.type == SDL_EVENT_KEY_DOWN) {
            KeyCode key = translate_sdl_scancode(event.key.scancode);
            if (key == KeyCode::Escape) running_flag = false;
            Input::instance().set_key_state(key, true);
        } else if (event.type == SDL_EVENT_KEY_UP) {
            KeyCode key = translate_sdl_scancode(event.key.scancode);
            Input::instance().set_key_state(key, false);
        } else if (event.type == SDL_EVENT_MOUSE_MOTION) {
            Input::instance().set_mouse_position(event.motion.x, event.motion.y);
        } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) Input::instance().set_mouse_button_state(MouseButton::Left, true);
            if (event.button.button == SDL_BUTTON_MIDDLE) Input::instance().set_mouse_button_state(MouseButton::Middle, true);
            if (event.button.button == SDL_BUTTON_RIGHT) Input::instance().set_mouse_button_state(MouseButton::Right, true);
        } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
            if (event.button.button == SDL_BUTTON_LEFT) Input::instance().set_mouse_button_state(MouseButton::Left, false);
            if (event.button.button == SDL_BUTTON_MIDDLE) Input::instance().set_mouse_button_state(MouseButton::Middle, false);
            if (event.button.button == SDL_BUTTON_RIGHT) Input::instance().set_mouse_button_state(MouseButton::Right, false);
        } else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
            m_width = static_cast<uint32_t>(event.window.data1);
            m_height = static_cast<uint32_t>(event.window.data2);
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
}

} // namespace kalara::runtime
