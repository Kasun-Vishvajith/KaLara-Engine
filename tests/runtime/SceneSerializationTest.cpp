#include "kalara/Scene.hpp"
#include "kalara/SceneSerializer.hpp"
#include "kalara/Hierarchy.hpp"
#include <cassert>

int main() {
    // 1. Construct original Scene A
    auto scene_a = kalara::runtime::Scene::create("Original Level");

    kalara::runtime::EntityID parent_id = scene_a->create_entity("PlayerParent");
    auto& p_trans = scene_a->registry().get_component<kalara::runtime::TransformComponent>(parent_id);
    p_trans.position = {120.0f, 240.0f, 0.0f};

    auto& p_sprite = scene_a->registry().add_component<kalara::runtime::SpriteRendererComponent>(parent_id);
    p_sprite.color = {0.8f, 0.2f, 0.4f, 1.0f};

    kalara::runtime::EntityID child_id = scene_a->create_entity("WeaponChild");
    auto& c_trans = scene_a->registry().get_component<kalara::runtime::TransformComponent>(child_id);
    c_trans.position = {15.0f, 10.0f, 0.0f};

    kalara::runtime::Hierarchy::set_parent(scene_a->registry(), child_id, parent_id);

    // 2. Serialize Scene A to JSON string
    kalara::runtime::SceneSerializer serializer_a(scene_a);
    std::string json_data = serializer_a.serialize_to_string();
    assert(!json_data.empty());

    // 3. Deserialize JSON string into empty Scene B
    auto scene_b = kalara::runtime::Scene::create();
    kalara::runtime::SceneSerializer serializer_b(scene_b);
    bool load_ok = serializer_b.deserialize_from_string(json_data);
    assert(load_ok);

    // 4. Verify Round-Trip State Equivalence
    assert(scene_b->name() == "Original Level");
    assert(scene_b->registry().entity_count() == 2);
    assert(scene_b->registry().valid(parent_id));
    assert(scene_b->registry().valid(child_id));

    // Verify parent attributes
    assert(scene_b->registry().get_component<kalara::runtime::TagComponent>(parent_id).name == "PlayerParent");
    const auto& b_p_trans = scene_b->registry().get_component<kalara::runtime::TransformComponent>(parent_id);
    assert(b_p_trans.position.x == 120.0f);
    assert(b_p_trans.position.y == 240.0f);
    assert(scene_b->registry().get_component<kalara::runtime::SpriteRendererComponent>(parent_id).color.x == 0.8f);

    // Verify child attributes & hierarchy links
    const auto& b_c_trans = scene_b->registry().get_component<kalara::runtime::TransformComponent>(child_id);
    assert(b_c_trans.parent == parent_id);

    // Verify world position calculation matches original
    kalara::core::Vector3 world_pos_a = kalara::runtime::Hierarchy::get_world_position(scene_a->registry(), child_id);
    kalara::core::Vector3 world_pos_b = kalara::runtime::Hierarchy::get_world_position(scene_b->registry(), child_id);
    assert(world_pos_a.x == world_pos_b.x && world_pos_a.y == world_pos_b.y);

    return 0;
}
