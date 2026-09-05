#pragma once

#include "kalara/Math.hpp"
#include "kalara/Entity.hpp"
#include <string>
#include <cstdint>

namespace kalara::runtime {

struct MovementBehaviorComponent {
    float move_speed{300.0f};
    float jump_force{500.0f};
    bool use_input_controls{true};
    core::Vector2 direction{0.0f, 0.0f};
};

struct HealthBehaviorComponent {
    float max_health{100.0f};
    float current_health{100.0f};
    bool is_dead{false};

    void take_damage(float amount) noexcept {
        current_health -= amount;
        if (current_health <= 0.0f) {
            current_health = 0.0f;
            is_dead = true;
        }
    }

    void heal(float amount) noexcept {
        current_health += amount;
        if (current_health > max_health) {
            current_health = max_health;
        }
        is_dead = false;
    }
};

struct DamageBehaviorComponent {
    float damage_amount{25.0f};
    bool destroy_on_impact{true};
};

struct ShootingBehaviorComponent {
    float fire_rate{2.0f}; // Shots per second
    float projectile_speed{600.0f};
    float cooldown_timer{0.0f};
    bool is_auto_firing{false};
};

struct CollectibleBehaviorComponent {
    int32_t score_value{100};
    float heal_amount{10.0f};
    bool is_collected{false};
};

struct CheckpointBehaviorComponent {
    core::Vector3 respawn_position{0.0f, 0.0f, 0.0f};
    bool is_active{false};
};

struct DoorBehaviorComponent {
    bool is_open{false};
    std::string required_key_tag{};
};

struct TriggerBehaviorComponent {
    std::string trigger_tag{};
    EntityID target_entity_id{NullEntityID};
    bool is_triggered{false};
};

} // namespace kalara::runtime
