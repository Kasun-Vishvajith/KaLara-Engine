#include "kalara/SceneSerializer.hpp"
#include "kalara/Json.hpp"
#include "kalara/Hierarchy.hpp"
#include "kalara/Log.hpp"
#include <fstream>
#include <sstream>

namespace kalara::runtime {

SceneSerializer::SceneSerializer(std::shared_ptr<Scene> scene) : m_scene(std::move(scene)) {
}

std::string SceneSerializer::serialize_to_string() const {
    if (!m_scene) return "{}";

    core::JsonValue root;
    root["scene"] = m_scene->name();

    core::JsonValue::ArrayType entity_list;

    for (EntityID entity : m_scene->registry().entities()) {
        core::JsonValue ent_json;
        ent_json["id"] = std::to_string(entity);

        if (m_scene->registry().has_component<TagComponent>(entity)) {
            ent_json["tag"] = m_scene->registry().get_component<TagComponent>(entity).name;
        }

        if (m_scene->registry().has_component<TransformComponent>(entity)) {
            const auto& t = m_scene->registry().get_component<TransformComponent>(entity);
            core::JsonValue t_json;
            t_json["position"] = core::JsonValue::ArrayType{t.position.x, t.position.y, t.position.z};
            t_json["rotation"] = core::JsonValue::ArrayType{t.rotation.x, t.rotation.y, t.rotation.z};
            t_json["scale"] = core::JsonValue::ArrayType{t.scale.x, t.scale.y, t.scale.z};
            t_json["parent"] = std::to_string(t.parent);
            ent_json["transform"] = t_json;
        }

        if (m_scene->registry().has_component<SpriteRendererComponent>(entity)) {
            const auto& s = m_scene->registry().get_component<SpriteRendererComponent>(entity);
            core::JsonValue s_json;
            s_json["color"] = core::JsonValue::ArrayType{s.color.x, s.color.y, s.color.z, s.color.w};
            s_json["texture_asset_id"] = std::to_string(s.texture_asset_id);
            ent_json["sprite"] = s_json;
        }

        entity_list.push_back(ent_json);
    }

    root["entities"] = entity_list;
    return root.dump(2);
}

bool SceneSerializer::serialize(std::string_view filepath) {
    std::string content = serialize_to_string();
    std::ofstream out(filepath.data());
    if (!out.is_open()) {
        KALARA_LOG_ERROR("Failed to open file '{}' for scene serialization!", filepath);
        return false;
    }

    out << content;
    out.close();
    KALARA_LOG_INFO("Serialized scene '{}' to '{}'", m_scene->name(), filepath);
    return true;
}

bool SceneSerializer::deserialize_from_string(std::string_view json_content) {
    if (!m_scene) return false;

    core::JsonValue root = core::JsonValue::parse(json_content);
    if (!root.contains("scene")) {
        KALARA_LOG_ERROR("Invalid scene JSON content!");
        return false;
    }

    m_scene->set_name(root["scene"].as_string("Untitled Scene"));
    m_scene->registry().clear();

    const auto& entities = root["entities"].as_array();
    for (const auto& ent_json : entities) {
        EntityID id = std::stoull(ent_json["id"].as_string("0"));
        std::string tag = ent_json["tag"].as_string("Entity");

        m_scene->create_entity_with_id(id, tag);

        if (ent_json.contains("transform")) {
            const auto& t_json = ent_json["transform"];
            auto& t = m_scene->registry().get_component<TransformComponent>(id);

            const auto& pos_arr = t_json["position"].as_array();
            if (pos_arr.size() >= 3) {
                t.position = {
                    static_cast<float>(pos_arr[0].as_number()),
                    static_cast<float>(pos_arr[1].as_number()),
                    static_cast<float>(pos_arr[2].as_number())
                };
            }

            const auto& rot_arr = t_json["rotation"].as_array();
            if (rot_arr.size() >= 3) {
                t.rotation = {
                    static_cast<float>(rot_arr[0].as_number()),
                    static_cast<float>(rot_arr[1].as_number()),
                    static_cast<float>(rot_arr[2].as_number())
                };
            }

            const auto& scale_arr = t_json["scale"].as_array();
            if (scale_arr.size() >= 3) {
                t.scale = {
                    static_cast<float>(scale_arr[0].as_number()),
                    static_cast<float>(scale_arr[1].as_number()),
                    static_cast<float>(scale_arr[2].as_number())
                };
            }

            t.parent = std::stoull(t_json["parent"].as_string("0"));
        }

        if (ent_json.contains("sprite")) {
            const auto& s_json = ent_json["sprite"];
            auto& s = m_scene->registry().add_component<SpriteRendererComponent>(id);
            const auto& col_arr = s_json["color"].as_array();
            if (col_arr.size() >= 4) {
                s.color = {
                    static_cast<float>(col_arr[0].as_number()),
                    static_cast<float>(col_arr[1].as_number()),
                    static_cast<float>(col_arr[2].as_number()),
                    static_cast<float>(col_arr[3].as_number())
                };
            }
            s.texture_asset_id = std::stoull(s_json["texture_asset_id"].as_string("0"));
        }
    }

    // Reconstruct hierarchy parent/children links
    for (EntityID entity : m_scene->registry().entities()) {
        const auto& t = m_scene->registry().get_component<TransformComponent>(entity);
        if (t.parent != NullEntityID && m_scene->registry().valid(t.parent)) {
            Hierarchy::set_parent(m_scene->registry(), entity, t.parent);
        }
    }

    KALARA_LOG_INFO("Deserialized scene '{}' (Loaded {} entities)", m_scene->name(), m_scene->registry().entity_count());
    return true;
}

bool SceneSerializer::deserialize(std::string_view filepath) {
    std::ifstream in(filepath.data());
    if (!in.is_open()) {
        KALARA_LOG_ERROR("Failed to open file '{}' for scene deserialization!", filepath);
        return false;
    }

    std::stringstream ss;
    ss << in.rdbuf();
    in.close();

    return deserialize_from_string(ss.str());
}

} // namespace kalara::runtime
