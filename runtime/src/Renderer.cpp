#include "kalara/Renderer.hpp"
#include "kalara/Log.hpp"

#include <SDL3/SDL_opengl.h>

namespace kalara::runtime {

void Renderer::initialize(uint32_t width, uint32_t height) noexcept {
    set_viewport({0, 0, width, height});
    KALARA_LOG_INFO("OpenGL Renderer initialized with viewport {}x{}.", width, height);
}

void Renderer::set_viewport(const Viewport& viewport) noexcept {
    m_viewport = viewport;
    glViewport(static_cast<GLint>(viewport.x), static_cast<GLint>(viewport.y),
               static_cast<GLsizei>(viewport.width), static_cast<GLsizei>(viewport.height));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, static_cast<double>(viewport.width), static_cast<double>(viewport.height), 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Renderer::set_clear_color(const core::Vector4& color) noexcept {
    m_clear_color = color;
}

void Renderer::begin_frame() noexcept {
    glClearColor(m_clear_color.x, m_clear_color.y, m_clear_color.z, m_clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw_quad(float x, float y, float width, float height, const core::Vector4& color) noexcept {
    glColor4f(color.x, color.y, color.z, color.w);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();
}

void Renderer::draw_triangle(const core::Vector2& p1, const core::Vector2& p2, const core::Vector2& p3, const core::Vector4& color) noexcept {
    glColor4f(color.x, color.y, color.z, color.w);
    glBegin(GL_TRIANGLES);
        glVertex2f(p1.x, p1.y);
        glVertex2f(p2.x, p2.y);
        glVertex2f(p3.x, p3.y);
    glEnd();
}

void Renderer::draw_line(const core::Vector2& p1, const core::Vector2& p2, const core::Vector4& color, float thickness) noexcept {
    glLineWidth(thickness);
    glColor4f(color.x, color.y, color.z, color.w);
    glBegin(GL_LINES);
        glVertex2f(p1.x, p1.y);
        glVertex2f(p2.x, p2.y);
    glEnd();
}

void Renderer::end_frame() noexcept {
}

} // namespace kalara::runtime
