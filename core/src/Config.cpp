#include "kalara/Config.hpp"

namespace kalara::core {

ConfigManager& ConfigManager::instance() noexcept {
    static ConfigManager instance;
    return instance;
}

} // namespace kalara::core
