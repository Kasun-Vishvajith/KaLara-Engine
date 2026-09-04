#pragma once

#include "kalara/SemanticType.hpp"
#include <string>
#include <vector>
#include <string_view>

namespace kalara::runtime {

struct SemanticComponent {
    SemanticType type{SemanticType::None};
    std::string custom_type_name{};
    std::string group_id{};
    std::vector<std::string> tags{};
    uint32_t collision_layer{CollisionLayer::Default};
    uint32_t collision_mask{CollisionLayer::All};

    [[nodiscard]] bool is_kind_of(SemanticType target_type) const noexcept {
        return SemanticRegistry::instance().is_kind_of(type, target_type);
    }

    [[nodiscard]] bool has_tag(std::string_view tag) const noexcept {
        for (const auto& t : tags) {
            if (t == tag) return true;
        }
        return false;
    }

    void add_tag(std::string_view tag) {
        if (!has_tag(tag)) {
            tags.emplace_back(tag);
        }
    }

    [[nodiscard]] bool can_collide_with(uint32_t other_layer) const noexcept {
        return (collision_mask & other_layer) != 0;
    }
};

} // namespace kalara::runtime
