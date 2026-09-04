#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>

namespace kalara::runtime {

enum class SemanticType : uint16_t {
    None = 0,
    Player = 1,
    Enemy = 2,
    Obstacle = 3,
    Wall = 4,
    Collectible = 5,
    Trigger = 6,
    Projectile = 7,
    Custom = 999
};

namespace CollisionLayer {
    constexpr uint32_t None       = 0;
    constexpr uint32_t Default    = 1 << 0; // 1
    constexpr uint32_t Player     = 1 << 1; // 2
    constexpr uint32_t Enemy      = 1 << 2; // 4
    constexpr uint32_t Obstacle   = 1 << 3; // 8
    constexpr uint32_t Trigger    = 1 << 4; // 16
    constexpr uint32_t Projectile = 1 << 5; // 32
    constexpr uint32_t All        = 0xFFFFFFFF;
}

class SemanticRegistry {
public:
    static SemanticRegistry& instance() noexcept;

    void register_custom_type(std::string_view type_name, SemanticType parent_type = SemanticType::None);
    void set_parent_type(SemanticType child_type, SemanticType parent_type);

    [[nodiscard]] bool is_kind_of(SemanticType child_type, SemanticType target_parent) const noexcept;
    [[nodiscard]] SemanticType get_parent_type(SemanticType type) const noexcept;

private:
    SemanticRegistry();

    std::unordered_map<SemanticType, SemanticType> m_parents;
    std::unordered_map<std::string, SemanticType> m_custom_types;
};

} // namespace kalara::runtime
