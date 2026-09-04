#include "kalara/SemanticType.hpp"
#include "kalara/SemanticComponent.hpp"
#include "kalara/Registry.hpp"
#include <cassert>

int main() {
    // 1. Test Built-in Semantic Vocabulary & Inheritance
    auto& sem_reg = kalara::runtime::SemanticRegistry::instance();
    assert(sem_reg.is_kind_of(kalara::runtime::SemanticType::Enemy, kalara::runtime::SemanticType::Obstacle));
    assert(sem_reg.is_kind_of(kalara::runtime::SemanticType::Wall, kalara::runtime::SemanticType::Obstacle));
    assert(!sem_reg.is_kind_of(kalara::runtime::SemanticType::Player, kalara::runtime::SemanticType::Obstacle));

    // 2. Test Custom Semantic Type Registration
    sem_reg.register_custom_type("BossEnemy", kalara::runtime::SemanticType::Enemy);
    assert(sem_reg.is_kind_of(kalara::runtime::SemanticType::Custom, kalara::runtime::SemanticType::Enemy));
    assert(sem_reg.is_kind_of(kalara::runtime::SemanticType::Custom, kalara::runtime::SemanticType::Obstacle));

    // 3. Test SemanticComponent Tags & Collision Layers
    kalara::runtime::Registry registry;
    kalara::runtime::EntityID player = registry.create_entity("PlayerEntity");
    auto& p_sem = registry.add_component<kalara::runtime::SemanticComponent>(player);
    p_sem.type = kalara::runtime::SemanticType::Player;
    p_sem.add_tag("Hero");
    p_sem.add_tag("MainPlayer");
    p_sem.collision_layer = kalara::runtime::CollisionLayer::Player;
    p_sem.collision_mask = kalara::runtime::CollisionLayer::Obstacle | kalara::runtime::CollisionLayer::Enemy;

    assert(p_sem.has_tag("Hero"));
    assert(p_sem.has_tag("MainPlayer"));
    assert(!p_sem.has_tag("EnemyTag"));

    assert(p_sem.can_collide_with(kalara::runtime::CollisionLayer::Enemy));
    assert(p_sem.can_collide_with(kalara::runtime::CollisionLayer::Obstacle));
    assert(!p_sem.can_collide_with(kalara::runtime::CollisionLayer::Trigger));

    return 0;
}
