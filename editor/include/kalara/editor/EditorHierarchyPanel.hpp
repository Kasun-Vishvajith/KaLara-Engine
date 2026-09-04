#pragma once

#include "kalara/editor/EditorState.hpp"
#include <vector>
#include <string>

namespace kalara::editor {

class EditorHierarchyPanel {
public:
    EditorHierarchyPanel() = default;
    ~EditorHierarchyPanel() = default;

    void render();
    runtime::EntityID create_entity_in_scene(std::string_view name = "New Entity");
    void delete_selected_entity();
};

} // namespace kalara::editor
