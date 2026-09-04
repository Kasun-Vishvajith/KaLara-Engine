#pragma once

#include "kalara/Scene.hpp"
#include "kalara/Entity.hpp"
#include <memory>
#include <string>

namespace kalara::editor {

enum class GizmoMode {
    Select,
    Translate,
    Rotate,
    Scale
};

class EditorState {
public:
    EditorState() = default;
    ~EditorState() = default;

    static EditorState& instance() noexcept;

    void set_active_scene(std::shared_ptr<runtime::Scene> scene) noexcept { m_active_scene = std::move(scene); }
    [[nodiscard]] std::shared_ptr<runtime::Scene> active_scene() const noexcept { return m_active_scene; }

    void select_entity(runtime::EntityID entity_id) noexcept { m_selected_entity_id = entity_id; }
    void deselect() noexcept { m_selected_entity_id = runtime::NullEntityID; }
    [[nodiscard]] runtime::EntityID selected_entity() const noexcept { return m_selected_entity_id; }
    [[nodiscard]] bool has_selection() const noexcept { return m_selected_entity_id != runtime::NullEntityID; }

    void set_gizmo_mode(GizmoMode mode) noexcept { m_gizmo_mode = mode; }
    [[nodiscard]] GizmoMode gizmo_mode() const noexcept { return m_gizmo_mode; }

    void save_scene(std::string_view filepath = "sample_scene.json");
    void load_scene(std::string_view filepath = "sample_scene.json");

private:
    std::shared_ptr<runtime::Scene> m_active_scene{nullptr};
    runtime::EntityID m_selected_entity_id{runtime::NullEntityID};
    GizmoMode m_gizmo_mode{GizmoMode::Select};
};

} // namespace kalara::editor
