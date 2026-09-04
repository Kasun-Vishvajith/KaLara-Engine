#pragma once

#include "kalara/Entity.hpp"
#include "kalara/Component.hpp"
#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>
#include <any>

namespace kalara::runtime {

class Registry {
public:
    Registry() = default;
    ~Registry() = default;

    EntityID create_entity(std::string_view name = "Entity");
    EntityID create_entity_with_id(EntityID id, std::string_view name = "Entity");
    void destroy_entity(EntityID entity_id);
    [[nodiscard]] bool valid(EntityID entity_id) const noexcept;

    template<typename T, typename... Args>
    T& add_component(EntityID entity_id, Args&&... args) {
        auto& pool = get_or_create_pool<T>();
        pool[entity_id] = T{std::forward<Args>(args)...};
        return pool[entity_id];
    }

    template<typename T>
    T& get_component(EntityID entity_id) {
        auto& pool = get_or_create_pool<T>();
        return pool.at(entity_id);
    }

    template<typename T>
    [[nodiscard]] const T& get_component(EntityID entity_id) const {
        const auto& pool = get_pool<T>();
        return pool.at(entity_id);
    }

    template<typename T>
    [[nodiscard]] bool has_component(EntityID entity_id) const noexcept {
        auto type_idx = std::type_index(typeid(T));
        auto it = m_pools.find(type_idx);
        if (it == m_pools.end()) return false;

        const auto& pool = std::any_cast<const std::unordered_map<EntityID, T>&>(it->second);
        return pool.find(entity_id) != pool.end();
    }

    template<typename T>
    void remove_component(EntityID entity_id) {
        auto& pool = get_or_create_pool<T>();
        pool.erase(entity_id);
    }

    [[nodiscard]] const std::vector<EntityID>& entities() const noexcept { return m_entities; }
    [[nodiscard]] size_t entity_count() const noexcept { return m_entities.size(); }

    void clear() noexcept;

private:
    template<typename T>
    std::unordered_map<EntityID, T>& get_or_create_pool() {
        auto type_idx = std::type_index(typeid(T));
        if (m_pools.find(type_idx) == m_pools.end()) {
            m_pools[type_idx] = std::unordered_map<EntityID, T>{};
        }
        return std::any_cast<std::unordered_map<EntityID, T>&>(m_pools[type_idx]);
    }

    template<typename T>
    const std::unordered_map<EntityID, T>& get_pool() const {
        auto type_idx = std::type_index(typeid(T));
        return std::any_cast<const std::unordered_map<EntityID, T>&>(m_pools.at(type_idx));
    }

    std::vector<EntityID> m_entities;
    std::unordered_map<std::type_index, std::any> m_pools;
};

} // namespace kalara::runtime
