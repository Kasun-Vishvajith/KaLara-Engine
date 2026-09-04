#pragma once

#include <cstdint>

namespace kalara::runtime {

struct Color {
    float r{1.0f};
    float g{1.0f};
    float b{1.0f};
    float a{1.0f};
};

class Renderer {
public:
    Renderer() = default;
    ~Renderer() = default;

    void initialize(uint32_t width, uint32_t height) noexcept;
    void begin_frame(const Color& clear_color = {0.1f, 0.12f, 0.15f, 1.0f}) noexcept;
    void draw_rectangle(float x, float y, float width, float height, const Color& color) noexcept;
    void end_frame() noexcept;
    void set_viewport(uint32_t width, uint32_t height) noexcept;
};

} // namespace kalara::runtime
