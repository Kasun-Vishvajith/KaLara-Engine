#include "kalara/Exporter.hpp"
#include "kalara/ProjectTemplate.hpp"
#include <filesystem>
#include <cassert>

namespace fs = std::filesystem;

int main() {
    fs::path temp_dir = fs::current_path() / "export_test_env";
    fs::path source_proj = temp_dir / "SourceProject";
    fs::path export_dist = temp_dir / "dist" / "WindowsGame";

    fs::create_directories(temp_dir);

    // 1. Create a template project to export
    bool proj_ok = kalara::runtime::TemplateManager::create_project(
        kalara::runtime::ProjectTemplateType::Platformer, source_proj.string(), "PlatformerDemo");
    assert(proj_ok);

    // 2. Export Windows Standalone Package
    kalara::exporter::ExportOptions options;
    options.project_dir = source_proj.string();
    options.output_dir = export_dist.string();
    options.game_title = "PlatformerDemo Standalone";
    options.width = 1920;
    options.height = 1080;
    options.fullscreen = true;

    bool export_ok = kalara::exporter::Exporter::export_windows_game(options);
    assert(export_ok);

    // 3. Verify Output Bundle Structure & Config Manifest
    assert(fs::exists(export_dist / "game_config.json"));
    assert(fs::exists(export_dist / "scenes" / "default_scene.json"));
    assert(fs::exists(export_dist / "scripts" / "PlayerController.cs"));

    // Cleanup test directory
    fs::remove_all(temp_dir);
    return 0;
}
