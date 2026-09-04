#include "kalara/Input.hpp"

namespace kalara::runtime {

Input& Input::instance() noexcept {
    static Input s_instance;
    return s_instance;
}

bool Input::is_key_pressed(KeyCode key) noexcept {
    const auto& keys = instance().m_pressed_keys;
    return keys.find(key) != keys.end();
}

bool Input::is_mouse_button_pressed(MouseButton button) noexcept {
    const auto& buttons = instance().m_pressed_mouse_buttons;
    return buttons.find(button) != buttons.end();
}

core::Vector2 Input::mouse_position() noexcept {
    return instance().m_mouse_pos;
}

void Input::set_key_state(KeyCode key, bool pressed) noexcept {
    if (pressed) {
        m_pressed_keys.insert(key);
    } else {
        m_pressed_keys.erase(key);
    }
}

void Input::set_mouse_button_state(MouseButton button, bool pressed) noexcept {
    if (pressed) {
        m_pressed_mouse_buttons.insert(button);
    } else {
        m_pressed_mouse_buttons.erase(button);
    }
}

void Input::set_mouse_position(float x, float y) noexcept {
    m_mouse_pos = {x, y};
}

void Input::clear() noexcept {
    m_pressed_keys.clear();
    m_pressed_mouse_buttons.clear();
}

} // namespace kalara::runtime
