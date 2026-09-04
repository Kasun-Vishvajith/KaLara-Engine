#include "kalara/Registry.hpp"
#include "kalara/Hierarchy.hpp"
#include "kalara/UUID.hpp"
#include <cassert>

int main() {
    kalara::runtime::Registry registry;

    // Test Entity Creation & Stable EntityID
    kalara::runtime::EntityID parent_id = registry.create_entity("ParentEntity");
    kalara::runtime::EntityID child_id = registry.create_entity("ChildEntity");

    assert(parent_id != 0);
    assert(child_id != 0);
    assert(parent_id != child_id);

    // Test Default Components
    assert(registry.has_component<kalara::runtime::TagComponent>(parent_id));
    assert(registry.has_component<kalara::runtime::TransformComponent>(parent_id));
    assert(registry.get_component<kalara::runtime::TagComponent>(parent_id).name == "ParentEntity");

    // Test Custom Component Addition
    auto& sprite = registry.add_component<kalara::runtime::SpriteRendererComponent>(parent_id);
    sprite.color = {0.5f, 0.8f, 0.2f, 1.0f};
    assert(registry.has_component<kalara::runtime::SpriteRendererComponent>(parent_id));
    assert(registry.get_component<kalara::runtime::SpriteRendererComponent>(parent_id).color.x == 0.5f);

    // Test Hierarchy & Relative World Position Calculation
    auto& parent_trans = registry.get_component<kalara::runtime::TransformComponent>(parent_id);
    parent_trans.position = {100.0f, 200.0f, 0.0f};

    auto& child_trans = registry.get_component<kalara::runtime::TransformComponent>(child_id);
    child_trans.position = {50.0f, 25.0f, 0.0f};

    kalara::runtime::Hierarchy::set_parent(registry, child_id, parent_id);
    assert(child_trans.parent == parent_id);
    assert(parent_trans.children.size() == 1);
    assert(parent_trans.children[0] == child_id);

    // World position of child = parent (100, 200) + child (50, 25) = (150, 225)
    kalara::core::Vector3 child_world_pos = kalara::runtime::Hierarchy::get_world_position(registry, child_id);
    assert(child_world_pos.x == 150.0f);
    assert(child_world_pos.y == 225.0f);

    // Test Entity Destruction
    registry.destroy_entity(parent_id);
    assert(!registry.valid(parent_id));

    return 0;
}
