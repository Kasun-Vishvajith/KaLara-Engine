#include "kalara/Exporter.hpp"
#include "kalara/Json.hpp"
#include "kalara/Log.hpp"
#include <filesystem>
#include <fstream>

namespace kalara::exporter {

namespace fs = std::filesystem;

bool Exporter::export_windows_game(const ExportOptions& options) {
    try {
        fs::path out_path(options.output_dir);
        fs::create_directories(out_path / "assets");
        fs::create_directories(out_path / "scenes");
        fs::create_directories(out_path / "scripts");

        // 1. Write game_config.json
        core::JsonValue config;
        config["title"] = options.game_title;
        config["width"] = static_cast<double>(options.width);
        config["height"] = static_cast<double>(options.height);
        config["fullscreen"] = options.fullscreen;
        config["initial_scene"] = options.initial_scene;

        std::ofstream config_out((out_path / "game_config.json").string());
        if (config_out.is_open()) {
            config_out << config.dump(2);
            config_out.close();
        }

        // 2. Copy bundled assets & scenes from project directory if available
        if (!options.project_dir.empty() && fs::exists(options.project_dir)) {
            fs::path proj_path(options.project_dir);
            if (fs::exists(proj_path / "scenes")) {
                fs::copy(proj_path / "scenes", out_path / "scenes", fs::copy_options::recursive | fs::copy_options::overwrite_existing);
            }
            if (fs::exists(proj_path / "assets")) {
                fs::copy(proj_path / "assets", out_path / "assets", fs::copy_options::recursive | fs::copy_options::overwrite_existing);
            }
            if (fs::exists(proj_path / "scripts")) {
                fs::copy(proj_path / "scripts", out_path / "scripts", fs::copy_options::recursive | fs::copy_options::overwrite_existing);
            }
        }

        KALARA_LOG_INFO("Successfully packaged Windows standalone game in '{}'", options.output_dir);
        return true;
    } catch (const std::exception& e) {
        KALARA_LOG_ERROR("Failed to export Windows standalone game: {}", e.what());
        return false;
    }
}

} // namespace kalara::exporter
