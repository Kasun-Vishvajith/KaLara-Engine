#pragma once

#include "kalara/Registry.hpp"

namespace kalara::runtime {

class Hierarchy {
public:
    static void set_parent(Registry& registry, EntityID child_id, EntityID parent_id);
    static void remove_parent(Registry& registry, EntityID child_id);
    static core::Vector3 get_world_position(const Registry& registry, EntityID entity_id);
};

} // namespace kalara::runtime
