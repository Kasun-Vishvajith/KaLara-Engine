#pragma once

#include <cstdint>

namespace kalara::runtime {

struct Viewport {
    uint32_t x{0};
    uint32_t y{0};
    uint32_t width{1280};
    uint32_t height{720};

    [[nodiscard]] float aspect_ratio() const noexcept {
        return height > 0 ? static_cast<float>(width) / static_cast<float>(height) : 1.0f;
    }
};

} // namespace kalara::runtime
