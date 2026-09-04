#include "kalara/Animation.hpp"
#include "kalara/Component.hpp"

namespace kalara::runtime {

void AnimationSystem::update(Registry& registry, float delta_time) {
    if (delta_time <= 0.0f) return;

    for (EntityID entity : registry.entities()) {
        if (!registry.has_component<AnimatorComponent>(entity)) continue;

        auto& animator = registry.get_component<AnimatorComponent>(entity);
        if (!animator.is_playing || animator.current_clip_name.empty()) continue;

        auto it = animator.clips.find(animator.current_clip_name);
        if (it == animator.clips.end() || it->second.frame_indices.empty()) continue;

        const auto& clip = it->second;
        float frame_duration = (clip.frame_rate > 0.0f) ? (1.0f / clip.frame_rate) : 0.1f;

        animator.elapsed_time += delta_time * animator.playback_speed;

        if (animator.elapsed_time >= frame_duration) {
            animator.elapsed_time -= frame_duration;
            animator.current_frame_index++;

            if (animator.current_frame_index >= clip.frame_indices.size()) {
                if (clip.is_looping) {
                    animator.current_frame_index = 0;
                } else {
                    animator.current_frame_index = static_cast<uint32_t>(clip.frame_indices.size() - 1);
                    animator.is_playing = false;
                }
            }
        }
    }
}

} // namespace kalara::runtime
