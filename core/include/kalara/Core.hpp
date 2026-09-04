#pragma once

#include <string_view>
#include "kalara/Log.hpp"
#include "kalara/Config.hpp"

namespace kalara::core {

[[nodiscard]] constexpr std::string_view version() noexcept {
    return "0.1.0";
}

void initialize() noexcept;
void shutdown() noexcept;

} // namespace kalara::core
