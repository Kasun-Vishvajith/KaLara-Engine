#pragma once

#include <cstdint>

namespace kalara::runtime {

enum class KeyCode : uint16_t {
    Unknown = 0,
    Space = 32,
    A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    Escape = 256,
    Enter,
    Tab,
    Backspace,
    Up,
    Down,
    Left,
    Right
};

enum class MouseButton : uint8_t {
    Left = 1,
    Middle = 2,
    Right = 3
};

} // namespace kalara::runtime
