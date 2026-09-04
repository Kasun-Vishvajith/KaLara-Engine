#include "kalara/Camera2D.hpp"

namespace kalara::runtime {

Camera2D::Camera2D(float width, float height)
    : m_width(width), m_height(height) {
}

void Camera2D::set_projection(float width, float height) noexcept {
    m_width = width;
    m_height = height;
}

core::Vector2 Camera2D::screen_to_world(const core::Vector2& screen_pos) const noexcept {
    float world_x = (screen_pos.x / m_zoom) + m_position.x;
    float world_y = (screen_pos.y / m_zoom) + m_position.y;
    return {world_x, world_y};
}

} // namespace kalara::runtime
