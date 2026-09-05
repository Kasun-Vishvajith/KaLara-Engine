#include "kalara/BehaviorComponents.hpp"
#include "kalara/BehaviorSystem.hpp"
#include "kalara/Registry.hpp"
#include "kalara/Component.hpp"
#include <cassert>

int main() {
    kalara::runtime::Registry registry;

    // 1. Test No-Code Movement & Health Components
    kalara::runtime::EntityID player = registry.create_entity("PlayerEntity");

    auto& move = registry.add_component<kalara::runtime::MovementBehaviorComponent>(player);
    move.use_input_controls = false;
    move.direction = {1.0f, 0.0f};
    move.move_speed = 100.0f;

    auto& health = registry.add_component<kalara::runtime::HealthBehaviorComponent>(player);
    health.max_health = 100.0f;
    health.current_health = 100.0f;

    // Step BehaviorSystem
    kalara::runtime::BehaviorSystem::update(registry, 1.0f);

    const auto& transform = registry.get_component<kalara::runtime::TransformComponent>(player);
    assert(transform.position.x == 100.0f);

    // Test Health Damage & Healing
    health.take_damage(40.0f);
    assert(health.current_health == 60.0f);
    assert(!health.is_dead);

    health.take_damage(70.0f);
    assert(health.current_health == 0.0f);
    assert(health.is_dead);

    health.heal(50.0f);
    assert(health.current_health == 50.0f);
    assert(!health.is_dead);

    // 2. Test Door & Collectible Components
    kalara::runtime::EntityID door = registry.create_entity("DoorEntity");
    auto& d_comp = registry.add_component<kalara::runtime::DoorBehaviorComponent>(door);
    d_comp.required_key_tag = "GoldKey";

    assert(!d_comp.is_open);
    d_comp.is_open = true;
    assert(d_comp.is_open);

    return 0;
}
