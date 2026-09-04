#include "kalara/Core.hpp"
#include "kalara/Runtime.hpp"
#include "kalara/Log.hpp"

int main() {
    kalara::core::EngineConfig config{};
    config.app_name = "KaLara Editor";
    config.window_width = 1600;
    config.window_height = 900;
    config.log_level = kalara::core::LogLevel::Info;

    kalara::runtime::ApplicationRuntime app;
    if (!app.initialize(config)) {
        KALARA_LOG_CRITICAL("Failed to initialize KaLara ApplicationRuntime!");
        return 1;
    }

    KALARA_LOG_INFO("KaLara Editor running (foundation v{}).", kalara::core::version());
    
    // Simulate initial frame cycles
    app.run_frame();
    app.run_frame();

    app.shutdown();
    return 0;
}
