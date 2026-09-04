#include "kalara/SpriteSheet.hpp"

namespace kalara::runtime {

static const AnimationFrame s_default_frame{};

SpriteSheet::SpriteSheet(std::shared_ptr<Texture2D> texture, uint32_t columns, uint32_t rows)
    : m_texture(std::move(texture)) {
    if (columns == 0 || rows == 0) return;

    float cell_w = 1.0f / static_cast<float>(columns);
    float cell_h = 1.0f / static_cast<float>(rows);

    for (uint32_t r = 0; r < rows; ++r) {
        for (uint32_t c = 0; c < columns; ++c) {
            AnimationFrame frame;
            frame.uv_min = {static_cast<float>(c) * cell_w, static_cast<float>(r) * cell_h};
            frame.uv_max = {frame.uv_min.x + cell_w, frame.uv_min.y + cell_h};
            frame.duration = 0.1f;
            m_frames.push_back(frame);
        }
    }
}

const AnimationFrame& SpriteSheet::get_frame(uint32_t index) const {
    if (index < m_frames.size()) {
        return m_frames[index];
    }
    return s_default_frame;
}

} // namespace kalara::runtime
