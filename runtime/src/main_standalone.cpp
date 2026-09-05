#include "kalara/Core.hpp"
#include "kalara/Runtime.hpp"
#include "kalara/Scene.hpp"
#include "kalara/SceneSerializer.hpp"
#include "kalara/BehaviorSystem.hpp"
#include "kalara/PhysicsWorld2D.hpp"
#include "kalara/ScriptEngine.hpp"
#include "kalara/Input.hpp"
#include "kalara/Json.hpp"
#include "kalara/Log.hpp"
#include <filesystem>
#include <fstream>
#include <memory>

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    // Read standalone game_config.json if available
    kalara::core::EngineConfig config{};
    config.app_name = "KaLara Standalone Game";
    config.window_width = 1280;
    config.window_height = 720;
    config.log_level = kalara::core::LogLevel::Info;

    std::string scene_path = "scenes/default_scene.json";

    if (fs::exists("game_config.json")) {
        std::ifstream config_file("game_config.json");
        if (config_file.is_open()) {
            std::string str((std::istreambuf_iterator<char>(config_file)), std::istreambuf_iterator<char>());
            auto json = kalara::core::JsonValue::parse(str);
            if (json.contains("title")) config.app_name = json["title"].as_string();
            if (json.contains("width")) config.window_width = static_cast<uint32_t>(json["width"].as_number());
            if (json.contains("height")) config.window_height = static_cast<uint32_t>(json["height"].as_number());
            if (json.contains("initial_scene")) scene_path = json["initial_scene"].as_string();
        }
    }

    kalara::runtime::ApplicationRuntime runtime;
    if (!runtime.initialize(config)) {
        KALARA_LOG_ERROR("Failed to initialize standalone game runtime.");
        return -1;
    }

    // Load initial scene
    std::shared_ptr<kalara::runtime::Scene> active_scene = kalara::runtime::Scene::create("Game Scene");
    if (fs::exists(scene_path)) {
        kalara::runtime::SceneSerializer serializer(active_scene);
        serializer.deserialize(scene_path);
    }

    kalara::runtime::PhysicsWorld2D physics_world;
    kalara::runtime::ScriptEngine::initialize(&active_scene->registry());

    KALARA_LOG_INFO("Standalone game '{}' running...", config.app_name);

    uint64_t last_ticks = 0;

    while (runtime.is_running()) {
        float delta_time = 0.016f; // Standard 60 FPS tick
        runtime.update(delta_time);

        // ESC quits standalone game
        if (kalara::runtime::Input::is_key_pressed(kalara::runtime::KeyCode::Escape)) {
            runtime.request_quit();
        }

        physics_world.step(active_scene->registry(), delta_time);
        kalara::runtime::BehaviorSystem::update(active_scene->registry(), delta_time);

        runtime.render();
    }

    kalara::runtime::ScriptEngine::shutdown();
    runtime.shutdown();
    KALARA_LOG_INFO("Standalone game terminated cleanly.");
    return 0;
}
