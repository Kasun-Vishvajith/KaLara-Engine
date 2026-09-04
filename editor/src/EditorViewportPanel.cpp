#include "kalara/editor/EditorViewportPanel.hpp"
#include "kalara/Hierarchy.hpp"
#include "kalara/Component.hpp"

namespace kalara::editor {

void EditorViewportPanel::render(runtime::IRenderer* renderer) {
    if (!renderer) return;

    draw_grid(renderer, 64.0f);
    draw_selection_outline(renderer);
}

void EditorViewportPanel::draw_grid(runtime::IRenderer* renderer, float grid_size) {
    float win_w = static_cast<float>(renderer->viewport().width);
    float win_h = static_cast<float>(renderer->viewport().height);
    core::Vector4 grid_color{0.2f, 0.22f, 0.26f, 0.5f};

    for (float x = 0.0f; x < win_w; x += grid_size) {
        renderer->draw_line({x, 0.0f}, {x, win_h}, grid_color, 1.0f);
    }
    for (float y = 0.0f; y < win_h; y += grid_size) {
        renderer->draw_line({0.0f, y}, {win_w, y}, grid_color, 1.0f);
    }
}

void EditorViewportPanel::draw_selection_outline(runtime::IRenderer* renderer) {
    auto scene = EditorState::instance().active_scene();
    runtime::EntityID selected = EditorState::instance().selected_entity();

    if (!scene || selected == runtime::NullEntityID || !scene->registry().valid(selected)) {
        return;
    }

    if (scene->registry().has_component<runtime::TransformComponent>(selected)) {
        const auto& t = scene->registry().get_component<runtime::TransformComponent>(selected);
        core::Vector3 world_pos = runtime::Hierarchy::get_world_position(scene->registry(), selected);

        core::Vector4 outline_color{0.95f, 0.75f, 0.2f, 1.0f}; // Gold outline
        float x = world_pos.x;
        float y = world_pos.y;
        float w = t.scale.x;
        float h = t.scale.y;

        // Top, Bottom, Left, Right selection bounds
        renderer->draw_line({x, y}, {x + w, y}, outline_color, 2.0f);
        renderer->draw_line({x + w, y}, {x + w, y + h}, outline_color, 2.0f);
        renderer->draw_line({x + w, y + h}, {x, y + h}, outline_color, 2.0f);
        renderer->draw_line({x, y + h}, {x, y}, outline_color, 2.0f);
    }
}

} // namespace kalara::editor
