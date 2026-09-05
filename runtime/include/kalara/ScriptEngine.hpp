#pragma once

#include "kalara/Registry.hpp"
#include "kalara/Entity.hpp"
#include <cstdint>

extern "C" {
    void KaLara_Log_Info(const char* message);
    void KaLara_Log_Warn(const char* message);
    void KaLara_Log_Error(const char* message);

    uint64_t KaLara_Entity_Create(const char* name);
    void KaLara_Entity_Destroy(uint64_t entity_id);

    void KaLara_Transform_GetPosition(uint64_t entity_id, float* outX, float* outY, float* outZ);
    void KaLara_Transform_SetPosition(uint64_t entity_id, float x, float y, float z);

    bool KaLara_Input_IsKeyPressed(uint16_t key_code);
}

namespace kalara::runtime {

class ScriptEngine {
public:
    static void initialize(Registry* active_registry) noexcept;
    static void shutdown() noexcept;

    static void set_active_registry(Registry* registry) noexcept { s_active_registry = registry; }
    [[nodiscard]] static Registry* active_registry() noexcept { return s_active_registry; }

private:
    static inline Registry* s_active_registry{nullptr};
};

} // namespace kalara::runtime
