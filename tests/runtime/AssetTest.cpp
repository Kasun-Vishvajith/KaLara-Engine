#include "kalara/UUID.hpp"
#include "kalara/Asset.hpp"
#include "kalara/Image.hpp"
#include "kalara/Texture.hpp"
#include "kalara/AssetManager.hpp"
#include <cassert>

int main() {
    // Test UUID Generation
    kalara::core::AssetID id1 = kalara::core::UUID::generate();
    kalara::core::AssetID id2 = kalara::core::UUID::generate();
    assert(id1 != 0);
    assert(id2 != 0);
    assert(id1 != id2);

    // Test Image memory creation
    uint8_t raw_pixels[16] = {
        255, 0, 0, 255,   0, 255, 0, 255,
        0, 0, 255, 255,   255, 255, 255, 255
    };
    auto img = kalara::runtime::Image::create_from_memory(2, 2, 4, raw_pixels);
    assert(img != nullptr);
    assert(img->width() == 2);
    assert(img->height() == 2);
    assert(img->channels() == 4);
    assert(img->pixels().size() == 16);

    // Test Asset Registration and AssetManager
    auto& mgr = kalara::runtime::AssetManager::instance();
    mgr.register_asset(img);
    assert(mgr.has_asset(img->id()));
    assert(mgr.get_asset(img->id()) == img);

    mgr.unload_asset(img->id());
    assert(!mgr.has_asset(img->id()));

    mgr.clear_cache();
    assert(mgr.cached_asset_count() == 0);

    return 0;
}
