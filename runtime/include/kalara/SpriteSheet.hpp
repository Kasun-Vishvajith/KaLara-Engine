#pragma once

#include "kalara/Texture.hpp"
#include "kalara/Math.hpp"
#include <vector>
#include <memory>
#include <cstdint>

namespace kalara::runtime {

struct AnimationFrame {
    core::Vector2 uv_min{0.0f, 0.0f};
    core::Vector2 uv_max{1.0f, 1.0f};
    float duration{0.1f};
};

class SpriteSheet {
public:
    SpriteSheet(std::shared_ptr<Texture2D> texture, uint32_t columns, uint32_t rows);
    ~SpriteSheet() = default;

    [[nodiscard]] const AnimationFrame& get_frame(uint32_t index) const;
    [[nodiscard]] uint32_t frame_count() const noexcept { return static_cast<uint32_t>(m_frames.size()); }
    [[nodiscard]] std::shared_ptr<Texture2D> texture() const noexcept { return m_texture; }

private:
    std::shared_ptr<Texture2D> m_texture;
    std::vector<AnimationFrame> m_frames;
};

} // namespace kalara::runtime
