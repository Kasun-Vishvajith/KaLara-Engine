#include "kalara/Registry.hpp"
#include "kalara/Log.hpp"
#include <algorithm>

namespace kalara::runtime {

EntityID Registry::create_entity(std::string_view name) {
    EntityID entity_id = core::UUID::generate();
    return create_entity_with_id(entity_id, name);
}

EntityID Registry::create_entity_with_id(EntityID id, std::string_view name) {
    if (valid(id)) return id;

    m_entities.push_back(id);
    add_component<TagComponent>(id, std::string(name));
    add_component<TransformComponent>(id);

    KALARA_LOG_INFO("Created entity '{}' [{}]", name, core::UUID::format_entity_id(id));
    return id;
}

bool Registry::valid(EntityID entity_id) const noexcept {
    return std::find(m_entities.begin(), m_entities.end(), entity_id) != m_entities.end();
}

void Registry::destroy_entity(EntityID entity_id) {
    auto it = std::find(m_entities.begin(), m_entities.end(), entity_id);
    if (it != m_entities.end()) {
        m_entities.erase(it);
        KALARA_LOG_INFO("Destroyed entity [{}]", core::UUID::format_entity_id(entity_id));
    }
}

void Registry::clear() noexcept {
    m_entities.clear();
    m_pools.clear();
    KALARA_LOG_INFO("Cleared entity registry.");
}

} // namespace kalara::runtime
