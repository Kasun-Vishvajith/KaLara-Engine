#include "kalara/Registry.hpp"
#include "kalara/PhysicsComponents.hpp"
#include "kalara/PhysicsWorld2D.hpp"
#include <cassert>

int main() {
    kalara::runtime::Registry registry;
    kalara::runtime::PhysicsWorld2D physics_world({0.0f, 980.0f});

    // 1. Create Dynamic Box Entity falling from Y = 0
    kalara::runtime::EntityID dynamic_box = registry.create_entity("DynamicBox");
    auto& d_trans = registry.get_component<kalara::runtime::TransformComponent>(dynamic_box);
    d_trans.position = {100.0f, 0.0f, 0.0f};

    auto& d_body = registry.add_component<kalara::runtime::RigidBody2DComponent>(dynamic_box);
    d_body.type = kalara::runtime::BodyType2D::Dynamic;

    auto& d_collider = registry.add_component<kalara::runtime::BoxCollider2DComponent>(dynamic_box);
    d_collider.size = {50.0f, 50.0f};

    // 2. Create Static Ground Entity at Y = 200
    kalara::runtime::EntityID static_ground = registry.create_entity("StaticGround");
    auto& g_trans = registry.get_component<kalara::runtime::TransformComponent>(static_ground);
    g_trans.position = {0.0f, 200.0f, 0.0f};

    auto& g_body = registry.add_component<kalara::runtime::RigidBody2DComponent>(static_ground);
    g_body.type = kalara::runtime::BodyType2D::Static;

    auto& g_collider = registry.add_component<kalara::runtime::BoxCollider2DComponent>(static_ground);
    g_collider.size = {500.0f, 50.0f};

    // 3. Step physics simulation over multiple frames
    float dt = 1.0f / 60.0f;
    for (int i = 0; i < 60; ++i) {
        physics_world.step(registry, dt);
    }

    // Dynamic box should fall and stop on top of ground platform (Y = 200 - 50 = 150)
    assert(d_trans.position.y > 0.0f);
    assert(d_trans.position.y <= 150.0f);

    return 0;
}
