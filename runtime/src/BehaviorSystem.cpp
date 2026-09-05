#include "kalara/BehaviorSystem.hpp"
#include "kalara/BehaviorComponents.hpp"
#include "kalara/Component.hpp"
#include "kalara/Input.hpp"
#include "kalara/Log.hpp"

namespace kalara::runtime {

void BehaviorSystem::update(Registry& registry, float delta_time) {
    if (delta_time <= 0.0f) return;

    for (EntityID entity : registry.entities()) {
        // 1. Movement Behavior Execution
        if (registry.has_component<MovementBehaviorComponent>(entity) &&
            registry.has_component<TransformComponent>(entity)) {
            auto& move = registry.get_component<MovementBehaviorComponent>(entity);
            auto& transform = registry.get_component<TransformComponent>(entity);

            if (move.use_input_controls) {
                float dx = 0.0f;
                float dy = 0.0f;
                if (Input::is_key_pressed(KeyCode::D) || Input::is_key_pressed(KeyCode::Right)) dx += 1.0f;
                if (Input::is_key_pressed(KeyCode::A) || Input::is_key_pressed(KeyCode::Left)) dx -= 1.0f;
                if (Input::is_key_pressed(KeyCode::S) || Input::is_key_pressed(KeyCode::Down)) dy += 1.0f;
                if (Input::is_key_pressed(KeyCode::W) || Input::is_key_pressed(KeyCode::Up)) dy -= 1.0f;

                transform.position.x += dx * move.move_speed * delta_time;
                transform.position.y += dy * move.move_speed * delta_time;
            } else {
                transform.position.x += move.direction.x * move.move_speed * delta_time;
                transform.position.y += move.direction.y * move.move_speed * delta_time;
            }
        }

        // 2. Shooting Behavior Execution
        if (registry.has_component<ShootingBehaviorComponent>(entity)) {
            auto& shoot = registry.get_component<ShootingBehaviorComponent>(entity);
            if (shoot.cooldown_timer > 0.0f) {
                shoot.cooldown_timer -= delta_time;
            }
        }
    }
}

} // namespace kalara::runtime
