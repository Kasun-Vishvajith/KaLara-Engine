#include "kalara/editor/EditorInspectorPanel.hpp"
#include "kalara/Component.hpp"
#include "kalara/Log.hpp"

namespace kalara::editor {

void EditorInspectorPanel::render() {
    auto scene = EditorState::instance().active_scene();
    runtime::EntityID selected = EditorState::instance().selected_entity();

    if (!scene || selected == runtime::NullEntityID || !scene->registry().valid(selected)) {
        return;
    }

    if (scene->registry().has_component<runtime::TagComponent>(selected)) {
        const auto& tag = scene->registry().get_component<runtime::TagComponent>(selected);
        (void)tag;
    }

    if (scene->registry().has_component<runtime::TransformComponent>(selected)) {
        const auto& transform = scene->registry().get_component<runtime::TransformComponent>(selected);
        (void)transform;
    }
}

void EditorInspectorPanel::set_position(float x, float y, float z) {
    auto scene = EditorState::instance().active_scene();
    runtime::EntityID selected = EditorState::instance().selected_entity();

    if (scene && selected != runtime::NullEntityID && scene->registry().has_component<runtime::TransformComponent>(selected)) {
        auto& t = scene->registry().get_component<runtime::TransformComponent>(selected);
        t.position = {x, y, z};
    }
}

void EditorInspectorPanel::set_scale(float sx, float sy, float sz) {
    auto scene = EditorState::instance().active_scene();
    runtime::EntityID selected = EditorState::instance().selected_entity();

    if (scene && selected != runtime::NullEntityID && scene->registry().has_component<runtime::TransformComponent>(selected)) {
        auto& t = scene->registry().get_component<runtime::TransformComponent>(selected);
        t.scale = {sx, sy, sz};
    }
}

} // namespace kalara::editor
