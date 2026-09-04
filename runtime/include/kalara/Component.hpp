#pragma once

#include "kalara/Entity.hpp"
#include "kalara/Math.hpp"
#include <string>
#include <vector>

namespace kalara::runtime {

struct TagComponent {
    std::string name{"Entity"};
};

struct TransformComponent {
    core::Vector3 position{0.0f, 0.0f, 0.0f};
    core::Vector3 rotation{0.0f, 0.0f, 0.0f}; // Pitch, Yaw, Roll (degrees)
    core::Vector3 scale{1.0f, 1.0f, 1.0f};

    EntityID parent{NullEntityID};
    std::vector<EntityID> children{};

    [[nodiscard]] core::Vector2 position2d() const noexcept {
        return {position.x, position.y};
    }

    [[nodiscard]] core::Vector2 scale2d() const noexcept {
        return {scale.x, scale.y};
    }
};

struct SpriteRendererComponent {
    core::Vector4 color{1.0f, 1.0f, 1.0f, 1.0f};
    core::AssetID texture_asset_id{0};
};

} // namespace kalara::runtime
