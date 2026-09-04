#pragma once

#include "kalara/editor/EditorState.hpp"
#include "kalara/IRenderer.hpp"

namespace kalara::editor {

class EditorViewportPanel {
public:
    EditorViewportPanel() = default;
    ~EditorViewportPanel() = default;

    void render(runtime::IRenderer* renderer);
    void draw_grid(runtime::IRenderer* renderer, float grid_size = 50.0f);
    void draw_selection_outline(runtime::IRenderer* renderer);
};

} // namespace kalara::editor
