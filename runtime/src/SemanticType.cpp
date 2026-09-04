#include "kalara/SemanticType.hpp"

namespace kalara::runtime {

SemanticRegistry& SemanticRegistry::instance() noexcept {
    static SemanticRegistry s_instance;
    return s_instance;
}

SemanticRegistry::SemanticRegistry() {
    // Setup built-in inheritance relationships
    m_parents[SemanticType::Player] = SemanticType::None;
    m_parents[SemanticType::Enemy] = SemanticType::Obstacle;
    m_parents[SemanticType::Wall] = SemanticType::Obstacle;
    m_parents[SemanticType::Collectible] = SemanticType::None;
    m_parents[SemanticType::Trigger] = SemanticType::None;
    m_parents[SemanticType::Projectile] = SemanticType::None;
}

void SemanticRegistry::register_custom_type(std::string_view type_name, SemanticType parent_type) {
    std::string name(type_name);
    m_custom_types[name] = SemanticType::Custom;
    m_parents[SemanticType::Custom] = parent_type;
}

void SemanticRegistry::set_parent_type(SemanticType child_type, SemanticType parent_type) {
    m_parents[child_type] = parent_type;
}

bool SemanticRegistry::is_kind_of(SemanticType child_type, SemanticType target_parent) const noexcept {
    if (child_type == target_parent) return true;

    SemanticType curr = child_type;
    while (curr != SemanticType::None) {
        auto it = m_parents.find(curr);
        if (it == m_parents.end()) break;
        curr = it->second;
        if (curr == target_parent) return true;
    }

    return false;
}

SemanticType SemanticRegistry::get_parent_type(SemanticType type) const noexcept {
    auto it = m_parents.find(type);
    if (it != m_parents.end()) return it->second;
    return SemanticType::None;
}

} // namespace kalara::runtime
