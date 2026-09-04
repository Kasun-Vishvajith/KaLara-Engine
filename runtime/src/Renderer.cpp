#include "kalara/Renderer.hpp"
#include "kalara/Log.hpp"

#include <SDL3/SDL_opengl.h>

namespace kalara::runtime {

void Renderer::initialize(uint32_t width, uint32_t height) noexcept {
    set_viewport({0, 0, width, height});
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    KALARA_LOG_INFO("OpenGL Renderer initialized with viewport {}x{} (Blending enabled).", width, height);
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
    glDisable(GL_TEXTURE_2D);
    glColor4f(color.x, color.y, color.z, color.w);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();
}

void Renderer::draw_textured_quad(float x, float y, float width, float height, uint32_t texture_id, const core::Vector4& tint) noexcept {
    if (texture_id == 0) {
        draw_quad(x, y, width, height, tint);
        return;
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glColor4f(tint.x, tint.y, tint.z, tint.w);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(x + width, y);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(x + width, y + height);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y + height);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void Renderer::draw_triangle(const core::Vector2& p1, const core::Vector2& p2, const core::Vector2& p3, const core::Vector4& color) noexcept {
    glDisable(GL_TEXTURE_2D);
    glColor4f(color.x, color.y, color.z, color.w);
    glBegin(GL_TRIANGLES);
        glVertex2f(p1.x, p1.y);
        glVertex2f(p2.x, p2.y);
        glVertex2f(p3.x, p3.y);
    glEnd();
}

void Renderer::draw_line(const core::Vector2& p1, const core::Vector2& p2, const core::Vector4& color, float thickness) noexcept {
    glDisable(GL_TEXTURE_2D);
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
