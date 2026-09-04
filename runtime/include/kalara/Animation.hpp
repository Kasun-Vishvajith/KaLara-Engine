#pragma once

#include "kalara/SpriteSheet.hpp"
#include "kalara/Registry.hpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace kalara::runtime {

struct AnimationClip {
    std::string name{"default"};
    std::vector<uint32_t> frame_indices{};
    float frame_rate{10.0f}; // Frames per second
    bool is_looping{true};
};

struct AnimatorComponent {
    std::shared_ptr<SpriteSheet> sprite_sheet{nullptr};
    std::unordered_map<std::string, AnimationClip> clips{};
    std::string current_clip_name{};
    uint32_t current_frame_index{0};
    float elapsed_time{0.0f};
    float playback_speed{1.0f};
    bool is_playing{true};

    void play(const std::string& clip_name) {
        if (current_clip_name != clip_name) {
            current_clip_name = clip_name;
            current_frame_index = 0;
            elapsed_time = 0.0f;
            is_playing = true;
        }
    }
};

class AnimationSystem {
public:
    static void update(Registry& registry, float delta_time);
};

} // namespace kalara::runtime
