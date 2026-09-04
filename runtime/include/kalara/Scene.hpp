#pragma once

#include "kalara/Registry.hpp"
#include "kalara/IRenderer.hpp"
#include <string>
#include <string_view>
#include <memory>

namespace kalara::runtime {

class Scene {
public:
    explicit Scene(std::string_view name = "Untitled Scene");
    ~Scene() = default;

    static std::shared_ptr<Scene> create(std::string_view name = "Untitled Scene");

    EntityID create_entity(std::string_view name = "Entity");
    EntityID create_entity_with_id(EntityID id, std::string_view name = "Entity");
    void destroy_entity(EntityID entity_id);

    void render(IRenderer* renderer);

    [[nodiscard]] const std::string& name() const noexcept { return m_name; }
    void set_name(std::string_view name) { m_name = std::string(name); }

    [[nodiscard]] Registry& registry() noexcept { return m_registry; }
    [[nodiscard]] const Registry& registry() const noexcept { return m_registry; }

private:
    std::string m_name;
    Registry m_registry;
};

} // namespace kalara::runtime
