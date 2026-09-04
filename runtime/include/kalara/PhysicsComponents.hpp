#pragma once

#include "kalara/Math.hpp"
#include <cstdint>

namespace kalara::runtime {

enum class BodyType2D : uint8_t {
    Static = 0,
    Dynamic = 1,
    Kinematic = 2
};

struct RigidBody2DComponent {
    BodyType2D type{BodyType2D::Dynamic};
    core::Vector2 linear_velocity{0.0f, 0.0f};
    float angular_velocity{0.0f};
    float gravity_scale{1.0f};
    float mass{1.0f};
    bool fixed_rotation{false};
};

struct BoxCollider2DComponent {
    core::Vector2 size{100.0f, 100.0f};
    core::Vector2 offset{0.0f, 0.0f};
    float density{1.0f};
    float friction{0.2f};
    float restitution{0.0f}; // Bounciness
    bool is_trigger{false};
};

struct CircleCollider2DComponent {
    float radius{50.0f};
    core::Vector2 offset{0.0f, 0.0f};
    float density{1.0f};
    float friction{0.2f};
    float restitution{0.0f};
    bool is_trigger{false};
};

} // namespace kalara::runtime
