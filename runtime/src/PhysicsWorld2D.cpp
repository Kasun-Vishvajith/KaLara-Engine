#include "kalara/PhysicsWorld2D.hpp"
#include "kalara/Hierarchy.hpp"
#include "kalara/Log.hpp"
#include <algorithm>
#include <cmath>

namespace kalara::runtime {

struct AABB {
    float min_x{0.0f};
    float min_y{0.0f};
    float max_x{0.0f};
    float max_y{0.0f};

    [[nodiscard]] bool intersects(const AABB& other) const noexcept {
        return (min_x < other.max_x && max_x > other.min_x &&
                min_y < other.max_y && max_y > other.min_y);
    }
};

PhysicsWorld2D::PhysicsWorld2D(const core::Vector2& gravity) : m_gravity(gravity) {
}

void PhysicsWorld2D::step(Registry& registry, float delta_time) {
    if (delta_time <= 0.0f) return;

    integrate_velocities(registry, delta_time);
    resolve_collisions(registry);
}

void PhysicsWorld2D::integrate_velocities(Registry& registry, float dt) {
    for (EntityID entity : registry.entities()) {
        if (!registry.has_component<RigidBody2DComponent>(entity) ||
            !registry.has_component<TransformComponent>(entity)) {
            continue;
        }

        auto& body = registry.get_component<RigidBody2DComponent>(entity);
        auto& transform = registry.get_component<TransformComponent>(entity);

        if (body.type == BodyType2D::Dynamic) {
            // Apply gravity
            body.linear_velocity.x += m_gravity.x * body.gravity_scale * dt;
            body.linear_velocity.y += m_gravity.y * body.gravity_scale * dt;

            // Integrate position
            transform.position.x += body.linear_velocity.x * dt;
            transform.position.y += body.linear_velocity.y * dt;
        } else if (body.type == BodyType2D::Kinematic) {
            transform.position.x += body.linear_velocity.x * dt;
            transform.position.y += body.linear_velocity.y * dt;
        }
    }
}

void PhysicsWorld2D::resolve_collisions(Registry& registry) {
    const auto& entities = registry.entities();
    size_t count = entities.size();

    for (size_t i = 0; i < count; ++i) {
        EntityID e1 = entities[i];
        if (!registry.has_component<BoxCollider2DComponent>(e1) ||
            !registry.has_component<TransformComponent>(e1)) continue;

        auto& t1 = registry.get_component<TransformComponent>(e1);
        const auto& c1 = registry.get_component<BoxCollider2DComponent>(e1);

        AABB box1{
            t1.position.x + c1.offset.x,
            t1.position.y + c1.offset.y,
            t1.position.x + c1.offset.x + c1.size.x,
            t1.position.y + c1.offset.y + c1.size.y
        };

        for (size_t j = i + 1; j < count; ++j) {
            EntityID e2 = entities[j];
            if (!registry.has_component<BoxCollider2DComponent>(e2) ||
                !registry.has_component<TransformComponent>(e2)) continue;

            auto& t2 = registry.get_component<TransformComponent>(e2);
            const auto& c2 = registry.get_component<BoxCollider2DComponent>(e2);

            AABB box2{
                t2.position.x + c2.offset.x,
                t2.position.y + c2.offset.y,
                t2.position.x + c2.offset.x + c2.size.x,
                t2.position.y + c2.offset.y + c2.size.y
            };

            if (box1.intersects(box2)) {
                // Calculate overlap depth
                float overlap_x = std::min(box1.max_x - box2.min_x, box2.max_x - box1.min_x);
                float overlap_y = std::min(box1.max_y - box2.min_y, box2.max_y - box1.min_y);

                bool e1_dynamic = registry.has_component<RigidBody2DComponent>(e1) &&
                                  registry.get_component<RigidBody2DComponent>(e1).type == BodyType2D::Dynamic;
                bool e2_dynamic = registry.has_component<RigidBody2DComponent>(e2) &&
                                  registry.get_component<RigidBody2DComponent>(e2).type == BodyType2D::Dynamic;

                if (overlap_x < overlap_y) {
                    // Resolve X collision
                    float sign = (t1.position.x < t2.position.x) ? -1.0f : 1.0f;
                    if (e1_dynamic && !e2_dynamic) {
                        t1.position.x += sign * overlap_x;
                        registry.get_component<RigidBody2DComponent>(e1).linear_velocity.x = 0.0f;
                    } else if (!e1_dynamic && e2_dynamic) {
                        t2.position.x -= sign * overlap_x;
                        registry.get_component<RigidBody2DComponent>(e2).linear_velocity.x = 0.0f;
                    }
                } else {
                    // Resolve Y collision
                    float sign = (t1.position.y < t2.position.y) ? -1.0f : 1.0f;
                    if (e1_dynamic && !e2_dynamic) {
                        t1.position.y += sign * overlap_y;
                        registry.get_component<RigidBody2DComponent>(e1).linear_velocity.y = 0.0f;
                    } else if (!e1_dynamic && e2_dynamic) {
                        t2.position.y -= sign * overlap_y;
                        registry.get_component<RigidBody2DComponent>(e2).linear_velocity.y = 0.0f;
                    }
                }
            }
        }
    }
}

} // namespace kalara::runtime
