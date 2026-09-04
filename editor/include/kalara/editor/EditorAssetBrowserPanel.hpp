#pragma once

#include "kalara/AssetManager.hpp"
#include <string>
#include <vector>

namespace kalara::editor {

class EditorAssetBrowserPanel {
public:
    EditorAssetBrowserPanel() = default;
    ~EditorAssetBrowserPanel() = default;

    void render();
    [[nodiscard]] size_t asset_count() const noexcept;
};

} // namespace kalara::editor
