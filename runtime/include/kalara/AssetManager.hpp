#pragma once

#include "kalara/Asset.hpp"
#include "kalara/Texture.hpp"
#include "kalara/Image.hpp"
#include <unordered_map>
#include <memory>
#include <string>
#include <string_view>

namespace kalara::runtime {

class AssetManager {
public:
    static AssetManager& instance() noexcept;

    std::shared_ptr<Texture2D> load_texture(std::string_view file_path);
    std::shared_ptr<Texture2D> get_texture(core::AssetID id) const;
    std::shared_ptr<Texture2D> get_texture(std::string_view file_path) const;

    void register_asset(std::shared_ptr<Asset> asset);
    std::shared_ptr<Asset> get_asset(core::AssetID id) const;
    bool has_asset(core::AssetID id) const noexcept;

    void unload_asset(core::AssetID id);
    void clear_cache() noexcept;

    [[nodiscard]] size_t cached_asset_count() const noexcept { return m_assets.size(); }

private:
    AssetManager() = default;

    std::unordered_map<core::AssetID, std::shared_ptr<Asset>> m_assets;
    std::unordered_map<std::string, core::AssetID> m_path_to_id;
};

} // namespace kalara::runtime
