#include "kalara/Core.hpp"
#include "kalara/Log.hpp"
#include "kalara/Config.hpp"
#include <cassert>

int main() {
    // Test Version string
    static_assert(!kalara::core::version().empty(), "Version string should not be empty");

    // Test Config Manager
    auto& cfg_mgr = kalara::core::ConfigManager::instance();
    kalara::core::EngineConfig cfg{};
    cfg.app_name = "TestApp";
    cfg.window_width = 800;
    cfg.window_height = 600;
    cfg_mgr.set_config(cfg);

    assert(cfg_mgr.config().app_name == "TestApp");
    assert(cfg_mgr.config().window_width == 800);
    assert(cfg_mgr.config().window_height == 600);

    // Test Logger
    kalara::core::Logger::instance().set_level(kalara::core::LogLevel::Debug);
    assert(kalara::core::Logger::instance().level() == kalara::core::LogLevel::Debug);

    KALARA_LOG_INFO("Core unit test passed successfully.");
    return 0;
}
