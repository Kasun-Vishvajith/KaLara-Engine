#include "kalara/Core.hpp"
#include "kalara/Runtime.hpp"
#include "kalara/Log.hpp"

int main() {
    kalara::core::EngineConfig config{};
    config.app_name = "KaLara Editor";
    config.window_width = 1280;
    config.window_height = 720;
    config.vsync = true;
    config.log_level = kalara::core::LogLevel::Info;

    kalara::runtime::ApplicationRuntime app;
    if (!app.initialize(config)) {
        KALARA_LOG_CRITICAL("Failed to initialize KaLara ApplicationRuntime!");
        return 1;
    }

    KALARA_LOG_INFO("Starting KaLara Editor main loop (foundation v{}). Press ESC to quit.", kalara::core::version());
    
    app.run();

    app.shutdown();
    return 0;
}
