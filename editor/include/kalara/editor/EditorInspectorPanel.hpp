#pragma once

#include "kalara/editor/EditorState.hpp"

namespace kalara::editor {

class EditorInspectorPanel {
public:
    EditorInspectorPanel() = default;
    ~EditorInspectorPanel() = default;

    void render();
    void set_position(float x, float y, float z);
    void set_scale(float sx, float sy, float sz);
};

} // namespace kalara::editor
