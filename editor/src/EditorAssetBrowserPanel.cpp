#include "kalara/editor/EditorAssetBrowserPanel.hpp"
#include "kalara/Log.hpp"

namespace kalara::editor {

void EditorAssetBrowserPanel::render() {
    size_t count = runtime::AssetManager::instance().cached_asset_count();
    (void)count;
}

size_t EditorAssetBrowserPanel::asset_count() const noexcept {
    return runtime::AssetManager::instance().cached_asset_count();
}

} // namespace kalara::editor
