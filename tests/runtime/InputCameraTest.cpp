#include "kalara/Input.hpp"
#include "kalara/Camera2D.hpp"
#include <cassert>

int main() {
    // 1. Test Input Abstraction State
    auto& input = kalara::runtime::Input::instance();
    input.clear();
    assert(!kalara::runtime::Input::is_key_pressed(kalara::runtime::KeyCode::W));
    assert(!kalara::runtime::Input::is_mouse_button_pressed(kalara::runtime::MouseButton::Left));

    input.set_key_state(kalara::runtime::KeyCode::W, true);
    assert(kalara::runtime::Input::is_key_pressed(kalara::runtime::KeyCode::W));

    input.set_mouse_position(400.0f, 300.0f);
    assert(kalara::runtime::Input::mouse_position().x == 400.0f);
    assert(kalara::runtime::Input::mouse_position().y == 300.0f);

    // 2. Test Camera2D Orthographic Math
    kalara::runtime::Camera2D camera(1280.0f, 720.0f);
    camera.set_position(100.0f, 200.0f);
    camera.set_zoom(2.0f);

    assert(camera.zoom() == 2.0f);
    assert(camera.position().x == 100.0f);

    // Test Screen to World Conversion: (100 / 2) + 100 = 150
    kalara::core::Vector2 world_pos = camera.screen_to_world({100.0f, 200.0f});
    assert(world_pos.x == 150.0f);
    assert(world_pos.y == 300.0f);

    return 0;
}
