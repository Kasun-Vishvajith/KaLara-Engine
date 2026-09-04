#include "kalara/editor/EditorState.hpp"
#include "kalara/SceneSerializer.hpp"
#include "kalara/Log.hpp"

namespace kalara::editor {

EditorState& EditorState::instance() noexcept {
    static EditorState s_instance;
    return s_instance;
}

void EditorState::save_scene(std::string_view filepath) {
    if (!m_active_scene) return;
    runtime::SceneSerializer serializer(m_active_scene);
    if (serializer.serialize(filepath)) {
        KALARA_LOG_INFO("Editor saved scene '{}' to '{}'", m_active_scene->name(), filepath);
    }
}

void EditorState::load_scene(std::string_view filepath) {
    if (!m_active_scene) {
        m_active_scene = runtime::Scene::create();
    }
    runtime::SceneSerializer serializer(m_active_scene);
    if (serializer.deserialize(filepath)) {
        deselect();
        KALARA_LOG_INFO("Editor loaded scene '{}' from '{}'", m_active_scene->name(), filepath);
    }
}

} // namespace kalara::editor
