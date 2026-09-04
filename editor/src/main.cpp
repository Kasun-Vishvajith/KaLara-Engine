#include "kalara/Core.hpp"
#include "kalara/Runtime.hpp"
#include "kalara/editor/EditorState.hpp"
#include "kalara/editor/EditorHierarchyPanel.hpp"
#include "kalara/editor/EditorInspectorPanel.hpp"
#include "kalara/editor/EditorViewportPanel.hpp"
#include "kalara/editor/EditorAssetBrowserPanel.hpp"
#include "kalara/Log.hpp"

int main() {
    kalara::core::EngineConfig config{};
    config.app_name = "KaLara Editor";
    config.window_width = 1280;
    config.window_height = 720;
    config.vsync = true;
    config.log_level = kalara::core::LogLevel::Info;

    kalara::runtime::ApplicationRuntime app;
    if (!app.initialize(config)) {
        KALARA_LOG_CRITICAL("Failed to initialize KaLara ApplicationRuntime!");
        return 1;
    }

    // Initialize Editor State & Panels
    auto active_scene = kalara::runtime::Scene::create("KaLara Viewport Scene");
    kalara::editor::EditorState::instance().set_active_scene(active_scene);

    kalara::editor::EditorHierarchyPanel hierarchy_panel;
    kalara::editor::EditorInspectorPanel inspector_panel;
    kalara::editor::EditorViewportPanel viewport_panel;
    kalara::editor::EditorAssetBrowserPanel asset_browser_panel;

    // Create initial scene entity and select it
    kalara::runtime::EntityID player = hierarchy_panel.create_entity_in_scene("PlayerQuad");
    inspector_panel.set_position(540.0f, 285.0f, 0.0f);
    inspector_panel.set_scale(200.0f, 150.0f, 1.0f);

    auto& sprite = active_scene->registry().add_component<kalara::runtime::SpriteRendererComponent>(player);
    sprite.color = {0.2f, 0.75f, 0.65f, 1.0f};

    // Save and load scene via EditorState
    kalara::editor::EditorState::instance().save_scene("sample_scene.json");
    kalara::editor::EditorState::instance().load_scene("sample_scene.json");

    KALARA_LOG_INFO("KaLara Editor ready [Viewport, Hierarchy, Inspector, Asset Browser]. Press ESC to quit.");

    app.run();

    app.shutdown();
    return 0;
}
