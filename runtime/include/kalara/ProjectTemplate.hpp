#pragma once

#include "kalara/Scene.hpp"
#include <string>
#include <string_view>
#include <memory>

namespace kalara::runtime {

enum class ProjectTemplateType {
    Blank,
    Platformer,
    TopDownRPG,
    TopDownShooter
};

class TemplateManager {
public:
    static bool create_project(ProjectTemplateType type, std::string_view project_dir, std::string_view project_name);

    [[nodiscard]] static std::shared_ptr<Scene> build_template_scene(ProjectTemplateType type, std::string_view scene_name = "Main Scene");
};

} // namespace kalara::runtime
