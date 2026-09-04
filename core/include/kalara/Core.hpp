#pragma once

#include <string_view>

namespace kalara::core {

[[nodiscard]] constexpr std::string_view version() noexcept {
    return "0.0.0";
}

} // namespace kalara::core
