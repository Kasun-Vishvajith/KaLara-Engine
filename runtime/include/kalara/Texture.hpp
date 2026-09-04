#pragma once

#include "kalara/Asset.hpp"
#include "kalara/Image.hpp"
#include <memory>

namespace kalara::runtime {

class Texture2D : public Asset {
public:
    Texture2D(uint32_t width, uint32_t height, const uint8_t* pixels);
    explicit Texture2D(const Image& image);
    ~Texture2D() override;

    static std::shared_ptr<Texture2D> create(const Image& image);

    void bind(uint32_t slot = 0) const noexcept;
    void unbind() const noexcept;

    [[nodiscard]] uint32_t width() const noexcept { return m_width; }
    [[nodiscard]] uint32_t height() const noexcept { return m_height; }
    [[nodiscard]] uint32_t renderer_id() const noexcept { return m_renderer_id; }

private:
    uint32_t m_renderer_id{0};
    uint32_t m_width{0};
    uint32_t m_height{0};
};

} // namespace kalara::runtime
