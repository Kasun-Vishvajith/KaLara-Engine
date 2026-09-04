#pragma once

#include "kalara/Asset.hpp"
#include <vector>
#include <memory>
#include <cstdint>
#include <string_view>

namespace kalara::runtime {

class Image : public Asset {
public:
    Image() = default;
    ~Image() override = default;

    static std::shared_ptr<Image> create_from_file(std::string_view file_path);
    static std::shared_ptr<Image> create_from_memory(uint32_t width, uint32_t height, uint32_t channels, const uint8_t* pixels);

    [[nodiscard]] uint32_t width() const noexcept { return m_width; }
    [[nodiscard]] uint32_t height() const noexcept { return m_height; }
    [[nodiscard]] uint32_t channels() const noexcept { return m_channels; }
    [[nodiscard]] const std::vector<uint8_t>& pixels() const noexcept { return m_pixels; }

private:
    uint32_t m_width{0};
    uint32_t m_height{0};
    uint32_t m_channels{0};
    std::vector<uint8_t> m_pixels;
};

} // namespace kalara::runtime
