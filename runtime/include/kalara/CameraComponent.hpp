#pragma once

#include "kalara/Camera2D.hpp"

namespace kalara::runtime {

struct CameraComponent {
    Camera2D camera{};
    bool is_primary{true};
};

} // namespace kalara::runtime
