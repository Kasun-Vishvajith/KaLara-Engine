#include "kalara/Hierarchy.hpp"
#include "kalara/Log.hpp"
#include <algorithm>

namespace kalara::runtime {

void Hierarchy::set_parent(Registry& registry, EntityID child_id, EntityID parent_id) {
    if (child_id == parent_id || child_id == NullEntityID) return;

    if (!registry.has_component<TransformComponent>(child_id) ||
        (parent_id != NullEntityID && !registry.has_component<TransformComponent>(parent_id))) {
        return;
    }

    remove_parent(registry, child_id);

    auto& child_transform = registry.get_component<TransformComponent>(child_id);
    child_transform.parent = parent_id;

    if (parent_id != NullEntityID) {
        auto& parent_transform = registry.get_component<TransformComponent>(parent_id);
        parent_transform.children.push_back(child_id);
        KALARA_LOG_INFO("Set [{}] parent to [{}]", core::UUID::format_entity_id(child_id), core::UUID::format_entity_id(parent_id));
    }
}

void Hierarchy::remove_parent(Registry& registry, EntityID child_id) {
    if (!registry.has_component<TransformComponent>(child_id)) return;

    auto& child_transform = registry.get_component<TransformComponent>(child_id);
    EntityID old_parent = child_transform.parent;

    if (old_parent != NullEntityID && registry.has_component<TransformComponent>(old_parent)) {
        auto& parent_transform = registry.get_component<TransformComponent>(old_parent);
        auto& vec = parent_transform.children;
        vec.erase(std::remove(vec.begin(), vec.end(), child_id), vec.end());
    }

    child_transform.parent = NullEntityID;
}

core::Vector3 Hierarchy::get_world_position(const Registry& registry, EntityID entity_id) {
    if (!registry.has_component<TransformComponent>(entity_id)) {
        return {0.0f, 0.0f, 0.0f};
    }

    const auto& transform = registry.get_component<TransformComponent>(entity_id);
    core::Vector3 pos = transform.position;

    EntityID curr_parent = transform.parent;
    while (curr_parent != NullEntityID && registry.has_component<TransformComponent>(curr_parent)) {
        const auto& parent_transform = registry.get_component<TransformComponent>(curr_parent);
        pos.x += parent_transform.position.x;
        pos.y += parent_transform.position.y;
        pos.z += parent_transform.position.z;
        curr_parent = parent_transform.parent;
    }

    return pos;
}

} // namespace kalara::runtime
