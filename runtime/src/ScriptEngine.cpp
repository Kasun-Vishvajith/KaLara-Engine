#include "kalara/ScriptEngine.hpp"
#include "kalara/Component.hpp"
#include "kalara/Input.hpp"
#include "kalara/Log.hpp"

extern "C" {

void KaLara_Log_Info(const char* message) {
    if (message) KALARA_LOG_INFO("[C# Script] {}", message);
}

void KaLara_Log_Warn(const char* message) {
    if (message) KALARA_LOG_WARN("[C# Script] {}", message);
}

void KaLara_Log_Error(const char* message) {
    if (message) KALARA_LOG_ERROR("[C# Script] {}", message);
}

uint64_t KaLara_Entity_Create(const char* name) {
    auto* registry = kalara::runtime::ScriptEngine::active_registry();
    if (!registry) return 0;
    return registry->create_entity(name ? name : "ScriptEntity");
}

void KaLara_Entity_Destroy(uint64_t entity_id) {
    auto* registry = kalara::runtime::ScriptEngine::active_registry();
    if (registry) {
        registry->destroy_entity(entity_id);
    }
}

void KaLara_Transform_GetPosition(uint64_t entity_id, float* outX, float* outY, float* outZ) {
    auto* registry = kalara::runtime::ScriptEngine::active_registry();
    if (!registry || !registry->valid(entity_id) || !registry->has_component<kalara::runtime::TransformComponent>(entity_id)) {
        if (outX) *outX = 0.0f;
        if (outY) *outY = 0.0f;
        if (outZ) *outZ = 0.0f;
        return;
    }

    const auto& t = registry->get_component<kalara::runtime::TransformComponent>(entity_id);
    if (outX) *outX = t.position.x;
    if (outY) *outY = t.position.y;
    if (outZ) *outZ = t.position.z;
}

void KaLara_Transform_SetPosition(uint64_t entity_id, float x, float y, float z) {
    auto* registry = kalara::runtime::ScriptEngine::active_registry();
    if (registry && registry->valid(entity_id) && registry->has_component<kalara::runtime::TransformComponent>(entity_id)) {
        auto& t = registry->get_component<kalara::runtime::TransformComponent>(entity_id);
        t.position = {x, y, z};
    }
}

bool KaLara_Input_IsKeyPressed(uint16_t key_code) {
    return kalara::runtime::Input::is_key_pressed(static_cast<kalara::runtime::KeyCode>(key_code));
}

} // extern "C"

namespace kalara::runtime {

void ScriptEngine::initialize(Registry* active_registry) noexcept {
    s_active_registry = active_registry;
    KALARA_LOG_INFO("ScriptEngine interop initialized with active engine registry.");
}

void ScriptEngine::shutdown() noexcept {
    s_active_registry = nullptr;
    KALARA_LOG_INFO("ScriptEngine interop shut down.");
}

} // namespace kalara::runtime
