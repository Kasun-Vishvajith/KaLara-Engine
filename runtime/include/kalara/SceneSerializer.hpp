#pragma once

#include "kalara/Scene.hpp"
#include <string>
#include <string_view>
#include <memory>

namespace kalara::runtime {

class SceneSerializer {
public:
    explicit SceneSerializer(std::shared_ptr<Scene> scene);

    bool serialize(std::string_view filepath);
    bool deserialize(std::string_view filepath);

    [[nodiscard]] std::string serialize_to_string() const;
    bool deserialize_from_string(std::string_view json_content);

private:
    std::shared_ptr<Scene> m_scene;
};

} // namespace kalara::runtime
