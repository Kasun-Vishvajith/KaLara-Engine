#pragma once

#include "kalara/IRenderer.hpp"

namespace kalara::runtime {

class Renderer : public IRenderer {
public:
    Renderer() = default;
    ~Renderer() override = default;

    void initialize(uint32_t width, uint32_t height) noexcept override;
    void set_viewport(const Viewport& viewport) noexcept override;
    void set_clear_color(const core::Vector4& color) noexcept override;
    void begin_frame() noexcept override;
    void draw_quad(float x, float y, float width, float height, const core::Vector4& color) noexcept override;
    void draw_textured_quad(float x, float y, float width, float height, uint32_t texture_id, const core::Vector4& tint = {1.0f, 1.0f, 1.0f, 1.0f}) noexcept override;
    void draw_triangle(const core::Vector2& p1, const core::Vector2& p2, const core::Vector2& p3, const core::Vector4& color) noexcept override;
    void draw_line(const core::Vector2& p1, const core::Vector2& p2, const core::Vector4& color, float thickness = 1.0f) noexcept override;
    void end_frame() noexcept override;

    [[nodiscard]] RendererBackend backend() const noexcept override { return RendererBackend::OpenGL; }
    [[nodiscard]] const Viewport& viewport() const noexcept override { return m_viewport; }

private:
    Viewport m_viewport{};
    core::Vector4 m_clear_color{0.12f, 0.14f, 0.18f, 1.0f};
};

} // namespace kalara::runtime
