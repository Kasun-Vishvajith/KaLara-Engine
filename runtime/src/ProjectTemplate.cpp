#include "kalara/ProjectTemplate.hpp"
#include "kalara/SceneSerializer.hpp"
#include "kalara/Component.hpp"
#include "kalara/CameraComponent.hpp"
#include "kalara/PhysicsComponents.hpp"
#include "kalara/BehaviorComponents.hpp"
#include "kalara/SemanticComponent.hpp"
#include "kalara/Json.hpp"
#include "kalara/Log.hpp"
#include <filesystem>
#include <fstream>

namespace kalara::runtime {

namespace fs = std::filesystem;

std::shared_ptr<Scene> TemplateManager::build_template_scene(ProjectTemplateType type, std::string_view scene_name) {
    auto scene = Scene::create(scene_name);

    // 1. Camera Entity
    EntityID camera = scene->create_entity("Main Camera");
    auto& cam_comp = scene->registry().add_component<CameraComponent>(camera);
    cam_comp.is_primary = true;

    if (type == ProjectTemplateType::Blank) {
        // Simple blank project with camera
        return scene;
    }

    // 2. Player Entity
    EntityID player = scene->create_entity("Player");
    auto& p_trans = scene->registry().get_component<TransformComponent>(player);
    p_trans.position = {640.0f, 360.0f, 0.0f};
    p_trans.scale = {64.0f, 64.0f, 1.0f};

    auto& p_sprite = scene->registry().add_component<SpriteRendererComponent>(player);
    p_sprite.color = {0.2f, 0.8f, 0.4f, 1.0f};

    auto& p_move = scene->registry().add_component<MovementBehaviorComponent>(player);
    p_move.move_speed = 300.0f;
    p_move.use_input_controls = true;

    auto& p_health = scene->registry().add_component<HealthBehaviorComponent>(player);
    p_health.max_health = 100.0f;
    p_health.current_health = 100.0f;

    auto& p_sem = scene->registry().add_component<SemanticComponent>(player);
    p_sem.type = SemanticType::Player;
    p_sem.add_tag("Player");

    if (type == ProjectTemplateType::Platformer) {
        // Platformer: Add Dynamic Physics to Player and Ground Platform
        auto& p_body = scene->registry().add_component<RigidBody2DComponent>(player);
        p_body.type = BodyType2D::Dynamic;

        auto& p_box = scene->registry().add_component<BoxCollider2DComponent>(player);
        p_box.size = {64.0f, 64.0f};

        // Ground Platform Entity
        EntityID ground = scene->create_entity("Ground Platform");
        auto& g_trans = scene->registry().get_component<TransformComponent>(ground);
        g_trans.position = {0.0f, 600.0f, 0.0f};
        g_trans.scale = {1280.0f, 64.0f, 1.0f};

        auto& g_sprite = scene->registry().add_component<SpriteRendererComponent>(ground);
        g_sprite.color = {0.5f, 0.35f, 0.2f, 1.0f};

        auto& g_body = scene->registry().add_component<RigidBody2DComponent>(ground);
        g_body.type = BodyType2D::Static;

        auto& g_box = scene->registry().add_component<BoxCollider2DComponent>(ground);
        g_box.size = {1280.0f, 64.0f};

        auto& g_sem = scene->registry().add_component<SemanticComponent>(ground);
        g_sem.type = SemanticType::Wall;
        g_sem.add_tag("Ground");
    } else if (type == ProjectTemplateType::TopDownRPG) {
        // TopDown RPG: Add Chest and Door triggers
        EntityID chest = scene->create_entity("Treasure Chest");
        auto& c_trans = scene->registry().get_component<TransformComponent>(chest);
        c_trans.position = {800.0f, 300.0f, 0.0f};
        c_trans.scale = {48.0f, 48.0f, 1.0f};

        auto& c_sprite = scene->registry().add_component<SpriteRendererComponent>(chest);
        c_sprite.color = {0.95f, 0.8f, 0.2f, 1.0f};

        auto& c_item = scene->registry().add_component<CollectibleBehaviorComponent>(chest);
        c_item.score_value = 500;

        auto& c_sem = scene->registry().add_component<SemanticComponent>(chest);
        c_sem.type = SemanticType::Collectible;
        c_sem.add_tag("Chest");
    } else if (type == ProjectTemplateType::TopDownShooter) {
        // TopDown Shooter: Add ShootingBehavior to Player and Enemy Target
        auto& p_shoot = scene->registry().add_component<ShootingBehaviorComponent>(player);
        p_shoot.fire_rate = 5.0f;
        p_shoot.is_auto_firing = true;

        EntityID enemy = scene->create_entity("Enemy Target");
        auto& e_trans = scene->registry().get_component<TransformComponent>(enemy);
        e_trans.position = {900.0f, 360.0f, 0.0f};
        e_trans.scale = {56.0f, 56.0f, 1.0f};

        auto& e_sprite = scene->registry().add_component<SpriteRendererComponent>(enemy);
        e_sprite.color = {0.85f, 0.2f, 0.2f, 1.0f};

        auto& e_health = scene->registry().add_component<HealthBehaviorComponent>(enemy);
        e_health.max_health = 50.0f;

        auto& e_dmg = scene->registry().add_component<DamageBehaviorComponent>(enemy);
        e_dmg.damage_amount = 15.0f;

        auto& e_sem = scene->registry().add_component<SemanticComponent>(enemy);
        e_sem.type = SemanticType::Enemy;
        e_sem.add_tag("Enemy");
    }

    return scene;
}

bool TemplateManager::create_project(ProjectTemplateType type, std::string_view project_dir, std::string_view project_name) {
    try {
        fs::path p_path(project_dir);
        fs::create_directories(p_path / "assets");
        fs::create_directories(p_path / "scenes");
        fs::create_directories(p_path / "scripts");

        // Write project.json manifest
        core::JsonValue manifest;
        manifest["name"] = std::string(project_name);
        manifest["version"] = "1.0.0";
        manifest["template"] = static_cast<int>(type);

        std::ofstream manifest_out((p_path / "project.json").string());
        if (manifest_out.is_open()) {
            manifest_out << manifest.dump(2);
            manifest_out.close();
        }

        // Build and serialize default scene
        auto scene = build_template_scene(type, "Main Scene");
        SceneSerializer serializer(scene);
        serializer.serialize((p_path / "scenes" / "default_scene.json").string());

        // Create starter C# script
        std::ofstream script_out((p_path / "scripts" / "PlayerController.cs").string());
        if (script_out.is_open()) {
            script_out << "using KaLara.Engine;\n\n"
                       << "public class PlayerController : ScriptableEntity\n"
                       << "{\n"
                       << "    public override void OnCreate()\n"
                       << "    {\n"
                       << "        Log.Info(\"PlayerController script initialized for project '" << project_name << "'\");\n"
                       << "    }\n\n"
                       << "    public override void OnUpdate(float deltaTime)\n"
                       << "    {\n"
                       << "    }\n"
                       << "}\n";
            script_out.close();
        }

        KALARA_LOG_INFO("Created KaLara project template '{}' in '{}'", project_name, project_dir);
        return true;
    } catch (const std::exception& e) {
        KALARA_LOG_ERROR("Failed to create project template: {}", e.what());
        return false;
    }
}

} // namespace kalara::runtime
