#include "kalara/Scene.hpp"
#include "kalara/Hierarchy.hpp"
#include "kalara/Log.hpp"

namespace kalara::runtime {

Scene::Scene(std::string_view name) : m_name(name) {
}

std::shared_ptr<Scene> Scene::create(std::string_view name) {
    return std::make_shared<Scene>(name);
}

EntityID Scene::create_entity(std::string_view name) {
    return m_registry.create_entity(name);
}

EntityID Scene::create_entity_with_id(EntityID id, std::string_view name) {
    return m_registry.create_entity_with_id(id, name);
}

void Scene::destroy_entity(EntityID entity_id) {
    m_registry.destroy_entity(entity_id);
}

void Scene::render(IRenderer* renderer) {
    if (!renderer) return;

    for (EntityID entity : m_registry.entities()) {
        if (m_registry.has_component<SpriteRendererComponent>(entity)) {
            const auto& sprite = m_registry.get_component<SpriteRendererComponent>(entity);
            const auto& transform = m_registry.get_component<TransformComponent>(entity);
            core::Vector3 world_pos = Hierarchy::get_world_position(m_registry, entity);

            if (sprite.texture_asset_id != 0) {
                renderer->draw_textured_quad(world_pos.x, world_pos.y, transform.scale.x, transform.scale.y, static_cast<uint32_t>(sprite.texture_asset_id), sprite.color);
            } else {
                renderer->draw_quad(world_pos.x, world_pos.y, transform.scale.x, transform.scale.y, sprite.color);
            }
        }
    }
}

} // namespace kalara::runtime
