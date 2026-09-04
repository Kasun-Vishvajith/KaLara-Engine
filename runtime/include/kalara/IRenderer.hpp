#pragma once

#include <cstdint>
#include "kalara/Math.hpp"
#include "kalara/Viewport.hpp"

namespace kalara::runtime {

enum class RendererBackend {
    OpenGL,
    Vulkan,
    DirectX
};

class IRenderer {
public:
    virtual ~IRenderer() = default;

    virtual void initialize(uint32_t width, uint32_t height) noexcept = 0;
    virtual void set_viewport(const Viewport& viewport) noexcept = 0;
    virtual void set_clear_color(const core::Vector4& color) noexcept = 0;
    virtual void begin_frame() noexcept = 0;
    virtual void draw_quad(float x, float y, float width, float height, const core::Vector4& color) noexcept = 0;
    virtual void draw_triangle(const core::Vector2& p1, const core::Vector2& p2, const core::Vector2& p3, const core::Vector4& color) noexcept = 0;
    virtual void draw_line(const core::Vector2& p1, const core::Vector2& p2, const core::Vector4& color, float thickness = 1.0f) noexcept = 0;
    virtual void end_frame() noexcept = 0;

    [[nodiscard]] virtual RendererBackend backend() const noexcept = 0;
    [[nodiscard]] virtual const Viewport& viewport() const noexcept = 0;
};

} // namespace kalara::runtime
