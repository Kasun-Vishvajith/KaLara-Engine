#include "kalara/SpriteSheet.hpp"
#include "kalara/Animation.hpp"
#include "kalara/Registry.hpp"
#include <cassert>

int main() {
    // 1. Test SpriteSheet Grid Slicing
    auto sprite_sheet = std::make_shared<kalara::runtime::SpriteSheet>(nullptr, 4, 4); // 4x4 grid = 16 frames
    assert(sprite_sheet->frame_count() == 16);

    const auto& frame0 = sprite_sheet->get_frame(0);
    assert(frame0.uv_min.x == 0.0f && frame0.uv_min.y == 0.0f);
    assert(frame0.uv_max.x == 0.25f && frame0.uv_max.y == 0.25f);

    // 2. Test AnimatorComponent & AnimationSystem Playback
    kalara::runtime::Registry registry;
    kalara::runtime::EntityID entity = registry.create_entity("AnimatedHero");

    auto& animator = registry.add_component<kalara::runtime::AnimatorComponent>(entity);
    animator.sprite_sheet = sprite_sheet;

    kalara::runtime::AnimationClip run_clip{
        .name = "run",
        .frame_indices = {0, 1, 2, 3},
        .frame_rate = 10.0f, // 0.1s per frame
        .is_looping = true
    };

    animator.clips["run"] = run_clip;
    animator.play("run");

    assert(animator.current_clip_name == "run");
    assert(animator.current_frame_index == 0);

    // Step simulation by 0.15 seconds (should advance to frame index 1)
    kalara::runtime::AnimationSystem::update(registry, 0.15f);
    assert(animator.current_frame_index == 1);

    // Step simulation by 0.30 seconds (should loop back)
    kalara::runtime::AnimationSystem::update(registry, 0.30f);
    assert(animator.current_frame_index < 4);

    return 0;
}
