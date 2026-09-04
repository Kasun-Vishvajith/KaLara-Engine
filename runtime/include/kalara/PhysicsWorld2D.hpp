#pragma once

#include "kalara/Registry.hpp"
#include "kalara/PhysicsComponents.hpp"
#include "kalara/Math.hpp"

namespace kalara::runtime {

class PhysicsWorld2D {
public:
    explicit PhysicsWorld2D(const core::Vector2& gravity = {0.0f, 980.0f});
    ~PhysicsWorld2D() = default;

    void step(Registry& registry, float delta_time);

    [[nodiscard]] const core::Vector2& gravity() const noexcept { return m_gravity; }
    void set_gravity(const core::Vector2& gravity) noexcept { m_gravity = gravity; }

private:
    void integrate_velocities(Registry& registry, float dt);
    void resolve_collisions(Registry& registry);
    void sync_transforms(Registry& registry);

    core::Vector2 m_gravity{0.0f, 980.0f};
};

} // namespace kalara::runtime
