#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "kalara/Image.hpp"
#include "kalara/Log.hpp"

namespace kalara::runtime {

std::shared_ptr<Image> Image::create_from_file(std::string_view file_path) {
    int w = 0, h = 0, ch = 0;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(file_path.data(), &w, &h, &ch, 4); // Force RGBA

    if (!data) {
        KALARA_LOG_WARN("Failed to load image file '{}': {}", file_path, stbi_failure_reason());
        return nullptr;
    }

    auto image = std::make_shared<Image>();
    image->m_metadata.id = core::UUID::generate();
    image->m_metadata.type = AssetType::Image;
    image->m_metadata.path = std::string(file_path);
    image->m_metadata.name = std::string(file_path);
    image->m_metadata.is_valid = true;

    image->m_width = static_cast<uint32_t>(w);
    image->m_height = static_cast<uint32_t>(h);
    image->m_channels = 4;
    image->m_pixels.assign(data, data + (w * h * 4));

    stbi_image_free(data);
    KALARA_LOG_INFO("Loaded Image '{}' ({}x{}, 4 channels)", file_path, w, h);
    return image;
}

std::shared_ptr<Image> Image::create_from_memory(uint32_t width, uint32_t height, uint32_t channels, const uint8_t* pixels) {
    auto image = std::make_shared<Image>();
    image->m_metadata.id = core::UUID::generate();
    image->m_metadata.type = AssetType::Image;
    image->m_metadata.name = "MemoryImage";
    image->m_metadata.is_valid = true;

    image->m_width = width;
    image->m_height = height;
    image->m_channels = channels;
    image->m_pixels.assign(pixels, pixels + (width * height * channels));
    return image;
}

} // namespace kalara::runtime
