#include "kalara/editor/EditorState.hpp"
#include "kalara/editor/EditorHierarchyPanel.hpp"
#include "kalara/editor/EditorInspectorPanel.hpp"
#include "kalara/editor/EditorViewportPanel.hpp"
#include "kalara/editor/EditorAssetBrowserPanel.hpp"
#include "kalara/Scene.hpp"
#include <cassert>

int main() {
    auto& state = kalara::editor::EditorState::instance();

    // 1. Test Active Scene Assignment
    auto scene = kalara::runtime::Scene::create("Editor Test Scene");
    state.set_active_scene(scene);
    assert(state.active_scene() == scene);

    // 2. Test Entity Creation & Selection via Hierarchy Panel
    kalara::editor::EditorHierarchyPanel hierarchy_panel;
    kalara::runtime::EntityID entity = hierarchy_panel.create_entity_in_scene("TestQuad");

    assert(state.has_selection());
    assert(state.selected_entity() == entity);

    // 3. Test Inspector Property Mutation
    kalara::editor::EditorInspectorPanel inspector_panel;
    inspector_panel.set_position(300.0f, 400.0f, 0.0f);

    const auto& transform = scene->registry().get_component<kalara::runtime::TransformComponent>(entity);
    assert(transform.position.x == 300.0f);
    assert(transform.position.y == 400.0f);

    // 4. Test Gizmo Mode Selection
    state.set_gizmo_mode(kalara::editor::GizmoMode::Translate);
    assert(state.gizmo_mode() == kalara::editor::GizmoMode::Translate);

    // 5. Test Deselection & Deletion
    hierarchy_panel.delete_selected_entity();
    assert(!state.has_selection());
    assert(scene->registry().entity_count() == 0);

    return 0;
}
