#include "kalara/Core.hpp"
#include "kalara/Runtime.hpp"
#include "kalara/Log.hpp"
#include <cassert>

int main() {
    kalara::core::EngineConfig config{};
    config.app_name = "KaLara Smoke Test";
    config.log_level = kalara::core::LogLevel::Debug;

    kalara::runtime::ApplicationRuntime runtime;
    bool init_ok = runtime.initialize(config);
    assert(init_ok);
    assert(runtime.is_running());

    runtime.run_frame();

    runtime.shutdown();
    assert(!runtime.is_running());

    KALARA_LOG_INFO("KaLara Smoke Test completed cleanly.");
    return 0;
}
