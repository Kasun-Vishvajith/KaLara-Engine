#pragma once

#include "kalara/KeyCodes.hpp"
#include "kalara/Math.hpp"
#include <unordered_set>

namespace kalara::runtime {

class Input {
public:
    static Input& instance() noexcept;

    static bool is_key_pressed(KeyCode key) noexcept;
    static bool is_mouse_button_pressed(MouseButton button) noexcept;
    static core::Vector2 mouse_position() noexcept;

    void set_key_state(KeyCode key, bool pressed) noexcept;
    void set_mouse_button_state(MouseButton button, bool pressed) noexcept;
    void set_mouse_position(float x, float y) noexcept;

    void clear() noexcept;

private:
    Input() = default;

    std::unordered_set<KeyCode> m_pressed_keys;
    std::unordered_set<MouseButton> m_pressed_mouse_buttons;
    core::Vector2 m_mouse_pos{0.0f, 0.0f};
};

} // namespace kalara::runtime
