#include "kalara/ProjectTemplate.hpp"
#include "kalara/Scene.hpp"
#include "kalara/Component.hpp"
#include <filesystem>
#include <cassert>

namespace fs = std::filesystem;

int main() {
    fs::path temp_dir = fs::current_path() / "test_projects";
    fs::create_directories(temp_dir);

    // 1. Test Blank Project Creation
    fs::path blank_dir = temp_dir / "BlankProject";
    bool ok_blank = kalara::runtime::TemplateManager::create_project(
        kalara::runtime::ProjectTemplateType::Blank, blank_dir.string(), "BlankProject");
    assert(ok_blank);
    assert(fs::exists(blank_dir / "project.json"));
    assert(fs::exists(blank_dir / "scenes" / "default_scene.json"));

    // 2. Test Platformer Project Creation
    fs::path platformer_dir = temp_dir / "PlatformerProject";
    bool ok_plat = kalara::runtime::TemplateManager::create_project(
        kalara::runtime::ProjectTemplateType::Platformer, platformer_dir.string(), "PlatformerProject");
    assert(ok_plat);
    assert(fs::exists(platformer_dir / "scripts" / "PlayerController.cs"));

    // 3. Test TopDown RPG Scene Build
    auto rpg_scene = kalara::runtime::TemplateManager::build_template_scene(
        kalara::runtime::ProjectTemplateType::TopDownRPG, "RPG Main");
    assert(rpg_scene != nullptr);
    assert(rpg_scene->registry().entity_count() >= 3); // Camera, Player, Chest

    // 4. Test TopDown Shooter Scene Build
    auto shooter_scene = kalara::runtime::TemplateManager::build_template_scene(
        kalara::runtime::ProjectTemplateType::TopDownShooter, "Shooter Main");
    assert(shooter_scene != nullptr);
    assert(shooter_scene->registry().entity_count() >= 3); // Camera, Player, Enemy Target

    // Cleanup test artifacts
    fs::remove_all(temp_dir);
    return 0;
}
