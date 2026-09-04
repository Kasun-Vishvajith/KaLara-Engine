#pragma once

#include <cmath>

namespace kalara::core {

struct Vector2 {
    float x{0.0f};
    float y{0.0f};
};

struct Vector3 {
    float x{0.0f};
    float y{0.0f};
    float z{0.0f};
};

struct Vector4 {
    float x{0.0f};
    float y{0.0f};
    float z{0.0f};
    float w{1.0f};
};

struct Vertex {
    Vector3 position;
    Vector4 color;
    Vector2 uv;
};

} // namespace kalara::core
