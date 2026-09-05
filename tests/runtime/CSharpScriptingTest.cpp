#include "kalara/ScriptEngine.hpp"
#include "kalara/Registry.hpp"
#include "kalara/Component.hpp"
#include <cassert>

int main() {
    kalara::runtime::Registry registry;
    kalara::runtime::ScriptEngine::initialize(&registry);

    // 1. Test Entity Creation via Script Interop
    uint64_t entity_id = KaLara_Entity_Create("ScriptHero");
    assert(entity_id != 0);
    assert(registry.valid(entity_id));

    // 2. Test Transform Set & Get Position via Interop
    KaLara_Transform_SetPosition(entity_id, 150.0f, 300.0f, 10.0f);

    float px = 0.0f, py = 0.0f, pz = 0.0f;
    KaLara_Transform_GetPosition(entity_id, &px, &py, &pz);

    assert(px == 150.0f);
    assert(py == 300.0f);
    assert(pz == 10.0f);

    // 3. Test Interop Logging Diagnostics
    KaLara_Log_Info("Script engine interop test passed cleanly!");

    // 4. Test Entity Destruction via Interop
    KaLara_Entity_Destroy(entity_id);
    assert(!registry.valid(entity_id));

    kalara::runtime::ScriptEngine::shutdown();
    return 0;
}
