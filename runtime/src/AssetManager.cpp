#include "kalara/AssetManager.hpp"
#include "kalara/Log.hpp"

namespace kalara::runtime {

AssetManager& AssetManager::instance() noexcept {
    static AssetManager s_instance;
    return s_instance;
}

std::shared_ptr<Texture2D> AssetManager::load_texture(std::string_view file_path) {
    std::string path_str(file_path);
    auto it = m_path_to_id.find(path_str);
    if (it != m_path_to_id.end()) {
        return get_texture(it->second);
    }

    auto image = Image::create_from_file(file_path);
    if (!image) {
        return nullptr;
    }

    auto texture = Texture2D::create(*image);
    if (texture) {
        register_asset(texture);
        m_path_to_id[path_str] = texture->id();
        KALARA_LOG_INFO("Cached Texture asset '{}' [AssetID: {}]", file_path, texture->id());
    }

    return texture;
}

std::shared_ptr<Texture2D> AssetManager::get_texture(core::AssetID id) const {
    auto asset = get_asset(id);
    return std::dynamic_pointer_cast<Texture2D>(asset);
}

std::shared_ptr<Texture2D> AssetManager::get_texture(std::string_view file_path) const {
    auto it = m_path_to_id.find(std::string(file_path));
    if (it != m_path_to_id.end()) {
        return get_texture(it->second);
    }
    return nullptr;
}

void AssetManager::register_asset(std::shared_ptr<Asset> asset) {
    if (asset && asset->id() != 0) {
        m_assets[asset->id()] = asset;
        if (!asset->metadata().path.empty()) {
            m_path_to_id[asset->metadata().path] = asset->id();
        }
    }
}

std::shared_ptr<Asset> AssetManager::get_asset(core::AssetID id) const {
    auto it = m_assets.find(id);
    if (it != m_assets.end()) {
        return it->second;
    }
    return nullptr;
}

bool AssetManager::has_asset(core::AssetID id) const noexcept {
    return m_assets.find(id) != m_assets.end();
}

void AssetManager::unload_asset(core::AssetID id) {
    auto it = m_assets.find(id);
    if (it != m_assets.end()) {
        std::string path = it->second->metadata().path;
        if (!path.empty()) {
            m_path_to_id.erase(path);
        }
        m_assets.erase(it);
        KALARA_LOG_INFO("Unloaded AssetID: {}", id);
    }
}

void AssetManager::clear_cache() noexcept {
    m_assets.clear();
    m_path_to_id.clear();
    KALARA_LOG_INFO("Cleared AssetManager cache.");
}

} // namespace kalara::runtime
