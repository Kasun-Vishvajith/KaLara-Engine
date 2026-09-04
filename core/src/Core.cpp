#include "kalara/Core.hpp"
#include "kalara/Log.hpp"

namespace kalara::core {

void initialize() noexcept {
    KALARA_LOG_INFO("Core subsystem initialized v{}", version());
}

void shutdown() noexcept {
    KALARA_LOG_INFO("Core subsystem shut down cleanly.");
}

} // namespace kalara::core
