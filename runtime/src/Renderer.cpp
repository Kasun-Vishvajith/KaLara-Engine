#include "kalara/Renderer.hpp"
#include "kalara/Log.hpp"

#include <SDL3/SDL_opengl.h>

namespace kalara::runtime {

void Renderer::initialize(uint32_t width, uint32_t height) noexcept {
    set_viewport(width, height);
    KALARA_LOG_INFO("Renderer initialized (OpenGL backend).");
}

void Renderer::set_viewport(uint32_t width, uint32_t height) noexcept {
    glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, static_cast<double>(width), static_cast<double>(height), 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Renderer::begin_frame(const Color& clear_color) noexcept {
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw_rectangle(float x, float y, float width, float height, const Color& color) noexcept {
    glColor4f(color.r, color.g, color.b, color.a);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();
}

void Renderer::end_frame() noexcept {
    // Frame commands completed
}

} // namespace kalara::runtime
