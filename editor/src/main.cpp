#include "kalara/Core.hpp"
#include "kalara/Runtime.hpp"
#include "kalara/Scene.hpp"
#include "kalara/SceneSerializer.hpp"
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

    // 1. Create a scene with entities
    auto active_scene = kalara::runtime::Scene::create("Editor Viewport Scene");

    auto parent = active_scene->create_entity("CenteredQuad");
    auto& p_trans = active_scene->registry().get_component<kalara::runtime::TransformComponent>(parent);
    p_trans.position = {540.0f, 285.0f, 0.0f};
    p_trans.scale = {200.0f, 150.0f, 1.0f};
    auto& p_sprite = active_scene->registry().add_component<kalara::runtime::SpriteRendererComponent>(parent);
    p_sprite.color = {0.2f, 0.75f, 0.65f, 1.0f};

    // 2. Serialize scene to disk
    kalara::runtime::SceneSerializer serializer(active_scene);
    serializer.serialize("sample_scene.json");

    // 3. Deserialize scene back from disk to verify round-trip
    auto loaded_scene = kalara::runtime::Scene::create();
    kalara::runtime::SceneSerializer deserializer(loaded_scene);
    deserializer.deserialize("sample_scene.json");

    KALARA_LOG_INFO("KaLara Editor active scene '{}' initialized ({} entities). Press ESC to quit.",
                    loaded_scene->name(), loaded_scene->registry().entity_count());

    app.run();

    app.shutdown();
    return 0;
}
