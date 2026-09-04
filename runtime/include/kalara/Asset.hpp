#pragma once

#include "kalara/UUID.hpp"
#include <string>

namespace kalara::runtime {

enum class AssetType {
    Unknown,
    Image,
    Texture,
    Shader,
    Audio,
    Scene
};

struct AssetMetadata {
    core::AssetID id{0};
    AssetType type{AssetType::Unknown};
    std::string path{};
    std::string name{};
    bool is_valid{false};
};

class Asset {
public:
    virtual ~Asset() = default;

    [[nodiscard]] core::AssetID id() const noexcept { return m_metadata.id; }
    [[nodiscard]] AssetType type() const noexcept { return m_metadata.type; }
    [[nodiscard]] const AssetMetadata& metadata() const noexcept { return m_metadata; }

protected:
    AssetMetadata m_metadata{};
};

} // namespace kalara::runtime
