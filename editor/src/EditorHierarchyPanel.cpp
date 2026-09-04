#include "kalara/editor/EditorHierarchyPanel.hpp"
#include "kalara/Component.hpp"
#include "kalara/Log.hpp"

namespace kalara::editor {

void EditorHierarchyPanel::render() {
    auto scene = EditorState::instance().active_scene();
    if (!scene) return;

    // Render tree nodes for each entity
    for (runtime::EntityID entity : scene->registry().entities()) {
        std::string tag = "Entity";
        if (scene->registry().has_component<runtime::TagComponent>(entity)) {
            tag = scene->registry().get_component<runtime::TagComponent>(entity).name;
        }

        bool is_selected = (EditorState::instance().selected_entity() == entity);
        (void)is_selected;
        (void)tag;
    }
}

runtime::EntityID EditorHierarchyPanel::create_entity_in_scene(std::string_view name) {
    auto scene = EditorState::instance().active_scene();
    if (!scene) return runtime::NullEntityID;

    runtime::EntityID id = scene->create_entity(name);
    EditorState::instance().select_entity(id);
    return id;
}

void EditorHierarchyPanel::delete_selected_entity() {
    auto scene = EditorState::instance().active_scene();
    runtime::EntityID selected = EditorState::instance().selected_entity();

    if (scene && selected != runtime::NullEntityID) {
        scene->destroy_entity(selected);
        EditorState::instance().deselect();
    }
}

} // namespace kalara::editor
