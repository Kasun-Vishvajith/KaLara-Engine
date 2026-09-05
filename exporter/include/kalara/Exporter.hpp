#pragma once

#include <string>
#include <string_view>
#include <cstdint>

namespace kalara::exporter {

struct ExportOptions {
    std::string project_dir{};
    std::string output_dir{};
    std::string game_title{"KaLara Game"};
    std::string initial_scene{"scenes/default_scene.json"};
    uint32_t width{1280};
    uint32_t height{720};
    bool fullscreen{false};
};

class Exporter {
public:
    static bool export_windows_game(const ExportOptions& options);
};

} // namespace kalara::exporter
