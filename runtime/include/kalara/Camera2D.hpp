#pragma once

#include "kalara/Math.hpp"

namespace kalara::runtime {

class Camera2D {
public:
    Camera2D(float width = 1280.0f, float height = 720.0f);
    ~Camera2D() = default;

    void set_projection(float width, float height) noexcept;

    [[nodiscard]] const core::Vector3& position() const noexcept { return m_position; }
    void set_position(const core::Vector3& pos) noexcept { m_position = pos; }
    void set_position(float x, float y) noexcept { m_position.x = x; m_position.y = y; }

    [[nodiscard]] float zoom() const noexcept { return m_zoom; }
    void set_zoom(float zoom) noexcept { m_zoom = zoom > 0.01f ? zoom : 0.01f; }

    [[nodiscard]] core::Vector2 screen_to_world(const core::Vector2& screen_pos) const noexcept;

private:
    core::Vector3 m_position{0.0f, 0.0f, 0.0f};
    float m_width{1280.0f};
    float m_height{720.0f};
    float m_zoom{1.0f};
};

} // namespace kalara::runtime
